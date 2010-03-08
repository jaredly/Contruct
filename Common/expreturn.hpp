
// EXPRETURN CLASS
// A data value in Construct.
#ifndef _EXPRETURN_HPP_
#define _EXPRETURN_HPP_

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#pragma once

// Float precision values
#include <cfloat>


// Data type
enum ExpType 
{
	EXPTYPE_INTEGER,
	EXPTYPE_FLOAT,
	EXPTYPE_STRING,
	EXPTYPE_ARRAY,
	EXPTYPE_BINARY,
	EXPTYPE_VARIABLENAME,
	EXPTYPE_NULL,
	EXPTYPE_FORCE_DWORD = 0x7FFFFFFF
};

// Represents the binary type
#pragma pack(push, 1)		// byte-per-byte packing to fit in 8 byte union

struct ExpBinaryType 
{
	unsigned int	size;
	char*			data;
};

struct ExpArrayType
{
	class ExpStore* pArray;
	int size;

	void Free();

	// Resize this array, copy content
	void Extend(int newsize);
};

struct VariableNameInfo {
	int varIndex;
	class CRunObjType* pOwnerType;
};

#pragma pack(pop)

class ExpBase {
protected:
	// Expression type
	ExpType eType;

	// Data
	union
	{
		__int64			iVal;	// Integer value.  EXPTYPE_INTEGER
		double			fVal;	// Float value.    EXPTYPE_FLOAT
		CString*		str;	// String value.   EXPTYPE_STRING
		ExpBinaryType	bin;	// Binary value - not yet implemented
		ExpArrayType	arr;	// Array value.	   EXPTYPE_ARRAY
		VariableNameInfo vni;	// Variable name type.  For translating family private variables
	} eData;

public:
	inline ExpType Type() const
	{
		return eType;
	}

	inline ExpType& GetEType() 
	{
		return eType;
	}

	inline ExpArrayType& GetArr()
	{
		return eData.arr;
	}

	// Value accessors
	bool operator ==(const ExpBase& e) const
	{
		// eTypes match: compare data
		switch (Type()) {
		case EXPTYPE_INTEGER:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return this->eData.iVal == e.eData.iVal;
			case EXPTYPE_FLOAT:
				return this->eData.iVal == e.eData.fVal;
			};
			break;
		case EXPTYPE_FLOAT:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return this->eData.fVal == e.eData.iVal;
			case EXPTYPE_FLOAT:
				return this->eData.fVal == e.eData.fVal;
			};
			break;
		case EXPTYPE_STRING:
			if (e.Type() == EXPTYPE_STRING)
				return (*(this->eData.str) == *(e.eData.str));
			break;
		}

		return false;
	}

	bool operator !=(const ExpBase& e) const
	{
		return !operator==(e);
	}

	// Cast content to a 32 bit int
	inline int GetInt() const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return (int)eData.iVal;
		case EXPTYPE_FLOAT:
			return (int)eData.fVal;
		case EXPTYPE_STRING:
			return atoi(*(eData.str));
		default:
			return 0;
		}
	}

	inline COLORREF GetColorRef() const
	{
		return (COLORREF)GetInt();
	}

#if defined(RUNTIME)
	inline D3DCOLOR GetD3DColor() const
	{
		COLORREF cref = GetColorRef();
		return cr::color(cref).getD3DCOLOR();
	}

	inline cr::color GetCrColor() const
	{
		return cr::color(GetColorRef());
	}
