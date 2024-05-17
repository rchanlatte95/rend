#pragma once
#include "inc/rac-types.h"
#include "inc/rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr u32 STACK_CAPACITY = 256;
	inline constexpr u32 STACK_MAX_LEN = STACK_CAPACITY - 1;
	template <class T> class StaticStack
	{
	private:

		mut_u32 len = 0;
		T data[STACK_MAX_LEN];

	public:

		INLINE u32 PenultLen() const noexcept
		{
			u32 l = len - 1u;
			return l > len ? 0u : l;
		}
		INLINE u32 Len() const noexcept { return len; }
		INLINE u32 Capacity() const noexcept { return STACK_MAX_LEN; }

		INLINE const T* Pointer() const noexcept { return data; }
		INLINE u32 Remaining() const noexcept { return STACK_MAX_LEN - len; }

		INLINE Bool Full() const noexcept { return STACK_CAPACITY == len; }
		INLINE Bool Empty() const noexcept { return len == 0u; }
		INLINE Bool Any() const noexcept { return len > 0u; }

		MAY_INLINE void Push(T v)
		{
			u32 idx = len;
			len = ++len & STACK_MAX_LEN;
			data[idx] = v;
		}
		MAY_INLINE const T Pop()
		{
			u32 l = len - 1u;
			len = l > STACK_MAX_LEN ? 0 : l;
			return data[len];
		}
		MAY_INLINE const T Peek() { return data[(len - 1) & STACK_MAX_LEN]; }
		MAY_INLINE const T Peek(u32 index) const noexcept
		{
			u32 idx = index & STACK_MAX_LEN;
			u32 l = (len - idx) - 1;
			return data[l >= STACK_MAX_LEN ? 0 : l];
		}
		INLINE const T operator[](i32 index) const noexcept { return Peek(index); }
	};
}