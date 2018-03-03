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

	t.Add(10, 3, 5);
	EXPECT_EQ(t[2], 2);
	EXPECT_EQ(t[3], 13);
	EXPECT_EQ(t[4], 14);
	EXPECT_EQ(t[5], 15);
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

//############################################################################

TEST(native, one) {
	CNative<int> t;

	t.Insert(3, 0);
	t.Insert(10, 0);
	t.Insert(100, 2);
	t.Delete(1);
	EXPECT_EQ(t[0], 10);
	EXPECT_EQ(t[1], 100);
}

//############################################################################

enum CEventType {INSERT = 0, DELETE, KTH, ADD, SUM, COUNT_OF_TYPES};

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
	else if (type == ADD) {
		result += to_string(key) + " " + to_string(l) + " " + to_string(r);
	}
	else if (type == SUM) {
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
	else {
		assert(false);
	}
	return 0;
}

bool CEvent::IsCorrect(size_t& size) const {
	if ((type != INSERT) && size == 0)
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
		if (events[i].DoIt(t1) != events[i].DoIt(t2)) {
			string log;
			for (size_t j = 0; j <= i; ++j) {
				log += events[j].Log();
			}
			cout << log;
			return false;
		}
	}
	return true;
}

TEST (auto_test, small) {
	for (size_t i = 0; i < 4e4; ++i) {
		if(!test(10, 10)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}

	for (size_t i = 0; i < 30; ++i) {
		if (!test(1000, 1e5)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();
	system("pause");
	return r;
}