#endif

	inline bool GetBool() const
	{
		return GetInt() != 0;
	}

	// Cast content to a 64 bit int
	inline __int64 GetInt64() const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return eData.iVal;
		case EXPTYPE_FLOAT:
			return (__int64)eData.fVal;
		case EXPTYPE_STRING:
			return _atoi64(*(eData.str));
		default:
			return 0;
		}
	}

	// Cast content to a 32 bit float
	inline float GetFloat() const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return (float)eData.iVal;
		case EXPTYPE_FLOAT:
			return eData.fVal;
		case EXPTYPE_STRING:
			return atof(*(eData.str));
		default:
			return 0;
		}
	}

	// Cast content to a 64 bit float
	inline double GetDouble() const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return (double)eData.iVal;
		case EXPTYPE_FLOAT:
			return eData.fVal;
		case EXPTYPE_STRING:
			return atof(*(eData.str));
		default:
			return 0;
		}
	}

	// Cast content to a string.  Invokes a copy if already a string - if performance matters,
	// check Type() == EXPTYPE_STRING then use GetStringPtr.
	inline CString GetString() const;
	inline const char* GetConstChar() const;

	inline CString* GetStringPtr() const
	{
		// Construct type checker uses string pointers for variable names
#ifdef CONSTRUCT
		if (eType == EXPTYPE_STRING || eType == EXPTYPE_VARIABLENAME)
#else
		if (eType == EXPTYPE_STRING || eType == EXPTYPE_VARIABLENAME)
#endif
			return eData.str;
		else	// Not valid for non string types!
			return NULL;
	}

	inline ExpStore* GetArray() const
	{
		if (eType == EXPTYPE_ARRAY)
			return eData.arr.pArray;
		else
			return NULL;	// Not valid for non array types!
	}

	inline int GetArraySize() const
	{
		if (eType == EXPTYPE_ARRAY)
			return eData.arr.size;
		else
			return -1;	// Not valid for non array types!
	}
	inline int& GetArrSize()
	{
		return eData.arr.size;
	}

#ifndef EDITTIME
	inline CRunObjType* GetObjectParam(VRuntime* pRuntime) const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return pRuntime->GetTypeFromOID(eData.iVal);
		case EXPTYPE_STRING:
			return pRuntime->GetTypeFromName(*(eData.str));
		default:
			return NULL;	// not a valid type
		}
	}

	inline CRunObject* GetObjectParamFirstInstance(VRuntime* pRuntime) const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return pRuntime->GetFirstInstance(pRuntime->GetTypeFromOID(eData.iVal));
		case EXPTYPE_STRING:
			return pRuntime->GetFirstInstance(pRuntime->GetTypeFromName(*(eData.str)));
		default:
			return NULL;	// not a valid type
		}
	}

	inline CRunObject* GetPairedObjectParam(VRuntime* pRuntime, CRunObject* _This) const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
			return pRuntime->GetPairedObject(pRuntime->GetTypeFromOID(eData.iVal), _This);
		case EXPTYPE_STRING:
			return pRuntime->GetPairedObject(pRuntime->GetTypeFromName(*(eData.str)), _This);
		default:
			return NULL;	// not a valid type
		}
	}

	inline CRunLayer* GetLayerParam(VRuntime* pRuntime, CRunLayout* pLayout) const
	{
		switch (eType) {
		case EXPTYPE_INTEGER:
		case EXPTYPE_FLOAT:
			return pRuntime->GetLayer(pLayout, GetInt() - 1);
		case EXPTYPE_STRING:
			return pRuntime->GetLayer(pLayout, *GetStringPtr());
		}
		return NULL;
	}

	inline int GetVariableIndex(VRuntime* pRuntime, CRunObjType* pType) const
	{
		switch (eType) {
		case EXPTYPE_VARIABLENAME:
			// Indices match: OK, same object
			if (pType == eData.vni.pOwnerType) {
				return eData.vni.varIndex;
			}
			else {			
 				return pRuntime->TranslatePrivateVariableIndex(pType, eData.vni.pOwnerType, eData.vni.varIndex);
			}
		case EXPTYPE_STRING:
			return pRuntime->GetPVIndexFromName(*GetStringPtr(), pType);
			//return find_index(pType->privateVars.begin(), pType->privateVars.end(), *GetStringPtr());
		case EXPTYPE_INTEGER:
			return GetInt();
		default:
			return -1;  //It will cause a crash if this index is accessed.
		}
	}
	
