#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr i32 QUEUE_TARGET_BYTE_SIZE = 128;
	inline constexpr i32 QUEUE_CAPACITY = QUEUE_TARGET_BYTE_SIZE - (sizeof(i32) * 3);
	inline constexpr i32 QUEUE_MAX_INDEX = QUEUE_CAPACITY - 1;
	template <class T> class StaticQueue
	{
	private:

		mut_i32 count = 0;
		mut_i32 front_index = 0;
		mut_i32 back_index = 0;
		T data[QUEUE_CAPACITY];

	public:

		StaticQueue() { }

		INLINE i32 PenultCount() const noexcept
		{
			i32 dec_ct = count - 1;
			return dec_ct > count ? 0 : dec_ct;
		}
		INLINE i32 Count() const noexcept { return count; }
		INLINE i32 Capacity() const noexcept { return QUEUE_CAPACITY; }

		INLINE const T* Ptr() const noexcept { return data; }
		INLINE i32 Remaining() const noexcept { return QUEUE_CAPACITY - count; }

		INLINE Bool Full() const noexcept { return QUEUE_CAPACITY >= count; }
		INLINE Bool Empty() const noexcept { return count <= 0; }
		INLINE Bool Any() const noexcept { return count > 0; }

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