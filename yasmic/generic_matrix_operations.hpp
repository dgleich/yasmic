#ifndef YASMIC_GENERIC_MATRIX_OPERATIONS
#define YASMIC_GENERIC_MATRIX_OPERATIONS


#include <yasmic/smatrix_traits.hpp>
    
namespace yasmic
{   
	
    /**
     * Determine the number of nonzeros in a matrix.
     *
     * This function fowards the call to the matrix class itself,
     * implement a partial specialization if you need other behavior.
     *
     * @param m a reference to the matrix.  
     *
     * @return the number of nonzeros in a matrix.
     */
	template <class Matrix>
	inline typename smatrix_traits<Matrix>::size_type
    nnz(Matrix& m)
    {
    	return (m.nnz());
    }

    /**
     * Return the dimensions of the matrix (nrows, ncols).
     *
     * This function fowards the call to the matrix class itself,
     * implement a partial specialization if you need other behavior.
     *
     * @param m a reference to the matrix.  
     *
     * @return (nrows, ncols).
     */
    template <class Matrix>		
    inline std::pair<typename smatrix_traits<Matrix>::size_type,
                      typename smatrix_traits<Matrix>::size_type>
    dimensions(Matrix& m)
    {
    	return (m.dimensions());
    }
    
    template <class Matrix>
    inline typename smatrix_traits<Matrix>::size_type 
    ncols(Matrix& m)
	{
		smatrix_traits<Matrix>::size_type nrows,ncols;
		
		boost::tie(nrows, ncols) = dimensions(m);
		
		return (ncols);
	}
	
	template <class Matrix>
    inline typename smatrix_traits<Matrix>::size_type 
	nrows(Matrix& m)
	{ 
		smatrix_traits<Matrix>::size_type nrows,ncols;
		
		boost::tie(nrows, ncols) = dimensions(m);
		
		return (nrows);
	}
	
    template <class Matrix>
    inline typename smatrix_traits<Matrix>::index_type 
    row(boost::tuples::tuple<
            typename smatrix_traits<Matrix>::index_type, 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f) 
    {
    	return (boost::tuples::get<0>(n));
    }
    
    template <class Matrix>
    inline typename smatrix_traits<Matrix>::index_type 
    column(boost::tuples::tuple<
            typename smatrix_traits<Matrix>::index_type, 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f)
    {
    	return (boost::tuples::get<1>(n));
    }

	template <class Matrix>
    inline typename smatrix_traits<Matrix>::index_type 
    column(boost::tuples::tuple< 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f)
    {
    	return (boost::tuples::get<0>(n));
    }
    
    template <class Matrix>
    inline typename smatrix_traits<Matrix>::value_type 
    value(boost::tuples::tuple<
            typename smatrix_traits<Matrix>::index_type, 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f) 
    {
    	return (boost::tuples::get<2>(n));
    }

	/**
	 * Overloaded function to return the value for a row/column non-zero as well.
	 */
	template <class Matrix>
    inline typename smatrix_traits<Matrix>::value_type 
    row_value(boost::tuples::tuple< 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f) 
    {
    	return (boost::tuples::get<1>(n));
    }

	/*template <class Matrix>
	inline typename smatrix_traits<Matrix>::value_type
	row_value(boost::tuples::tuple<
            typename smatrix_traits<Matrix>::index_type, 
            typename smatrix_traits<Matrix>::index_type,
            typename smatrix_traits<Matrix>::value_type> n, const Matrix& f)
	{
		return (boost::tuples::get<1>(n));
	}*/
    
    template <class Matrix>
    inline std::pair<typename smatrix_traits<Matrix>::nonzero_iterator,
                      typename smatrix_traits<Matrix>::nonzero_iterator>
    nonzeros(Matrix& m)
    {
    	return (make_pair(m.begin_nonzeros(), m.end_nonzeros()));
    }
    
    template <class Matrix>
    inline std::pair<typename smatrix_traits<Matrix>::row_nonzero_iterator,
                      typename smatrix_traits<Matrix>::row_nonzero_iterator>
    row_nonzeros(typename smatrix_traits<Matrix>::index_type r, Matrix& m)
    {
    	return (make_pair(m.begin_row(r), m.end_row(r)));
    }
}

#endif // YASMIC_GENERIC_MATRIX_OPERATIONS