/*	inline int GetVariableIndex(VRuntime* pRuntime, CRunObjType* pType) const
	{
		if (eType == EXPTYPE_VARIABLENAME) {

			// Indices match: OK, same object
			if (pType == eData.vni.pOwnerType) {
				return eData.vni.varIndex;
			}
			else {			
 				return pRuntime->TranslatePrivateVariableIndex(pType, eData.vni.pOwnerType, eData.vni.varIndex);
			}
		}
		else
			return -1;
	}
*/
#endif

	bool operator <(const ExpBase& e) const
	{
		// eTypes match: compare data
		switch (eType) {
		case EXPTYPE_INTEGER:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.iVal < e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.iVal < e.eData.fVal;
			}
			break;
		case EXPTYPE_FLOAT:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.fVal < e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.fVal < e.eData.fVal;
			}
			break;
		}

		return false;
	}

	bool operator >(const ExpBase& e) const
	{
		// eTypes match: compare data
		switch (eType) {
		case EXPTYPE_INTEGER:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.iVal > e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.iVal > e.eData.fVal;
			}
			break;
		case EXPTYPE_FLOAT:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.fVal > e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.fVal > e.eData.fVal;
			}
			break;
		}

		return false;
	}

	bool operator <=(const ExpBase& e) const
	{
		// eTypes match: compare data
		switch (eType) {
		case EXPTYPE_INTEGER:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.iVal <= e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.iVal <= e.eData.fVal;
			}
			break;
		case EXPTYPE_FLOAT:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.fVal <= e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.fVal <= e.eData.fVal;
			}
			break;
		}

		return false;
	}

	bool operator >=(const ExpBase& e) const
	{
		// eTypes match: compare data
		switch (eType) {
		case EXPTYPE_INTEGER:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.iVal >= e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.iVal >= e.eData.fVal;
			}
			break;
		case EXPTYPE_FLOAT:
			switch (e.Type()) {
			case EXPTYPE_INTEGER:
				return eData.fVal >= e.eData.iVal;
			case EXPTYPE_FLOAT:
				return eData.fVal >= e.eData.fVal;
			}
			break;
		}

		return false;
	}

	friend class ExpReturn;
	friend class ExpStore;
	friend class bin;
};

// Prototypes (both cross reference)
class ExpReturn;

class ExpStore : public ExpBase {
public:
	inline ExpStore();
	inline ExpStore(int x);
	inline ExpStore(__int64 x);
	inline ExpStore(float x);
	inline ExpStore(double x);
	inline ExpStore(const char* str);
	inline void Free();
	inline ~ExpStore();
	inline ExpStore(const ExpStore& r);
	inline ExpStore(const ExpReturn& r);
	inline void operator=(const ExpStore& r);
	inline void operator=(const ExpReturn& r);
	inline void operator=(__int64 x);
	inline void operator=(int x);
	inline void operator=(unsigned int x);
	inline void operator=(float x);
	inline void operator=(double x);
	inline void operator=(const char* str);
	inline ExpStore& operator+=(const ExpReturn& r);
	inline ExpStore& operator+ (const ExpReturn& r);
	inline ExpStore& operator+= (const ExpStore& r);
	inline ExpStore& operator+ (const ExpStore& r);
	inline ExpStore& operator-=(const ExpReturn& r);
	inline ExpStore& operator- (const ExpReturn& r);
	inline ExpStore& operator-= (const ExpStore& r);
	inline ExpStore& operator- (const ExpStore& r);
};

// ExpReturn is the class used by the runtime during expression evaluation.  It is not safe to store
// or instantiate an ExpReturn, since it uses references to its held data.  Use an ExpStore for that.
// It is essentially a reference semantics variant.
class ExpReturn : public ExpBase
{
	// Prevent user instantiation.
#ifdef EDITTIME
public:
#else
private:
#endif
	ExpReturn() {}

	// Bit-for-bit copy ctor should ExpReturn be held in a vector
public:
	ExpReturn(const ExpReturn& r) {
		eType = r.eType;
		eData.iVal = r.eData.iVal;
	}

