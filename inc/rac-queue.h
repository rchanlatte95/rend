#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr u32 QUEUE_TARGET_BYTE_SIZE = 128;
	inline constexpr u32 QUEUE_CAPACITY = QUEUE_TARGET_BYTE_SIZE - sizeof(u32);
	inline constexpr u32 QUEUE_MAX_INDEX = QUEUE_CAPACITY - 1;
	template <class T> class StaticQueue
	{
	private:

		mut_u32 len = 0;
		T data[QUEUE_CAPACITY];

	public:

		StaticQueue() { }

		INLINE static constexpr u32 ClampToBounds(u32 x)
		{
			return x > QUEUE_MAX_INDEX ? QUEUE_MAX_INDEX : x;
		}

		INLINE u32 PenultLen() const noexcept
		{
			u32 dec_len = len - 1u;
			return dec_len > len ? 0u : dec_len;
		}
		INLINE u32 Len() const noexcept { return len; }
		INLINE u32 Capacity() const noexcept { return QUEUE_CAPACITY; }

		INLINE const T* Ptr() const noexcept { return data; }
		INLINE u32 Remaining() const noexcept { return QUEUE_CAPACITY - len; }

		INLINE Bool Full() const noexcept { return QUEUE_CAPACITY == len; }
		INLINE Bool Empty() const noexcept { return len == 0u; }
		INLINE Bool Any() const noexcept { return len > 0u; }

		MAY_INLINE void Enqueue(T v)
		{

		}
		MAY_INLINE const T Dequeue()
		{

		}
		MAY_INLINE const T Peek()
		{
			if (len == 0) return static_cast<T>(NULL);
			return data[len - 1];
		}
		MAY_INLINE const T Peek(u32 index) const noexcept
		{

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