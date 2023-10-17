#pragma once

#ifndef CUBE_DEVELOPMENT
#define GK_CHECK false
#endif

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
[[nodiscard]] inline static gk::String gk::String::from<glm::vec3>(const glm::vec3& value) {
	return gk::String("x: ") + gk::String::fromFloat(value.x) + gk::String(", y: ") + gk::String::fromFloat(value.y) + gk::String(", z: ") + gk::String::fromFloat(value.z);
}

template<>
[[nodiscard]] inline static gk::String gk::String::from<glm::dvec3>(const glm::dvec3& value) {
	return gk::String("x: " ) + gk::String::fromFloat(value.x) + gk::String(", y: ") + gk::String::fromFloat(value.y) + gk::String(", z: ") + gk::String::fromFloat(value.z);
}

inline bool IsNearlyEqual(double a, double b, double errorTolerance = (1.e-8)) {
	return std::abs(a - b) <= errorTolerance;
}

inline bool IsNearlyEqual(float a, float b, float errorTolerance = (1.e-8)) {
	return std::abs(a - b) <= errorTolerance;
}