	inline long operator=(__int64 x)
	{
		eType = EXPTYPE_INTEGER;
		eData.iVal = x;
		return 0;
	}

	inline long operator=(int x)
	{
		eType = EXPTYPE_INTEGER;
		eData.iVal = x;
		return 0;
	}

	inline long operator=(unsigned __int64 x)
	{
		eType = EXPTYPE_INTEGER;
		eData.iVal = x;
		return 0;
	}

	inline long operator=(unsigned int x)
	{
		eType = EXPTYPE_INTEGER;
		eData.iVal = x;
		return 0;
	}

	inline long operator=(double x)
	{
		eType = EXPTYPE_FLOAT;
		eData.fVal = x;
		return 0;
	}

	inline long operator=(float x)
	{
		eType = EXPTYPE_FLOAT;
		eData.fVal = x;
		return 0;
	}

#ifndef EDITTIME
	inline long ReturnString(VRuntime* pRuntime, const char* str)
	{
		CString& rStr = pRuntime->tempStrings[pRuntime->curtempstr++];
		pRuntime->AssignCString(&rStr, str);
		eType = EXPTYPE_STRING;
		eData.str = &rStr;
		pRuntime->curtempstr %= 32;
		return 0;
	}

	inline CString* GetReturnString(VRuntime* pRuntime)
	{
		CString& rStr = pRuntime->tempStrings[pRuntime->curtempstr++];
		eType = EXPTYPE_STRING;
		eData.str = &rStr;
		pRuntime->curtempstr %= 32;
		return &rStr;
	}
#endif
	// Bitwise copy!
	inline long operator=(const ExpReturn& r)
	{
		eType = r.Type();
		eData.iVal = r.eData.iVal;
		return 0;
	}

	// Set from an ExpStore: use runtime to copy string, since ExpReturn has permission to modify
	// from runtime heap space
#ifndef EDITTIME
	inline long ReturnCustom(VRuntime* pRuntime, const ExpStore& r)
	{
		if (r.Type() == EXPTYPE_STRING)
			ReturnString(pRuntime, (const char*)*(r.eData.str));
		else {
			// Bitwise copy
			eType = r.Type();
			eData.iVal = r.eData.iVal;
		}

		return 0;
	}
	inline long ReturnArray(ExpStore* ptr, int count)
	{
		eType = EXPTYPE_ARRAY;
		eData.arr.pArray = ptr;
		eData.arr.size = count;
		return 0;
	}
#endif
	// Construct runtime accessibility - these classes do direct ExpReturn modifications
	friend class CRuntime;
	friend vector<ExpReturn>;
	friend class CExpression;
	friend class ExpPart;
	friend class SystemObject;
	friend struct condition;
	friend struct action;

	friend class CollapseConstantTreeObj;
	friend class ExpInteger;
	friend class ExpFloat;
	friend class ExpString;
	friend class ExpBinary;
	friend class ExpIdent;
	friend class ExpAdd;
	friend class ExpSubtract;
	friend class ExpMultiply;
	friend class ExpDivide;
	friend class ExpMod;
	friend class ExpPower;
	friend class ExpEqual;
	friend class ExpNotEqual;
	friend class ExpLess;
	friend class ExpLessEqual;
	friend class ExpGreater;
	friend class ExpGreaterEqual;
	friend class ExpAnd;
	friend class ExpOr;
	friend class ExpConditional;
	friend class ExpDot;
	friend class ExpSin;
	friend class ExpAsin;
	friend class ExpCos;
	friend class ExpAcos;
	friend class ExpTan;
	friend class ExpAtan;
	friend class ExpSqrt;
	friend class ExpFuncInt;
	friend class ExpFuncFloat;
	friend class ExpFuncStr;
	friend class ExpAbs;
	friend class ExpExp;
	friend class ExpLn;
	friend class ExpLog10;
	friend class ExpRound;
	friend class ExpCeil;
	friend class ExpRandom;
	friend class ExpLen;
	friend class ExpArray;
	friend class ExpAt;
	friend class ExpVariableName;
};

