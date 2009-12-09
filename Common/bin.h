
//////////////////////////////////////////////////
// Binary data class							//
// Copyright (c) 2006 Ashley Gullen				//
// www.scirra.com								//
//////////////////////////////////////////////////
// Vector-style dynamically resizing binary data buffer

// Prevent multiple inclusion
#ifndef __BIN_H__
#define __BIN_H__

#include <stdlib.h>
#include <string.h>

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif//BYTE
#ifndef NULL
#define NULL 0
#endif//NULL

#ifndef max
#define max(a , b) ((a) > (b) ? (a) : (b))
#endif//max

class bin_allocator {
public:
	virtual BYTE* allocNew(unsigned int _size) = 0;
	virtual void allocDelete(void* ptr) = 0;
};

class bin {
public:
	// ctors
	bin() { datasize = capacity = readcursor = 0; data = NULL; }
	bin(size_t _size) { bin(); bin::allocate(_size); }
	bin(const void* p, size_t _size) { bin(); bin::allocate(_size); memcpy(data, p, _size); }

	// dtor
	~bin() { bin::free(); }

	// string subclass for generic string I/O
	class string {
	public:
		string(const char* _str) { pstr = _str; len = strlen(_str); }
		string(const char* _str, size_t _len) { pstr = _str; len = _len; }
		const char* pstr;
		size_t len;
	};

	// binary subclass for general binary I/O
	class binary {
	public:
		binary(void* _ptr, size_t _size) {
			ptr = _ptr;
			size = _size;
		}
		void* ptr;
		size_t size;
	};

	bin_allocator* allocator;

	bin(bin_allocator* _allocator) {
		datasize = capacity = readcursor = 0; data = NULL;
		allocator = _allocator;
	}
	bin(bin_allocator& _allocator) {
		datasize = capacity = readcursor = 0; data = NULL;
		allocator = &_allocator;
	}

	// create new buffer (useAll sets datasize to capacity)
	virtual bool allocate(int _size, bool useAll = false) {
		if (data) bin::free();
		if (_size < 0) return false;
		//data = new BYTE[_size];
		data = allocator->allocNew(_size);
		if (data) capacity = _size;
		if (useAll) datasize = capacity;
		return valid();		
	}
	// resize buffer (useAll sets datasize to capacity)
	virtual bool realloc(size_t _newsize, bool useAll = false) {
		if (!valid()) return allocate(_newsize);
		if (_newsize < 0) return false;
		if (_newsize == capacity) return true;
		BYTE* _newData = (BYTE*)allocator->allocNew(_newsize);
		//BYTE* _newData = new BYTE[_newsize];
		if (!_newData) return false;
		if (_newsize < datasize) {
			memcpy(_newData, data, _newsize);
			datasize = _newsize;
		}
		else memcpy(_newData, data, datasize);
		allocator->allocDelete(data);
		//delete [] data;
		data = _newData;
		capacity = _newsize;
		if (useAll) datasize = capacity;
		return true;
	}
	// deallocate buffer
	virtual void free()
	{
		if (data) {
			//delete [] data;
			allocator->allocDelete(data);
			data = NULL;
			datasize = 0;
			capacity = 0;
			readcursor = 0;
		}
	}

