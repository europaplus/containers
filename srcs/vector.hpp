//
// Created by Knot Fonda on 5/2/21.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
#define FT_CONTAINERS_VECTOR_HPP
# include <iostream>
# include "utils.hpp"

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
	public:
		class reverse_iterator;
		class iterator;
		class const_iterator;
		class const_reverse_iterator;
	public:
		typedef T											value_type;
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference ;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef ptrdiff_t									difference_type;
		typedef size_t										size_type;
	public:
		explicit vector(const allocator_type &alloc = allocator_type())
				: _alloc(alloc), _sz(0), _elem(nullptr), _space(0) {};
		explicit vector (size_type n, const value_type &val = value_type(),
						 const allocator_type &alloc = allocator_type())
				: _alloc(alloc), _sz(0), _elem(nullptr), _space(0)
		{
			if (n < 0)
				throw std::length_error("vector");
			resize(n, val);
		};
		vector (const vector &copy)
				: _alloc(copy._alloc), _sz(copy._sz), _space(copy._space)
		{
			this->_elem = this->_alloc.allocate(this->_space);
			pointer p = this->_elem;
			pointer pp = copy.begin().getPointer();
			for (size_type i = 0; i < this->_sz; ++i) {
				_alloc.construct(p++, *pp++);
			}
		};
		~vector()
		{
			clear();
			_elem = nullptr;
			_space = 0;
		};
		vector& operator=(const vector& x) {
			clear();
			insert(begin(), x.begin(), x.end());
			return *this;
		}
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
			   typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
				: _alloc(alloc), _sz(0), _elem(nullptr), _space(0)
		{
			difference_type _sz_vec = last.operator->() - first.operator->();
			if (_sz_vec < 0)
				throw std::length_error("vector");
			reserve(static_cast<size_type>(_sz_vec));
			for (difference_type i = 0; i < _sz_vec; ++i)
				this->_alloc.construct(&(this->_elem[i]), *(first++));
			this->_sz = static_cast<size_type>(_sz_vec);
		};
		//############ITERATORS:############//
		iterator begin() { return iterator(this->_elem); };
		const_iterator begin() const { return const_iterator(this->_elem); };
		iterator end() { return iterator(&(this->_elem[_sz])); };
		const_iterator end() const { return const_iterator(&(this->_elem[_sz])); };
		reverse_iterator rbegin() { return reverse_iterator(--(this->end())); };
		const_reverse_iterator rbegin() const { return const_reverse_iterator(--(this->end())); };
		reverse_iterator rend() { return reverse_iterator(this->begin() - 1); };
		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin() - 1); };
		//############CAPACITY:############//
		size_type size() const { return (this->_sz); };
		size_type max_size() const { return (this->_alloc.max_size()); }
		size_type capacity() const { return (this->_space); };
		bool empty() const { return (this->_sz == 0); };
		void resize(size_type n, value_type val = value_type())
		{
			if (n < this->_sz)
				for (size_type i = n; i <= n; ++i)
					this->_alloc.destroy(&(this->_elem[i]));
			else
			{
				reserve(n);
				for (size_type i = this->_sz; i < this->_space; ++i)
					_alloc.construct(&(this->_elem[i]), val);
			}
			this->_sz = n;
		};
		void reserve (size_type n)
		{
			if (n <= this->_space)
				return ;
			pointer p = this->_alloc.allocate(n);
			for (size_type i = 0; i < this->_sz; ++i)
				this->_alloc.construct(&p[i], this->_elem[i]);
			for (size_type i = 0; i < this->_sz; ++i)
				this->_alloc.destroy(&(this->_elem[i]));
			this->_alloc.deallocate(this->_elem, this->_sz);
			this->_elem = p;
			this->_space = n;
		};
//			############Element access:############//
		reference operator[](size_type n) { return (this->_elem[n]); };
		const_reference operator[](size_type n) const { return (this->_elem[n]); };
		reference at(size_type n)
		{
			if (n < 0 || n >= this->_sz)
				throw std::out_of_range("index out of range");
			return (this->_elem[n]);
		};
		const_reference at(size_type n) const
		{
			if (n < 0 || n > this->_sz)
				throw std::out_of_range("vector: Out of range");
			return (this->_elem[n]);
		};
		reference front() { return *(this->_elem); };
		const_reference  front() const { return *(this->_elem); };
		reference  back() { return (this->_elem[this->_sz - 1]); };
		const_reference back() const { return (this->_elem[this->_sz - 1]); };
