#pragma once
#include "rac-types.h"
#include "rac-logic.h"

namespace rac::static_collections
{
	using namespace rac::logic;

	inline constexpr u32 PRIORITY_QUEUE_TARGET_BYTE_SIZE = 128;
	inline constexpr u32 PRIORITY_QUEUE_CAPACITY = PRIORITY_QUEUE_TARGET_BYTE_SIZE - (sizeof(u32) * 3);
	inline constexpr u32 PRIORITY_QUEUE_MAX_INDEX = PRIORITY_QUEUE_CAPACITY - 1;
	template <class T> class StaticPriorityQueue
	{
	private:

		mut_u32 count = 0;

	public:

		StaticPriorityQueue() { }
	};
}