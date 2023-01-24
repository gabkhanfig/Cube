#pragma once

#include <GkTypes/GkTypesLib.h>
#include <glm/glm.hpp>
#include <unordered_map>

/**/
template<uint8 BitCount>
using bitset = gk::bitset<BitCount>;

/**/
template<typename T>
using darray = gk::darray<T>;

/**/
typedef gk::string string;

/**/
typedef gk::GlobalString GlobalString;

#define forceinline __forceinline