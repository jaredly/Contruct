/*

Copyright (c) 2005 Kristoffer Gronlund

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation 
files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.

	Version: 0.2
	See changelog in readme file.
*/
#ifndef OBJECTPOOL_CHAIN_HPP_
#define OBJECTPOOL_CHAIN_HPP_

#include "objectpool_dynamic.hpp"

template <class T> struct chain_malloc // must be last in chain
{
	static const bool chainable;
	size_t max_size() const { return 0x7fffffff; }
	void* allocate(size_t nbytes)
	{
		// call default constructor
		void* the_block = malloc(nbytes);
		new (the_block) T;
		return the_block;
	}
	void deallocate(void* ptr)
	{
		// call destructor
		reinterpret_cast<T*>(ptr)->~T();
		return free(ptr);
	}
	void clear() {}
	bool full() const { return false; }
	bool empty() const { return false; }
	bool contains(void* ptr) const { return true; }

	chain_malloc() : chainable(false) {}
};

template <class T, detail::index_t MAX_SIZE, typename TNext = chain_malloc<T> > struct objectpool_chain
	: private objectpool_dynamic<T, MAX_SIZE>
{
	objectpool_chain()
		: objectpool_dynamic<T, MAX_SIZE>()
	{
	}

	~objectpool_chain()
	{
		clear();
	}

	void clear()
	{
		objectpool_dynamic<T, MAX_SIZE>::clear();
		m_next.clear();
	}

	size_type max_size() const
	{
		const size_type my_size = objectpool_dynamic<T, MAX_SIZE>::max_size();
		const size_type next_size = m_next.max_size();
		return (my_size > next_size) ? my_size : next_size;
	}

	void* allocate(size_t nbytes)
	{
		if (objectpool_dynamic<T, MAX_SIZE>::max_size() < nbytes)
			return m_next.allocate(nbytes);
		return objectpool_dynamic<T, MAX_SIZE>::allocate(nbytes);
	}

	void deallocate(void* ptr)
	{
		if (contains(ptr) == false)
			m_next.deallocate(ptr);
		else
			objectpool_dynamic<T, MAX_SIZE>::deallocate(ptr);
	}

	bool empty() const { return objectpool_dynamic<T, MAX_SIZE>::empty() && m_next.empty(); }
	bool full() const { return objectpool_dynamic<T, MAX_SIZE>::full() && m_next.full(); }
	
	bool contains(void* ptr) const
	{
		return objectpool_dynamic<T, MAX_SIZE>::contains(ptr) || m_next.contains(ptr);
	}

	TNext m_next;
};

#endif//OBJECTPOOL_CHAIN_HPP_