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
	lint size = 6;
	CTree t(size);
	EXPECT_EQ(t.BestSize(), size);
	EXPECT_EQ(t.BestPos(), 0);
}

TEST(manual, reserve) {
	lint size = 6;
	CTree t(size);
	t.Reserve(0, 3);
	EXPECT_EQ(t.BestPos(), 3);
	EXPECT_EQ(t.BestSize(), 3);
}

TEST(manual, equial_bests) {
	lint size = 7;
	CTree t(size);
	t.Reserve(3, 5);
	t.Reserve(0, 1);
	EXPECT_EQ(t.BestPos(), 1);
	EXPECT_EQ(t.BestSize(), 2);
}

TEST(manual, free) {
	lint size = 8;
	CTree t(size);
	t.Reserve(3, 5);
	t.Free(3, 4);
	EXPECT_EQ(t.BestPos(), 0);
	EXPECT_EQ(t.BestSize(), 4);
}

TEST(manual, big) {
	lint size = 18;
	CTree t(size);
	t.Reserve(0, 9);
}

TEST(manual, fatal1) {
	CTree t(7);
	t.Free(0, 5);
	t.Reserve(0, 3);
	t.Free(5, 7);
	t.Free(3, 5);
	EXPECT_EQ(t.BestPos(), 3);
	EXPECT_EQ(t.BestSize(), 4);
}

TEST(native, fatal1) {
	CNative t(2);
	t.Reserve(0, 1);
	EXPECT_EQ(t.BestPos(), 1);
	EXPECT_EQ(t.BestSize(), 1);
}

//############################################################################

enum CEventType {BEST_POS = 0, BEST_SIZE, FREE, RESERVE, COUNT_OF_TYPES};

class CEvent{
public:
	CEventType type;
	size_t l, r;

	CEvent (size_t size) {
		type = static_cast<CEventType>(rand() % COUNT_OF_TYPES);
		r = (rand() % size) + 1;
		l = rand() % r;
	}

	template<class TTreap>
	int DoIt(TTreap&) const;

	string Log() const;

	bool IsCorrect() const;
};

string CEvent::Log() const {
	string result = to_string(type) + " ";
	
	if (type == FREE || type == RESERVE) {
		result += to_string(l) + " " + to_string(r);
	}
	
	result += "\n";
	return result;
}

template<class TTree>
int CEvent::DoIt(TTree& tree) const {
	if (type == FREE) {
		tree.Free(l, r);
	}
	else if (type == RESERVE) {
		tree.Reserve(l, r);
	}
	else if (type == BEST_SIZE) {
		return tree.BestSize();
	}
	else if (type == BEST_POS) {
		return tree.BestPos();
	}
	else {
		assert(false);
	}
	
	return 0;
}

bool CEvent::IsCorrect() const {
	return true;
}

vector<CEvent> GenEvents(size_t size, size_t count) {
	vector<CEvent> events;
	
	for (size_t i = 0; i < count; ++i) {
		while (true) {
			CEvent new_event(size);
			if (new_event.IsCorrect()) {
				events.push_back(new_event);
				break;
			}
		}
	}

	return events;
}

bool test(size_t max_size, size_t count) {
	size_t size = (rand() % max_size) + 1;
	vector<CEvent> events = GenEvents(size, count);
	CTree t1(size);
	CNative t2(size);

	for (size_t i = 0; i < count; ++i) {
		int r1 = events[i].DoIt(t1);
		int r2 = events[i].DoIt(t2);
		if (r1 != r2) {
			string log;
			for (size_t j = 0; j <= i; ++j) {
				log += events[j].Log();
			}
			cout << size << "\n";
			cout << log;
			EXPECT_EQ(r1, r2);
			return false;
		}
	}
	return true;
}

TEST (auto_test, all) {
	for (size_t i = 0; i < 1e4; ++i) {
		if(!test(10, 8)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}

	cout << "First test done\n";

	for (size_t i = 0; i < 5e4; ++i) {
		if(!test(20, 15)) {
			cout << i << "\n";
			ASSERT_TRUE(false);
		}
	}

	cout << "Second test done\n";

	for (size_t i = 0; i < 50; ++i) {
		if (!test(1000, 100)) {
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