	// reset datasize
	void clear()
	{
		datasize = 0;
	}
	// set datasize
	void resize(size_t _size)
	{
		if (_size <= datasize)
			datasize = _size;
		else
			realloc(_size, true);
	}
	// reset read cursor
	void reset()
	{
		readcursor = 0;
	}
	// set read cursor
	void setcursor(size_t _pos)
	{
		readcursor = max(0, _pos);
	}
	// advance read cursor
	void advance(size_t amt)
	{
		readcursor += amt;
	}
	// fill buffer with zeroes
	void zero()
	{
		if (data) memset(data, 0, capacity);
		datasize = capacity;
	}
	// get size of data written
	size_t size() const
	{
		return datasize;
	}
	// get buffer
	void* ptr() const
	{
		return data;
	}
	void* curptr() const
	{
		return &(data[readcursor]);
	}
	// get capacity
	size_t maxsize() const
	{
		return capacity;
	}
	// attach to a binary buffer
	void attach(void* _data, int _size)
	{
		data = (BYTE*)_data;
		datasize = capacity = _size;
		reset();
		loading = true;
	}
	// detach from binary buffer (prevents dtor free)
	void detach()
	{
		datasize = capacity = readcursor = 0; data = NULL;
	}
	// buffer allocated
	bool valid() const
	{
		return data != NULL;
	}
	// fill buffer
	void fill(int c)
	{
		if (data) memset(data, c, datasize);
	}
	// access byte
	_inline BYTE& at(unsigned int byte) const
	{
		return data[byte];
	}
	// access byte
	_inline BYTE& operator[](unsigned int byte) const
	{
		return at(byte);
	}
	// get buffer as string
	char* str() const
	{
		return (char*)ptr();
	}
	// get buffer as string from certain byte
	char* str(unsigned int _at) const
	{
		return (char*)&(data[_at]);
	}
	template<class T> void put_copy(const T d, unsigned int byte)
	{
		*((T*)&(data[byte])) = d;
	}
	template<class T> void put(const T& pd, unsigned int byte)
	{
		*((T*)&(data[byte])) = pd;
	}
	template<class T> void put_ref(const T* pd, unsigned int byte)
	{
		*((T*)&(data[byte])) = *pd;
	}
	template<class T> void put_bits(const T& d, unsigned int byte)
	{
		memcpy(&(data[byte]), &d, sizeof(T));
	}
	void putstr(const void* ptr, unsigned int byte, bool withnull = false)
	{
		if (withnull) strcpy((char*)&(data[byte]),(const char*)ptr);
		else memcpy(&(data[byte]), ptr, strlen((const char*)ptr));
	}
	void putarr(const void* ptr, size_t _size, unsigned int byte)
	{
		memcpy(&(data[byte]), ptr, _size);
	}
	template<class T> void get(T*& pd, unsigned int byte = 0) const
	{
		get_ref(pd, byte);
	}
	template<class T> void get_copy(T& d, unsigned int byte) const
	{
		d = *((T*)&(data[byte]));
	}
	template<class T> void get_ref(T*& pd, unsigned int byte = 0) const
	{
		pd = (T*)&(data[byte]);
	}
	void copyto(void* p, size_t _size, unsigned int byte = 0) const
	{
		if (valid())
			memcpy(p, &(data[byte]), _size);
	}
	// get length needed for hex string output
	size_t hexlen() const
	{
		return datasize * 3;
	}
	// write hex string to std::string
	void hexout(std::string& _str) const
	{
		_str = "";
		_str.reserve(hexlen());
		char ibuf[3];
		for (int i = 0; i < datasize; i++) {
			_itoa(data[i], ibuf, 16);
			if (ibuf[1] == NULL) { _str += '0'; _str += ibuf[0]; }
			else _str += ibuf;
			if (i < datasize - 1) _str += " ";
		}
	}
	// write hex string to string
	void hexout(char* _str) const
	{
		char ibuf[3];
		for (int i = 0; i < datasize; i++) {
			_itoa(data[i], ibuf, 16);
			if (ibuf[1] == NULL) { _str[i*3] = '0'; _str[i*3+1] = ibuf[0]; }
			else { _str[i*3] = ibuf[0]; _str[i*3+1] = ibuf[1]; }
			if (i < datasize - 1) _str[i * 3 + 2] = ' ';
			else _str[i * 3 + 2] = NULL;
		}
	}
	//////////////////////////////////////
	// WRITE OPERATIONS
	bin& operator=(const bin& b)
	{
		bin::free();
		bin::allocate(b.size());
		bin::append(b.ptr(), b.size());
		return *this;
	}
	bin& operator<<(bool x)
	{
		extend(sizeof(bool));
		data[datasize] = x;
		datasize += sizeof(bool);
		return *this;
	}
	bin& operator<<(char x)
	{
		extend(sizeof(char));
		data[datasize] = x;
		datasize += sizeof(char);
		return *this;
	}
	bin& operator<<(const char* x)
	{
		size_t len = strlen(x) + 1;
		extend(len + sizeof(size_t));
		operator<<(len);
		append(x, len);
		operator<<('\0');
		return *this;
	}
	bin& operator<<(unsigned char x)
	{
		extend(sizeof(unsigned char));
		data[datasize] = x;
		datasize += sizeof(unsigned char);
		return *this;
	}
	bin& operator<<(short x)
	{
		extend(sizeof(short));
		*((short*)&(data[datasize])) = x;
		datasize += sizeof(short);
		return *this;
	}
	bin& operator<<(unsigned short x)
	{
		extend(sizeof(unsigned short));
		*((unsigned short*)&(data[datasize])) = x;
		datasize += sizeof(unsigned short);
		return *this;
	}
	bin& operator<<(int x)
	{
		extend(sizeof(int));
		*((int*)&(data[datasize])) = x;
		datasize += sizeof(int);
		return *this;
	}
	bin& operator<<(unsigned int x)
	{
		extend(sizeof(unsigned int));
		*((unsigned int*)&(data[datasize])) = x;
		datasize += sizeof(unsigned int);
		return *this;
	}
	bin& operator<<(long x)
	{
		extend(sizeof(long));
		*((long*)&(data[datasize])) = x;
		datasize += sizeof(long);
		return *this;
	}
	bin& operator<<(unsigned long x)
	{
		extend(sizeof(unsigned long));
		*((unsigned long*)&(data[datasize])) = x;
		datasize += sizeof(unsigned long);
		return *this;
	}
	bin& operator<<(__int64 x)
	{
		extend(sizeof(__int64));
		*((__int64*)&(data[datasize])) = x;
		datasize += sizeof(__int64);
		return *this;
	}
	bin& operator<<(unsigned __int64 x)
	{
		extend(sizeof(unsigned __int64));
		*((unsigned __int64*)&(data[datasize])) = x;
		datasize += sizeof(unsigned __int64);
		return *this;
	}
	bin& operator<<(double x)
	{
		extend(sizeof(double));
		*((double*)&(data[datasize])) = x;
		datasize += sizeof(double);
		return *this;
	}
	bin& operator<<(float x)
	{
		extend(sizeof(float));
		*((float*)&(data[datasize])) = x;
		datasize += sizeof(float);
		return *this;
	}
	void append(const void* buf, int len)
	{
		extend(len);
		memcpy(&(data[datasize]), buf, len);
		datasize += len;
	}
	bin& operator<<(const bin& b)
	{
		extend(b.size() + sizeof(size_t));
		operator<<(b.size());
		append(b.ptr(), b.size());
		return *this;
	}
	bin& operator<<(const bin::binary& b)
	{
		extend(b.size + sizeof(size_t));
		operator<<(b.size);
		append(b.ptr, b.size);
		return *this;
	}
	bin& operator<<(const bin::string& s)
	{
		extend(s.len + 1 + sizeof(size_t));
		operator<<(s.len + 1);
		append(s.pstr, s.len);
		operator<<('\0');
		return *this;
	}
	bin& operator<<(const CString& s)
	{
		extend(s.GetLength() + 1 + sizeof(size_t));
		operator<<((size_t)s.GetLength() + 1);
		append((const char*)s, s.GetLength());
		operator<<('\0');
		return *this;
	}
#ifndef CONSTRUCT
	bin& operator<<(const cr::color& c)
	{
		(*this) << c.a << c.r << c.g << c.b;
		return (*this);
	}
#endif
#ifdef BIN_CXIMAGE
	void setimageformat(unsigned long format)
	{
		imgformat = format;
	}
	/*
	// Broken - moans about Encode() overloads :P

	bin& operator<<(const CxImage& im)
	{
		BYTE* temp;
		long len;
		im.Encode(temp, len, imgformat);
		operator<<(len);
		append(temp, len);
		delete[] temp;
	}
	*/
#endif//CXIMAGE

