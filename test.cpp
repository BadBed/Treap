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

typedef unsigned int uint;

TEST(manual, smth) {
	CTreap<uint> t;
	t.Insert(3, 10);
	t.Insert(5, 10);
	t.Insert(4, 10);
	t.Delete(4);
	EXPECT_EQ(t.Kth(0), 3);
	EXPECT_EQ(t.Kth(1), 5);
}

TEST(manual, big_value) {
	CTreap<uint> t;
	t.Insert(1e9, 10);
	t.Insert(123456, 10);
	t.Delete(1e9);
	EXPECT_EQ(t.Kth(0), 123456);
	EXPECT_TRUE(t.Exist(123456));
	EXPECT_TRUE(t.Exist(123456));
	EXPECT_FALSE(t.Exist(1e9));
}

TEST(manual, max) {
	CTreap<uint> t;
	t.Insert(3, 4);
	t.Insert(1, 100);
	t.Insert(2, 1);
	EXPECT_EQ(t.Max(), 100);
	EXPECT_EQ(t.KeyOfMax(), 1);
}

TEST(native, max) {
	CTreap<uint> t;
	t.Insert(3, 4);
	t.Insert(1, 100);
	t.Insert(2, 1);
	EXPECT_EQ(t.Max(), 100);
	EXPECT_EQ(t.KeyOfMax(), 1);
}

//############################################################################
enum CEventType { INSERT = 0, DELETE, EXIST, MAX, KEY_OF_MAX, COUNT_OF_TYPES };

class CEvent {
public:
	CEventType type;
	uint key, value;

	CEvent(int max_key, size_t size) :
		type(static_cast<CEventType>(rand() % COUNT_OF_TYPES)),
		key((rand() % max_key) + 1),
		value((rand() % max_key) + 1) {}

	template<class TTreap>
	int DoIt(TTreap&) const;

	string Log() const;

	bool IsCorrect(std::set<int>&) const;
};

string CEvent::Log() const {//!!!!!!!!!!!!!!!!
	string result = to_string(type) + " ";

	if (type == INSERT) {
		result += to_string(key) + " " + to_string(value);
	}
	if (type == DELETE || type == EXIST) {
		result += to_string(key);
	}

	result += "\n";
	return result;
}

template<class TTreap>
int CEvent::DoIt(TTreap& treap) const {//!!!!!!!!!!!!!!!!
	if (type == INSERT) {
		treap.Insert(key, value);
	}
	else if (type == DELETE) {
		treap.Delete(key);
	}
	else if (type == EXIST) {
		return treap.Exist(key) ? 0 : 1;
	}
	else if (type == MAX) {
		return treap.Max();
	}
	else if (type == KEY_OF_MAX) {
		return treap.KeyOfMax();
	}
	else {
		assert(false);
	}
	return 0;
}

bool CEvent::IsCorrect(std::set<int>& a) const {//!!!!!!!!!!!!!!!!
	if (type == DELETE && a.count(key) == 0)
		return false;
	if ((type == MAX || type == KEY_OF_MAX) && a.empty())
		return false;

	if (type == INSERT)
		a.insert(key);
	if (type == DELETE)
		a.erase(key);

	return true;
}

vector<CEvent> GenEvents(size_t size, int max_key) {
	set<int> s;
	vector<CEvent> events;

	for (size_t i = 0; i < size; ++i) {
		while (true) {
			CEvent new_event(max_key, s.size());
			if (new_event.IsCorrect(s)) {
				events.push_back(new_event);
				break;
			}
		}
	}

	return events;
}

bool test(size_t size, int max_key) {
	vector<CEvent> events = GenEvents(size, max_key);
	CTreap<uint> t1;
	CNative<uint> t2;

	for (size_t i = 0; i < size; ++i) {
		int r1 = events[i].DoIt(t1);
		int r2 = events[i].DoIt(t2);
		if (r1 != r2) {
			EXPECT_EQ(r1, r2);
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

TEST(auto_test, small) {
	for (size_t i = 0; i < 5e4; ++i) {
		if (!test(10, 10)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
		if (i % 10000 == 0) {
			cout << i / 10000 << " of 5\n";
		}
	}
}

TEST(auto_test, big) {
	for (size_t i = 0; i < 30; ++i) {
		if (!test(1000, 1e9)) {
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