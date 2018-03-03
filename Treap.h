#pragma once
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "pch.h"

using std::cin;
using std::cout;
using std::pair;
using std::make_pair;

template <typename T>
class CNode {
public:
	T key;
	int priority;
	CNode<T> *left, *right;
	size_t size;
	T add, sum;

	CNode();
	explicit CNode(T);
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
	static T GetSum(const CNode<T>* node);

	static void SetAdd(CNode<T>* node, T add);

	static void Print(CNode<T>* node);
	static void UpdateAll(CNode<T>* node);

public:
	CTreap();
	CTreap(const T* array, const T* end);
	~CTreap();

	size_t Size();

	void Insert(T key, size_t pos);
	void Delete(size_t pos);
	T operator[](size_t k) const;
	void Add(T add, size_t l, size_t r);
	T Sum(size_t l, size_t r);

	void Print();
};

//###################################################################

template <typename T>
CNode<T>::CNode(){
	assert(false);
}

template <typename T>
CNode<T>::CNode(T key):
	key(key),
	priority(rand()),
	left(nullptr),
	right(nullptr),
	size(1),
	add(0),
	sum(key) {}

template <typename T>
CNode<T>::~CNode() {}

//----------------------------------------------------------------------

template <typename T>
CTreap<T>::CTreap() {
	root_ = nullptr;
}

template <typename T>
CTreap<T>::~CTreap() {
	DeleteNodes(root_);
}

template <typename T>
size_t CTreap<T>::Size() {
	return GetSize(root_);
} 

//----------------------------------------------------------------------

template <typename T>
void CTreap<T>::DeleteNodes (CNode<T>* node) {
	if (node == nullptr)
		return;

	DeleteNodes(node->left);
	DeleteNodes(node->right);
	delete node;
}

template <typename T>
void CTreap<T>::Update (CNode<T>* node) {
	if (node == nullptr)
		return;

	node->size = GetSize(node->left) + GetSize(node->right) + 1;
	node->sum = GetSum(node->left) + GetSum(node->right) + node->key;
}

template <typename T>
void CTreap<T>::Push (CNode<T>* node) {
	if (node == nullptr)
		return;

	SetAdd(node->left, node->add);
	SetAdd(node->right, node->add);
	node->add = 0;
}

//--------------------------------------------------------------------

template <typename T>
size_t CTreap<T>::GetSize (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->size;
}

template <typename T>
T CTreap<T>::GetSum (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->sum;
}

template <typename T>
void CTreap<T>::SetAdd (CNode<T>* node, T add) {
	if (node == nullptr)
		return;

	node->add += add;
	node->key += add;
}

//--------------------------------------------------------------------

template <typename T>
CTreap<T>::CTreap (const T* array, const T* end) {
	assert(true);
	std::vector<CNode<T>*> right_way;
	size_t size = (size_t)(end - array);
	if (size == 0)
		return;
	
	root_ = new CNode<T>(array[0]);
	right_way.push_back(root_);

	for (size_t i = 1; i < size; ++i) {
		CNode<T>* new_node = new CNode<T>(array[i]);
		if (right_way.back()->priority > new_node->priority) {
			right_way.back()->right = new_node;
		}
		else if (root_->priority <= new_node->priority){
			right_way.clear();
			right_way.push_back(new_node);
			new_node->left = root_;
			root_ = new_node;
		}
		else {
			CNode<T>* just_popped = nullptr;
			while (right_way.back()->priority <= new_node->priority) {
				just_popped = right_way.back();
				right_way.pop_back();
			}
			right_way.back()->right = new_node;
			new_node->left = just_popped;
		}
		right_way.push_back(new_node);
	}

	UpdateAll(root_);
}

template <typename T>
CNode<T>* CTreap<T>::Merge(CNode<T>* first, CNode<T>* second) {
	Push(first); Push(second);
	if (first == nullptr)
		return second;
	if (second == nullptr)
		return first;

	if (first->priority <= second->priority) {
		second->left = Merge(first, second->left);
		Update(second);
		return second;
	}
	else {
		first->right = Merge(first->right, second);
		Update(first);
		return first;
	}
}

template <typename T>
pair<CNode<T>*, CNode<T>* > CTreap<T>::Split(CNode<T>* node, size_t count) {
	Push(node);
	assert(count <= GetSize(node));
	if (count == 0)
		return pair<CNode<T>*, CNode<T>* >(nullptr, node);
	if (count == GetSize(node))
		return pair<CNode<T>*, CNode<T>* >(node, nullptr);

	size_t lsize = GetSize(node->left);
	if (count > lsize) {
		auto p = Split(node->right, count - lsize - 1);
		node->right = p.first;
		Update(node);
		return make_pair(node, p.second);
	}
	else {
		auto p = Split(node->left, count);
		node->left = p.second;
		Update(node);
		return make_pair(p.first, node);
	}
}

template <typename T>
CNode<T>* CTreap<T>::Find(CNode<T>* node, size_t k) {
	Push(node);
	assert(k < GetSize(node));
	size_t size_left = GetSize(node->left);
	if (size_left == k) {
		return node;
	}
	else if (size_left > k) {
		return Find(node->left, k);
	}
	else {
		return Find(node->right, k - size_left - 1);
	}
}

//--------------------------------------------------------------------------------

template <typename T>
void CTreap<T>::Insert(T key, size_t pos) {
	assert(pos <= Size());

	CNode<T>* new_node =  new CNode<T>(key);
	auto p = Split(root_, pos);
	p.first = Merge(p.first, new_node);
	root_ = Merge(p.first, p.second);
}

template <typename T>
void CTreap<T>::Delete(size_t pos) {
	assert(pos < Size());

	auto p = Split(root_, pos);
	auto p2 = Split(p.second, 1);
	delete p2.first;
	root_ = Merge(p.first, p2.second);
}

template <typename T>
T CTreap<T>::operator[](size_t k) const {
	assert(k < GetSize(root_));
	return Find(root_, k)->key;
}

template <typename T>
void CTreap<T>::Add(T add, size_t l, size_t r) {
	++r;
	auto pair1_23 = Split(root_, l);
	auto pair2_3 = Split(pair1_23.second, r - l);
	SetAdd(pair2_3.first, add);
	pair1_23.second = Merge(pair2_3.first, pair2_3.second);
	root_ = Merge(pair1_23.first, pair1_23.second);
}

template <typename T>
T CTreap<T>::Sum(size_t l, size_t r) {
	++r;
	auto pair1_23 = Split(root_, l);
	auto pair2_3 = Split(pair1_23.second, r - l);
	T result = GetSum(pair2_3.first);
	pair1_23.second = Merge(pair2_3.first, pair2_3.second);
	root_ = Merge(pair1_23.first, pair1_23.second);
	return result;
}

//---------------------------------------------------------------------------------

template <typename T>
void CTreap<T>::Print() {
	Print(root_);
	cout << "\n";
}

template <typename T>
void CTreap<T>::Print(CNode<T>* node) {
	if (node == nullptr)
		return;

	Print(node->left);
	cout << "(" << node->key << ", " << node->size << ") ";
	Print(node->right);
}

template <typename T>
void CTreap<T>::UpdateAll(CNode<T>* node) {
	if (node == nullptr)
		return;

	UpdateAll(node->left);
	UpdateAll(node->right);
	Update(node);
}

//#################################################################################

void solution() {

}
/*
int main() {
	solution();
	//system("pause");
}
*/