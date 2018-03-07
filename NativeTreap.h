#pragma once
#include <vector>
#include <algorithm>

class CNative {
private:
	std::vector<bool> arr;
public:
	CNative(lint size) { arr = std::vector<bool>(size, true);  }

	void Free(lint l, lint r) {
		for (lint i = l; i < r; ++i) {
			arr[i] = true;
		}
	}

	void Reserve(lint l, lint r) {
		for (lint i = l; i < r; ++i) {
			arr[i] = false;
		}
	}

	lint BestPos() {
		lint best = 0;
		lint now = 0;
		lint pos = 0;
		for (lint i = 0; i < arr.size(); ++i) {
			if (arr[i]) {
				now++;
			}
			else {
				now = 0;
			}

			if (now > best) {
				best = now;
				pos = i;
			}
		}

		return best == 0 ? 0 : pos + 1 - best;
	}

	lint BestSize() {
		lint best = 0;
		lint now = 0;
		lint pos = 0;
		for (lint i = 0; i < arr.size(); ++i) {
			if (arr[i]) {
				now++;
			}
			else {
				now = 0;
			}

			if (now > best) {
				best = now;
				pos = i;
			}
		}

		return best;
	}
};