#include "pch.h"
#include "Treap.h"
#include "NativeTreap.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <map>

using std::vector;
using std::string;
using std::swap;
using std::to_string;
using std::cout;
using std::max;
using std::set;

TEST(manual, construct) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	EXPECT_EQ(a[3], 3);
}

TEST(manual, insert) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	t.Insert(100, 4);
	EXPECT_EQ(t[4], 100);
	EXPECT_EQ(t[3], 3);
	EXPECT_EQ(t[5], 4);
}

TEST(manual, delete) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	t.Delete(4);
	EXPECT_EQ(t[4], 5);
	EXPECT_EQ(t[3], 3);
}

TEST(manual, add) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	t.Add(-10, 3, 5);
	EXPECT_EQ(t[2], 2);
	EXPECT_EQ(t[3], -7);
	EXPECT_EQ(t[4], -6);
	EXPECT_EQ(t[5], -5);
	EXPECT_EQ(t[6], 6);
}

TEST(manual, sum) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);
	t.Insert(100, 4);
	EXPECT_EQ(t.Sum(3, 4), 103);
	EXPECT_EQ(t.Sum(5, 7), 15);
	EXPECT_EQ(t.Sum(0, 5), 110);
}

TEST(manual, assign_sum) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	t.Assign(10, 3, 6);
	t.Assign(100, 4, 5);
	EXPECT_EQ(t.Sum(2, 6), 222);
}

TEST(manual, add_sum) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);
	
	t.Insert(8, 0);
	t.Add(6, 0, 0);
	t.Add(4, 0, 0);
	EXPECT_EQ(t.Sum(0, 0), 18);
}

TEST(manual, reverse) {
	int* a = new int[9];
	for (int i = 0; i < 9; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 9);

	t.Reverse(2, 6);
	EXPECT_EQ(t.Sum(0, 3), 12);
}

TEST(manual, first_last) {
	int* a = new int[7];
	for (int i = 0; i < 7; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 7);

	EXPECT_EQ(t.First(), 0);
	EXPECT_EQ(t.Last(), 6);
	t.Reverse(0, 4);
	t.Reverse(4, 6);
	t.Add(10, 3, 6);
	EXPECT_EQ(t.First(), 4);
	EXPECT_EQ(t.Last(), 10);
}

TEST(manual, maxinc_maxdec) {
	int* a = new int[3];
	for (int i = 0; i < 3; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 3);

	EXPECT_EQ(t.IncSuff(), 3);
	EXPECT_EQ(t.DecSuff(), 1);
	EXPECT_EQ(t.IncPref(), 3);
	EXPECT_EQ(t.DecPref(), 1);
	t.Reverse(1, 2);
	EXPECT_EQ(t.DecSuff(), 2);
	EXPECT_EQ(t.IncPref(), 2);
}

TEST(manual, all_operations) {
	int* a = new int[15];
	for (int i = 0; i < 15; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 15);

	t.Add(1000, 3, 8);
	t.Assign(100, 5, 10);
	t.Add(10, 4, 5);
	t.Reverse(0, 10);
	EXPECT_EQ(t.Sum(2, 10), 2430);
}

TEST(manual, suc_pred) {
	int* a = new int[15];
	for (int i = 0; i < 15; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 15);

	EXPECT_EQ(t.PredInc(6), 5);
	t.Reverse(0, 14);
	EXPECT_EQ(t.SucDec(10), 3);
	t.Reverse(0, 14);

	t.Assign(5, 3, 7);
	EXPECT_EQ(t.PredInc(5), 2);
	t.Add(10, 2, 14);
	EXPECT_EQ(t.PredInc(18), 7);
}

TEST(manual, swap) {
	int* a = new int[15];
	for (int i = 0; i < 15; ++i)
		a[i] = i;
	CTreap<int> t(a, a + 15);

	t.Swap(4, 7);
	EXPECT_EQ(t[4], 7);
	EXPECT_EQ(t[7], 4);
	EXPECT_EQ(t[5], 5);
}

TEST(manual, fatal1) {
	CTreap<int> t;
	t.Insert(3, 0);
	t.Insert(0, 0);
	t.Insert(7, 0);
	t.Reverse(0, 1);
	t.Reverse(0, 2);
	EXPECT_EQ(t.IncSuff(), 1);
	EXPECT_EQ(t.DecSuff(), 2);
	EXPECT_EQ(t.IncPref(), 2);
	EXPECT_EQ(t.DecPref(), 1);
}

TEST(manual, next_permutation) {
	int* a = new int[5];
	//a = {3, 100, 7, 7, 8};
	a[0] = 3; a[1] = 100; a[2] = 7; a[3] = 7; a[4] = 8;
	CTreap<int> t(a, a + 5);

	t.NextPermutation(0, 4);
	EXPECT_EQ(t[0], 3);
	EXPECT_EQ(t[1], 100);
	EXPECT_EQ(t[2], 7);
	EXPECT_EQ(t[3], 8);
	EXPECT_EQ(t[4], 7);
}

TEST(manual, prev_permutation) {
	int* a = new int[5];
	a[0] = 3; a[1] = 100; a[2] = 7; a[3] = 7; a[4] = 8;
	CTreap<int> t(a, a + 5);

	t.PrevPermutation(0, 4);
	EXPECT_EQ(t[0], 3);
	EXPECT_EQ(t[1], 8);
	EXPECT_EQ(t[2], 100);
	EXPECT_EQ(t[3], 7);
	EXPECT_EQ(t[4], 7);
}

