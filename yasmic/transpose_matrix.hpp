#ifndef YASMIC_TRANSPOSE_MATRIX
#define YASMIC_TRANSPOSE_MATRIX

/**
 * @file transpose_matrix.hpp
 * @author David Gleich
 * 
 * A transpose filter for a matrix. 
 *
 * Regardless of the input, the output is just a nonzero-matrix.
 */

#include <boost/tuple/tuple.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace yasmic
{

namespace impl
{
	/** 
	 * This struct is just a simple wrapper to switch the row
	 * and column options for an iterator (i.e. transpose) the
	 * matrix. 
	 */
	template <class nonzero, class Matrix>
	struct nonzero_transpose_xform
	{
		typedef boost::tuple<
			typename smatrix_traits<Matrix>::index_type, 
			typename smatrix_traits<Matrix>::index_type, 
			typename smatrix_traits<Matrix>::value_type > result_type;

		typedef const nonzero argument_type;

		result_type operator() (argument_type arg) const
		{
			return (boost::make_tuple(column(arg, *_m), row(arg, *_m), value(arg, *_m)));
		}

		Matrix *_m;

		nonzero_transpose_xform()
			: _m(NULL)
		{}

		nonzero_transpose_xform(Matrix& m)
			: _m(&m)
		{}
	};
		
		
} // namespace impl


template <class Matrix>
struct transpose_matrix
{
	Matrix& _m;

	transpose_matrix(Matrix& m)
		: _m(m)
	{}
	
	typedef typename smatrix_traits<Matrix>::index_type index_type;
	typedef typename smatrix_traits<Matrix>::value_type value_type;

	typedef boost::tuple<index_type, index_type, value_type> nonzero_descriptor;
	//typedef void nonzero_iterator;
	typedef boost::transform_iterator<
		impl::nonzero_transpose_xform<
			typename smatrix_traits<Matrix>::nonzero_descriptor, Matrix>,
		typename smatrix_traits<Matrix>::nonzero_iterator> nonzero_iterator;

	typedef void row_iterator;
	typedef void row_nonzero_descriptor;
	typedef void row_nonzero_iterator;

	typedef void column_iterator;

	typedef typename smatrix_traits<Matrix>::size_type size_type;
	typedef typename smatrix_traits<Matrix>::nz_index_type nz_index_type;
	typedef typename smatrix_traits<Matrix>::symmetry_category symmetry_category;

	std::pair<size_type, size_type> dimensions()
	{
		return std::make_pair(ncols(_m), nrows(_m));
	}

	size_type nnz()
	{
		return nnz(_m);
	}

	nonzero_iterator begin_nonzeros()
	{
		typename smatrix_traits<Matrix>::nonzero_iterator nzi, nziend;
		boost::tie(nzi, nziend) = nonzeros(_m);

		impl::nonzero_transpose_xform
			<typename smatrix_traits<Matrix>::nonzero_descriptor,
			Matrix> f(_m);

		return (nonzero_iterator(nzi, f));
	}

	nonzero_iterator end_nonzeros()
	{
		typename smatrix_traits< Matrix>::nonzero_iterator nzi, nziend;
		boost::tie(nzi, nziend) = nonzeros(_m);

		impl::nonzero_transpose_xform
			<typename smatrix_traits<Matrix>::nonzero_descriptor,
			Matrix> f(_m);

		return (nonzero_iterator(nziend, f));
	}
};


} // namespace yasmic

#endif // YASMIC_TRANSPOSE_MATRIX