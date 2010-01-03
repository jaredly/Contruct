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

*/

#ifndef POOLALLOCATOR_HPP_
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
#define POOLALLOCATOR_HPP_

#include <cassert>
#include <memory>

#ifdef USE_DLMALLOC
#include "dlmalloc.h" // Doug Lea's malloc(), with USE_DL_PREFIX defined
#endif

// For multithreading-support
#define POOL_DECLARE_MUTEX // static mutex ms_mutex;
#define POOL_IMPLEMENT_MUTEX // mutex pool_allocator_base::ms_mutex();
#define POOL_MUTEX_LOCK // ms_mutex.lock();
#define POOL_MUTEX_UNLOCK // ms_mutex.unlock();

template <typename Pool> struct pool_allocator_base
{
	static int ms_allocCount;
	static unsigned int ms_totalAllocs;

	static Pool* ms_pool;

	POOL_DECLARE_MUTEX
};

template <typename Pool> int pool_allocator_base<Pool>::ms_allocCount = 0;
template <typename Pool> unsigned int pool_allocator_base<Pool>::ms_totalAllocs = 0;
template <typename Pool> Pool* pool_allocator_base<Pool>::ms_pool = 0;

template <typename Pool> inline void set_allocator_pool(Pool* pool) { pool_allocator_base<Pool>::ms_pool = pool; }

template <typename Pool> struct scoped_pool
{
	explicit inline scoped_pool()
		: m_pool(new Pool())
	{
		pool_allocator_base<Pool>::ms_pool = m_pool;
	}
	inline ~scoped_pool()
	{
		pool_allocator_base<Pool>::ms_pool = 0;
		delete m_pool;
	}
	inline operator Pool* () const { return m_pool; }
	inline Pool* operator->() const { return m_pool; }
	inline Pool& operator*() { return *m_pool; }
	inline const Pool& operator*() const { return *m_pool; }
	Pool* m_pool;

private:
	scoped_pool(const scoped_pool&);
	void operator=(const scoped_pool&);
};

// A standards-compliant (I think) pooled allocator
template<typename T, typename Pool>
class pool_allocator : public pool_allocator_base<Pool>
{
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T			value_type;
	typedef T*			pointer;
	typedef T const*	const_pointer;
	typedef T&			reference;
	typedef T const&	const_reference;
	
	template<typename U> 
	struct rebind { typedef typename pool_allocator<U, Pool> other; };

	pool_allocator()
	{
	}

	template<typename U, typename Pool>
	pool_allocator( pool_allocator<U, Pool> const& arg )
	{
	}

	//! The largest value that can meaningfully be passed to allocate.
	size_type max_size() const { return ms_pool->max_size(); }

	// O(1) for small objects
	pointer allocate( size_type count, std::allocator<void>::const_pointer /*hint*/ = 0 ) const
	{
		POOL_MUTEX_LOCK;

		assert(ms_pool);
		ms_totalAllocs++;
		ms_allocCount++;

		pointer ret = 0;
		size_type to_alloc = count*sizeof(T);
		ret = (pointer)ms_pool->allocate(to_alloc);

		if (ret == 0)
		{
#ifdef USE_DLMALLOC
			ret = reinterpret_cast<T*>( dlmalloc( to_alloc ) );
#else
			ret = reinterpret_cast<T*>( malloc(to_alloc) );
#endif
		}


		POOL_MUTEX_UNLOCK;
		return ret;
	}

	// O(1) for small objects
	void deallocate( pointer block, size_type count ) const throw()
	{
		assert(ms_pool);
		assert( block && "null pointer argument" );
		POOL_MUTEX_LOCK;

		if (ms_pool->contains(block))
		{
			ms_pool->deallocate(block);
		}
		else
		{
#ifdef USE_DLMALLOC
			dlfree((char*)block);
#else
			free((void*)block);
#endif
		}
		ms_allocCount--;
		
		POOL_MUTEX_UNLOCK;
	}

	// call the copy constructor for the object
	void construct(pointer obj, T const& arg)
	{
		new (obj) T(arg);
	}

	// call the destructor of the object
	void destroy(pointer obj)
	{
		obj;
		obj->~T();
	}

	pointer address( reference element ) const
	{
		return &element;
	}

	const_pointer address( const_reference element ) const
	{
		return &element;
	}
};

// specialisation of the pooled allocator for void
/*
template<typename Pool>
class pool_allocator<void, Pool>
{
public:
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	
	typedef void		value_type;
	typedef void*		pointer;
	typedef void const*	const_pointer;

	// construct an allocator for a different type.
	template<typename U> 
	struct rebind { typedef typename pool_allocator<U, Pool> other; };

	pool_allocator() {}
};
*/

// Returns true if allocators can deallocate each other's memory
template<typename T, typename U, typename Pool>
bool operator==( pool_allocator<T, Pool> const& left, pool_allocator<U, Pool> const& right )
{
	return true;
}

// Returns true if allocators can't deallocate each other's memory
template<typename T, typename U, typename Pool>
bool operator!=( pool_allocator<T, Pool> const& left, pool_allocator<U, Pool> const& right )
{
	return false;
}

#endif//POOLALLOCATOR_HPP_