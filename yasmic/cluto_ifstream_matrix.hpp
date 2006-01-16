#ifndef YASMIC_CLUTO_IFSTREAM_MATRIX
#define YASMIC_CLUTO_IFSTREAM_MATRIX

#include <fstream>
#include <boost/tuple/tuple.hpp>
#include <iterator>
#include <string>
#include <sstream>
#include <boost/iterator/iterator_facade.hpp>

#include <yasmic/generic_matrix_operations.hpp>

#ifdef BOOST_MSVC
#if _MSC_VER >= 1400
	// disable the warning for ifstream::read
	#pragma warning( disable : 4996 )
#endif // _MSC_VER >= 1400
#endif // BOOST_MSVC

namespace yasmic
{
	namespace impl
	{
		template <class i_index_type, class i_value_type>
		class cluto_ifstream_matrix_const_iterator
		: public boost::iterator_facade<
            cluto_ifstream_matrix_const_iterator<i_index_type, i_value_type>,
            boost::tuple<
                i_index_type, i_index_type, i_value_type> const,
            boost::forward_traversal_tag, 
            boost::tuple<
                i_index_type, i_index_type, i_value_type> const >
        {
        public:
            cluto_ifstream_matrix_const_iterator() 
				: _str(0), _r(0), _c(0), _v(0), _line(0)
			{}
            
            cluto_ifstream_matrix_const_iterator(std::ifstream &str, std::istringstream &line)
				: _str(&str), _r(0), _c(0), _v(0), _line(&line)
            {
				std::string curline;
				getline( *(_str), curline );
				_line->str(curline);

				increment(); 
			}
            
            
        private:
            friend class boost::iterator_core_access;

            void increment() 
            {  
				if (_str != 0)
				{
					if (!_line)
					{
						std::string curline;
						getline( *(_str), curline );
						_line->str(curline);

						++_r;
					}

					*(_line) >> _c;
					*(_line) >> _v;

					if (_str->eof()) { _str = 0; }
				}
            }
            
            bool equal(cluto_ifstream_matrix_const_iterator const& other) const
            {
				return (_str == other._str);
            }
            
            boost::tuple<
                i_index_type, i_index_type, i_value_type>
            dereference() const 
            { 
            	return boost::make_tuple(_r, _c, _v);
            }

			i_index_type _r, _c;
			i_value_type _v;

			std::istringstream* _line;

			std::ifstream* _str;
        };

	}


	template <class index_type = int, class value_type = double, class size_type = unsigned int>
	struct cluto_ifstream_matrix
	{
		std::ifstream& _f;
		std::istringstream _line;

		cluto_ifstream_matrix(std::ifstream& f)
			: _f(f) 
		{}
	};

	template <class i_index_type, class i_value_type, class i_size_type>
    struct smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> > 
    {
    	typedef i_size_type size_type;
    	typedef i_index_type index_type;
		typedef i_value_type value_type;
		
		typedef boost::tuple<index_type, index_type, value_type> nonzero_descriptor;

		typedef impl::cluto_ifstream_matrix_const_iterator<i_index_type, i_value_type> nonzero_iterator;

		typedef size_type nz_index_type;

		typedef void row_iterator;
		
		typedef void row_nonzero_descriptor;
		typedef void row_nonzero_iterator;
		
		typedef void column_iterator;
    };
    
	template <class i_index_type, class i_value_type, class i_size_type>
    inline std::pair<typename smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type,
                     typename smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type >
    dimensions(cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
    {
		typedef smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	typename traits::size_type nrows,ncols;
    	
    	m._f.clear();
    	m._f.seekg(0, std::ios_base::beg);

		m._f >> nrows >> ncols;
    	        
        return (std::make_pair(nrows, ncols));
    }
	
	template <class i_index_type, class i_value_type, class i_size_type>
	typename smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> >::size_type
	nnz(cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
	{
		typedef smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	typename traits::size_type d1, d2, nnz;
		
		// clear any error bits
		m._f.clear();

		m._f >> d1 >> d2 >> nnz;
		
		// seek after nrows, ncols
        /*m._f.seekg(2*sizeof(i_index_type), std::ios_base::beg);

        m._f.read((char *)&nnz, sizeof(i_size_type));*/
        
        return (nnz);
	}
	
	template <class i_index_type, class i_value_type, class i_size_type>
	std::pair<typename smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> >::nonzero_iterator,
              typename smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> >::nonzero_iterator>
    nonzeros(cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type>& m)
    {
    	typedef smatrix_traits<cluto_ifstream_matrix<i_index_type, i_value_type, i_size_type> > traits;
    	typename traits::size_type d1, d2, d3;

    	m._f.clear();

		m._f >> d1 >> d2 >> d3;

		// seek after nrows,ncols,nnz
    	//m._f.seekg(2*sizeof(i_index_type)+sizeof(i_size_type), std::ios_base::beg);
        
        typedef typename traits::nonzero_iterator nz_iter;
        
        return (std::make_pair(nz_iter(m._f, m._line), nz_iter()));
    }
}


#ifdef BOOST_MSVC
#if _MSC_VER >= 1400
	// restore the warning for ifstream::read
	#pragma warning( default : 4996 )
#endif // _MSC_VER >= 1400
#endif // BOOST_MSVC


#endif //YASMIC_CLUTO_IFSTREAM_MATRIX
