
template<typename T, typename U>
bool exists_element(T begin, T end, const U& elem)
{
	return find(begin, end, elem) != end;
}

template<typename T, typename U>
int find_index(T begin, T end, const U& elem)
{
	return find(begin, end, elem) - begin;
}

template<typename T, typename U>
T& push_back_ref(T container, const U& elem)
{
	container.push_back(elem);
	return container.back();
}

template<typename T, typename U>
void push_back_unique(T container, const U& elem)
{
	if (!exists_element(container.begin(), container.end(), elem))
		container.push_back(elem);
}


class NameComparable {
public:
	CString name;

	bool operator==(const CString& other) const {
		CString l = name;
		CString r = other;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}

	bool operator==(const NameComparable& other) const {
		CString l = name;
		CString r = other.name;

		l.MakeLower();
		r.MakeLower();

		return l == r;
	}
};