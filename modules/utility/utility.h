#pragma once

#include <filesystem>


struct Image 
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = nullptr;
	// std::byte* data2;
};

Image readImage(const char* path);



template <class T, std::size_t N>
constexpr std::size_t byte_count(const std::array<T, N>& a) noexcept {
    return N * sizeof(T);
}


std::filesystem::path getExecutableDir();
