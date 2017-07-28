#pragma once
#include <vector>

template<typename T>
int vector_indexof(const std::vector<T>& v, const T& item)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i] == item)
			return i;
	}

	return -1;
}
