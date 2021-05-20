//
// Created by Knot Fonda on 5/2/21.
//

#ifndef FT_CONTAINERS_STACK_HPP
#define FT_CONTAINERS_STACK_HPP

# include "vector.hpp"
namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
	public:
		typedef Container                                	container_type;
		typedef T											value_type;
		typedef size_t 										size_type;
	protected:
		container_type c;
	public:
		explicit stack (const container_type &ctnr = container_type()) : c(ctnr) {};
		bool empty() { return (c.empty()); };
		size_type size () const { return (c.size()); };
		value_type &top () { return (c.back()); };
		const value_type  &top() const { return (c.back()); };
		void push (const value_type &val) { c.push_back(val); };
		void pop() { c.pop_back(); };
	private:
		template <class QQ, class EE>
		friend bool operator== (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
		template <class QQ, class EE>
		friend bool operator!= (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
		template <class QQ, class EE>
		friend bool operator<  (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
		template <class QQ, class EE>
		friend bool operator<= (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
		template <class QQ, class EE>
		friend bool operator>  (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
		template <class QQ, class EE>
		friend bool operator>= (const stack<QQ,EE>& lhs, const stack<QQ,EE>& rhs);
	};
	template <class T, class Container>
	bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{ return (lhs.c == rhs.c); };
	template <class T, class Container>
	bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{ return (lhs.c != rhs.c); };
	template <class T, class Container>
	bool operator<  (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{ return (lhs.c < rhs.c); };
	template <class T, class Container>
	bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{ return (lhs.c <= rhs.c); };
	template <class T, class Container>
	bool operator>  (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{  return (lhs.c > rhs.c); };
	template <class T, class Container>
	bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
	{ return (lhs.c >= rhs.c); };
};

#endif //FT_CONTAINERS_STACK_HPP
