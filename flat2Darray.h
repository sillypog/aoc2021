#pragma once

#include <vector>

template <class T>
class Flat2DArray {
private:
	std::vector<T> values;
	int width;

public:
	Flat2DArray() {}
	Flat2DArray(int size, T defaultValue) : values(size, defaultValue) {}

	void setWidth(int w) {
		width = w;
	}

	void insert(T v) {
		values.push_back(v);
	}

	int size() const {
		return values.size();
	}

	const T& operator[](int i) const {
		return values[i];
	}

	T& operator[](int i) {
		return values[i];
	}

	int left(int i) const {
		if (i < 0) {
			return -1;
		}
		// Check that this won't take us past the left edge
		return (i % width == 0) ? -1 : i - 1;
	}

	int right(int i) const{
		if (i < 0) {
			return -1;
		}
		// Check that this won't take us past the right edge
		return (i % width == (width - 1)) ? -1 : i + 1;
	}

	int up(int i) const{
		if (i < 0) {
			return -1;
		}
		// Check that this won't take us past the top edge
		return (i < width - 1) ? -1 : i - width;
	}

	int down (int i) const{
		if (i < 0) {
			return -1;
		}
		// Check that this won't take us past the bottom edge
		return (i > values.size() - width) ? -1 : i + width;
	}

	friend std::ostream& operator<<(std::ostream& os, const Flat2DArray<T>& instance);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Flat2DArray<T>& instance) {
	for (int i = 0; i < instance.values.size(); i++) {
		if (i % instance.width == 0) {
			os << std::endl;
		}
		os << instance.values[i];
	}
	os << std::endl;
}

