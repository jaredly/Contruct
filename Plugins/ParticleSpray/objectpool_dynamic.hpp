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
#ifndef OBJECTPOOL_DYNAMIC_HPP_
#define OBJECTPOOL_DYNAMIC_HPP_

#include "objectpool.hpp"

template <class T, detail::index_t MAX_SIZE> class objectpool_dynamic
{
	template <class Y, detail::index_t Y_MAX_SIZE> struct node
	{
		objectpool<Y, Y_MAX_SIZE> m_pool;
		node<Y, Y_MAX_SIZE>* m_next;
	};

	typedef node<T, MAX_SIZE> node_t;

public:
	static const bool chainable;

	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T			value_type;
	typedef T*			pointer;
	typedef T const*	const_pointer;
	typedef T&			reference;
	typedef T const&	const_reference;

	objectpool_dynamic()
		: m_first(0)
		: chainable(false)
	{
	}

	~objectpool_dynamic()
	{
		clear();
	}

	void clear()
	{
		for (node_t* i = m_first; i != 0; i = i->m_next)
		{
			i->m_pool.clear();
			delete i;
			break;
		}
		m_first = 0;
	}

	size_type max_size() const { return sizeof(T); }

	void* allocate(size_t nbytes)
	{
		for (node_t* i = m_first; i != 0; i = i->m_next)
			if (i->m_pool.full() == false)
				return i->m_pool.allocate(nbytes);

		node_t* prev = m_first;
		m_first = new node_t;
		m_first->m_next = prev;
		return m_first->m_pool.allocate(nbytes);
	}

	void deallocate(void* ptr)
	{
		if (ptr)
		{
			node_t* i = m_first;
			for (; i != 0; i = i->m_next)
			{
				if (i->m_pool.contains(ptr))
				{
					i->m_pool.deallocate(ptr);
					if (i->m_pool.empty())
					{
						remove(i);
					}
				}
			}
		}
	}

	bool full() const { return false; }
	bool empty() const { return (m_first == 0) || m_first->m_pool.empty(); }
	bool contains(void* ptr) const
	{
		for (node_t* i = m_first; i != 0; i = i->m_next)
			if (i->m_pool.contains(ptr))
				return true;
		return false;
	}

private:
	// efficiency heuristics:
	// 1. if this is the final block or the first block, keep it (fast)
	// 2. if front is empty, discard it (slow)
	// 3. else move it to the front (slow)
	void remove(node_t* n)
	{
		if ((n == 0) || (n == m_first)) return;
		
		if (m_first->m_pool.empty())
		{
			for (node_t* i = m_first; i != 0; i = i->m_next)
			{
				if (i->m_next == n)
				{
					i->m_next = n->m_next;
					delete n;
				}
			}
		}
		else
		{
			// unlink n
			for (node_t* i = m_first; i != 0; i = i->m_next)
			{
				if (i->m_next == n)
				{
					i->m_next = n->m_next;
				}
			}

			// link it to the front
			node_t* tmp = m_first;
			m_first = n;
			n->m_next = tmp;
		}
	}

	node_t* m_first;
};

#endif//OBJECTPOOL_DYNAMIC_HPP_