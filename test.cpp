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

TEST(manual, smth) {
	CTreap<int> t;
	t.Insert(3);
	t.Insert(5);
	t.Insert(4);
	t.Delete(4);
	EXPECT_EQ(t.Kth(0), 3);
	EXPECT_EQ(t.Kth(1), 5);
}

TEST(manual, big_value) {
	CTreap<int> t;
	t.Insert(1e9);
	t.Insert(-1e9);
	t.Insert(123456);
	t.Delete(-1e9);
	EXPECT_EQ(t.Kth(0), 123456);
	EXPECT_EQ(t.Kth(1), 1e9);
	EXPECT_TRUE(t.Exist(123456));
	EXPECT_FALSE(t.Exist(-1e9));
}

//############################################################################

enum CEventType {INSERT = 0, DELETE, KTH, EXIST, COUNT_OF_TYPES};

class CEvent{
public:
	CEventType type;
	int i, key;

	CEvent (int max_key, size_t size):
		type (static_cast<CEventType>(rand()%COUNT_OF_TYPES)),
		i (rand()%max(size, (size_t)1)),
		key (rand()%max_key) {}

	template<class TTreap>
	int DoIt(TTreap&) const;

	string Log() const;

	bool IsCorrect(std::set<int>&) const;
};

string CEvent::Log() const {
	string result = to_string(type) + " ";

	if (type == KTH) {
		result += to_string(i);	
	}
	else {
		result += to_string(key);
	}
	
	result += "\n";
	return result;
}

template<class TTreap>
int CEvent::DoIt(TTreap& treap) const {
	if (type == INSERT) {
		treap.Insert(key);
	}
	else if (type == DELETE) {
		treap.Delete(key);
	}
	else if (type == EXIST) {
		return treap.Exist(key) ? 0 : 1;
	}
	else if (type == KTH) {
		return treap.Kth(i);
	}
	else {
		assert(false);
	}
	return 0;
}

bool CEvent::IsCorrect(std::set<int>& a) const {
	if (type == KTH && a.size() == 0)
		return false;
	if (type == DELETE && a.count(key) == 0)
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
	for (size_t i = 0; i < 2e5; ++i) {
		if(!test(10, 10)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
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
/*
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();
	system("pause");
	return r;
}*/