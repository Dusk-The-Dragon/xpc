#include <vector>
#include <algorithm>
template <typename T>
class List : public std::vector<T> {
public:
	using std::vector<T>::vector;

	bool contains(const T& val) const {
		return std::find(this->begin(), this->end(), val) != this->end();
	}
};