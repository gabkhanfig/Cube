#include "StringUtil.h"

void RemoveUnnecessaryChars(std::string& number)
{
}

template<typename T>
inline std::string ToString(T value)
{
	return std::string();
}

std::string ToString(int8 value)
{
	return ToString(int(value));
}

std::string ToString(int16 value)
{
	return std::to_string(value);
}

std::string ToString(int value)
{
	return std::to_string(value);
}

std::string ToString(int64 value)
{
	return std::to_string(value);
}

std::string ToString(float value)
{
	std::string numAsString = std::to_string(value);
	RemoveUnnecessaryChars(numAsString);
	return numAsString;
}

std::string ToString(double value)
{
	std::string numAsString = std::to_string(value);
	RemoveUnnecessaryChars(numAsString);
	return numAsString;
}

std::string ToString(uint8 value)
{
	return ToString(uint32(value));
}

std::string ToString(uint16 value)
{
	return std::to_string(value);
}

std::string ToString(uint32 value)
{
	return std::to_string(value);
}

std::string ToString(uint64 value)
{
	return std::to_string(value);
}

std::string ToString(glm::vec2 vector)
{
	return "x: " + ToString(vector.x) + " y: " + ToString(vector.y);
}

std::string ToString(const glm::vec3& vector)
{
	return "x: " + ToString(vector.x) + " y: " + ToString(vector.y) + " z: " + ToString(vector.z);
}

std::string ToString(const glm::vec4& vector)
{
	return "x: " + ToString(vector.x) + " y: " + ToString(vector.y) + " z: " + ToString(vector.z) + " w: " + ToString(vector.w);
}

