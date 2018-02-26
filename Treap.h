#pragma once
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::pair;

template <typename T>
class CNode {
public:
	T key;
	int priority;
	CNode<T> *left, *right;
	size_t size;

	CNode ();
	explicit CNode (T);
	~CNode();
};

template <typename T>
class CTreap {
private:
	CNode<T>* root_;

	static void DeleteNodes(CNode<T>* node);
	static void Update(CNode<T>* node);
	static void Push(CNode<T>* node);

	static CNode<T>* Find(CNode<T>* node, size_t pos);
	static CNode<T>* Merge(CNode<T>* first, CNode<T>* second);
	static pair<CNode<T>*, CNode<T>* > Split(CNode<T>* root, size_t count);
	// <k, >=k

	static size_t GetSize(const CNode<T>* node);

	static void Print(CNode<T>* node);
	static void UpdateAll (CNode<T>* node);
	
public:
	CTreap();
	CTreap(const T* array, const T* end);
	~CTreap();

	size_t Size();

	void Insert(T key, size_t pos);
	void Delete(size_t pos);
	T operator[](size_t k) const;
	void ToBegin(size_t l, size_t r);

	void Print();
};
