#pragma once
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "pch.h"

//Delete Swap
//Main
//Successor
//Next/Prev Permutation
//Enum
//

using std::cin;
using std::cout;
using std::pair;
using std::make_pair;
using std::swap;

typedef unsigned int uint;
typedef long long lint;

template <typename T>
struct CNode {
public:
	T key;
	int priority;
	CNode<T> *left, *right;
	size_t size;
	T add, sum, assign;
	bool is_assign, is_reverse;
	T leftist, rightist;
	size_t inc_suff, dec_suff;
	size_t inc_pref, dec_pref;

	CNode() = delete;
	explicit CNode(T);
	~CNode();
};

//-----------------------------------------------------------------

template <typename T>
class CSlicer {
public:
	CNode<T>* first;
	CNode<T>* second;
	CNode<T>* third;

	CSlicer(CNode<T>* node, size_t l, size_t r);
	CNode<T>* Repair();
};

//-----------------------------------------------------------------

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
	static size_t GetIncSuff(const CNode<T>* node);
	static size_t GetDecSuff(const CNode<T>* node);
	static size_t GetIncPref(const CNode<T>* node);
	static size_t GetDecPref(const CNode<T>* node);
	static size_t GetSegment(const CNode<T>* node, bool is_dec, bool is_suf);
	static T GetEdge(const CNode<T>* node, bool is_leftist);

	static void SetAdd(CNode<T>* node, T add);
	static void SetAssign(CNode<T>* node, T value);
	static void SetReverse(CNode<T>* node);

	static void UpdateSegment(CNode<T>* node, bool is_dec, bool is_suf);

	static int SucInDec(CNode<T>* node, T value);
	static int PredInInc(CNode<T>* node, T value);

	static void Print(CNode<T>* node);
	static void UpdateAll(CNode<T>* node);

	friend class CSlicer<T>;
public:
	CTreap();
	CTreap(const T* array, const T* end);
	~CTreap();

	size_t Size();

	void Insert(T key, size_t pos);
	void Delete(size_t pos);
	
	void Add(T add, size_t l, size_t r);
	void Assign(T value, size_t l, size_t r);
	void Reverse(size_t l, size_t r);
	T Sum(size_t l, size_t r);
	void NextPermutation(size_t l, size_t r);
	void PrevPermutation(size_t l, size_t r);

	T operator[](size_t k) const;
	T First() const { return root_->leftist; }
	T Last() const { return root_->rightist; }
	size_t IncSuff() const { return GetIncSuff(root_); }
	size_t DecSuff() const { return GetDecSuff(root_); }
	size_t DecPref() const { return GetDecPref(root_); }
	size_t IncPref() const { return GetIncPref(root_); }
	int SucDec(T value) { return SucInDec(root_, value); }
	int PredInc(T value) { return PredInInc(root_, value); }
	void Swap(size_t pos1, size_t pos2);

	void Print();
};

//###################################################################

template <typename T>
CNode<T>::CNode(T key):
	key(key),
	priority(rand()),
	left(nullptr),
	right(nullptr),
	size(1),
	add(0),
	sum(key),
	assign(0),
	is_assign(false),
	is_reverse(false),
	leftist(key),
	rightist(key),
	inc_suff(1),
	dec_suff(1),
	inc_pref(1),
	dec_pref(1) {}

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

//---------------------------------------------------------------------

template <typename T>
CSlicer<T>::CSlicer(CNode<T>* node, size_t l, size_t r) {
	++r;
	auto pair1_23 = CTreap<T>::Split(node, l);
	auto pair2_3 = CTreap<T>::Split(pair1_23.second, r - l);
	first = pair1_23.first;
	second = pair2_3.first;
	third = pair2_3.second;
}

template <typename T>
CNode<T>* CSlicer<T>::Repair() {
	CNode<T>* nodes12 = CTreap<T>::Merge(first, second);
	return CTreap<T>::Merge(nodes12, third);
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

	node->leftist = node->left == nullptr ? node->key : node->left->leftist;
	node->rightist = node->right == nullptr ? node->key : node->right->rightist;

	UpdateSegment(node, false, false);
	UpdateSegment(node, false, true);
	UpdateSegment(node, true, false);
	UpdateSegment(node, true, true);
}

