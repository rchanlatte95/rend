#pragma warning(push, 0)

#include <windows.h>
#include <vector>
#include <stdio.h>

#include <SDL.h>
#include <glad.h>
#include <khrplatform.h>

#include "rac-clr.h"
#include "inc/rac-types.h"
#include "inc/rac-logic.h"
#include "inc/rac-mth.h"
#include "inc/rac-gl.h"
#include "inc/rac-stack.h"
#include "inc/rac-queue.h"

#ifdef NDEBUG
#define RELEASE true
#define DEBUG false
#else
#define RELEASE false
#define DEBUG true
#endif

using namespace rac::mth;
using namespace rac::gl;
using namespace rac::string;
using namespace rac::logic;
using namespace rac::static_collections;
using namespace rac::gfx;

#pragma warning(pop)

f64 MAX_FPS = 60.0 * 3.0;
f64 MS_PER_FRAME = 1.0 / MAX_FPS;
f64 MAX_DT = 1.0 / 60.0;

mut_screen SystemScreen;
mut_Win MainWin;

static mut_SdlEvent event;

f64 TIMER_RESOLUTION = 1000.0;
mut_f64 PC_FREQ = 0.0;
mut_f64 INV_PC_FREQ = 0.0;
static mut_i64 counter = 0;

i32 GL_SUCCESS = GL_TRUE;
i32 GL_FAILED = GL_FALSE;
mut_u32 VBO, VAO, shaderProgram;
v3 verts[] = {	{ -0.5f, -0.5f, 0.0f },
				{ 0.5f, -0.5f, 0.0f },
				{ 0.0f,  0.5f, 0.0f} };

cstr vertShdrSrc =	"#version 450 core\n"
					"layout (location = 0) in vec3 aPos;\n"
					"void main()\n"
					"{\n"
					"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
					"}\0";
cstr fragShdrSrc =	"#version 450 core\n"
					"out vec4 FragColor; \n"
					"uniform float time;\n"
					"void main()\n"
					"{\n"
					"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
					"}\0";

static void AbortSDL(StaticStr abortMsg)
{
	fprintf(stderr, "%s: %s\n", abortMsg.ToCstr(), SDL_GetError());

	exit(EXIT_FAILURE);
}

static i32 CompileShader()
{
	mut_Str log;
	mut_cstr logCharPtr = log.ToMutCstr();
	mut_i32 compileResult;

	u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShdrSrc, NULL);
	glCompileShader(vertShader);

	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FAILED)
	{
		glGetShaderInfoLog(vertShader, log.Capacity(), NULL, logCharPtr);
		printf(log.ToCstr());
		log.Clear();
		return GL_FAILED;
	}

	u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShdrSrc, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FAILED)
	{
		glGetShaderInfoLog(vertShader, log.Capacity(), NULL, logCharPtr);
		printf(log.ToCstr());
		log.Clear();
		return GL_FAILED;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compileResult);

	if (compileResult == GL_FAILED)
	{
		glGetShaderInfoLog(shaderProgram, log.Capacity(), NULL, logCharPtr);
		printf(log.ToCstr());
		log.Clear();
		return GL_FAILED;
	}
	i32 test = glGetUniformLocation(shaderProgram, "time");
	printf("%d", test);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	return GL_SUCCESS;
}

static void InitRenderer()
{
	i32 shaderResult = CompileShader();
	if (shaderResult == GL_FAILED)
		AbortSDL("\r\nFailed to compile shaders.");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v3), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void MainInit()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		AbortSDL("Couldn't initialize SDL");

	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(nullptr); // Default OpenGL is fine.

	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SystemScreen.InitSystemInfo();
	WindowInitResult res;

	SmallStr mainWindowTitle = "Rend";
#if RELEASE
	res = MainWin.CreateCentered(mainWindowTitle, SystemScreen.SizeToV2I(0.5f));
#elif DEBUG
	res = MainWin.CreateCentered("DEBUG " + mainWindowTitle, SystemScreen.SizeToV2I(0.5f));
#endif

	if (res != Succeeded)
	{
		switch (res)
		{
		case WindowCreateFailed: AbortSDL("Couldn't create SDL Window.");
		case ContextCreateFailed: AbortSDL("Couldn't set video mode.");
		default: AbortSDL("Unknown error occured in creating and setting SDL window.");
		}
	}

