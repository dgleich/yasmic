#ifndef YASMIC_COMPRESSED_ROW_MATRIX
#define YASMIC_COMPRESSED_ROW_MATRIX

#include <iterator>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include <boost/functional.hpp>

#include <yasmic/tuple_utility.hpp>


#include <yasmic/generic_matrix_operations.hpp>


namespace yasmic
{
	namespace impl
	{
		/** 
		 * The nonzero iterator for the crm matrix.
		 */

		template <class RowIter, class ColIter, class ValIter>
		class compressed_row_nonzero_const_iterator
		: public boost::iterator_facade<
            compressed_row_nonzero_const_iterator<RowIter, ColIter, ValIter>,
			yasmic::simple_nonzero<
				typename std::iterator_traits<RowIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				typename std::iterator_traits<RowIter>::value_type> const,
            boost::forward_traversal_tag, 
            yasmic::simple_nonzero<
				typename std::iterator_traits<RowIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				typename std::iterator_traits<RowIter>::value_type> const >
        {
        public:
            compressed_row_nonzero_const_iterator() {}
            
            compressed_row_nonzero_const_iterator(
                RowIter ri, RowIter rend, ColIter ci, ValIter vi)
            : _ri(ri), _rend(rend), _ci(ci), _vi(vi), _id(0), _row(0)
            {
				_ri++;
            	// skip over any empty rows
            	while ( _ri != _rend && *(_ri) == _id )
                {
                   	// keep incrementing the row 
                   	++_ri; ++_row;
                }
            }

			compressed_row_nonzero_const_iterator(
                RowIter ri, RowIter rend, ColIter ci, ValIter vi, 
				typename std::iterator_traits<RowIter>::value_type id,
				typename std::iterator_traits<RowIter>::value_type row = 0)
			: _ri(ri), _rend(rend), _ci(ci), _vi(vi), _id(id), _row(row)
			{
			}
            
            
        private:
            friend class boost::iterator_core_access;

            inline void increment() 
            {  
            	// just increment everything!
            	++_ci; ++_vi; ++_id;
            	
                if (_id == *_ri)
                {
                	++_ri; ++_row;
                	
                	// while we aren't at the end and the row isn't empty
                	// (if *_ri == _id, then the row is empty because _id 
                	// is the current index in the column/val array.)
                	while ( _ri != _rend && *(_ri) == _id )
                    {
                    	// keep incrementing the row 
                    	++_ri; ++_row;
                    }
                }
            }
            
            bool equal(compressed_row_nonzero_const_iterator const& other) const
            {
				return (_ri == other._ri && _ci == other._ci);
            }

			/*difference_type distance_to(compressed_row_nonzero_const_iterator const& other) const */
            
            yasmic::simple_nonzero<
				typename std::iterator_traits<RowIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				typename std::iterator_traits<RowIter>::value_type> 
            dereference() const 
            { 
            	//return boost::make_tuple(_row, *_ci, *_vi);
				return make_simple_nonzero(_row, *_ci, *_vi, _id);
            }


            typename std::iterator_traits<RowIter>::value_type _row;
            typename std::iterator_traits<RowIter>::value_type _id;
            RowIter _ri, _rend;
            ColIter _ci;
            ValIter _vi;
        };

