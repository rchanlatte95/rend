#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr i32 STACK_TARGET_BYTE_SIZE = 128;
	inline constexpr i32 STACK_CAPACITY = STACK_TARGET_BYTE_SIZE - sizeof(i32);
	inline constexpr i32 STACK_MAX_INDEX = STACK_CAPACITY - 1;
	template <class T> class StaticStack
	{
	private:

		mut_i32 len = 0;
		T data[STACK_CAPACITY];

	public:

		StaticStack() { }

		INLINE static constexpr i32 ClampToBounds(i32 x)
		{
			return x > STACK_MAX_INDEX ? STACK_MAX_INDEX : x;
		}

		INLINE i32 PenultLen() const noexcept
		{
			i32 l = len - 1u;
			return l > len ? 0u : l;
		}
		INLINE i32 Len() const noexcept { return len; }
		INLINE i32 Capacity() const noexcept { return STACK_CAPACITY; }

		INLINE const T* Ptr() const noexcept { return data; }
		INLINE i32 Remaining() const noexcept { return STACK_CAPACITY - len; }

		INLINE Bool Full() const noexcept { return len > STACK_MAX_INDEX; }
		INLINE Bool Empty() const noexcept { return len == 0; }
		INLINE Bool Any() const noexcept { return len > 0; }

		MAY_INLINE void Push(T v)
		{
			if (len > STACK_MAX_INDEX) return;
			data[len++] = v;
		}
		MAY_INLINE const T Pop()
		{
			return len < 1 ? static_cast<T>(NULL) : data[--len];
		}
		MAY_INLINE const T Peek()
		{
			return len > 0 ? data[len - 1] : static_cast<T>(NULL);
		}
		MAY_INLINE const T Peek(i32 index) const noexcept
		{
			if (len < 1) return static_cast<T>(NULL);

			u32 dec_len = len - 1;
			return index > STACK_MAX_INDEX ? data[dec_len] : data[dec_len - index];
		}
		INLINE const T operator[](i32 index) const noexcept { return Peek(index); }

		MAY_INLINE void Reverse()
		{
			if (len < 2) return;

			T tmp;
			if (len == 2)
			{
				tmp = data[0];
				data[0] = data[1];
				data[1] = tmp;
				return;
			}

			T* bot = data;
			T* top = data + len - 1;
			mut_i32 half_len = len >> 1;
			while (--half_len > -1)
			{
				tmp = *top;
				*top-- = *bot;
				*bot++ = tmp;
			}
		}
	};
}