TEST(manual, fatal2) {
	int* a = new int[3];
	a[0] = 6; a[1] = 0; a[2] = 0;
	CTreap<int> t(a, a + 3);

	t.PrevPermutation(0, 2);

	EXPECT_EQ(t[0], 0);
	EXPECT_EQ(t[1], 6);
	EXPECT_EQ(t[2], 0);
}
//############################################################################

TEST(native, one) {
	CNative<int> t;

	t.Insert(3, 0);
	t.Insert(10, 0);
	t.Insert(100, 2);
	t.Delete(1);
	EXPECT_EQ(t[0], 10);
	EXPECT_EQ(t[1], 100);
	t.Swap(1, 1);
	EXPECT_EQ(t[1], 100);
}

TEST(native, prev) {
	CNative<int> t;
	t.Insert(0, 0);
	t.Insert(0, 0);
	t.Insert(6, 0);
	t.Sum(0, 0);
	t.IncSuff();
	t.PrevPermutation(0, 2);
	EXPECT_EQ(t.IncPref(), 2);
}

//############################################################################

enum CEventType {INSERT = 0, DELETE, KTH, ADD, SUM, ASSIGN, REVERSE, FIRST, LAST,
INC_SUFF, DEC_SUFF, INC_PREF, DEC_PREF, SWAP, NEXT_PERM, PREV_PERM, COUNT_OF_TYPES};

class CEvent{
public:
	CEventType type;
	int key;
	size_t i, l, r;

	CEvent (int max_key, size_t size) {
		type = static_cast<CEventType>(rand() % COUNT_OF_TYPES);
		i = rand() % max(size, (size_t)1);
		key = rand() % max_key;
		r = rand() % max(size, (size_t)1);
		l = rand() % (r + 1);
	}

	template<class TTreap>
	int DoIt(TTreap&) const;

	string Log() const;

	bool IsCorrect(size_t&) const;
};

string CEvent::Log() const {
	string result = to_string(type) + " ";

	if (type == INSERT) {
		result += to_string(i) + " " + to_string(key);
	}
	else if (type == KTH || type == DELETE){
		result += to_string(i);
	}
	else if (type == ADD || type == ASSIGN) {
		result += to_string(key) + " " + to_string(l) + " " + to_string(r);
	}
	else if (type == SUM || type == REVERSE || type == SWAP ||
			type == NEXT_PERM || type == PREV_PERM) {
		result += to_string(l) + " " + to_string(r);
	}
	
	result += "\n";
	return result;
}

template<class TTreap>
int CEvent::DoIt(TTreap& treap) const {
	if (type == INSERT) {
		treap.Insert(key, i);
	}
	else if (type == DELETE) {
		treap.Delete(i);
	}
	else if (type == KTH) {
		return treap[i];
	}
	else if (type == ADD) {
		treap.Add(key, l, r);
	}
	else if (type == SUM) {
		return treap.Sum(l, r);
	}
	else if (type == ASSIGN) {
		treap.Assign(key, l, r);
	}
	else if (type == REVERSE) {
		treap.Reverse(l, r);
	}
	else if (type == FIRST) {
		return treap.First();
	}
	else if (type == LAST) {
		return treap.Last();
	}
	else if (type == INC_SUFF) {
		return treap.IncSuff();
	}
	else if (type == DEC_SUFF) {
		return treap.DecSuff();
	}
	else if (type == INC_PREF) {
		return treap.IncPref();
	}
	else if (type == DEC_PREF) {
		return treap.DecPref();
	}
	else if (type == SWAP) {
		treap.Swap(l, r);
	}
	else if (type == NEXT_PERM) {
		treap.NextPermutation(l, r);
	}
	else if (type == PREV_PERM) {
		treap.PrevPermutation(l, r);
	}
	else {
		assert(false);
	}
	return 0;
}

bool CEvent::IsCorrect(size_t& size) const {
	if ((type != INSERT && type != INC_SUFF && type != DEC_SUFF &&
			type != INC_PREF && type != DEC_PREF) && size == 0)
		return false;

	if (type == INSERT)
		++size;
	if (type == DELETE)
		--size;

	return true;
}

vector<CEvent> GenEvents(size_t size, int max_key) {
	size_t vsize = 0;
	vector<CEvent> events;
	
	for (size_t i = 0; i < size; ++i) {
		while (true) {
			CEvent new_event(max_key, vsize);
			if (new_event.IsCorrect(vsize)) {
				events.push_back(new_event);
				break;
			}
		}
	}

	return events;
}

bool test(size_t size, int max_key) {
	vector<CEvent> events = GenEvents(size, max_key);
	CTreap<int> t1;
	CNative<int> t2;

	for (size_t i = 0; i < size; ++i) {
		int r1 = events[i].DoIt(t1);
		int r2 = events[i].DoIt(t2);
		if (r1 != r2) {
			string log;
			for (size_t j = 0; j <= i; ++j) {
				log += events[j].Log();
			}
			cout << log;
			EXPECT_EQ(r1, r2);
			return false;
		}
	}
	return true;
}

TEST (auto_test, all) {
	for (size_t i = 0; i < 4e4; ++i) {
		if(!test(10, 10)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}

	for (size_t i = 0; i < 30; ++i) {
		if (!test(10000, 1e5)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}
}
/*
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();
	system("pause");
	return r;
}*/
