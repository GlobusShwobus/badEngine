#pragma once

#include <memory>
#include <ranges>
#include <limits>
#include <assert.h>
#include <stdexcept>
#include <concepts>
#include "CoreUtils.h"

//TODO: proper insert and all pushes/emplaces hierarchy

namespace badCore
{
	/**
	* \brief
	* An inhouse variant of std::vector with some differences.
	* 
	* Mainly the insert is not complete. The cannon insert method is emplace_back. There is no true insert pos.
	* 
	* Additionally, using C++ 20 language tools. Some type traits are demanded  up front:
	* 
	*		- type must be destructible.
	* 
	*		- type must be move constructible
	* 
	*		- type must not be const/volatile/reference
	*/
	template<typename T> 
		requires
			std::destructible<T> &&
			std::is_nothrow_move_constructible_v<T> &&
			std::same_as<T, std::remove_cvref_t<T>>
	class Sequence final
	{
	public:
		using type = Sequence<T>;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

	private:
		//allocator/deallocator functions
		pointer seq_allocate(size_type count)const
		{
			assert(count < max_size());
			return static_cast<pointer>(::operator new(count * sizeof(value_type)));
		}

		void seq_deallocate(pointer mem)noexcept
		{
			::operator delete(mem);
		}

		void seq_realloc(size_type new_capacity)
		{
			pointer newArray = seq_allocate(new_capacity);   //temp buffer
			pointer newEnd = newArray;				     //temp buffer

			try {
				//if trivial then go all in
				if constexpr (std::is_trivially_move_constructible_v<value_type> && std::is_trivially_destructible_v<value_type>){
					if (mArray && mSize > 0) {
						std::memmove(newArray, mArray, mSize * sizeof(value_type));
						newEnd = newArray + mSize;//just in case...
					}
				}
				else{
					if (mArray)
						newEnd = std::uninitialized_move(mArray, mArray + mSize, newArray);
				}
			}
			catch (...) {
				std::destroy(newArray, newEnd);	  //cleanup temp
				seq_deallocate(newArray);			  //cleanup temp
				throw;
			}

			if (mArray){							   //cleanup curr
				std::destroy(mArray, mArray + mSize);  //cleanup curr
				seq_deallocate(mArray);					   //cleanup curr
			}

			mArray = newArray;					//assign new
			mCapacity = new_capacity;			//assign new
			//mSize = mSize;					//not changed
		}

		//growth math
		constexpr size_type growthFactor(size_type seed)const noexcept
		{
			return size_type(seed + (seed * 0.5f) + mAdditive);
		}

	public:

		constexpr Sequence()noexcept = default;

		explicit Sequence(size_type count)
			requires std::default_initializable<value_type>
		{
			if (count == 0)return;

			pointer mem = seq_allocate(count);
			try {
				std::uninitialized_value_construct(mem, mem + count);
			}
			catch (...) {
				seq_deallocate(mem);
				throw;
			}
			mArray = mem;
			mCapacity = count;
			mSize = count;
		}

		explicit Sequence(size_type count, const_reference value)
			requires std::constructible_from<value_type, const_reference>
		{
			if (count == 0)return;

			pointer mem = seq_allocate(count);
			try {
				std::uninitialized_fill(mem, mem + count, value);
			}
			catch (...) {
				seq_deallocate(mem);
				throw;
			}

			mArray = mem;
			mCapacity = count;
			mSize = count;
		}

		template <std::forward_iterator FwdIt>
		Sequence(FwdIt first, FwdIt last)
			requires std::constructible_from<value_type, std::iter_reference_t<FwdIt>>
		{
			size_type count = std::distance(first, last);

			if (count == 0)
				return;

			pointer mem = seq_allocate(count);

			try {
				std::uninitialized_copy(first, last, mem);
			}
			catch (...) {
				seq_deallocate(mem);
				throw;
			}

			mArray = mem;
			mCapacity = count;
			mSize = count;
		}

		Sequence(std::initializer_list<value_type> init)
			requires std::constructible_from<value_type, const_reference>
		:Sequence(init.begin(), init.end())
		{
		}

		Sequence(const Sequence& rhs)
			requires std::constructible_from<value_type, const_reference>
		:Sequence(rhs.begin(), rhs.end())
		{
		}

		Sequence(Sequence&& rhs)noexcept
		{
			mArray = rhs.mArray;
			rhs.mArray = nullptr;

			mSize = rhs.mSize;
			rhs.mSize = 0;

			mCapacity = rhs.mCapacity;
			rhs.mCapacity = 0;

			mAdditive = rhs.mAdditive;
			rhs.mAdditive = 1;
		}

		Sequence& operator=(Sequence rhs)noexcept
		{
			rhs.swap(*this);
			return *this;
		}

