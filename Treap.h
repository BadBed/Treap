#pragma once
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T>
class CNode {
public:
	T key;
	int priority;
	CNode<T> *left, *right;
	size_t size;

	CNode();
	explicit CNode(const T&);
	~CNode();
};

template <typename T>
class CTreap {
private:
	CNode<T>* root_;

	static void DeleteNodes(CNode<T>* node);
	static void Update(CNode<T>* node);
	static void Push(CNode<T>* node);

	static CNode<T>* Merge(CNode<T>* first, CNode<T>* second);
	//static pair<CNode<T>*, CNode<T>* > Split(CNode<T>* root, const T& key);
	// <k, >=k

	static size_t GetSize(const CNode<T>* node);

	static T Kth(CNode<T>* node, size_t k);

	static void Print(CNode<T>* node);

public:
	CTreap();
	CTreap(const T* array, const T* end);
	~CTreap();

	size_t Size();

	void Insert(T key);
	void Delete(T key);
	bool Exist(T key) const;
	T Kth(size_t k) const;

	void Print();
};
