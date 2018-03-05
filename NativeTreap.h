#pragma once
#include <vector>
#include <algorithm>

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
	T First() { return arr[0]; }
	T Last() { return arr.back(); }

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

	void Assign(T value, size_t l, size_t r) {
		for (size_t i = l; i <= r; ++i)
			arr[i] = value;
	}

	void Reverse(size_t l, size_t r) {
		int j = r;
		for (int i = l; i < j; ++i) {
			std::swap(arr[i], arr[j]);
			--j;
		}
	}

	size_t IncSuff() {
		if (arr.empty()) return 0;
		size_t i = arr.size() - 1;
		while (i > 0 && arr[i - 1] <= arr[i])
			i--;
		return arr.size() - i; 
	}

	size_t DecSuff() {
		if (arr.empty()) return 0;
		size_t i = arr.size() - 1;
		while (i > 0 && arr[i - 1] >= arr[i])
			i--;
		return arr.size() - i; 
	}

	size_t IncPref() {
		if (arr.empty()) return 0;
		size_t i = 1;
		while (i < arr.size() && arr[i] >= arr[i - 1])
			i++;
		return i;
	}

	size_t DecPref() {
		if (arr.empty()) return 0;
		size_t i = 1;
		while (i < arr.size() && arr[i] <= arr[i - 1])
			i++;
		return i;
	}

	void Swap(size_t pos1, size_t pos2) {
		swap(arr[pos1], arr[pos2]);
	}

	void NextPermutation(size_t l, size_t r) {
		if (r - l <= 0) {
			return;
		}

		int v = r - 1;
		while (arr[v] >= arr[v + 1]) {
			if (v == l) {
				Reverse(l, r);
				return;
			}
			v--;
		}

		int best = v + 1;
		for (int i = v + 2; i <= r; ++i) {
			if (arr[i] > arr[v] && arr[i] <= arr[best]) {
				best = i;
			}
		}
		swap(arr[v], arr[best]);
		Reverse(v + 1, r);
	}

	void PrevPermutation(size_t l, size_t r) {
		if (r - l <= 0) {
			return;
		}

		int v = r - 1;
		while (arr[v] <= arr[v + 1]) {
			if (v == l) {
				Reverse(l, r);
				return;
			}
			v--;
		}

		int best = v + 1;
		for (int i = v + 2; i <= r; ++i) {
			if (arr[i] < arr[v] && arr[i] >= arr[best]) {
				best = i;
			}
		}
		swap(arr[v], arr[best]);
		Reverse(v + 1, r);
	}
};