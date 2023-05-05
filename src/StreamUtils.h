#pragma once

#include <fstream>
#include <vector>

// TODO: Surely there's a native version of this in the STL.
template<typename T>
T readType(std::fstream &stream) {
	T temp{};
	stream.read((char *) &temp, sizeof(T));
	return temp;
}

template<typename T>
std::vector<T> readTypeArr(std::fstream &stream, size_t count = 1) {
	std::vector<T> arr(count);
	stream.read((char *) arr.data(), sizeof(T) * count);
	return arr;
}

template<typename T>
void seekType(std::fstream &stream) {
	stream.seekp(sizeof(T), std::ios::cur);
}

template<typename T>
void seekTypeArr(std::fstream &stream, size_t count = 1) {
	stream.seekp(sizeof(T) * count, std::ios::cur);
}

template<typename T>
void writeType(std::fstream &stream, const T &src) {
	stream.write((char *) &src, sizeof(T));
}

void writeBytes(std::fstream &stream, const void *pData, size_t length) {
	stream.write((const char *) pData, (std::streamsize) length);
}

std::fstream openFile(const char *path) {
	std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

	file.seekg(0, std::ios::beg);
	file.seekp(0, std::ios::beg);

	return file;
}