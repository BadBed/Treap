#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
//#include "pch.h"

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

	CNode ();
	explicit CNode (T);
	~CNode () {}
};

template <typename T>
class CTreap {
private:
	CNode<T>* root_;

	static void DeleteNodes (CNode<T>* node);
	static void Update (CNode<T>* node);
	static void Push (CNode<T>* node);

	static CNode<T>* Find(CNode<T>* node, T key);
	static CNode<T>* Merge(CNode<T>* first, CNode<T>* second);
	static pair<CNode<T>*, CNode<T>* > Split(CNode<T>* root, T key);
	// <k, >=k

	static size_t GetSize (const CNode<T>* node);

	static T Kth (CNode<T>* node, size_t k);

	static void Print (CNode<T>* node);

public:
	CTreap():root_(nullptr) {}
	CTreap(const T* array, const T* end);
	~CTreap() { DeleteNodes(root_); }

	size_t Size() { return GetSize(root_); }

	void Insert (T key);
	void Delete (T key);
	bool Exist (T key) const;
	T Kth (size_t k) const;

	void Print ();
};

//###################################################################

template <typename T>
CNode<T>::CNode():
	key(0),
	priority(rand()),
	left(nullptr),
	right(nullptr),
	size(1) {}

template <typename T>
CNode<T>::CNode(T key):
	key(key),
	priority(rand()),
	left(nullptr),
	right(nullptr),
	size(1) {}


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
size_t CTreap<T>::GetSize (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->size;
}

template <typename T>
void CTreap<T>::Update (CNode<T>* node) {
	if (node == nullptr)
		return;

	node->size = GetSize(node->left) + GetSize(node->right) + 1;
}

template <typename T>
void CTreap<T>::Push (CNode<T>* node) {

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
		right_way.back();
	}
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
pair<CNode<T>*, CNode<T>* > CTreap<T>::Split(CNode<T>* node, T key) {
	Push(node);
	if (node == nullptr)
		return pair<CNode<T>*, CNode<T>* >(nullptr, nullptr);

	if (key > node->key) {
		auto p = Split(node->right, key);
		node->right = p.first;
		Update(node);
		return make_pair(node, p.second);
	}
	else {
		auto p = Split(node->left, key);
		node->left = p.second;
		Update(node);
		return make_pair(p.first, node);
	}
}

template <typename T>
CNode<T>* CTreap<T>::Find(CNode<T>* node, T key) {
	if (node == nullptr || node->key == key)
		return node;

	Push(node);
	if (node->key > key) {
		return Find(node->left, key);
	}
	else {
		return Find(node->right, key);
	}
}

//--------------------------------------------------------------------------------

template <typename T>
void CTreap<T>::Insert(T key) {
	if (Exist(key))
		return;

	CNode<T>* new_node =  new CNode<T>(key);
	auto p = Split(root_, key);
	p.first = Merge(p.first, new_node);
	root_ = Merge(p.first, p.second);
}

template <typename T>
void CTreap<T>::Delete(T key) {
	assert(Exist(key));

	auto p = Split(root_, key);
	auto p2 = Split(p.second, key + 1);
	delete p2.first;
	root_ = Merge(p.first, p2.second);
}

template <typename T>
T CTreap<T>::Kth(size_t k) const {
	assert(k < GetSize(root_));
	return Kth(root_, k);
}

template <typename T>
T CTreap<T>::Kth(CNode<T>* node, size_t k) {
	assert(k < GetSize(node));
	Push(node);
	size_t size_left = GetSize(node->left);
	if (size_left == k) {
		return node->key;
	}
	else if (size_left > k) {
		return Kth(node->left, k);
	}
	else {
		return Kth(node->right, k - size_left - 1);
	}
}

template <typename T>
bool CTreap<T>::Exist(T key) const {
	return Find(root_, key) != nullptr;
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
	cout << node->key << " ";
	Print(node->right);
}

//#################################################################################

int solution() {
	CTreap<int> treap;
	size_t n;
	cin >> n;
	for (size_t i = 0; i < n; ++i) {
		int type, arg;
		cin >> type >> arg;

		if (type == 1) {
			treap.Insert(arg);
		}
		else if (type == -1) {
			treap.Delete(arg);
		}
		else if (type == 0) {
			cout << treap.Kth(treap.Size() - arg) << "\n";
		}
	}

	return 0;
}

int main() {
	return solution();
}

