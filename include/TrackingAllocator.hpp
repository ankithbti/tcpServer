/*
 * TrackingAllocator.hpp
 *
 *  Created on: 28-Aug-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_TRACKINGALLOCATOR_HPP_
#define INCLUDE_TRACKINGALLOCATOR_HPP_

#include <memory>
#include <limits>

namespace amt{

template<typename T>
class TrackingAllocator final{

public:

	// optional, rebind to another type
	template<class U>
	struct rebind
	{
		using other = TrackingAllocator<U>;
	};

	using value_type = T;

	// Optional aliases
	using pointer = T*;
	using const_pointer = const T*;
	using void_pointer = void*;
	using const_void_pointer = const void*;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;

	TrackingAllocator() = default;
	~TrackingAllocator() = default;

	struct propagate_on_container_copy_assignment : std::true_type
	{
	};

	struct propagate_on_container_move_assignment : std::true_type
	{
	};

	struct propagate_on_container_swap : std::true_type
	{
	};

	TrackingAllocator select_on_container_copy_construction(const TrackingAllocator &a)
	{
		return a;
	}

	template<class U>
	TrackingAllocator(const TrackingAllocator<U> &other)
	{
	}

	template<class U>
	TrackingAllocator(TrackingAllocator<U> &&other)
	{
	}

	TrackingAllocator(const TrackingAllocator &) = default;
	TrackingAllocator(TrackingAllocator &&) = default;

	TrackingAllocator &operator =(const TrackingAllocator &) = default;
	TrackingAllocator &operator =(TrackingAllocator &&) = default;

	pointer allocate(size_type numOfObjects){
		_totalAllocations += numOfObjects;
		std::cout << " Total Allocations: " << _totalAllocations <<  std::endl;
		return static_cast<pointer>(operator new(numOfObjects*sizeof(T)));
	}

	// Optional - For better locality
	pointer allocate(size_type numObjects, const_void_pointer hint)
	{
		return allocate(numObjects);
	}

	void deallocate(pointer p, size_type numOfObjects){
		operator delete(p);
	}

	// Optional
	size_type max_size() const{
		return std::numeric_limits<size_type>::max();
	}

	// optional, constructs an object in place
	template<class U, class... Args>
	void construct(U *p, Args && ...args)
	{
		new(p) U(std::forward<Args>(args)...);
	}

	// optional, destroys an object
	template<class U>
	void destroy(U *p)
	{
		p->~U();
	}

	// NEW: return total allocations
	size_type get_total_allocations() const noexcept
			{
		return _totalAllocations;
			}

private:

	size_type _totalAllocations = 0;
};

template<class T>
bool operator ==(const TrackingAllocator<T> &a1, const TrackingAllocator<T> &a2)
{
	return true;
}

}



#endif /* INCLUDE_TRACKINGALLOCATOR_HPP_ */