typedef const ExpReturn* LPVAL;

// Construct to a null type
ExpStore::ExpStore() { eType = EXPTYPE_NULL; }
ExpStore::ExpStore(int x) { eType = EXPTYPE_INTEGER; eData.iVal = x; }
ExpStore::ExpStore(__int64 x) { eType = EXPTYPE_INTEGER; eData.iVal = x; }
ExpStore::ExpStore(float x) { eType = EXPTYPE_FLOAT; eData.fVal = x; }
ExpStore::ExpStore(double x) { eType = EXPTYPE_FLOAT; eData.fVal = x; }
ExpStore::ExpStore(const char* str)
{
	eType = EXPTYPE_STRING;
	eData.str = new CString;
	*(eData.str) = str;
}

void ExpStore::Free() {
	switch (Type()) {
	case EXPTYPE_STRING:
		delete eData.str;
		break;
	case EXPTYPE_ARRAY:
		eData.arr.Free();
		break;
	}
	eType = EXPTYPE_NULL;
}

// Destructor
ExpStore::~ExpStore() { Free(); }

ExpStore::ExpStore(const ExpStore& r) {
	switch (r.Type()) {
	case EXPTYPE_STRING:
		eData.str = new CString;
		*(eData.str) = (const char*)*(r.GetStringPtr());
		break;
	case EXPTYPE_ARRAY:
		// TODO: value-based array copy
		break;
	default:
		// Other types can simply copy the eData union content
		eData.iVal = r.eData.iVal;
	}

	eType = r.Type();
}

ExpStore::ExpStore(const ExpReturn& r) {
	switch (r.Type()) {
	case EXPTYPE_STRING:
		eData.str = new CString;
		*(eData.str) = (const char*)*(r.GetStringPtr());
		break;
	case EXPTYPE_ARRAY:
		// TODO: value-based array copy
		break;
	default:
		// Other types can simply copy the eData union content
		eData.iVal = r.eData.iVal;
	}

	eType = r.Type();
}

void ExpStore::operator=(const ExpStore& r)
{
	Free();

	switch (r.Type()) {
	case EXPTYPE_STRING:
		eData.str = new CString;
		*(eData.str) = (const char*)*(r.GetStringPtr());
		break;
	case EXPTYPE_ARRAY:
		// TODO: value-based array copy
		break;
	default:
		// Other types can simply copy the eData union content
		eData.iVal = r.eData.iVal;
	}

	eType = r.Type();
}

void ExpStore::operator=(const ExpReturn& r)
{
	operator=((const ExpStore&)r);
}

void ExpStore::operator=(__int64 x)
{
	Free();
	eType = EXPTYPE_INTEGER;
	eData.iVal = x;
}

void ExpStore::operator=(int x)
{
	operator=((__int64)x);
}

void ExpStore::operator=(unsigned int x)
{
	operator=((__int64)x);
}

void ExpStore::operator=(float x)
{
	Free();
	eType = EXPTYPE_FLOAT;
	eData.fVal = x;
}

void ExpStore::operator=(double x)
{
	Free();
	eType = EXPTYPE_FLOAT;
	eData.fVal = x;
}

void ExpStore::operator=(const char* str)
{
	Free();
	eType = EXPTYPE_STRING;
	eData.str = new CString;
	*(eData.str) = str;
}

ExpStore& ExpStore::operator +=(const ExpStore& r)
{
	switch (Type()) {
	case EXPTYPE_INTEGER:
		// If the value being added is a float, convert this type to a float
		if (r.Type() == EXPTYPE_FLOAT) {
			eData.fVal = (double)eData.iVal + r.eData.fVal;
			eType = EXPTYPE_FLOAT;
		}
		else
			eData.iVal += r.GetInt64();
		break;
	case EXPTYPE_FLOAT:
		eData.fVal += r.GetDouble();
		break;
	case EXPTYPE_STRING:
		*(eData.str) += r.GetString();
		break;
	}

	return *this;
}