template <typename T>
void CTreap<T>::UpdateSegment(CNode<T>* node, bool is_dec, bool is_suf) {
	CNode<T>* first = is_suf ? node->right : node->left;
	CNode<T>* second = is_suf ? node->left : node->right;
	size_t result;

	if (first == nullptr || (node->key <= GetEdge(first, is_suf) &&
				GetSegment(first, is_dec, is_suf) == GetSize(node))) {
		result = GetSize(first) + 1;
		if (second != nullptr && GetEdge(second, !is_suf) <= node->key)
			result += GetSegment(second, is_dec, is_suf);
	}
	else {
		result = GetSegment(second, is_dec, is_suf);
	}

	if (is_dec && is_suf) {
		node->dec_suff = result;
	}
	else if (is_dec && !is_suf) {
		node->dec_pref = result;
	}
	else if (!is_dec && is_suf) {
		node->inc_suff = result;
	}
	else if (!is_dec && !is_suf) {
		node->inc_pref = result;
	}
}

template <typename T>
void CTreap<T>::Push (CNode<T>* node) {
	if (node == nullptr)
		return;

	if (node->is_reverse) {
		SetReverse(node->left);
		SetReverse(node->right);
		node->is_reverse = false;
	}

	if (node->is_assign) {
		assert(node->add == 0);
		SetAssign(node->left, node->assign);
		SetAssign(node->right, node->assign);
		node->is_assign = false;
	}
	else {
		SetAdd(node->left, node->add);
		SetAdd(node->right, node->add);
		node->add = 0;
	}
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
size_t CTreap<T>::GetIncSuff (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->inc_suff;
}

template <typename T>
size_t CTreap<T>::GetDecSuff (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->dec_suff;
}

template <typename T>
size_t CTreap<T>::GetDecPref (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->dec_pref;
}

template <typename T>
size_t CTreap<T>::GetIncPref (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->inc_pref;
}

template <typename T>
T CTreap<T>::GetSum (const CNode<T>* node) {
	if (node == nullptr)
		return 0;
	else
		return node->sum;
}

template <typename T>
size_t CTreap<T>::GetSegment(const CNode<T>* node, bool is_dec, bool is_suf) {
	if (is_dec && is_suf) {
		return GetDecSuff(node);
	}
	else if (is_dec && !is_suf) {
		return GetDecPref(node);
	}
	else if (!is_dec && is_suf) {
		return GetIncSuff(node);
	}
	else if (!is_dec && !is_suf) {
		return GetIncPref(node);
	}
	else {
		assert(false);
	}
}

template <typename T>
T CTreap<T>::GetEdge(const CNode<T>* node, bool is_leftist) {
	assert(node != nullptr);

	if (is_leftist) {
		return node->leftist;
	}
	else {
		return node->rightist;
	}
}

//---------------------------------------------------------------------

template <typename T>
void CTreap<T>::SetAssign (CNode<T>* node, T value) {
	if (node == nullptr)
		return;

	node->key = value;
	node->assign = value;
	node->is_assign = true;
	node->add = 0;
	node->sum = value * node->size;
	node->leftist = value;
	node->rightist = value;
	node->inc_suff = node->size;
	node->dec_suff = node->size;
	node->inc_pref = node->size;
	node->dec_pref = node->size;
}

template <typename T>
void CTreap<T>::SetAdd (CNode<T>* node, T add) {
	if (node == nullptr)
		return;

	node->key += add;
	node->sum += add * node->size;
	node->leftist += add;
	node->rightist += add;
	if (node->is_assign) {
		node->assign += add;
	}
	else {
		node->add += add;
	}
}

template <typename T>
void CTreap<T>::SetReverse (CNode<T>* node) {
	if (node == nullptr)
		return;

	swap(node->left, node->right);
	swap(node->leftist, node->rightist);
	swap(node->inc_pref, node->dec_suff);
	swap(node->inc_suff, node->dec_pref);
	node->is_reverse = !node->is_reverse;
}

//-------------------------------------------------------------------

template <typename T>
int CTreap<T>::SucInDec(CNode<T>* node, T value) {
	if (node == nullptr)
		return -1;

	Push(node);
	assert(GetSize(node) == GetDecSuff(node));

	if (node->key <= value) {
		return SucInDec(node->left, value);
	}
	else {
		return SucInDec(node->right, value) + 1 + GetSize(node->left);
	}
}

template <typename T>
int CTreap<T>::PredInInc(CNode<T>* node, T value) {
	if (node == nullptr)
		return -1;

	Push(node);
	assert(GetSize(node) == GetIncSuff(node));

	if (node->key >= value) {
		return PredInInc(node->left, value);
	}
	else {
		return PredInInc(node->right, value) + 1 + GetSize(node->left);
	}
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
	CSlicer<T> slicer(root_, l, r);
	SetAdd(slicer.second, add);
	root_ = slicer.Repair();
}

template <typename T>
void CTreap<T>::Assign(T value, size_t l, size_t r) {
	CSlicer<T> slicer(root_, l, r);
	SetAssign(slicer.second, value);
	root_ = slicer.Repair();
}

template <typename T>
void CTreap<T>::Reverse(size_t l, size_t r) {
	CSlicer<T> slicer(root_, l, r);
	SetReverse(slicer.second);
	root_ = slicer.Repair();
}

template <typename T>
T CTreap<T>::Sum(size_t l, size_t r) {
	CSlicer<T> slicer(root_, l, r);
	T result = GetSum(slicer.second);
	root_ = slicer.Repair();
	return result;
}

template <typename T>
void CTreap<T>::NextPermutation(size_t l, size_t r) {
	CSlicer<T> slicer(root_, l, r);
	size_t unsorted = GetSize(slicer.second) - GetDecSuff(slicer.second);
	auto p = Split(slicer.second, unsorted);
	if (p.first == nullptr) {
		SetReverse(p.second);
	}
	else {
		size_t i = SucInDec(p.second, p.first->rightist);
		CSlicer<T> slicer2(p.second, i, i);
		auto p2 = Split(p.first, GetSize(p.first) - 1);

		swap(p2.second, slicer2.second);

		p.first = Merge(p2.first, p2.second);
		p.second = slicer2.Repair();

		SetReverse(p.second);
	}
	slicer.second = Merge(p.first, p.second);
	root_ = slicer.Repair();
}

template <typename T>
void CTreap<T>::PrevPermutation(size_t l, size_t r) {
	CSlicer<T> slicer(root_, l, r);
	size_t unsorted = GetSize(slicer.second) - GetIncSuff(slicer.second);
	auto p = Split(slicer.second, unsorted);
	if (p.first == nullptr) {
		SetReverse(p.second);
	}
	else {
		size_t i = PredInInc(p.second, p.first->rightist);
		CSlicer<T> slicer2(p.second, i, i);
		auto p2 = Split(p.first, GetSize(p.first) - 1);

		swap(p2.second, slicer2.second);

		p.first = Merge(p2.first, p2.second);
		p.second = slicer2.Repair();

		SetReverse(p.second);
	}
	slicer.second = Merge(p.first, p.second);
	root_ = slicer.Repair();
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
	Push(node);
	Print(node->left);
	cout << node->key << " ";
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
	uint n;
	cin >> n;
	lint* a = new lint[n];
	for (int i = 0; i < n; ++i)
		cin >> a[i];

	CTreap<lint> treap(a, a + n);
	uint q;
	cin >> q;
	for (int i = 0; i < q; ++i) {
		uint type;
		cin >> type;
		if (type == 1) {
			uint l, r;
			cin >> l >> r;
			cout << treap.Sum(l, r) << "\n";
		}
		else if (type == 2) {
			uint i;
			lint value;
			cin >> value >> i;
			treap.Insert(value, i);
		}
		else if (type == 3) {
			uint i;
			cin >> i;
			treap.Delete(i);
		}
		else if (type == 4) {
			uint l, r;
			lint x;
			cin >> x >> l >> r;
			treap.Assign(x, l, r);
		}
		else if (type == 5) {
			uint l, r;
			lint x;
			cin >> x >> l >> r;
			treap.Add(x, l, r);
		}
		else if (type == 6) {
			uint l, r;
			cin >> l >> r;
			treap.NextPermutation(l, r);
		}
		else if (type == 7) {
			uint l, r;
			cin >> l >> r;
			treap.PrevPermutation(l, r);
		}
	}
	treap.Print();
}
/*
int main() {
	solution();
	system("pause");
}*/
