#pragma once

#include <vector>

class IndexBuffer {
public:
	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(const IndexBuffer& other) = delete;

public:
	IndexBuffer();
	~IndexBuffer();

	void Set(unsigned int* rr, unsigned int len);
	void Set(std::vector<unsigned int>& input);

	unsigned int Count();
	unsigned int GetHandle();

public:
	unsigned int mHandle;
	unsigned int mCount;
};