//			############MODIFIERS:############
		template<class InputIterator>
		void assign(InputIterator first, InputIterator last,
					typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
		{
			clear();
			insert(this->begin(), first, last);
		};
		void assign(size_type n, const value_type &val)
		{
			clear();
			insert(this->begin(), n, val);
		};
		void push_back(const value_type &val)
		{
			insert(end(), val);
		};
		void pop_back() { _alloc.destroy(&(this->_elem[this->_sz--])); };
		iterator insert(iterator position, const value_type &val)
		{
			difference_type index = position.operator->() - this->begin().operator->();
			if (this->size() == this->capacity())
				reserve(size() == 0 ? 8 : 2 * this->size());
			_alloc.construct(_elem + _sz, (this->back()));
			++_sz;
			iterator pp = this->begin() + index;
			for (iterator pos = (this->end() - 1); pos != pp; --pos)
				*pos = *(pos - 1);
			*(this->begin() + index) = val;
			return (pp);
		};
		void insert(iterator position, size_type n, const value_type &val)
		{
			for (size_type count = 0; count < n; ++count)
				position = insert(position, val);
		}
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
					typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
		{
			while (last != first)
				position = insert(position, *(--last));
		};
		iterator erase(iterator position)
		{
			if (position == this->end())
				return (position);
			for (iterator pos = position + 1; pos != this->end(); ++pos)
				*(pos - 1) = *pos;
			_alloc.destroy(&*(this->end() - 1));
			--this->_sz;
			return (position);
		};
		iterator erase(iterator first, iterator last)
		{
			while (first != last)
				erase(--last);
			return (last);
		};
		void swap (vector &x)
		{
			allocator_type tmpAlloc = _alloc;
			size_type tmpSz = _sz;
			pointer tmpElem = _elem;
			size_type tmpSpace = _space;

			this->_alloc = x._alloc;
			this->_sz = x._sz;
			this->_elem = x._elem;
			this->_space = x._space;

			x._alloc = tmpAlloc;
			x._sz = tmpSz;
			x._elem = tmpElem;
			x._space = tmpSpace;
		};
		void clear()
		{
			for (size_type i = 0; i < this->_sz; ++i)
				this->_alloc.destroy(&(this->_elem[i]));
			this->_sz = 0;
		};

		class iterator : public std::iterator<std::random_access_iterator_tag, value_type>
		{
		private:
			pointer _cur;
		public:
			iterator(pointer cur = 0) : _cur(cur) {};
			iterator(const iterator &copy) { *this = copy; };
			iterator &operator=(const iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			~iterator() {};
			reference	operator*() { return (*(this->_cur)); };
			const_reference	operator*() const { return (*(this->_cur)); };
			pointer		operator->() { return (this->_cur); };
			iterator	&operator++() { ++_cur; return (*this); };
			iterator	&operator--() { --_cur; return (*this); };
			iterator	operator++(int) { iterator oldIterator = *this; ++_cur; return (oldIterator); };
			iterator	operator--(int) { iterator oldIterator = *this; --_cur; return (oldIterator); };
			iterator	operator+(difference_type n) const { return iterator(_cur + n); };
			iterator	operator-(difference_type n) const { return iterator(_cur - n); };
			iterator	operator+=(difference_type n) { _cur += n; return (*this); };
			iterator	operator-=(difference_type n) { _cur -= n; return (*this); };

			bool		operator==(const iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!=(const iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator<(const iterator& rhs) const { return (this->_cur < rhs._cur); };
			bool		operator<=(const iterator& rhs) const { return (this->_cur <= rhs._cur); };
			bool		operator>(const iterator& rhs) const { return (this->_cur > rhs._cur); };
			bool		operator>=(const iterator& rhs) const { return (this->_cur >= rhs._cur); };

			bool		operator==(const const_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!=(const const_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator<(const const_iterator& rhs) const { return (this->_cur < rhs.getPointer()); };
			bool		operator<=(const const_iterator& rhs) const { return (this->_cur <= rhs.getPointer()); };
			bool		operator>(const const_iterator& rhs) const { return (this->_cur > rhs.getPointer()); };
			bool		operator>=(const const_iterator& rhs) const { return (this->_cur >= rhs.getPointer()); };

			reference	operator[](difference_type n) const { return (_cur[n]); };
			pointer		getPointer() const { return (_cur); };
		};

		class const_iterator : public std::iterator<std::random_access_iterator_tag, value_type>
		{
		private:
			pointer _cur;
		public:
			const_iterator(pointer cur = 0) : _cur(cur) {};
			const_iterator(const_iterator const &copy) { *this = copy; };
			const_iterator(iterator const &copy) { *this = copy; };
			const_iterator &operator=(const_iterator const &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this); };
			const_iterator &operator=(iterator const &assign) { this->_cur = assign.getPointer(); return (*this); };
			~const_iterator() {};
			const_reference	operator*() { return (*(this->_cur)); };
			const_pointer	operator->() { return (this->_cur); };
			const_iterator	operator++(int) { const_iterator oldIterator = *this; ++_cur; return (oldIterator); };
			const_iterator	operator--(int) { const_iterator oldIterator = *this; --_cur; return (oldIterator); };
			const_iterator	&operator++() { ++_cur; return (*this); };
			const_iterator	&operator--() { --_cur; return (*this); };
			const_iterator	operator+(difference_type n) const { return const_iterator(_cur + n); };
			const_iterator	operator-(difference_type n) const { return const_iterator(_cur - n); };
			const_iterator	operator+=(difference_type n) { _cur += n; return (*this); };
			const_iterator	operator-=(difference_type n) { _cur -= n; return (*this); };

			bool		operator==(const iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!=(const iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator<(const iterator& rhs) const { return (this->_cur < rhs.getPointer()); };
			bool		operator<=(const iterator& rhs) const { return (this->_cur <= rhs.getPointer()); };
			bool		operator>(const iterator& rhs) const { return (this->_cur > rhs.getPointer()); };
			bool		operator>=(const iterator& rhs) const { return (this->_cur >= rhs.getPointer()); };

			bool		operator==(const const_iterator& rhs) const { return ( this->_cur == rhs._cur); };
			bool		operator!=(const const_iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator<(const const_iterator& rhs) const { return (this->_cur < rhs._cur); };
			bool		operator<=(const const_iterator& rhs) const { return (this->_cur <= rhs._cur); };
			bool		operator>(const const_iterator& rhs) const { return (this->_cur > rhs._cur); };
			bool		operator>=(const const_iterator& rhs) const { return (this->_cur >= rhs._cur); };

			const_reference	operator[](difference_type n) const { return (_cur[n]); };
			pointer		getPointer() const { return (_cur); };
		};

		class reverse_iterator : public std::reverse_iterator<vector::iterator>
		{
		private:
			iterator _cur;
		public:
			reverse_iterator() {};
			explicit reverse_iterator(iterator cur) : _cur(cur) {};
			reverse_iterator(const reverse_iterator &copy) { *this = copy; };
			reverse_iterator &operator=(const reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this); }
			~reverse_iterator() {};
			const_reference	operator*() { return (*(this->_cur)); };
			const_pointer	operator->() { return (this->_cur); };
			reverse_iterator	operator++(int) { reverse_iterator oldIterator = *this; --_cur; return (oldIterator); };
			reverse_iterator	operator--(int) { reverse_iterator oldIterator = *this; ++_cur; return (oldIterator); };
			reverse_iterator	&operator++() { --_cur; return (*this); };
			reverse_iterator	&operator--() { ++_cur; return (*this); };
			reverse_iterator	operator+(difference_type n) const { return reverse_iterator(this->_cur - n); };
			reverse_iterator	operator-(difference_type n) const { return reverse_iterator(this->_cur + n); };
			reverse_iterator	operator+=(difference_type n) { _cur -= n; return (*this); };
			reverse_iterator	operator-=(difference_type n) { _cur += n; return (*this); };

			bool		operator==(const reverse_iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!=(const reverse_iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator<(const reverse_iterator& rhs) const { return (this->_cur > rhs._cur); };
			bool		operator<=(const reverse_iterator& rhs) const { return (this->_cur >= rhs._cur); };
			bool		operator>(const reverse_iterator& rhs) const { return (this->_cur < rhs._cur); };
			bool		operator>=(const reverse_iterator& rhs) const { return (this->_cur <= rhs._cur); };

			bool		operator==(const const_reverse_iterator& rhs) const { return (this->_cur == rhs.getIterator()); };
			bool		operator!=(const const_reverse_iterator& rhs) const { return (this->_cur != rhs.getIterator()); };
			bool		operator<(const const_reverse_iterator& rhs) const { return (this->_cur > rhs.getIterator()); };
			bool		operator<=(const const_reverse_iterator& rhs) const { return (this->_cur >= rhs.getIterator()); };
			bool		operator>(const const_reverse_iterator& rhs) const { return (this->_cur < rhs.getIterator()); };
			bool		operator>=(const const_reverse_iterator& rhs) const { return (this->_cur <= rhs.getIterator()); };

			reference 	operator[](difference_type n) const { return *(_cur - n); };
			iterator getIterator() const { return (_cur); };
		};

		class const_reverse_iterator : public std::reverse_iterator<vector::iterator>
		{
		private:
			iterator _cur;
		public:
			const_reverse_iterator(){};
			explicit const_reverse_iterator(reverse_iterator cur) : _cur(cur) {};
			const_reverse_iterator(const const_reverse_iterator &copy) { *this = copy; };
			const_reverse_iterator(const reverse_iterator &copy) { *this = copy; };
			const_reverse_iterator &operator=(const const_reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this); }
			const_reverse_iterator &operator=(const reverse_iterator &assign) { this->_cur = assign.getIterator(); return (*this); }
			~const_reverse_iterator(){};
			const_reference	operator*() { return (*(this->_cur)); };
			const_pointer	operator->() { return (this->_cur); };
			const_reverse_iterator	operator++(int) { const_reverse_iterator oldIterator = *this; --_cur; return (oldIterator); };
			const_reverse_iterator	operator--(int) { const_reverse_iterator oldIterator = *this; ++_cur; return (oldIterator); };
			const_reverse_iterator	&operator++() { --_cur; return (*this); };
			const_reverse_iterator	&operator--() { ++_cur; return (*this); };
			const_reverse_iterator	operator+(difference_type n) const { return reverse_iterator(_cur - n); };
			const_reverse_iterator	operator-(difference_type n) const { return reverse_iterator(_cur + n); };
			const_reverse_iterator	operator+=(difference_type n) { _cur -= n; return (*this); };
			const_reverse_iterator	operator-=(difference_type n) { _cur += n; return (*this); };

			bool		operator==(const reverse_iterator& rhs) const { return (this->_cur == rhs.getIterator()); };
			bool		operator!=(const reverse_iterator& rhs) const { return (this->_cur != rhs.getIterator()); };
			bool		operator<(const reverse_iterator& rhs) const { return (this->_cur > rhs.getIterator()); };
			bool		operator<=(const reverse_iterator& rhs) const { return (this->_cur >= rhs.getIterator()); };
			bool		operator>(const reverse_iterator& rhs) const { return (this->_cur < rhs.getIterator()); };
			bool		operator>=(const reverse_iterator& rhs) const { return (this->_cur <= rhs.getIterator()); };

			bool		operator==(const const_reverse_iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!=(const const_reverse_iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator<(const const_reverse_iterator& rhs) const { return (this->_cur > rhs._cur); };
			bool		operator<=(const const_reverse_iterator& rhs) const { return (this->_cur >= rhs._cur); };
			bool		operator>(const const_reverse_iterator& rhs) const { return (this->_cur < rhs._cur); };
			bool		operator>=(const const_reverse_iterator& rhs) const { return (this->_cur <= rhs._cur); };

			const_reference	operator[](difference_type n) const { return *(_cur - n); };
			iterator getIterator() const { return (_cur); };
		};

	private:
		Alloc	_alloc;
		size_t	_sz;
		pointer	_elem;
		size_t	_space;
	};
	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return (false);
		return (true);
	};
	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{ return (!(lhs == rhs)); };
	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); };
	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{ return !(rhs < lhs); };
	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{ return (rhs < lhs); };
	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc>& rhs)
	{ return !(lhs < rhs); };
	template <class T, class Alloc>
	void swap(vector<T, Alloc> &x, vector<T, Alloc>&y) { x.swap(y); };
}
#endif //FT_CONTAINERS_VECTOR_HPP
