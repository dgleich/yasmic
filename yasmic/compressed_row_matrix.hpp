#ifndef YASMIC_COMPRESSED_ROW_MATRIX
#define YASMIC_COMPRESSED_ROW_MATRIX

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>

#include <yasmic/tuple_utility.hpp>

#include <yasmic/generic_matrix_operations.hpp>


namespace yasmic
{
	namespace impl
	{
	
		template <class RowIter, class ColIter, class ValIter>
		class compressed_row_nonzero_const_iterator
		: public boost::iterator_facade<
            compressed_row_nonzero_const_iterator<RowIter, ColIter, ValIter>,
            boost::tuple<
                typename std::iterator_traits<RowIter>::value_type,
                typename std::iterator_traits<ColIter>::value_type,
                typename std::iterator_traits<ValIter>::value_type> const,
            boost::forward_traversal_tag, 
            boost::tuple<
                typename std::iterator_traits<RowIter>::value_type,
                typename std::iterator_traits<ColIter>::value_type,
                typename std::iterator_traits<ValIter>::value_type> const >
        {
        public:
            compressed_row_nonzero_const_iterator() {}
            
            compressed_row_nonzero_const_iterator(
                RowIter ri, RowIter rend, ColIter ci, ValIter vi)
            : _ri(ri), _rend(rend), _ci(ci), _vi(vi), _id(0), _row(0)
            {
            	// skip over any empty rows
            	while ( _ri != _rend && *_ri == _id )
                {
                   	// keep incrementing the row 
                   	++_ri; ++_row;
                }
            }
            
            
        private:
            friend class boost::iterator_core_access;

            void increment() 
            {  
            	// just increment everything!
            	++_ci; ++_vi; ++_id;
            	
                if (_id == *_ri)
                {
                	++_ri; ++_row;
                	
                	// while we aren't at the end and the row isn't empty
                	// (if *_ri == _id, then the row is empty because _id 
                	// is the current index in the column/val array.)
                	while ( _ri != _rend && *_ri == _id )
                    {
                    	// keep incrementing the row 
                    	++_ri; ++_row;
                    }
                }
            }
            
            bool equal(compressed_row_nonzero_const_iterator const& other) const
            {
                return (_ri == other._ri &&
                    (_ri == _rend || _ci == other._ci) );
            }
            
            boost::tuple<
                typename std::iterator_traits<RowIter>::value_type,
                typename std::iterator_traits<ColIter>::value_type,
                typename std::iterator_traits<ValIter>::value_type>  
            dereference() const 
            { 
            	return boost::make_tuple(_row, *_ci, *_vi);
            }


            typename std::iterator_traits<RowIter>::value_type _row;
            typename std::iterator_traits<RowIter>::value_type _id;
            RowIter _ri, _rend;
            ColIter _ci;
            ValIter _vi;
        };

                
        
	}
	
	template <class RowIter, class ColIter, class ValIter>
	class compressed_row_matrix
	{
    public:
		typedef typename std::iterator_traits<RowIter>::value_type size_type;
		
		typedef size_type index_type;
		
		typedef typename std::iterator_traits<ValIter>::value_type value_type;
		
		typedef boost::tuple<index_type, index_type, value_type> nonzero_descriptor;

		typedef impl::compressed_row_nonzero_const_iterator<RowIter, ColIter, ValIter>
             nonzero_iterator;
             
        
		typedef boost::counting_iterator<size_type> row_iterator;
		
		typedef boost::binder1st(tuple_get_2<1,2, nonzero_descriptor>) 
            nonzero_to_row_nonzero_transform;
		
		typedef boost::tuple<index_type, value_type> row_nonzero_descriptor;
		typedef boost::transform_iterator< nonzero_to_row_nonzero_transform, 
            row_nonzero_iterator > row_nonzero_iterator;
		
		
		typedef void column_iterator;
		typedef void column_nonzero_iterator;
		
		/**
		 * Simple constructor for all the iterators.  This function
		 * requires one pass through the data in order to determine
		 * the number of rows, columns, and non-zeros.
		 */
		compressed_row_matrix(RowIter rstart, RowIter rend, 
            ColIter cstart, ColIter cend, ValIter vstart, ValIter vend)
        : _rstart(rstart), _rend(rend), _cstart(cstart), _cend(cend),
          _vstart(vstart), _vend(vend), _nnz(0), _nrows(0), _ncols(0)
        {
			RowIter ri = _rstart;
			ColIter ci = _cstart;
			ValIter vi = _vstart;
			
			// elminate the first entry, so we get the correct # of rows
			++ri;
			while (ri != _rend)
			{
				++_nrows;  ++ri;
			}
			
			
			while (ci != _cend)
			{
				_ncols = std::max(*ci,_ncols);
				++ci; ++vi;
				++_nnz;
			}
			
			if (vi != _vend)
			{
				// warning, incorrect data!
			}
        }
          
		compressed_row_matrix(RowIter rstart, RowIter rend, 
            ColIter cstart, ColIter cend, ValIter vstart, ValIter vend,
            size_type nrows, size_type ncols, size_type nnz)
        : _rstart(rstart), _rend(rend), _cstart(cstart), _cend(cend),
          _vstart(vstart), _vend(vend), _nrows(nrows), _ncols(ncols), _nnz(nnz)
        {}
        
        nonzero_iterator begin_nonzeros()
        {
        	return (nonzero_iterator(_rstart, _rend, _cstart, _vstart));
        }
        
        nonzero_iterator end_nonzeros()
        {
        	return (nonzero_iterator(_rend, _rend, _cend, _vstart));
        }
        
        inline std::pair<size_type, size_type> dimensions()
        {
        	return (std::make_pair(_nrows, _ncols));
        }
        
        inline size_type nnz()
        {
        	return (_nnz);
        }
        	
        row_iterator begin_rows()
        {
        	return (row_iterator(0));
        }
        
        row_iterator end_rows()
        {
        	return (row_iterator(_nrows));
        }
        
        row_nonzero_iterator begin_row(index_type r)
        {
        	return (row_nonzero_iterator(nonzero_iterator(
                _rstart + r, _rstart + r+1, _cstart + *(_rstart + _r), 
                _vstart + *(_rstart + _r)), 
                boost::bind(yasmic::tuple_get_2<1,2, nonzero_descriptor>, _1) ) );
        }
        
        row_nonzero_iterator end_row(index_type r)
        {
        	return (row_nonzero_iterator(nonzero_iterator(
                _rstart + r + 1, _rend + r+1, _cstart + *(_rstart + (_r+1) ), 
                _vstart + *(_rstart + _r)),
                boost::bind(yasmic::tuple_get_2<1,2, nonzero_descriptor>, _1) ) );
        }
		
    private:
        RowIter _rstart,_rend;
        ColIter _cstart,_cend;
        ValIter _vstart,_vend;
        
        size_type _nrows, _ncols, _nnz;
	};
	
    
}

#endif // YASMIC_COMPRESSED_ROW_MATRIX