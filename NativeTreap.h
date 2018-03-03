#pragma once
#include <vector>

template <typename T>
class CNative {
private:
	std::vector<T> arr;
public:

	void Insert(T key, size_t pos) {
		arr.push_back(key);
		for (int i = arr.size() - 2; i >= (int)pos; --i)
			swap(arr[i], arr[i + 1]);
	}
	void Delete(size_t pos) { 
		for (size_t i = pos; i < arr.size() - 1; ++i)
			swap(arr[i], arr[i + 1]);
		arr.pop_back();
	}
	T operator[](size_t pos) { return arr[pos]; }
	void Add(T add, size_t l, size_t r) {
		for (size_t i = l; i <= r; ++i)
			arr[i] += add;
	}
	T Sum(size_t l, size_t r) {
		T result = 0;
		for (size_t i = l; i <= r; ++i)
			result += arr[i];
		return result;
	}
};