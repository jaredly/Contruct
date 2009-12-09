
template<typename T, typename E>
bool exists_element(T start, T end, const E& elem)
{
	return find(start, end, elem) != end;
}

template<typename T, typename E>
typename T::iterator push_back_unique_find(T& container, const E& val)
{
	T::iterator i = find(container.begin(), container.end(), val);

	if (i == container.end()) {
		container.push_back(val);
		return --(container.end());
	}
	else
		return i;
}

template<typename T, typename E>
void push_back_unique(T& container, const E& val)
{
	if (!exists_element(container.begin(), container.end(), val))
		container.push_back(val);
}

template<typename T, typename E>
int find_index(T start, T end, const E& elem)
{
	return find(start, end, elem) - start;
}

template<typename C, typename T>
void set_one(C& container, const T& val)
{
	container.resize(0);
	container.push_back(val);
}

template<typename T>
class vectorx : public vector<T> {
public:
	iterator find(const T& elem) {
		return std::find(begin(), end(), elem);
	}
	int find_index(const T& elem) {
		return find(elem) - begin();
	}
	bool exists_element(const T& elem) {
		return find(elem) != end();
	}
	void push_back_unique(const T& elem) {
		if (!exists_element(elem))
			push_back(elem);
	}

};

template<typename T>
class listx : public list<T> {
public:
	iterator find(const T& elem) {
		return std::find(begin(), end(), elem);
	}
	bool exists_element(const T& elem) {
		return find(elem) != end();
	}
	void push_back_unique(const T& elem) {
		if (!exists_element(elem))
			push_back(elem);
	}

};

template<typename T>
void Clamp(T& v, T l, T m)
{
	if (v < l) v = l;
	if (v > m) v = m;
}

template<typename T>
void LBound(T& v, T l)
{
	if (v < l) v = l;
}

template<typename T>
void UBound(T& v, T u)
{
	if (v > u) v = u;
}

template <typename T, typename M, typename C>
bool members_equal(T start, T end, M C::* mem)
{
	if (start == end) return false;

	const M& first = (*start).*mem;
	++start;

	for ( ; start != end; ++start) {
		if ((*start).*mem != first)
			return false;
	}

	return true;
}

template <typename T, typename M, typename C>
bool ptr_members_equal(T start, T end, M C::* mem)
{
	if (start == end) return false;

	const M& first = (**start).*mem;
	++start;

	for ( ; start != end; ++start) {
		if ((**start).*mem != first)
			return false;
	}

	return true;
}

template <typename T, typename iterMem, typename iterClass, typename memM, typename memC>
bool ptrmem_members_equal(T start, T end, iterMem iterClass::* thru, memM memC::* mem)
{
	if (start == end) return false;

	const memM& first = (**start).*thru->*mem;
	++start;

	for ( ; start != end; ++start) {
		if ((**start).*thru->*mem != first)
			return false;
	}

	return true;
}

class NameComparableCIS {
public:
	CString name;

	bool operator==(const NameComparableCIS& other) const {
		CString l = name;
		CString r = other.name;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
	bool operator==(const CString& other) const {
		CString l = name;
		CString r = other;
		l.MakeLower();
		r.MakeLower();
		return l == r;
	}
	NameComparableCIS() {}
	NameComparableCIS(const CString& _name) : name(_name) {}

	bool operator<(const NameComparableCIS& other) const {
		return name < other.name;
	}
};

inline void RotatePoint(float& x, float& y, float radians)
{
	/*
	cr_float sin_a;
	cr_float cos_a;
	sincosf(rads, &sin_a, &cos_a);

	cr_float x_temp = (x * cos_a) - (y * sin_a);
	y = (y * cos_a) + (x * sin_a);
	x = x_temp;
	*/
	//float sin_a = sin(radians);
	//float cos_a = cos(radians);
	float sin_a, cos_a;
	cr::sincosf(radians, &sin_a, &cos_a);

	float x_temp = (x * cos_a) - (y * sin_a);
	y = (y * cos_a) + (x * sin_a);
	x = x_temp;

	/*
	float dist = sqrt(x*x + y*y);
	float angle = atan2(y, x);
	angle += radians;
	x = cos(angle) * dist;
	y = sin(angle) * dist;
	*/
}

inline void RotatePoint(double& x, double& y, double radians)
{
	double dist = sqrt(x*x + y*y);
	double angle = atan2(y, x);
	angle += radians;

	x = cos(angle) * dist;
	y = sin(angle) * dist;
}