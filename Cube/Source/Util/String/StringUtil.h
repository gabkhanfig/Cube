#pragma once

#include <Engine/Types/EngineTypes.h>

void RemoveUnnecessaryChars(std::string& number);

template<typename T>
std::string ToString(T value);

std::string ToString(int8 value);

std::string ToString(int16 value);

std::string ToString(int value);

std::string ToString(int64 value);

std::string ToString(float value);

std::string ToString(double value);

std::string ToString(uint8 value);

std::string ToString(uint16 value);

std::string ToString(uint32 value);

std::string ToString(uint64 value);

std::string ToString(glm::vec2 vector);

std::string ToString(const glm::vec3& vector);

std::string ToString(const glm::vec4& vector);


