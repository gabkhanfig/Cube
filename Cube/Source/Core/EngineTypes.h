#pragma once

#include <GkTypes/GkTypesLib.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>

/**/
template<uint8 BitCount>
using bitset = gk::bitset<BitCount>;

/**/
template<typename T>
using darray = gk::darray<T>;

/**/
typedef gk::String String;

/**/
typedef gk::GlobalString GlobalString;

template<typename K, typename T>
using HashMap = std::unordered_map<K, T>;

template<typename T>
using HashSet = std::unordered_set<T>;


#define forceinline __forceinline

template<>
[[nodiscard]] inline static gk::String gk::String::From<glm::dvec3>(const glm::dvec3& value) {
	return gk::String("x: " ) + gk::String::FromFloat(value.x) + gk::String(", y: ") + gk::String::FromFloat(value.y) + gk::String(", z: ") + gk::String::FromFloat(value.z);
}