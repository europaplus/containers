//
// Created by Knot Fonda on 5/2/21.
//

#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP

# include "utils.hpp"

namespace ft {
	template <	class Key,
			class T,
			class Compare = std::less<Key>,
			class Alloc = std::allocator<std::pair<const Key, T> >
	> class map
	{
	public:
		typedef Key											key_type;
		typedef T											mapped_type;
		typedef std::pair<const key_type, mapped_type>		value_type;
		typedef Compare										key_compare;
		typedef Alloc										allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef ptrdiff_t 									diffrence_type;
		typedef size_t 										size_type;

		class value_compare : public std::binary_function<value_type, value_type, bool>
		{   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
			friend class map;
		protected:
			Compare comp;
			value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
		public:
			typedef bool result_type;
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator() (const value_type& x, const value_type& y) const
			{ return comp(x.first, y.first); };
		};
	public:
		class iterator;
		class const_iterator;
		class reverse_iterator;
		class const_reverse_iterator;
	private:
		typedef struct				_s_BinTreeNode
		{
			struct _s_BinTreeNode	*_parent;
			struct _s_BinTreeNode 	*_pLeft;
			struct _s_BinTreeNode	*_pRight;
			value_type 				*_data;
		}							_t_BinTreeNode;
		typedef typename Alloc::template rebind<_t_BinTreeNode>::other	alloc_type_list;
		typedef typename alloc_type_list::pointer						_pointer_node;
		typedef typename alloc_type_list::const_pointer         		_const_pointer_node;
		alloc_type_list 		_alloc_node;
		allocator_type 			_alloc;
		size_type				_sz;
		key_compare 			_comp;
		_pointer_node			_pRoot;
		_pointer_node 			_beginNode;
		_pointer_node 			_endNode;
		_pointer_node 			_emptyEnd;
		_t_BinTreeNode 			*_emptyBegin;
	public:
		//#########MEMBER FUNCTIONS:#########//
		explicit map (const key_compare &comp = key_compare(),
					  const allocator_type &alloc = allocator_type())
				: _alloc(alloc), _sz(0), _comp(comp) , _pRoot(nullptr), _beginNode(nullptr), _endNode(), _emptyEnd(nullptr)
		{
			_createNode(this->_emptyEnd);
			_createNode(this->_emptyBegin);
		};
		template <class InputIterator>
		map (InputIterator first, InputIterator last,
			 const key_compare &comp = key_compare(),
			 const allocator_type &alloc = allocator_type(),
			 typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
				: _alloc(alloc), _sz(0), _comp(comp) , _pRoot(nullptr), _beginNode(nullptr), _endNode(), _emptyEnd(nullptr)
		{
			_createNode(this->_emptyEnd);
			_createNode(this->_emptyBegin);
			insert(first, last);
		};
		~map ()
		{
			clear();
			_alloc_node.deallocate(this->_emptyEnd, 1);
			_alloc_node.deallocate(this->_emptyBegin, 1);
		};
		map (const map &x)
				: _alloc(x._alloc), _sz(0), _comp(x._comp) , _pRoot(nullptr), _beginNode(nullptr), _endNode(), _emptyEnd(nullptr)
		{
			_createNode(this->_emptyEnd);
			_createNode(this->_emptyBegin);
			insert(x.begin(), x.end());
		};
		map &operator= (const map &assign)
		{
			if (this == &assign)
				return (*this);
			clear();
			insert(assign.begin(), assign.end());
			return (*this);
		}
		//#########ITERATORS:#########//
		iterator begin () { if (this->_sz == 0) return (iterator(this->_emptyEnd)); else return ++iterator(this->_emptyBegin); };
		const_iterator begin () const { if (this->_sz == 0) return (const_iterator(this->_emptyEnd)); else return ++const_iterator(this->_emptyBegin); };
		iterator end () { return iterator(this->_emptyEnd); };
		const_iterator end () const { return const_iterator(this->_emptyEnd); };
		reverse_iterator rbegin () { if (this->_sz == 0) return reverse_iterator(iterator(this->_emptyBegin)); else  return ++reverse_iterator(iterator(this->_emptyEnd)); };
		const_reverse_iterator rbegin () const {if (this->_sz == 0) return const_reverse_iterator(iterator(this->_emptyBegin)); else  return ++const_reverse_iterator(iterator(this->_emptyEnd)); };
		reverse_iterator rend () { return reverse_iterator(iterator(this->_emptyBegin)); };
		const_reverse_iterator rend () const { return const_reverse_iterator(iterator(this->_emptyBegin)); };
		//#########CAPACITY:#########//
		size_type size () const { return (this->_sz); };
		bool empty () { return (this->_sz == 0); };
		size_type max_size () { return (this->_alloc.max_size()/sizeof(ft::map<key_type, mapped_type>)); };
		//#########MODIFIERS:#########//
		std::pair<iterator, bool> insert (const value_type &val)
		{
			std::pair<typename map<key_type, T>::iterator, bool> ret_val;
			ret_val = _insert_imp(this->_pRoot, val, this->_comp);
			_setBound();
			return (ret_val);
		};
		iterator insert (iterator position, const value_type &val)
		{
			static_cast<void>(position);
			return (insert(val).first);
		};
		template <class InputIterator>
		void insert (InputIterator first, InputIterator second,
					 typename ft::enable_if<std::__is_input_iterator<InputIterator>::value, T>::type* = 0)
		{
			while (first != second)
			{
				insert(*first);
				++first;
			}
		}
		void erase (iterator position)
		{
			if (this->_pRoot)
			{
				this->_pRoot = _erase_imp(this->_pRoot, position->first);
				_setBound();
			}
		};
		size_type erase (const key_type &k)
		{
			size_type tmpSize = this->_sz;
			this->_pRoot = _erase_imp(this->_pRoot, k);
			_setBound();
			return (tmpSize - this->_sz);
		};
		void erase (iterator first, iterator last)
		{
			while (first != last)
				erase(first++);
		};
		void swap (map& x)
		{
			allocator_type tmpAlloc = this->_alloc;
			size_type tmpSz = this->_sz;
			_t_BinTreeNode *tmpBeginNode = this->_beginNode;
			_t_BinTreeNode *tmpEndNode = this->_endNode;
			_t_BinTreeNode *tmpEmptyBegin = this->_emptyBegin;
			_t_BinTreeNode *tmpEmptyEnd = this->_emptyEnd;
			_t_BinTreeNode *tmpRoot = this->_pRoot;
			key_compare tmpKeyCompare = this->_comp;

			this->_alloc = x._alloc;
			this->_sz = x._sz;
			this->_beginNode = x._beginNode;
			this->_endNode = x._endNode;
			this->_emptyBegin = x._emptyBegin;
			this->_emptyEnd = x._emptyEnd;
			this->_pRoot = x._pRoot;
			this->_comp = x._comp;

			x._alloc = tmpAlloc;
			x._sz = tmpSz;
			x._beginNode = tmpBeginNode;
			x._endNode = tmpEndNode;
			x._emptyBegin = tmpEmptyBegin;
			x._emptyEnd = tmpEmptyEnd;
			x._pRoot = tmpRoot;
			x._comp = tmpKeyCompare;
		};
		void clear ()
		{
			for (; 0 != this->_sz;)
				erase(this->begin());
		};
		//#########OBSERVERS:#########//
		key_compare key_comp () const { return (this->_comp); };
		value_compare value_comp () const { return (value_compare(this->_comp)); };
		//#########ELEMENT ACCESS:#########//
		mapped_type& operator[] (const key_type& k)
		{ return ((*((this->insert(std::make_pair(k,mapped_type()))).first)).second); };
		//#########OPERATIONS:#########//
		iterator find (const key_type &k)
		{
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			iterator first = this->begin();
			iterator last = this->end();
			for ( ; first != last; ++first)
				if (first->first == k)
					return (first);
			return (first);
		};
		const_iterator find (const key_type &k) const
		{
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			const_iterator first = this->begin();
			const_iterator last = this->end();
			for ( ; first != last; ++first)
				if (first->first == k)
					return (first);
			return (first);
		};
		size_type count (const key_type &k) const
		{
			const_iterator first = this->begin();
			const_iterator last = this->end();
			for ( ; first != last; ++first)
				if (first->first == k)
					return (1);
			return (0);
		}
		iterator lower_bound(const key_type &k)
		{
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			iterator first = this->begin();
			iterator last = this->end();
			for ( ; first != last; ++first)
			{
				if (!(_comp(first->first, k)))
					return (first);
			}
			return (this->end());
		};
		const_iterator lower_bound (const key_type &k) const
		{
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			const_iterator first = this->begin();
			const_iterator last = this->end();
			for ( ; first != last; ++first)
			{
				if (!(_comp(first->first, k)))
					return (first);
			}
			return (this->end());
		};
		iterator upper_bound(const key_type &k)
		{
			iterator first = this->begin();
			iterator last = this->end();
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			for ( ; first != last; ++first)
			{
				if ((_comp(k, first->first)))
					return (first);
			}
			return (this->end());
		};
		const_iterator upper_bound (const key_type &k) const
		{
			if (this->_beginNode == nullptr || this->_endNode == nullptr)
				return (iterator(this->_emptyEnd));
			const_iterator first = this->begin();
			const_iterator last = this->end();
			for ( ; first != last; ++first)
			{
				if ((_comp(k, first->first)))
					return (first);
			}
			return (this->end());
		};
		std::pair<const_iterator, const_iterator> equal_range (const key_type &k) const
		{ return (std::make_pair(lower_bound(k), upper_bound(k))); };
		std::pair<iterator, iterator> equal_range (const key_type &k)
		{ return (std::make_pair(lower_bound(k), upper_bound(k))); };
	private:
		_t_BinTreeNode* _balance(_pointer_node &tmp)
		{
			int bal_factor = _diff(tmp);
			if (bal_factor > 1)
				tmp = (_diff(tmp->_pLeft) > 0 ? _ll_rotation(tmp) : _lr_rotation(tmp));
			else if (bal_factor < -1)
				tmp = (_diff(tmp->_pRight) > 0 ? _rl_rotation(tmp) : _rr_rotation(tmp));
			return (tmp);
		}
		int _diff (_pointer_node _node)
		{
			int l_depth = _depth(_node->_pLeft);
			int r_depth = _depth(_node->_pRight);
			int b_factor = l_depth - r_depth;
			return (b_factor);
		}
		int	_depth (_pointer_node _node)
		{
			int depth = 0;

			if (_node != nullptr && _node != this->_emptyEnd && _node != this->_emptyBegin)
			{
				int l_depth = _depth(_node->_pLeft);
				int r_depth = _depth(_node->_pRight);
				int max_depth = l_depth > r_depth ? l_depth : r_depth;
				depth = max_depth + 1;
			}
			return (depth);
		}
		_pointer_node _ll_rotation (_pointer_node apNode)
		{
			_t_BinTreeNode *tmp = apNode->_pLeft;
			tmp->_parent = apNode->_parent;
			apNode->_parent = apNode->_pLeft;
			if (apNode == this->_pRoot)
				this->_pRoot = apNode->_pLeft;
			apNode->_pLeft = tmp->_pRight;
			if (tmp->_pRight)
				tmp->_pRight->_parent = apNode;
			tmp->_pRight = apNode;
			return (tmp);
		}
		_pointer_node _rr_rotation (_pointer_node apNode)
		{
			_t_BinTreeNode *tmp = apNode->_pRight;
			tmp->_parent = apNode->_parent;
			apNode->_parent = apNode->_pRight;
			if (apNode == this->_pRoot)
				this->_pRoot = apNode->_pRight;
			apNode->_pRight = tmp->_pLeft;
			if (tmp->_pLeft)
				tmp->_pLeft->_parent = apNode;
			tmp->_pLeft = apNode;
			return (tmp);
		}
		_pointer_node _lr_rotation (_pointer_node apNode)
		{
			_t_BinTreeNode *tmp = apNode->_pLeft;
			apNode->_pLeft = _rr_rotation(tmp);
			return (_ll_rotation(apNode));
		}
		_pointer_node _rl_rotation (_pointer_node apNode)
		{
			_t_BinTreeNode *tmp = apNode->_pRight;
			apNode->_pRight = _ll_rotation(tmp);
			return (_rr_rotation(apNode));
		}
		std::pair<iterator, bool> _insert_imp (_pointer_node &_apTree, const value_type &apLeaf
				, const key_compare &comp)
		{
			std::pair<typename map<key_type, T>::iterator, bool> ret_val;

			if (_apTree == nullptr || _apTree == this->_emptyEnd || _apTree == this->_emptyBegin)
			{
				_createNode(_apTree, apLeaf);
				ret_val = std::make_pair(iterator(_apTree), true);
				++this->_sz;
				return (ret_val);
			}
			else
			{
				if (comp(apLeaf.first, _apTree->_data->first))
				{
					ret_val = _insert_imp(_apTree->_pLeft, apLeaf, comp);
					_apTree->_pLeft->_parent = _apTree;
					_apTree = _balance(_apTree);
					return (ret_val);
				}
				if (comp(_apTree->_data->first, apLeaf.first))
				{
					ret_val = _insert_imp(_apTree->_pRight, apLeaf, comp);
					_apTree->_pRight->_parent = _apTree;
					_apTree = _balance(_apTree);
					return (ret_val);
				}
			};
			ret_val = std::make_pair(iterator(_apTree), false);
			return (ret_val);
		};
		_pointer_node _getBegin (_pointer_node cur)
		{ return (cur->_pLeft && cur->_pLeft != this->_emptyBegin ? _getBegin(cur->_pLeft) : cur); };
		_pointer_node _getEnd (_pointer_node cur)
		{ return (cur->_pRight && cur->_pRight != this->_emptyEnd ? _getEnd(cur->_pRight) : cur); };
		void _setBound ()
		{
			if (this->_pRoot)
			{
				if (this->_sz != 0)
				{
					this->_beginNode = _getBegin(this->_pRoot);
					this->_beginNode->_pLeft = this->_emptyBegin;
					this->_emptyBegin->_parent = this->_beginNode;
					this->_endNode = _getEnd(this->_pRoot);
					this->_endNode->_pRight = this->_emptyEnd;
					this->_emptyEnd->_parent = this->_endNode;
				}
				else
					this->_pRoot = nullptr;
			}
			else
			{
				this->_emptyEnd->_parent = nullptr;
				this->_emptyBegin->_parent = nullptr;
				this->_beginNode = this->_emptyBegin;
				this->_endNode = this->_emptyEnd;
			}
		};
		_pointer_node _createNode (_t_BinTreeNode *&_apTree, const value_type &apLeaf = value_type())
		{
			_apTree = _alloc_node.allocate(1);
			_apTree->_data = _alloc.allocate(1);
			_alloc.construct(_apTree->_data, apLeaf);
			_apTree->_pLeft = nullptr;
			_apTree->_pRight = nullptr;
			_apTree->_parent = nullptr;
			return (_apTree);
		};
		_t_BinTreeNode *_erase_imp(_t_BinTreeNode *node, const key_type &k)
		{
			if (!node)
				return (nullptr);
			if (k < node->_data->first)
				node->_pLeft = _erase_imp(node->_pLeft, k);
			else if (k > node->_data->first)
				node->_pRight = _erase_imp(node->_pRight, k);
			else
			{
				_t_BinTreeNode *q = node->_pLeft;
				_t_BinTreeNode *r = node->_pRight;
				_t_BinTreeNode *tmpParent = node->_parent;
				_deleteNode(node);
				if (!r || r == this->_emptyEnd)
				{
					if (q)
						q->_parent = tmpParent;
					return (q);
				}
				_t_BinTreeNode *min = _getBegin(r);
				min->_pRight = _removeMin(r);
				min->_pLeft = q;
				if (min->_pLeft)
					min->_pLeft->_parent = min;
				min->_parent = tmpParent;
				return (_balance(min));
			}
			return (_balance(node));
		};
		_t_BinTreeNode *_removeMin(_t_BinTreeNode *p)
		{
			if (p->_pLeft == 0)
				return (p->_pRight);
			p->_pLeft = _removeMin(p->_pLeft);
			return (_balance(p));
		}
		void _deleteNode(_t_BinTreeNode *deleteNode)
		{
			_alloc.destroy(deleteNode->_data);
			_alloc.deallocate(deleteNode->_data, 1);
			_alloc_node.deallocate(deleteNode, 1);
			--this->_sz;
		}
	public:
		class iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
		private:
			_pointer_node _cur;
		public:
			iterator () : _cur(nullptr) {};
			explicit iterator (_pointer_node cur) { this->_cur = cur; };
			iterator (iterator const &copy) { *this = copy; };
			iterator &operator=(const iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			~iterator () {};
			reference operator*() { return *(this->_cur->_data); };
			const_reference operator*() const { return *(this->_cur->_data); };
			pointer operator->() { return (this->_cur->_data); };
			const_pointer operator->() const { return (this->_cur->_data); };
			iterator	&operator++ () { _cur = _incNode(_cur); return (*this); };
			iterator	&operator-- () { _cur = _decNode(_cur); return (*this); };
			iterator	operator++ (int) { iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			iterator	operator-- (int) { iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!= (const iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator== (const const_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			_pointer_node 	getPointer() const { return (_cur); };
		private:
			_pointer_node _incNode(_pointer_node cur)
			{
				_pointer_node tmp = cur;
				if (!cur->_pRight)
				{
					while (tmp->_parent && tmp == tmp->_parent->_pRight)
						tmp = tmp->_parent;
					tmp = tmp->_parent;
				}
				else
				{
					tmp = cur->_pRight;
					while (tmp->_pLeft)
						tmp = tmp->_pLeft;
				}
				return (tmp);
			};
			_pointer_node _decNode(_pointer_node cur)
			{
				_pointer_node tmp = cur;
				if (!cur->_pLeft)
				{
					while (tmp->_parent && tmp == tmp->_parent->_pLeft)
						tmp = tmp->_parent;
					tmp = tmp->_parent;
				}
				else
				{
					tmp = cur->_pLeft;
					while (tmp->_pRight)
						tmp = tmp->_pRight;
				}
				return (tmp);
			};
		};
		class const_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type const>
		{
		private:
			_pointer_node _cur;
		public:
			const_iterator () : _cur(nullptr) {};
			explicit const_iterator (_pointer_node cur) { this->_cur = cur; };
			const_iterator (const_iterator const &copy) { *this = copy; };
			const_iterator (iterator const &copy) { *this = copy; };
			const_iterator &operator=(const const_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			const_iterator &operator=(const iterator &assign) { this->_cur = assign.getPointer(); return (*this);}
			~const_iterator () {};
			const_reference operator*() const { return *(this->_cur->_data); };
			const_pointer operator->() const { return (this->_cur->_data); };
			const_iterator	&operator++ () { _cur = _incNode(_cur); return (*this); };
			const_iterator	&operator-- () { _cur = _decNode(_cur); return (*this); };
			const_iterator	operator++ (int) { const_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			const_iterator	operator-- (int) { const_iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator== (const const_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			_pointer_node 	getPointer() const { return (_cur); };
		private:
			_pointer_node _incNode(_pointer_node cur)
			{
				_pointer_node tmp = cur;
				if (!cur->_pRight)
				{
					while (tmp->_parent && tmp == tmp->_parent->_pRight)
						tmp = tmp->_parent;
					tmp = tmp->_parent;
				}
				else
				{
					tmp = cur->_pRight;
					while (tmp->_pLeft)
						tmp = tmp->_pLeft;
				}
				return (tmp);
			};
			_pointer_node _decNode(_pointer_node cur)
			{
				_pointer_node tmp = cur;
				if (!cur->_pLeft)
				{
					while (tmp->_parent && tmp == tmp->_parent->_pLeft)
						tmp = tmp->_parent;
					tmp = tmp->_parent;
				}
				else
				{
					tmp = cur->_pLeft;
					while (tmp->_pRight)
						tmp = tmp->_pRight;
				}
				return (tmp);
			};
		};
		class reverse_iterator : public std::reverse_iterator<map::iterator>
		{
		private:
			iterator _cur;
		public:
			reverse_iterator () : _cur(nullptr) {};
			explicit reverse_iterator (iterator cur) { this->_cur = cur; };
			reverse_iterator (reverse_iterator const &copy) { *this = copy; };
			reverse_iterator &operator=(const reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			~reverse_iterator () {};
			reference operator*() { return *(this->_cur); };
			pointer operator->() { return (this->_cur.operator->()); };
			reverse_iterator	&operator++ () { --_cur; return (*this); };
			reverse_iterator	&operator-- () { ++_cur; return (*this); };
			reverse_iterator	operator++ (int) { reverse_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			reverse_iterator	operator-- (int) { reverse_iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const reverse_iterator& rhs) const { return (this->_cur == rhs._cur); };
			bool		operator!= (const reverse_iterator& rhs) const { return (this->_cur != rhs._cur); };
			bool		operator== (const const_reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			iterator 	getPointer() const { return (_cur); };
		};
		class const_reverse_iterator : public std::reverse_iterator<map::iterator>
		{
		private:
			iterator _cur;
		public:
			const_reverse_iterator () : _cur(nullptr) {};
			explicit const_reverse_iterator (reverse_iterator cur) { this->_cur = cur; };
			const_reverse_iterator (const_reverse_iterator const &copy) { *this = copy; };
			const_reverse_iterator (reverse_iterator const &copy) { *this = copy; };
			const_reverse_iterator &operator=(const const_reverse_iterator &assign) { if (this == &assign) return (*this); this->_cur = assign._cur; return (*this);}
			const_reverse_iterator &operator=(const reverse_iterator &assign) { this->_cur = assign.getPointer(); return (*this);}
			~const_reverse_iterator () {};
			const_reference operator*() const { return *(this->_cur); };
			const_pointer operator->() const { return this->_cur.operator->(); };
			const_reverse_iterator	&operator++ () { --_cur; return (*this); };
			const_reverse_iterator	&operator-- () { ++_cur; return (*this); };
			const_reverse_iterator	operator++ (int) { const_reverse_iterator oldIterator = *this; this->operator++(); return (oldIterator); };
			const_reverse_iterator	operator-- (int) { const_reverse_iterator oldIterator = *this; this->operator--(); return (oldIterator); };

			bool		operator== (const const_reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const const_reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			bool		operator== (const reverse_iterator& rhs) const { return (this->_cur == rhs.getPointer()); };
			bool		operator!= (const reverse_iterator& rhs) const { return (this->_cur != rhs.getPointer()); };
			iterator 	getPointer() const { return (_cur); };
		};
	};
	template <class Key, class T>
	bool operator==(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::map<Key, T>::const_iterator lhsIt = lhs.begin();
		typename ft::map<Key, T>::const_iterator rhsIt = rhs.begin();
		for (size_t i = 0; i < lhs.size(); i++, lhsIt++, rhsIt++)
			if (lhsIt->first != rhsIt->first || lhsIt->second != rhsIt->second)
				return (false);
		return (true);
	};
	template <class Key, class T>
	bool operator!=(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{ return (!(lhs == rhs)); };
	template <class Key, class T>
	bool operator<(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); };
	template <class Key, class T>
	bool operator<=(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{ return !(rhs < lhs); };
	template <class Key, class T>
	bool operator>(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{ return (rhs < lhs); };
	template <class Key, class T>
	bool operator>=(const map<Key, T> &lhs, const map<Key, T>& rhs)
	{ return !(lhs < rhs); };
	template <class Key, class T>
	void swap(map<Key, T> &x, map<Key, T>&y) { x.swap(y); };
};

#endif //FT_CONTAINERS_MAP_HPP
