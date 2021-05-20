//
// Created by Knot Fonda on 5/2/21.
//

#ifndef FT_CONTAINERS_LIST_HPP
#define FT_CONTAINERS_LIST_HPP

# include "utils.hpp"
# include <iterator>

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class list
	{
	public:
		class iterator;
		class const_iterator;
		class reverse_iterator;
		class const_reverse_iterator;
	protected:
		typedef T											value_type;
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef ptrdiff_t									difference_type;
		typedef size_t										size_type;
	private:
		typedef struct _s_node
		{
			struct _s_node *_next;
			struct _s_node *_prev;
			value_type 		*_content;
		}				_t_node;
		allocator_type 	_alloc;
		_t_node			*_head;
		_t_node			*_tail;
		size_type		_sz;
		typedef typename Alloc::template rebind<_s_node>::other	alloc_type_list;
		typedef typename alloc_type_list::pointer				_ptr_node;
		typedef typename alloc_type_list::const_pointer         _const_ptr_node;
		alloc_type_list 										_alloc_node;
	public:
		explicit list (const allocator_type &alloc = allocator_type())
				: _alloc(alloc), _head(nullptr), _tail(nullptr), _sz(0)
		{ createEmptyNode(); };
		explicit list (size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
				: _alloc(alloc), _head(nullptr), _tail(nullptr), _sz(0)
		{
			createEmptyNode();
			for (size_type size = 0; size < n; ++size)
				push_front(val);
		};
		template <class InputIterator>
		list (InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
			  typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
				: _alloc(alloc), _sz(0)
		{
			createEmptyNode();
			insert(this->begin(), first, last);
		};
		list (const list &copy)
				: _alloc(copy._alloc), _sz(0)
		{
			createEmptyNode();
			insert(this->begin(), copy.begin(), copy.end());
		};
		list &operator= (const list &assign)
		{
			if (this == &assign)
				return (*this);
			if (this->_sz)
				clear();
			insert(this->begin(), assign.begin(), assign.end());
			return (*this);
		};
		//#############ITERATORS:#############//
		~list () { clear(); _alloc_node.deallocate(this->_tail, 1); };
		iterator begin () { return iterator(this->_head); };
		const_iterator begin () const { return const_iterator(this->_head); };
		iterator end () { return iterator(this->_tail); };
		const_iterator end () const { return const_iterator(this->_tail); };
		reverse_iterator rbegin() { return reverse_iterator(--this->end()); };
		const_reverse_iterator rbegin() const { return const_reverse_iterator(--this->end()); };
		reverse_iterator rend() { return reverse_iterator(--this->begin()); };
		const_reverse_iterator rend() const { return const_reverse_iterator(--this->begin()); };
		//#############CAPACITY:#############//
		bool empty () const { return (this->_sz == 0); };
		size_type size () const { return (this->_sz); };
		size_type max_size () const { return (std::allocator<T>::max_size()); };
		//#############ELEMENT ACCESS:#############//
		reference front () { return *(this->_head->_content); };
		const_reference  front() const { return *(this->_head->_content); };
		reference  back () { return *(this->_tail->_prev->_content); };
		const_reverse_iterator back () const { return *(this->_tail->_prev->_content); };
		//#############MODIFIERS:#############//
		template <class InputIterator>
		void assign (InputIterator first, InputIterator last,
					 typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
		{
			clear();
			createEmptyNode();
			insert(this->begin(), first, last);
		};
		void assign (size_type n, const value_type &val)
		{
			clear();
			createEmptyNode();
			insert(this->begin(), n, val);
		};
		void push_front (const value_type &val)
		{ insertHead(createNode(val)); };
		void pop_front ()
		{ erase(this->begin()); };
		void pop_back ()
		{ erase( iterator(this->end().getPointer()->_prev)); };
		void push_back (const value_type &val)
		{
			if (this->_tail->_prev == this->_tail)
				insertHead(createNode(val));
			else
				insertRandom(this->_tail, createNode(val));
		};
		iterator insertBase(_t_node *target, _t_node *newNode)
		{
			if (target->_prev == this->_tail)
				insertHead(newNode);
			else
				insertRandom(target, newNode);
			return (iterator(newNode));
		};
		iterator insert (iterator position, const value_type &val)
		{ return (insertBase(position.getPointer(), createNode(val))); };
		void insert (iterator position, size_type n, const value_type &val)
		{
			for (size_type i = 0; i < n; ++i)
				insert(position, val);
		};
		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last,
					 typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
		{
			for (; first != last; ++first)
				insert(position, *first);
		};
		void clear ()
		{
			_t_node *head = this->_tail->_next;
			_t_node *tmpNode;
			while (head != this->_tail)
			{
				tmpNode = head->_next;
				deleteNode(head);
				head = tmpNode;
			}
			this->_sz = 0;
		};
		iterator erase (iterator position)
		{
			iterator tmp = (++position)--;
			deleteNode(position.getPointer());
			--this->_sz;
			return (tmp);
		};
		iterator erase (iterator first, iterator last)
		{
			while (first != last)
				first = erase(first);
			return (first);
		};
		void swap (list &x)
		{
			allocator_type tmpAlloc = this->_alloc;
			size_type tmpSz = this->_sz;
			_ptr_node tmpHead = this->_tail->_next;
			_ptr_node tmpTail = this->_tail;

			this->_alloc = x._alloc;
			this->_sz = x._sz;
			this->_head = x._head;
			this->_tail = x._tail;

			x._alloc = tmpAlloc;
			x._sz = tmpSz;
			x._head = tmpHead;
			x._tail = tmpTail;
		};
		void resize (size_type n, value_type val = value_type())
		{
			for (; n < this->_sz;)
				erase(iterator(this->_tail->_prev));
			if (n > this->_sz)
				insert(this->end(), (n - this->_sz), val);
		};
		//#############OPERATIONS:#############//
		void splice (iterator position, list &x)
		{ splice(position, x, x.begin(), x.end()); };
		void splice (iterator position, list &x, iterator i)
		{
			x.unlinkNode(i.getPointer());
			insertBase(position.getPointer(), i.getPointer());
			--x._sz;
		};
		void splice (iterator position, list &x, iterator first, iterator last)
		{
			iterator tmp;
			for (; first != last;)
			{
				tmp = (++first)--;
				x.unlinkNode(first.getPointer());
				insertBase(position.getPointer(), first.getPointer());
				first = tmp;
				--x._sz;
			}
		};
		void remove (const value_type &val)
		{
			for (iterator first = this->begin(); first != this->end(); ++first)
				if (*(first) == val)
					erase(first);
		};
		template<class Predicate>
		void remove_if (Predicate pred)
		{
			for (iterator first = this->begin(); first != this->end(); ++first)
				if (pred(*(first)))
					first = erase(first);
		};
		void unique()
		{
			iterator first = this->begin();
			iterator last = this->end();
			iterator tmp;
			while (first != last)
			{
				tmp = first++;
				if (*tmp == *first)
					erase(tmp);
			}
		};
		template <class BinaryPredicate>
		void unique (BinaryPredicate binary_pred)
		{
			iterator i = ++this->begin();
			iterator predI = this->begin();
			while (i != this->end())
			{
				if (binary_pred(*predI, *i))
					i = erase(i);
				else
					predI = i++;
			}
		};
		void merge (list &x)
		{
			if (&x == this)
				return ;
			merge(x, less());
		};
		template <class Compare>
		void merge (list &x, Compare comp)
		{
			if (&x == this)
				return ;
			iterator itF = this->begin();
			iterator itFe = this->end();
			iterator itS = x.begin();
			iterator itSe = x.end();
			iterator tmp;
			while (itS != itSe)
			{
				while (itF != itFe && !comp(*itS, *itF))
					++itF;
				tmp = (++itS)--;
				splice(itF, x, itS);
				itS = tmp;
			}
		};
		void sort () { sort(less()); };
		template <class Compare>
		void sort (Compare comp)
		{
			iterator first = this->begin();
			iterator second;
			iterator tmp = this->end();
			while (this->begin() != tmp)
			{
				first = this->begin();
				for (second = ++this->begin(); second != tmp; ++second)
				{
					if (!comp(*first, *second))
					{
						splice(first, *this, second);
						iterator tmpIt = first;
						first = second;
						second = tmpIt;
					}
					++first;
				}
				tmp = first;
			}
		};
		void reverse()
		{
			iterator first = this->begin();
			iterator last = this->end();
			while (first != last && first != --last)
			{
				value_type tmp = *first;
				*first = *last;
				*last = tmp;
				first++;
			}
		};
		class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
		private:
			_t_node *_cur;
		public:
			iterator () : _cur(nullptr) {};
			explicit iterator (_t_node *cur) { this->_cur = cur; };
			iterator (iterator const &copy) { *this = copy; };
			iterator &operator=(const iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			~iterator () {};
			reference	operator* () { return (*(this->_cur->_content)); };
			const_reference	operator* () const { return (*(this->_cur->_content)); };
			pointer		operator-> () { return (this->_cur->_content); };
			const_pointer		operator-> () const { return (this->_cur->_content); };
			iterator	&operator++ () { _cur = _cur->_next; return (*this); };
			iterator	&operator-- () { _cur = _cur->_prev; return (*this); };
			iterator	operator++ (int) { iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			iterator	operator-- (int) { iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!= (const iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator== (const const_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };

			_ptr_node 	getPointer() const { return (_cur); };
		};

		class const_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
		private:
			_t_node *_cur;
		public:
			const_iterator () : _cur(nullptr) {};
			explicit const_iterator (_t_node *cur) { this->_cur = cur; };
			const_iterator (const_iterator const &copy) { *this = copy; };
			const_iterator (iterator const &copy) { *this = copy; };
			const_iterator &operator=(const const_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			const_iterator &operator=(const iterator &assign) { this->_cur = assign.getPointer(); return (*this);}
			~const_iterator () {};
			const_reference	operator* () const { return (*(this->_cur->_content)); };
			const_pointer	operator-> () { return (this->_cur->_content); };
			const_iterator	&operator++ () { _cur = _cur->_next; return (*this); };
			const_iterator	&operator-- () { _cur = _cur->_prev; return (*this); };
			const_iterator	operator++ (int) { const_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			const_iterator	operator-- (int) { const_iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator== (const const_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };

			_ptr_node 	getPointer() const { return (_cur); };
		};

		class reverse_iterator : public std::reverse_iterator<list::iterator>
		{
		private:
			iterator _cur;
		public:
			reverse_iterator () {};
			explicit reverse_iterator(iterator cur) : _cur(cur) {};
			reverse_iterator(const reverse_iterator &copy) { *this = copy; };
			reverse_iterator &operator=(const reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this); }
			~reverse_iterator() {};
			const_reference	operator*() { return (*(this->_cur)); };
			const_pointer	operator->() { return (this->_cur); };
			reverse_iterator	operator++(int) { reverse_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			reverse_iterator	operator--(int) { reverse_iterator oldIterator = *this; this->operator--(); return (oldIterator); };
			reverse_iterator	&operator++() { --_cur; return (*this); };
			reverse_iterator	&operator--() { ++_cur; return (*this); };

			bool		operator==(const reverse_iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!=(const reverse_iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator<(const reverse_iterator& rhs) const { return (this->_cur < rhs._cur); };
			bool		operator<=(const reverse_iterator& rhs) const { return (this->_cur <= rhs._cur); };
			bool		operator>(const reverse_iterator& rhs) const { return (this->_cur > rhs._cur); };
			bool		operator>=(const reverse_iterator& rhs) const { return (this->_cur >= rhs._cur); };

			bool		operator==(const const_reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!=(const const_reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator<(const const_reverse_iterator& rhs) const { return (this->_cur > rhs.getPointer()); };
			bool		operator<=(const const_reverse_iterator& rhs) const { return (this->_cur >= rhs.getPointer()); };
			bool		operator>(const const_reverse_iterator& rhs) const { return (this->_cur < rhs.getPointer()); };
			bool		operator>=(const const_reverse_iterator& rhs) const { return (this->_cur <= rhs.getPointer()); };

			iterator 	getPointer() const { return (_cur); };
		};

		class const_reverse_iterator : public std::reverse_iterator<list::iterator>
		{
		private:
			iterator _cur;
		public:
			const_reverse_iterator () : _cur(nullptr) {};
			explicit const_reverse_iterator(reverse_iterator &cur) : _cur(cur) {};
			const_reverse_iterator(const const_reverse_iterator &copy) { *this = copy; };
			const_reverse_iterator(const reverse_iterator &copy) { *this = copy; };
			const_reverse_iterator &operator=(const const_reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this); }
			const_reverse_iterator &operator=(const reverse_iterator &assign) { this->_cur = assign.getPointer(); return (*this); }
			~const_reverse_iterator() {};
			const_reference	operator*() { return (*(this->_cur)); };
			const_pointer	operator->() { return (this->_cur); };
			const_reverse_iterator	operator++(int) { const_reverse_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			const_reverse_iterator	operator--(int) { const_reverse_iterator oldIterator = *this; this->operator--(); return (oldIterator); };
			const_reverse_iterator	&operator++() { --_cur; return (*this); };
			const_reverse_iterator	&operator--() { ++_cur; return (*this); };

			bool		operator==(const reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!=(const reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator<(const reverse_iterator& rhs) const { return (this->_cur < rhs.getPointer()); };
			bool		operator<=(const reverse_iterator& rhs) const { return (this->_cur <= rhs.getPointer()); };
			bool		operator>(const reverse_iterator& rhs) const { return (this->_cur > rhs.getPointer()); };
			bool		operator>=(const reverse_iterator& rhs) const { return (this->_cur >= rhs.getPointer()); };

			bool		operator==(const const_reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!=(const const_reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator<(const const_reverse_iterator& rhs) const { return (this->_cur > rhs.getPointer()); };
			bool		operator<=(const const_reverse_iterator& rhs) const { return (this->_cur >= rhs.getPointer()); };
			bool		operator>(const const_reverse_iterator& rhs) const { return (this->_cur < rhs.getPointer()); };
			bool		operator>=(const const_reverse_iterator& rhs) const { return (this->_cur <= rhs.getPointer()); };
			iterator 	getPointer() const { return (_cur); };
		};
	private:
		void createEmptyNode()
		{
			this->_tail = _alloc_node.allocate(1);
			this->_tail->_prev = this->_tail;
			this->_tail->_next = this->_tail;
			this->_tail->_content = _alloc.allocate(1);
			this->_head = this->_tail;
		}
		_ptr_node createNode (const value_type &val)
		{
			_ptr_node tmpNode = _alloc_node.allocate(1);
			tmpNode->_content = _alloc.allocate(1);
			_alloc.construct(tmpNode->_content, val);
			return (tmpNode);
		};
		void insertHead(_t_node *tmpNode)
		{
			if (this->_tail->_next == this->_tail)
			{
				tmpNode->_next = this->_tail;
				this->_tail->_prev = tmpNode;
				this->_head = tmpNode;
				tmpNode->_prev = this->_tail;
				this->_tail->_next = this->_head;
			}
			else
			{
				tmpNode->_prev = this->_tail;
				this->_tail->_next = tmpNode;
				tmpNode->_next = this->_head;
				this->_head->_prev = tmpNode;
				this->_head = tmpNode;
			}
			++this->_sz;
		};
		void unlinkNode(_t_node *unlinkNode)
		{
			if (this->_sz == 1)
			{
				this->_tail->_prev = this->_tail;
				this->_tail->_next = this->_tail;
				this->_head = this->_tail;
			}
			else if (unlinkNode == this->begin().getPointer())
			{
				this->_head = unlinkNode->_next;
				this->_head->_prev = this->_tail;
				this->_tail->_next = this->_head;
			}
			else
			{
				unlinkNode->_next->_prev = unlinkNode->_prev;
				unlinkNode->_prev->_next = unlinkNode->_next;
			}
		}
		void deleteNode(_t_node *deleteNode)
		{
			unlinkNode(deleteNode);
			_alloc.destroy(deleteNode->_content);
			_alloc.deallocate(deleteNode->_content, 1);
			_alloc_node.destroy(deleteNode);
			_alloc_node.deallocate(deleteNode, 1);
		};
		void insertRandom(_t_node *target, _t_node *newNode)
		{
			newNode->_next = target;
			newNode->_prev = target->_prev;
			target->_prev->_next = newNode;
			target->_prev = newNode;
			++this->_sz;
		};
		struct less
		{ bool operator()(value_type const &first, value_type const &second) { return (first < second); }; };
	};
	template <class T, class Alloc>
	bool operator==(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::list<T>::const_iterator lhsIt = lhs.begin();
		typename ft::list<T>::const_iterator rhsIt = rhs.begin();
		for (size_t i = 0; i < lhs.size(); i++, lhsIt++, rhsIt++)
			if (*(lhsIt.getPointer()->_content) != *(rhsIt.getPointer()->_content))
				return (false);
		return (true);
	};
	template <class T, class Alloc>
	bool operator!=(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{ return (!(lhs == rhs)); };
	template <class T, class Alloc>
	bool operator<(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); };
	template <class T, class Alloc>
	bool operator<=(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{ return !(rhs < lhs); };
	template <class T, class Alloc>
	bool operator>(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{ return (rhs < lhs); };
	template <class T, class Alloc>
	bool operator>=(const list<T, Alloc> &lhs, const list<T, Alloc>& rhs)
	{ return !(lhs < rhs); };
	template <class T, class Alloc>
	void swap(list<T, Alloc> &x, list<T, Alloc>&y) { x.swap(y); };
}

#endif //FT_CONTAINERS_LIST_HPP
