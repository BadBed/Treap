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
using std::swap;

typedef long long lint;

class CNode {
public:
	CNode * left;
	CNode *right;
	lint best_seg_size, best_seg_pos;
	lint best_pref, best_suf;
	bool assign_value;
	bool is_assign;

	CNode() { assert(false); }
	CNode(lint size);
	~CNode() {}
};

//-----------------------------------------------------------------

class CTree {
private:
	CNode * root_;
	lint size_;

	static void Push(CNode* node, lint size);
	static void Update(CNode* node, lint size);
	static void InitChilds(CNode* node, lint size);

	static lint GetBestSize(CNode* node, lint size);
	static lint GetBestPos(CNode* node, lint size);
	static lint GetBestPref(CNode* node, lint size);
	static lint GetBestSuf(CNode* node, lint size);

	static void SetAssign(CNode* node, bool value, lint size);
	static void Assign(bool value, lint op_l, lint op_r,
		CNode* node, lint n_l, lint n_r);//****

	void Assign(bool value, lint op_l, lint op_r) {
		Assign(value, op_l, op_r, root_, 0, size_);
	}

public:
	CTree(lint size);

	lint BestSize() { return GetBestSize(root_, size_); }
	lint BestPos() { return GetBestPos(root_, size_); }


	void Free(lint l, lint r) { Assign(true, l, r); }
	void Reserve(lint l, lint r) { Assign(false, l, r); }
};

//#################################################################################

CNode::CNode(lint size) {
	assert(size > 0);
	left = right = nullptr;
	best_seg_size = best_pref = best_suf = size;
	best_seg_pos = 0;
	is_assign = false;
}

CTree::CTree(lint size) {
	size_ = 1;
	while (size_ <= size)
		size_ *= 2;

	root_ = new CNode(size_);

	Reserve(size, size_);
}

//--------------------------------------------------------------------------------

void CTree::InitChilds(CNode* node, lint size) {
	assert(node != nullptr);
	if (size == 1)
		return;

	if (node->left == nullptr)
		node->left = new CNode(size / 2);
	if (node->right == nullptr)
		node->right = new CNode(size / 2);
}

void CTree::Push(CNode* node, lint size) {
	assert(node != nullptr);
	InitChilds(node, size);
	if (node->is_assign) {
		InitChilds(node, size);
		SetAssign(node->left, node->assign_value, size / 2);
		SetAssign(node->right, node->assign_value, size / 2);
		node->is_assign = false;
	}
}

void CTree::Update(CNode* node, lint size) {
	assert(node != nullptr);

	if (GetBestSize(node->left, size / 2) == size / 2) {
		node->best_pref = GetBestPref(node->right, size / 2) + size / 2;
	}
	else {
		node->best_pref = GetBestPref(node->left, size / 2);
	}

	if (GetBestSize(node->right, size / 2) == size / 2) {
		node->best_suf = GetBestSuf(node->left, size / 2) + size / 2;
	}
	else {
		node->best_suf = GetBestSuf(node->right, size / 2);
	}

	if (GetBestSize(node->left, size / 2) >=
		GetBestSuf(node->left, size / 2) + GetBestPref(node->right, size / 2)
		&& GetBestSize(node->left, size / 2) >= GetBestSize(node->right, size / 2)) {
		node->best_seg_size = GetBestSize(node->left, size / 2);
		node->best_seg_pos = GetBestPos(node->left, size / 2);
	}
	else if (GetBestSuf(node->left, size / 2) + GetBestPref(node->right, size / 2)
		>= GetBestSize(node->right, size / 2)) {
		node->best_seg_size = GetBestSuf(node->left, size / 2) + GetBestPref(node->right, size / 2);
		node->best_seg_pos = size / 2 - GetBestSuf(node->left, size / 2);
	}
	else {
		node->best_seg_size = GetBestSize(node->right, size / 2);
		node->best_seg_pos = size / 2 + GetBestPos(node->right, size / 2);
	}
}

//------------------------------------------------------------------------

lint CTree::GetBestPos(CNode* node, lint size) {
	if (node == nullptr) {
		return 0;
	}
	else {
		return node->best_seg_pos;
	}
}

lint CTree::GetBestSize(CNode* node, lint size) {
	if (node == nullptr) {
		return size;
	}
	else {
		return node->best_seg_size;
	}
}

lint CTree::GetBestPref(CNode* node, lint size) {
	if (node == nullptr) {
		return size;
	}
	else {
		return node->best_pref;
	}
}

lint CTree::GetBestSuf(CNode* node, lint size) {
	if (node == nullptr) {
		return size;
	}
	else {
		return node->best_suf;
	}
}

//------------------------------------------------------------------------

void CTree::SetAssign(CNode* node, bool value, lint size) {
	node->is_assign = true;
	node->assign_value = value;

	node->best_seg_pos = 0;
	if (node->assign_value) {
		node->best_seg_size = node->best_pref = node->best_suf = size;
	}
	else {
		node->best_seg_size = node->best_pref = node->best_suf = 0;
	}
}

void CTree::Assign(bool value, lint op_l, lint op_r,
	CNode* node, lint n_l, lint n_r) {
	assert(node != nullptr);
	if (op_l <= n_l && n_r <= op_r) {
		SetAssign(node, value, n_r - n_l);
		return;
	}

	if (op_l >= n_r || n_l >= op_r) {
		return;
	}

	Push(node, n_r - n_l);
	lint n_m = (n_r + n_l) / 2;
	Assign(value, op_l, op_r, node->left, n_l, n_m);
	Assign(value, op_l, op_r, node->right, n_m, n_r);
	Update(node, n_r - n_l);
}

//#################################################################################

void solution() {
	lint n, m;
	cin >> n >> m;
	std::vector<lint> poses(m);
	std::vector<lint> sizes(m);
	CTree tree(n);

	for (lint i = 0; i < m; ++i) {
		lint q;
		cin >> q;
		if (q > 0) {
			if (tree.BestSize() >= q) {
				lint pos = tree.BestPos();
				cout << pos + 1 << "\n";
				tree.Reserve(pos, pos + q);
				poses[i] = pos;
				sizes[i] = q;
			}
			else {
				cout << "-1\n";
			}
		}
		else {
			q *= -1;
			q--;
			tree.Free(poses[q], poses[q] + sizes[q]);
		}
	}
}
/*
int main() {
	solution();
	system("pause");
}
*/
