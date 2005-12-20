#ifndef YASMIC_IFSTREAM_MATRIX
#define YASMIC_IFSTREAM_MATRIX

#include <fstream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp> 
#include <yasmic/smatrix_traits.hpp>
#include <iterator>

#include <yasmic/generic_matrix_operations.hpp>

namespace yasmic
{
	template <class index_type = int, class value_type = double, class size_type = unsigned int>
	struct ifstream_matrix
	{
		std::ifstream& _f;

		ifstream_matrix(std::ifstream& f)
			: _f(f) 
		{}
	};

	template <class i_index_type, class i_value_type, class i_size_type>
    struct smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> > 
    {
    	typedef i_size_type size_type;
    	typedef i_index_type index_type;
		typedef i_value_type value_type;
		
		typedef boost::tuple<index_type, index_type, value_type> nonzero_descriptor;

		typedef std::istream_iterator<nonzero_descriptor> nonzero_iterator;

		typedef void row_iterator;
		
		typedef void row_nonzero_descriptor;
		typedef void row_nonzero_iterator;
		
		typedef void column_iterator;
    };
    
	template <class i_index_type, class i_value_type, class i_size_type>
    inline std::pair<typename smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type,
                     typename smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type >
    dimensions(ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
    {
		typedef smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	traits::size_type nrows,ncols;
    	
    	m._f.clear();
    	m._f.seekg(0, std::ios_base::beg);
    	
        m._f >> nrows >> ncols;
        
        return (std::make_pair(nrows, ncols));
    }
	
	template <class i_index_type, class i_value_type, class i_size_type>
	typename smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type
	nnz(ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
	{
		typedef smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	traits::size_type d1,d2,nnz;
		
		// clear any error bits
		m._f.clear();
        m._f.seekg(0, std::ios_base::beg);
        
        m._f >> d1 >> d2 >> nnz;
        
        return (nnz);
	}
	
	template <class i_index_type, class i_value_type, class i_size_type>
	std::pair<typename smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> >::nonzero_iterator,
              typename smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> >::nonzero_iterator>
    nonzeros(ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
    {
    	typedef smatrix_traits<ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	typename traits::size_type d1,d2,d3;
    	
    	m._f.clear();
    	m._f.seekg(0, std::ios_base::beg);
    	
        m._f >> d1  >> d2 >> d3;
        
        m._f >> boost::tuples::set_open(' ') >> boost::tuples::set_close(' ');
        
        typedef typename traits::nonzero_iterator nz_iter;

		
        
        return (std::make_pair(nz_iter(m._f), nz_iter()));
    }
}

#endif //YASMIC_IFSTREAM_MATRIX
