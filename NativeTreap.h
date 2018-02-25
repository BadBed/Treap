#pragma once
#include <set>

template <typename T>
class CNative {
private:
	std::set<T> a;
public:

	void Insert(T key) { a.insert(key); }
	void Delete(T key) { a.erase(key); }
	bool Exist(T key) const { return a.count(key) != 0; }
	T Kth(int key) {
		auto it = a.begin();
		for (int i = 0; i < key; ++i) {
			++it;
		}
		return *it;
	}
};