		/*template <class IndexType, class ValType, class NzIndexType>
		struct zip_tuple_to_row_nonzero
		{
			typedef typename yasmic::simple_nonzero<IndexType, ValType, NzIndexType> result_type;
			typedef boost::tuple<IndexType, ValType, NzIndexType> argument_type;

			// default required for transform_iterator
			zip_tuple_to_row_nonzero()
				: _r(0)
			{}

			zip_tuple_to_row_nonzero(IndexType r)
				: _r(r)
			{}

			IndexType _r;
			//NzIndexType _nzi;

			result_type operator() (argument_type arg) const
			{
				return make_simple_nonzero(
					_r,
					arg.get<0>(),
					arg.get<1>(),
					arg.get<2>());
			}
		};

		template <class RowIter, class ColIter, class ValIter>
		struct crm_row_nonzero_iter_help
		{
			typedef boost::counting_iterator<
				typename std::iterator_traits<RowIter>::value_type > nz_index_iter;

			typedef boost::tuple<ColIter, ValIter, nz_index_iter> row_nonzero_iter_tuple;

			typedef boost::zip_iterator<row_nonzero_iter_tuple> row_nonzero_zip_iter;

			typedef zip_tuple_to_row_nonzero<
					typename std::iterator_traits<RowIter>::value_type,
					typename std::iterator_traits<ValIter>::value_type,
					typename std::iterator_traits<RowIter>::value_type> tuple_xform_func;

			typedef boost::transform_iterator<
				tuple_xform_func, row_nonzero_zip_iter> row_iter;
		};*/

		
		template <class IndexType, class ColIter, class ValIter>
		class crm_row_nonzero_const_iterator
		: public boost::iterator_facade<
            crm_row_nonzero_const_iterator<IndexType, ColIter, ValIter>,
			yasmic::simple_nonzero<
				typename std::iterator_traits<ColIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				IndexType> const,
            boost::forward_traversal_tag, 
            yasmic::simple_nonzero<
				typename std::iterator_traits<ColIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				IndexType> const >
		{
		public:
			crm_row_nonzero_const_iterator() {}
            
            crm_row_nonzero_const_iterator(
                IndexType r, IndexType nzi, ColIter ci, ValIter vi)
            :  _r(r), _nzi(nzi), _ci(ci), _vi(vi)
            {}


		private:
			IndexType _r;
			IndexType _nzi;
			ColIter _ci;
			ValIter _vi;

            friend class boost::iterator_core_access;

			void increment() 
            {
				++_nzi;
				++_ci;
				++_vi;
			}

			bool equal(crm_row_nonzero_const_iterator const& other) const
			{
				return (_ci == other._ci);
			}

			yasmic::simple_nonzero<
				typename std::iterator_traits<ColIter>::value_type,
				typename std::iterator_traits<ValIter>::value_type,
				IndexType>
            dereference() const 
			{
				return (make_simple_nonzero(_r, *_ci, *_vi, _nzi));
			}
		};

		template <class RowIter, class ColIter, class ValIter>
		struct crm_row_nonzero_iter_help
		{
			typedef crm_row_nonzero_const_iterator<
				typename std::iterator_traits<RowIter>::value_type,
				ColIter, ValIter> row_iter;
		};
	}
	
	template <class RowIter, class ColIter, class ValIter>
	class compressed_row_matrix
	{
    public:
		typedef typename std::iterator_traits<RowIter>::value_type size_type;
		
		typedef size_type index_type;
		
		typedef typename std::iterator_traits<ValIter>::value_type value_type;

		typedef size_type nz_index_type;
		
		
		typedef simple_nonzero<index_type, value_type, nz_index_type> nonzero_descriptor;

		typedef impl::compressed_row_nonzero_const_iterator<RowIter, ColIter, ValIter>
             nonzero_iterator;
        
		typedef boost::counting_iterator<size_type> row_iterator;

		typedef nonzero_descriptor row_nonzero_descriptor;
		/*typedef impl::compressed_row_nonzero_const_iterator<RowIter, ColIter, ValIter>
             row_nonzero_iterator;*/

		typedef typename impl::crm_row_nonzero_iter_help<RowIter, ColIter, ValIter>::row_iter
			row_nonzero_iterator;

		typedef ValIter value_iterator;
		
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
        
        nonzero_iterator begin_nonzeros() const
        {
        	return (nonzero_iterator(_rstart, _rend, _cstart, _vstart));
        }
        
        nonzero_iterator end_nonzeros() const
        {
        	return (nonzero_iterator(_rend-1, _rend, _cend, _vstart));
        }
        
        inline std::pair<size_type, size_type> dimensions() const
        {
        	return (std::make_pair(_nrows, _ncols));
        }
        
        inline size_type nnz() const
        {
        	return (_nnz);
        }
        	
        row_iterator begin_rows() const
        {
        	return (row_iterator(0));
        }
        
        row_iterator end_rows() const
        {
        	return (row_iterator(_nrows));
        }

