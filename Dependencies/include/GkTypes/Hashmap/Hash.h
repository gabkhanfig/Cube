#pragma once

#include <type_traits>

namespace gk 
{
	template<typename KeyType>
	constexpr uint64 hash(KeyType key) = delete;

	template<>
	constexpr uint64 hash<size_t>(size_t key) {
		return key;
	}

	template<>
	constexpr uint64 hash<int>(int key) {
		return uint64(key);
	}

}