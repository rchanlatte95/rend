#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr u32 QUEUE_TARGET_BYTE_SIZE = 128;
	inline constexpr u32 QUEUE_CAPACITY = QUEUE_TARGET_BYTE_SIZE - (sizeof(u32) * 3);
	inline constexpr u32 QUEUE_MAX_INDEX = QUEUE_CAPACITY - 1;
	template <class T> class StaticQueue
	{
	private:

		mut_u32 count = 0;
		mut_u32 front_index = 0;
		mut_u32 back_index = 0;
		T data[QUEUE_CAPACITY];

	public:

		StaticQueue() { }

		INLINE u32 PenultCount() const noexcept
		{
			u32 dec_ct = count - 1u;
			return dec_ct > count ? 0u : dec_ct;
		}
		INLINE u32 Count() const noexcept { return count; }
		INLINE u32 Capacity() const noexcept { return QUEUE_CAPACITY; }

		INLINE const T* Ptr() const noexcept { return data; }
		INLINE u32 Remaining() const noexcept { return QUEUE_CAPACITY - count; }

		INLINE Bool Full() const noexcept { return QUEUE_CAPACITY == count; }
		INLINE Bool Empty() const noexcept { return count == 0u; }
		INLINE Bool Any() const noexcept { return count > 0u; }

		MAY_INLINE void Enqueue(T v)
		{
			if (count > QUEUE_MAX_INDEX) return;

			++count;
			data[front_index] = v;
			front_index = (front_index + 1) % QUEUE_CAPACITY;
		}
		MAY_INLINE const T Dequeue()
		{
			if (count < 1) return static_cast<T>(NULL);

			--count;

			u32 idx = back_index;
			back_index = (back_index + 1) % QUEUE_CAPACITY;
			return data[idx];
		}
		MAY_INLINE const T Peek()
		{
			if (count < 1) return static_cast<T>(NULL);
			return data[back_index];
		}
		INLINE const T operator[](i32 index) const noexcept { return data[index]; }

		MAY_INLINE void Reverse()
		{
			if (count < 2) return;

			T tmp;
			if (count == 2)
			{
				tmp = data[0];
				data[0] = data[1];
				data[1] = tmp;
				return;
			}

			T* bot = data;
			T* top = data + count - 1;
			mut_i32 half_len = count >> 1;
			while (--half_len > -1)
			{
				tmp = *top;
				*top-- = *bot;
				*bot++ = tmp;
			}
		}
	};
}