		Sequence& operator=(std::initializer_list<value_type> ilist)
		{
			Sequence temp(ilist);
			temp.swap(*this);
			return *this;
		}

		~Sequence()noexcept
		{
			if (mArray) {
				std::destroy(begin(), end());
				seq_deallocate(mArray);
				mArray = nullptr;
				mSize = 0;
				mCapacity = 0;
			}
		}

		constexpr void swap(Sequence& rhs)noexcept
		{
			std::swap(mArray, rhs.mArray);
			std::swap(mSize, rhs.mSize);
			std::swap(mCapacity, rhs.mCapacity);
			std::swap(mAdditive, rhs.mAdditive);
		}

		using iterator = pointer;
		using const_iterator = const_pointer;

		constexpr iterator begin()noexcept
		{
			return  mArray;
		}

		constexpr iterator end()noexcept
		{
			return  mArray + mSize;
		}

		constexpr const_iterator begin()const noexcept
		{
			return mArray;
		}

		constexpr const_iterator end()const noexcept
		{
			return mArray + mSize;
		}

		constexpr const_iterator cbegin()const noexcept
		{
			return mArray;
		}

		constexpr const_iterator cend()const noexcept
		{
			return mArray + mSize;
		}

		constexpr pointer data() noexcept
		{
			return mArray;
		}

		constexpr const_pointer data() const noexcept
		{
			return mArray;
		}


		//UB if container is empty, otherwise returns reference to the first element
		constexpr reference front() noexcept
		{
			assert(!isEmpty());
			return *begin();
		}

		//UB if container is empty, otherwise returns reference to the first element
		constexpr const_reference front()const noexcept
		{
			assert(!isEmpty());
			return *begin();
		}

		//UB if container is empty, otherwise returns reference to the last element
		constexpr reference back() noexcept
		{
			assert(!isEmpty());
			return mArray[mSize - 1];
		}

		//UB if container is empty, otherwise returns reference to the last element
		constexpr const_reference back()const noexcept
		{
			assert(!isEmpty());
			return mArray[mSize - 1];
		}

		//UB if index is not in the range of [begin -> end), otherwise returns reference to the element
		constexpr reference operator[](size_type index) noexcept
		{
			assert(!isEmpty());
			return mArray[index];
		}

		//UB if index is not in the range of [begin -> end), otherwise returns reference to the element
		constexpr const_reference operator[](size_type index)const noexcept
		{
			assert(!isEmpty());
			return mArray[index];
		}

		//throws BadException if indeex is not in the range of [begin -> end), otherwise returns reference to the element
		constexpr reference at(size_type index)
		{
			if (index >= mSize)
				throw std::out_of_range("out of range index");
			return mArray[index];
		}

		//throws BadException if indeex is not in the range of [begin -> end), otherwise returns reference to the element
		constexpr const_reference at(size_type index)const
		{
			if (index >= mSize)
				throw std::out_of_range("out of range index");
			return mArray[index];
		}

		//number of constructed elements
		constexpr size_type size()const noexcept
		{
			return mSize;
		}

		// maximum number elements depending on sizeof(T)
		constexpr size_type max_size()const noexcept
		{
			return (std::numeric_limits<size_type>::max)() / sizeof(value_type);
		}

		//current size of allocated memory
		constexpr size_type capacity() const noexcept
		{
			return mCapacity;
		}

		//if any constructed elements
		constexpr bool isEmpty()const noexcept
		{
			return mSize == 0;
		}

		//set addative for growth calculation, can't go lower than 1 in debug, otherwise all types of memes will happen
		constexpr void set_additive(size_type additive)noexcept
		{
			assert(additive >= 1);
			mAdditive = additive;
		}

		//destroy all constructed elements
		void clear()noexcept
		{
			std::destroy(begin(), end());
			mSize = 0;
		}

		//clears and sets to nullptr
		void wipe()noexcept
		{
			if (!mArray || mCapacity == 0)
				return;

			std::destroy(begin(), end());
			secure_zero_bytes(mArray, mCapacity * sizeof(value_type));

			::operator delete(mArray);

			mArray = nullptr;
			mSize = 0;
			mCapacity = 0;;
		}

		//copies elements
		void push_back(const_reference value)
			requires std::constructible_from<value_type, const_reference>
		{
			emplace_back(value);
		}

		//moves elements
		void push_back(value_type&& value)
			requires std::constructible_from<value_type, value_type&&>
		{
			emplace_back(std::move(value));
		}

		//creates elements in place
		template<typename... Args>
		void emplace_back(Args&&... args)
			requires std::constructible_from<value_type, Args...>
		{
			//if at capacity, reallocate with extra memory
			if (mSize == mCapacity) {
				seq_realloc(growthFactor(mCapacity));
			}
			new(mArray + mSize)value_type(std::forward<Args>(args)...);
			++mSize;
		}