	//////////////////////////////////////
	// READ OPERATIONS
	bin& operator>>(bool& x)
	{
		x = *((bool*)&(data[readcursor]));
		readcursor += sizeof(bool);
		return *this;
	}
	bin& operator>>(char& x)
	{
		x = *((char*)&(data[readcursor]));
		readcursor += sizeof(char);
		return *this;
	}
	bin& operator>>(unsigned char& x)
	{
		x = *((unsigned char*)&(data[readcursor]));
		readcursor += sizeof(unsigned char);
		return *this;
	}
	bin& operator>>(short& x)
	{
		x = *((short*)&(data[readcursor]));
		readcursor += sizeof(short);
		return *this;
	}
	bin& operator>>(unsigned short& x)
	{
		x = *((unsigned short*)&(data[readcursor]));
		readcursor += sizeof(unsigned short);
		return *this;
	}
	bin& operator>>(int& x)
	{
		x = *((int*)&(data[readcursor]));
		readcursor += sizeof(int);
		return *this;
	}
	template<typename T>
	T read() {
		T& toRet = *(T*)&(data[readcursor]);
		readcursor += sizeof(T);
		return toRet;
	}
	bin& operator>>(unsigned int& x)
	{
		x = *((unsigned int*)&(data[readcursor]));
		readcursor += sizeof(unsigned int);
		return *this;
	}
	bin& operator>>(long& x)
	{
		x = *((long*)&(data[readcursor]));
		readcursor += sizeof(long);
		return *this;
	}
	bin& operator>>(unsigned long& x)
	{
		x = *((unsigned long*)&(data[readcursor]));
		readcursor += sizeof(unsigned long);
		return *this;
	}
	bin& operator>>(__int64& x)
	{
		x = *((__int64*)&(data[readcursor]));
		readcursor += sizeof(__int64);
		return *this;
	}
	bin& operator>>(unsigned __int64& x)
	{
		x = *((unsigned __int64*)&(data[readcursor]));
		readcursor += sizeof(unsigned __int64);
		return *this;
	}
	bin& operator>>(float& x)
	{
		x = *((float*)&(data[readcursor]));
		readcursor += sizeof(float);
		return *this;
	}
	bin& operator>>(double& x)
	{
		x = *((double*)&(data[readcursor]));
		readcursor += sizeof(double);
		return *this;
	}
	bin& operator>>(char*& x)
	{
		size_t len;
		operator>>(len);
		strcpy(x, (char*)&(data[readcursor]));
		readcursor += len;
		return *this;
	}
	bin& operator>>(CString& s)
	{
		size_t len;
		operator>>(len);
		s = (const char*)&(data[readcursor]);
		readcursor += len;
		return *this;
	}
#ifndef CONSTRUCT
	bin& operator>>(cr::color& c)
	{
		(*this) >> c.a >> c.r >> c.g >> c.b;
		return *this;
	}
#endif
	bin& operator>>(bin& b)
	{
		size_t len;
		operator>>(len);
		b.free();
		b.allocate(len);
		b.append(&(data[readcursor]), len);
		readcursor += len;
		return *this;
	}

