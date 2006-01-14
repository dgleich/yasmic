#ifndef YASMIC_ITERATOR_UTILITY
#define YASMIC_ITERATOR_UTILITY

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

namespace yasmic
{
	/*template <class Type, class SizeType = ptrdiff_t>
	struct constant_iterator_data
	{
		Type _val;
		SizeType _pos;
	};

	template <class Type, class SizeType = ptrdiff_t>
	class constant_iterator
		: public boost::iterator_adaptor<
			constant_iterator<Type, SizeType>,			// Derived
			constant_iterator_data<Type, SizeType>,		// Base type 
			Type,										// Value
			boost::random_access_traversal_tag,			// Traversal
			SizeType>									// Difference
	{
	private:
		struct enabler {};

	public:
		constant_iterator()
			: constant_iterator::iterator_adaptor_() {}

		constant_iterator(Type v)
			: constant_iterator::iterator_adaptor_(
	};*/

	template <class Type, class SizeType = ptrdiff_t>
	struct constant_iterator
		: public boost::iterator_facade<
            constant_iterator<Type>,
			Type,
            boost::random_access_traversal_tag, 
            Type >
	{
	public:
		constant_iterator()
		{}

		constant_iterator(Type v)
			: _v(v), _pos(0)
		{}

		constant_iterator(Type v, SizeType pos )
			: _v(v), _pos(pos)
		{}

	private:
		friend class boost::iterator_core_access;

		Type _v;
		SizeType _pos;

		inline void increment() 
		{ ++_pos; }

		inline void decrement()
		{ --_pos;}

		inline void advance(SizeType n)
		{ _pos += n; }

		inline difference_type distance_to(constant_iterator const& other)
		{ return (other._pos - _pos); }

		inline Type dereference() const
		{ return (_v); }

		inline bool equal(constant_iterator const& other) const
		{
			return (other._v == _v && other._pos == _pos);
		}

	};
}

#endif // YASMIC_ITERATOR_UTILITY