		template<std::input_iterator It>
		void insert_back(It first, It last)
			requires std::constructible_from<value_type, std::iter_reference_t<It>>
		{
			for (; first != last; ++first)
				emplace_back(*first);
		}


		//UB if the container is empty, otherwise erases the last element
		void pop_back()noexcept
		{
			assert(!isEmpty());
			erase(end() - 1, end());
		}

		//UB if pos is not in the range of [begin -> end) and if container is empty (probably terminate)
		iterator erase(const_iterator pos) noexcept
			requires std::is_nothrow_move_assignable_v<value_type>
		{
			assert(!isEmpty() && pos >= begin() && pos < end());
			return erase(pos, pos + 1);
		}

		//UB if given range is not in the range of [begin -> end) and if container is empty (probably terminate)
		iterator erase(const_iterator first, const_iterator last) noexcept
			requires std::is_nothrow_move_assignable_v<value_type>
		{
			if (first == last)
				return const_cast<iterator>(first);

			iterator targetBegin = const_cast<iterator>(first);
			iterator targetEnd = const_cast<iterator>(last);
			iterator thisBegin = begin();
			iterator thisEnd = end();
			assert(!isEmpty());
			assert(thisBegin <= targetBegin && thisEnd >= targetEnd && targetBegin <= targetEnd);
			//get distance
			size_type destroy_size = static_cast<size_type>(targetEnd - targetBegin);

			//move objects
			for (; targetEnd != thisEnd; ++targetBegin, ++targetEnd) {
				*targetBegin = std::move(*targetEnd);
			}

			mSize -= destroy_size;
			std::destroy(end(), thisEnd);

			return const_cast<iterator>(first);
		}

		//swaps given element with last and pops back. UB if pos is not in the range of [begin -> end)
		void swap_with_last_erase(const_iterator pos) noexcept
		{
			assert(!isEmpty() && pos >= begin() && pos < end());
			swap_with_last_erase(pos, pos + 1);
		}

		//swaps given range with last one by one and pops range. UB if given range is not in the range of [begin -> end)
		void swap_with_last_erase(const_iterator first, const_iterator last) noexcept
			requires std::is_nothrow_move_assignable_v<value_type>
		{
			if (first == last)return;
			iterator destination = const_cast<iterator>(first);
			iterator targetEnd = const_cast<iterator>(last);
			iterator thisEnd = end();
			assert(!isEmpty());
			assert(begin() <= destination && thisEnd >= targetEnd && destination <= targetEnd);
			//get distance
			size_type destroy_size = static_cast<size_type>(targetEnd - destination);

			iterator src = thisEnd - 1;

			for (size_type i = 0; i < destroy_size; ++i) {
				*destination++ = std::move(*src--);
			}
			mSize -= destroy_size;
			std::destroy(end(), thisEnd);
		}

		// if new capacity is more than old capacity, reallocates, otherwise does nothing
		void reserve(size_type new_capacity)
			requires std::is_nothrow_move_assignable_v<value_type>
		{
			if (new_capacity > mCapacity) {
				seq_realloc(new_capacity);
			}
		}
		// default constructs or erases elements by the difference amount of current size and given count
		void resize(size_type count)
			requires std::default_initializable<value_type>
		{
			//check if need growing
			if (count > mCapacity)
				seq_realloc(count);

			//check if count is less than current size, then just cut the end but don't reallocate
			if (count < mSize) {
				erase(begin() + count, end());
			}//otherwise use std:: to bulk init instead of a manual loop
			else if (count > mSize) {
				size_type amount = count - mSize;

				std::uninitialized_value_construct(mArray + mSize, mArray + count);
				mSize = count;
			}
		}

		// default constructs or erases elements by the difference amount of current size and given count
		void resize(size_type count, const_reference value)
			requires std::constructible_from<value_type, const_reference>
		{
			//check if need growing
			if (count > mCapacity)
				seq_realloc(count);
			//check if count is less than current size, then just cut the end but don't reallocate
			if (count < mSize) {
				erase(begin() + count, end());
			}//otherwise use std:: to bulk init instead of a manual loop
			else if (count > mSize) {
				size_type amount = count - mSize;

				std::uninitialized_fill_n(mArray + mSize, amount, value);
				mSize = count;
			}
		}

		//shrinks to current size
		void shrink_to_fit()
		{
			if (mCapacity > mSize) {
				seq_realloc(mSize);
			}
		}

	private:
		//member variables
		pointer mArray = nullptr;
		size_type mSize = 0;
		size_type mCapacity = 0;

		size_type mAdditive = 1;
	};
}