	// ExpReturn serialization
#ifndef EDITTIME
	bin& operator<<(const ExpBase& x)
	{
		// Save type
		operator<<((int)x.Type());

		switch (x.Type()) {
		case EXPTYPE_INTEGER:
			operator<<(x.GetInt64());
			break;
		case EXPTYPE_FLOAT:
			operator<<(x.GetDouble());
			break;
		case EXPTYPE_STRING:
			operator<<(x.GetString());
			break;
		case EXPTYPE_ARRAY:
			// Number of elements
			operator<<(x.GetArraySize());

			// Iterate all elements
			const ExpBase* i = x.GetArray();
			const ExpBase* end = x.GetArray() + x.GetArraySize();

			for ( ; i != end; i++)
				operator<<(*i);			
			
			break;
		}

		return *this;
	}
	bin& operator<<(const ExpReturn& x)
	{
		return operator<<((const ExpBase&)x);
	}
	bin& operator<<(const ExpStore& x)
	{
		return operator<<((const ExpBase&)x);
	}

	// Only support loading to ExpStore; ExpReturns will leak any string/memory content
	bin& operator>>(ExpStore& x)
	{
		int type;
		operator>>(type);

		switch ((ExpType)type) {
		case EXPTYPE_INTEGER:
			{
			__int64 v;
			operator>>(v);
			x = v;
			break;
			}
		case EXPTYPE_FLOAT:
			{
			double v;
			operator>>(v);
			x = v;
			break;
			}
		case EXPTYPE_STRING:
			{
			CString s;
			operator>>(s);
			x = s;
			break;
			}
		case EXPTYPE_ARRAY:
			{
			int count;
			operator>>(count);

			x.eData.arr.Extend(count);

			ExpBase* i = x.GetArray();
			ExpBase* end = i + x.GetArraySize();

			for ( ; i != end; i++)
				operator>>((ExpStore&)*i);			

			}
			break;
		}

		return *this;
	}

