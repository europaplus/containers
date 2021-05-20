//
// Created by Knot Fonda on 5/2/21.
//

#ifndef FT_CONTAINERS_QUEUE_HPP
#define FT_CONTAINERS_QUEUE_HPP

# include "list.hpp"

namespace ft {
	template <class T, class Container = ft::list<T> >
	class queue
	{
	public:
		typedef Container                                	container_type;
		typedef T											value_type;
		typedef size_t 										size_type;
	protected:
		container_type c;
	public:
		explicit queue (const container_type &ctnr = container_type()) : c(ctnr) {};
		bool empty() { return (c.empty()); };
		size_type size () const { return (c.size()); };
		value_type &front () { return (c.front()); };
		const value_type  &front () const { return (c.front()); };
		value_type &back () { return (c.back()); };
		const value_type  &back () const { return (c.back()); };
		void push (const value_type &val) { c.push_back(val); };
		void pop() { c.pop_front(); };
	private:
		template <class TT, class CContainer>
		friend bool operator== (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
		template <class TT, class CContainer>
		friend bool operator!= (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
		template <class TT, class CContainer>
		friend bool operator<  (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
		template <class TT, class CContainer>
		friend bool operator<= (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
		template <class TT, class CContainer>
		friend bool operator>  (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
		template <class TT, class CContainer>
		friend bool operator>= (const ft::queue<TT,CContainer>& lhs, const ft::queue<TT,CContainer>& rhs);
	};
	template <class T, class Container>
	bool operator== (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{ return (lhs.c == rhs.c); };
	template <class T, class Container>
	bool operator!= (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{ return (lhs.c != rhs.c); };
	template <class T, class Container>
	bool operator<  (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{ return (lhs.c < rhs.c); };
	template <class T, class Container>
	bool operator<= (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{ return (lhs.c <= rhs.c); };
	template <class T, class Container>
	bool operator>  (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{  return (lhs.c > rhs.c); };
	template <class T, class Container>
	bool operator>= (const ft::queue<T,Container>& lhs, const ft::queue<T,Container>& rhs)
	{ return (lhs.c >= rhs.c); };
};

#endif //FT_CONTAINERS_QUEUE_HPP
