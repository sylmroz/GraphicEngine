#pragma once
#include <cstdint>

namespace GraphicEngine::Core::Utils
{
	template<template<typename> typename Container, typename T>
	void copyValuesToAlignedBuffer(const Container<T>& values, Container<char>& buffer, uint32_t alignedSize)
	{
		uint32_t offset{ 0 };
		for (auto value : values)
		{
			*(T*)(buffer.data() + offset) = value;
			offset += alignedSize;
		}
	}
}