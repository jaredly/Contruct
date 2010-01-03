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

#ifndef OBJECTPOOL_HPP_
#define OBJECTPOOL_HPP_

//#define POOL_NEW


namespace detail
{
	typedef ptrdiff_t index_t;
};

template <class T, detail::index_t MAX_SIZE>
struct objectpool
{
	static const bool chainable;

	typedef detail::index_t index_t;

	// re: alignment
	// this could be a bad idea, but it should work..
	// sizeof(T) should be aligned, and new-allocated
	// data is guaranteed to be aligned, so data[] should
	// be aligned as well, unless the block structure is
	// padded at the head rather than at the tail (hopefully not)
	// 
	// not sure if every element in an array is guaranteed to be aligned however..
	struct block { char data[sizeof(T)]; };

	static __forceinline index_t& next(block* b) { return *((index_t*)b); }
	static __forceinline index_t& prev(block* b) { return *(((index_t*)b)+1); }
	static __forceinline index_t& next(block& b) { return *((index_t*)&b); }
	static __forceinline index_t& prev(block& b) { return *(((index_t*)&b)+1); }

	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T			value_type;
	typedef T*			pointer;
	typedef T const*	const_pointer;
	typedef T&			reference;
	typedef T const&	const_reference;

	objectpool()
	{
		// setup the block structures as a circular linked list
		for (index_t i = 0; i < (MAX_SIZE); ++i)
		{
			next(space[i]) = i+1;
			prev(space[i]) = i-1;
		}
		next(space[MAX_SIZE-1]) = 0;
		prev(space[0]) = MAX_SIZE-1;

		alloc_count = 0;
		free_list = 0;
		chainable = false;
	}

	~objectpool()
	{
		assert(alloc_count == 0);
		validate();
		clear();
	}

	void clear()
	{
		alloc_count = 0;
		free_list = 0;
	}

	inline void validate()
	{
#ifndef NDEBUG
		assert(alloc_count <= MAX_SIZE);
		assert(free_list >= 0 && free_list < MAX_SIZE);
#endif
	}

	size_type max_size() const { return sizeof(T); }

	void* allocate(size_t nbytes)
	{
		assert(nbytes <= sizeof(T));
		validate();

		assert(alloc_count < MAX_SIZE);

		block* the_block = &space[free_list];

		const index_t our_index = (index_t)(the_block - space);
		const index_t our_next = next(the_block);
		const index_t our_prev = prev(the_block);

		// remove from free list
		// and add to alloc list
		switch (alloc_count)
		{
		case 0:
			free_list = our_next;
			next(space[our_prev]) = our_next;
			prev(space[our_next]) = our_prev;
			break;

		default:
			free_list = our_next;
			next(space[our_prev]) = our_next;
			prev(space[our_next]) = our_prev;
			// fall through...
		case MAX_SIZE-1:
			next(space[prev(the_block)]) = our_index;
			break;
		};

		alloc_count++;

		validate();
		// call default constructor
		new (the_block) T;
		return &(the_block->data);
	}

	void deallocate(void* adata)
	{
		validate();

		block* the_block = (block*)adata;

		const index_t our_index = (detail::index_t)(the_block - space);

		// call destructor
		reinterpret_cast<T*>(&the_block->data)->~T();

		// add to free list
		switch (alloc_count)
		{
		case 1:
			next(the_block) = free_list;
			prev(the_block) = prev(space[free_list]);
			prev(space[free_list]) = our_index;
			next(space[prev(the_block)]) = our_index;
			free_list = our_index;
			break;
		case MAX_SIZE:
			next(the_block) = our_index;
			prev(the_block) = our_index;
			free_list = our_index;
			break;
		default:
			next(the_block) = free_list;
			prev(the_block) = prev(space[free_list]);
			prev(space[free_list]) = our_index;
			next(space[prev(the_block)]) = our_index;
			free_list = our_index;
			break;
		};

		alloc_count--;

		validate();
	}

	bool full() const
	{
		return alloc_count == MAX_SIZE;
	}

	bool empty() const
	{
		return alloc_count == 0;
	}

	bool contains(void* ptr) const
	{
		return ((const block*)ptr >= space) && ((const block*)ptr < (space + MAX_SIZE));
	}

private:
	block space[MAX_SIZE];
	index_t free_list; // index of first element in free list
	index_t alloc_count;
};

#ifdef POOL_NEW
template<class T, detail::index_t n> inline void* operator new(size_t nbytes, objectpool<T, n>& the_pool)
{
	return the_pool.allocate(nbytes);
}

template<class T, detail::index_t n> inline void operator delete(void* data, objectpool<T, n>& the_pool)
{
	the_pool.deallocate(data);
}
#endif

#endif//OBJECTPOOL_HPP_