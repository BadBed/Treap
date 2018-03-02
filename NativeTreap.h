#pragma once
#include <set>
#include <map>

template <typename T>
class CNative {
private:
	std::map<T, T> a;
public:

	void Delete(T key) { a.erase(key); }
	bool Exist(T key) const { return a.count(key) != 0; }
	void Insert(T key, T value) {
		if (!Exist(key))
			a[key] = value;
	}
	
	T Max() const {
		T result = a.begin()->second;
		for (auto it = a.begin(); it != a.end(); ++it) {
			result = max(result, it->second);
		}
		return result;
	}
	T KeyOfMax() const {
		T vmax = a.begin()->second;
		T result = a.begin()->first;
		for (auto it = a.begin(); it != a.end(); ++it) {
			if (it->second > vmax) {
				vmax = it->second;
				result = it->first;
			}
		}
		return result;
	}
};