	bin& operator>>(OBJHEADER& info)
	{
		return (*this) >> info.x >> info.y >> info.w >> info.h >> info.angle >> info.uid
			>> info.filter_deprecated >> info.global >> info.visible >> info.displayangle
			>> info.isMirrored >> info.isFlipped >> info.pInfo->filter;
	}

	bin& operator<<(const OBJHEADER& info)
	{
		return (*this) << info.x << info.y << info.w << info.h << info.angle << info.uid
			<< info.filter_deprecated << info.global << info.visible << info.displayangle
			<< info.isMirrored << info.isFlipped << info.pInfo->filter;
	}

	bin& operator>>(AnimInfo& ai)
	{
		return (*this) >> ai.handle >> ai.playing >> ai.angle >> ai.counter >> ai.curFrame
			>> ai.autoFlip >> ai.autoMirror >> ai.autoRotations >> ai.speed >> ai.repeat
			>> ai.repeat_to >> ai.pingpong >> ai.updated;
	}

	bin& operator<<(const AnimInfo& ai)
	{
		return (*this) << ai.handle << ai.playing << ai.angle << ai.counter << ai.curFrame
			<< ai.autoFlip << ai.autoMirror << ai.autoRotations << ai.speed << ai.repeat
			<< ai.repeat_to << ai.pingpong << ai.updated;
	}

	bin& operator<<(const vector<ExpStore>& vars) 
	{
		(*this) << vars.size();

		vector<ExpStore>::const_iterator i = vars.begin();
		vector<ExpStore>::const_iterator vars_end = vars.end();

		for ( ; i != vars_end; i++)
			(*this) << (*i);

		return (*this);
	}

	bin& operator>>(vector<ExpStore>& vars) 
	{
		//int count = read<int>();
		// Workaround for lame compilers
		int count;
		(*this) >> count;

		vars.resize(0);

		for (int i = 0; i < count; i++) {
			ExpStore e;
			(*this) >> e;
			vars.push_back(e);
		}

		return (*this);
	}
#endif

	// Saving/loading
	void save(const char* fname)
	{
		FILE* f = fopen(fname, "wb");
		if (f == NULL) return;
		fwrite(data, datasize, 1, f);
		fclose(f);
	}

	void load(const char* fname)
	{
		FILE* f = fopen(fname, "rb");
		if (f == NULL) return;
		fseek(f, 0, SEEK_END);
		int fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		free();
		if (allocate(fsize, true))
			fread(data, fsize, 1, f);
		fclose(f);
	}

	// flag for serialization
	bool			loading;
public:
	BYTE*			data;			// the buffer
	size_t			datasize;		// size of data written
	size_t			capacity;		// size of buffer
	size_t			readcursor;		// cursor for reading data
#ifdef BIN_CXIMAGE
	unsigned long	imgformat;
#endif

	// Resize capacity, using the greatest of:
	// - Capacity + 512
	// - The requested size (eg datasize + sizeof(myclass)) plus 512 bytes
	// - A 50% increase in capacity
	void extend(size_t requested)
	{
		if (datasize + requested > capacity)
			bin::realloc(max(capacity + 512, max(datasize + requested + 512, capacity + (capacity / 2))));
	}
};

#endif // __BIN_H__