#if DEBUG
	gladLoadGLLoader(SDL_GL_GetProcAddress); // Check OpenGL properties
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
#endif

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	MainWin.SetViewport();

	InitRenderer();
}

static void GetFrequency()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		AbortSDL("QueryPerformanceFrequency failed!\n");

	PC_FREQ = f64(li.QuadPart) / TIMER_RESOLUTION;
	INV_PC_FREQ = 1.0 / PC_FREQ;
}

static f64 GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return f64(li.QuadPart) * INV_PC_FREQ;
}

static void Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	MainWin.Swap();
}

static i32 PollInput()
{
#if DEBUG
	u32 CLEAR_MAX = 32;
	u32 CLEAR_LEN = CLEAR_MAX - 1;
	char clear[CLEAR_MAX];
	memset(clear, ' ', CLEAR_LEN);
	clear[CLEAR_LEN] = '\0';

	printf("Input IDLE...\r");
#endif

	while (SDL_PollEvent(&event))
	{
		const SDL_Keycode keycode = event.key.keysym.sym;
		switch (event.type)
		{

		case SDL_KEYDOWN:

#if DEBUG
			printf("%s\r", clear);
#endif

			switch (keycode)
			{
			case SDLK_ESCAPE: return 0;
			default:

#if DEBUG
				printf("%s DOWN\n", SDL_GetKeyName(keycode));
#endif
				return 1;
			}

		case SDL_QUIT: return 0;

		}
	}
	return 1;
}

static const unsigned char BitsSetTable256[256] =
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};

static inline constexpr i32 PopCt(mut_u32 v)
{
	return	BitsSetTable256[v & 0xff] +
			BitsSetTable256[(v >> 8) & 0xff] +
			BitsSetTable256[(v >> 16) & 0xff] +
			BitsSetTable256[v >> 24];
}

using namespace std;

static inline constexpr int Bsr(unsigned int k)
{
	int i = -1;
	for (; k; k >>= 1, ++i) {}
	return i;
}

static const int DeBruijnBitPos[32] =
{
  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};
static inline constexpr int BsrDB(unsigned int v)
{
#pragma warning( disable : 4146) // Unsigned negation
	return DeBruijnBitPos[((u32)((v & -v) * 0x077CB531U)) >> 27];
#pragma warning( default : 4146)
}

static inline vector<int> ConstructSubset(const vector<int>& nums, int subsetIndex)
{
	const int subsetSz = PopCt(subsetIndex);
	vector<int> currSubset(subsetSz);

	for (int i = 0; i < subsetSz; subsetIndex >>= 1, ++i)
	{
		i32 idx = BsrDB(subsetIndex);
		currSubset[i] = nums[idx];
	}

	return currSubset;
}

class Solution
{
public:
	const vector<int> EMPTY_INT_VEC = vector<int>{};

	vector<vector<int>> subsets(vector<int>& nums)
	{
		const int ct = (int)nums.size();
		const int subsetCt = 1 << ct;
		vector<vector<int>> res(subsetCt);

		int currSubsetIdx = 0;
		res[currSubsetIdx++] = EMPTY_INT_VEC;
		for (; currSubsetIdx < subsetCt; ++currSubsetIdx)
			res[currSubsetIdx] = ConstructSubset(nums, currSubsetIdx);

		return res;
	}
};

int main(int argc, char* argv[])
{
	(void)argc; argv = NULL;

	mut_color c(128);
	mut_Str str;
	c.ToStr(str);
	printf("%s", str.ToCstr());

	/*
	GetFrequency();
	MainInit();

	// NOTE(RYAN_2024-04-27):	I have no physics simulations running so I don't
	//							anticipate any need for more sophisticated time
	//							step handling. This is already a halfway accum
	//							time step impl.
	mut_f64 dt = 0.01;
	mut_f64 accumulator = 0.0;
	mut_f64 currentTime = GetCounter();

	Bool running = true;
	while (running)
	{
		mut_f64 newTime = GetCounter();
		mut_f64 frameTime = newTime - currentTime;
		if (frameTime > MAX_DT)
			frameTime = MAX_DT;

		currentTime = newTime;
		accumulator += frameTime;
		while (accumulator >= MAX_DT)
		{
			running = PollInput();
			accumulator -= dt;
		}

		Render();
	}
	*/
	return EXIT_SUCCESS;
}