		row_nonzero_iterator begin_row(index_type r) const
		{
			index_type nzi = *(_rstart + (r));
			return (row_nonzero_iterator(r, nzi, _cstart + nzi, _vstart + nzi));

			//typedef impl::crm_row_nonzero_iter_help<RowIter, ColIter, ValIter> type_helper;

			/*typedef typename type_helper::nz_index_iter nzi_iter;
			typedef typename type_helper::row_nonzero_iter_tuple iter_tuple;
			typedef typename type_helper::row_nonzero_zip_iter row_zip_iterator;
			typedef typename type_helper::tuple_xform_func zip_xform;

			return (row_nonzero_iterator(
						row_zip_iterator(
							iter_tuple(_cstart + *(_rstart + r),
									   _vstart + *(_rstart + r),
									   nzi_iter(*(_rstart + r)))
									   ),
					   zip_xform(r)));*/

			/*return (row_nonzero_iterator(
						row_zip_iterator(
							iter_tuple(_cstart + *(_rstart + r),
									   _vstart + *(_rstart + r))
									   ),
					   zip_xform(r, *(_rstart + (r)))));*/
		}
										
							
		row_nonzero_iterator end_row(index_type r) const
        {
			index_type nzi = *(_rstart + (r+1));
			return (row_nonzero_iterator(r, nzi, _cstart + nzi, _vstart + nzi));

			/*typedef impl::crm_row_nonzero_iter_help<RowIter, ColIter, ValIter> type_helper;

			typedef typename type_helper::nz_index_iter nzi_iter;
			typedef typename type_helper::row_nonzero_iter_tuple iter_tuple;
			typedef typename type_helper::row_nonzero_zip_iter row_zip_iterator;
			typedef typename type_helper::tuple_xform_func zip_xform;

			return (row_nonzero_iterator(
						row_zip_iterator(
							iter_tuple(_cstart + *(_rstart + (r+1)),
									   _vstart + *(_rstart + (r+1)),
									   nzi_iter(*(_rstart + (r+1))))
									   ),
					   zip_xform(r)));*/
		}
        
        /*row_nonzero_iterator begin_row(index_type r) const
        {
			return (row_nonzero_iterator(
                _rstart + r, _rstart + r+1, _cstart + *(_rstart + r), 
                _vstart + *(_rstart + r), *(_rstart+(r)), r));
        }
        
        row_nonzero_iterator end_row(index_type r) const
        {
			return (row_nonzero_iterator(
                _rstart + r, _rstart + r+1, _cstart + *(_rstart + (r+1) ), 
                _vstart + *(_rstart + r), *(_rstart+(r+1)), r ) );
        }*/

		value_iterator begin_values() const
		{
			return (_vstart);
		}

		value_iterator end_values() const
		{
			return (_vend);
		}

		
    private:
        RowIter _rstart,_rend;
        ColIter _cstart,_cend;
        ValIter _vstart,_vend;
        
        size_type _nrows, _ncols, _nnz;
	};
	
    /*template <class Matrix, class RowIter, class ColIter, class ValIter>
	copy_matrix_to_compressed_row_data(Matrix& m,
		RowIter rstart, RowIter rend, 
        ColIter cstart, ColIter cend, ValIter vstart, ValIter vend,
		)
	{
		smatrix_traits<simple_matrix>::nonzero_iterator nzi, nzend;
    	tie(nzi, nzend) = nonzeros(m);
    	for (; nzi != nzend; ++nzi)
    	{
            rows[row(*nzi, m)+1]++;
    	}
    	
    	// compute the reduction
    	partial_sum(rows.begin(), rows.end(), rows.begin());
    	
    	tie(nzi, nzend) = nonzeros(m);
    	for (; nzi != nzend; ++nzi)
    	{
    		cout << "cur entry " << rows[row(*nzi, m)]+cur[row(*nzi, m)]
    		     << " = " << boost::tuples::make_tuple(row(*nzi, m), column(*nzi, m), value(*nzi, m)) << endl;
            cols[rows[row(*nzi, m)]+cur[row(*nzi, m)]] = column(*nzi, m);
            vals[rows[row(*nzi, m)]+cur[row(*nzi, m)]] = value(*nzi, m);
            cur[row(*nzi, m)]++;
    	}
	}*/



}

#endif // YASMIC_COMPRESSED_ROW_MATRIX


