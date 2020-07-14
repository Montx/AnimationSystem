#pragma once

#include <vector>

template<typename T>
class Attribute {
public:
	Attribute(const Attribute& other) = delete;
	Attribute& operator=(const Attribute& other) = delete;

	Attribute();

	~Attribute();

	void SetAttribPointer(unsigned int slot);

	void Set(T* inputArray, unsigned int arrayLength);
	void Set(std::vector<T>& input);

	void BindTo(unsigned int slot);
	void UnBindFrom(unsigned int slot);

	unsigned int Count();
	unsigned int GetHandle();

protected:
	unsigned int mHandle;
	unsigned int mCount;
};