ExpStore& ExpStore::operator+=(const ExpReturn& r)
{
	return operator+=((const ExpStore&)r);
}

ExpStore& ExpStore::operator+(const ExpStore& r)
{
	return operator+=(r);
}

ExpStore& ExpStore::operator+(const ExpReturn& r)
{
	return operator+=((const ExpStore&)r);
}

ExpStore& ExpStore::operator -=(const ExpStore& r)
{
	switch (Type()) {
	case EXPTYPE_INTEGER:
		// If the value being subtracted is a float, convert this type to a float
		if (r.Type() == EXPTYPE_FLOAT) {
			eData.fVal = (double)eData.iVal - r.eData.fVal;
			eType = EXPTYPE_FLOAT;
		}
		else
			eData.iVal -= r.GetInt64();
		break;
	case EXPTYPE_FLOAT:
		eData.fVal -= r.GetDouble();
		break;
	}

	return *this;
}

ExpStore& ExpStore::operator-=(const ExpReturn& r)
{
	return operator-=((const ExpStore&)r);
}

ExpStore& ExpStore::operator-(const ExpStore& r)
{
	return operator-=(r);
}

ExpStore& ExpStore::operator-(const ExpReturn& r)
{
	return operator-=((const ExpStore&)r);
}

CString ExpBase::GetString() const
{
	switch (eType) {
	case EXPTYPE_INTEGER:
		{
			char strbuf[32];
			_i64toa(eData.iVal, strbuf, 10);
			return strbuf;
		}
	case EXPTYPE_FLOAT:
		{
			char strbuf[32];
			_gcvt(eData.fVal, DBL_DIG, strbuf);
			CString str = strbuf;
			if (str.Right(1) == ".")
				str.Delete(str.GetLength() - 1);
			return str;
		}
	case EXPTYPE_STRING:
		return (const char*)*(eData.str);
	case EXPTYPE_ARRAY:
		{
		// Build an expression style array string eg {1, 3, 5, 6}
		CString s = "{";

		ExpStore* ptr = eData.arr.pArray;
		ExpStore* end = eData.arr.pArray + eData.arr.size;
		ExpStore* last = end - 1;

		for ( ; ptr != end; ptr++) {

			bool isString = (ptr->Type() == EXPTYPE_STRING);

			if (isString)
				s += '"';

			s += (const char*)(ptr->GetString());

			if (isString)
				s += '"';

			if (ptr != last)
				s += ", ";
		}

		s += "}";

		return s;
		}
	default:
		return "";
	}
}

inline void ExpArrayType::Free()
{
	// Iterate and free
	ExpStore* ptr = pArray;
	ExpStore* end = pArray + size;

	for ( ; ptr != end; ptr++)
		ptr->Free();

	// Delete my data
	delete[] pArray;
}

// Resize this array, copy content
inline void ExpArrayType::Extend(int newsize)
{
	if (newsize <= 0 || newsize == size) return;

	// Store old members
	ExpStore* oldPtr = pArray;
	int oldsize = size;

	// Allocate a newly sized array
	pArray = new ExpStore[newsize];

	// Get ptrs to start copying
	ExpStore* fromPtr = oldPtr;
	ExpStore* toPtr = pArray;
	ExpStore* end = oldPtr + size;

	// Check if copying less than 'size' elements
	if (newsize < size)
		end = oldPtr + newsize;

	// Copy (operator=)
	for ( ; fromPtr != end; fromPtr++, toPtr++)
		*toPtr = *fromPtr;

	// Other elements to cover?  Insert float 0.0 elements
	if (newsize > size) {
		ExpStore* newend = pArray + newsize;

		for ( ; toPtr != newend; toPtr++)
			*toPtr = 0.0;
	}

	// Done
	size = newsize;
	delete[] oldPtr;
}


#endif