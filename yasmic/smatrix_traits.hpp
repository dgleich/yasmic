#ifndef YASMIC_SMATRIX_TRAITS
#define YASMIC_SMATRIX_TRAITS

/**
 * @file smatrix_traits.hpp
 * 
 * Provide a default traits fallback which looks inside the class.
 * 
 * If you have a non-class/struct matrix type, then you have to use partial 
 * specialization on your class to make it work.  
 */

namespace yasmic
{
	struct unsymmetric_tag { };
	struct symmetric_tag { };

	template <class Op>
	struct op_traits
	{
		typedef typename Op::size_type size_type;
	};

	template <class Mat>
	struct smatrix_traits : public op_traits<Mat>
	{
		typedef typename Mat::index_type index_type;
		typedef typename Mat::value_type value_type;

		typedef typename Mat::nonzero_iterator nonzero_iterator;
		typedef typename Mat::nonzero_descriptor nonzero_descriptor;

		typedef typename Mat::row_iterator row_iterator;
		
		typedef typename Mat::row_nonzero_iterator row_nonzero_iterator;
		typedef typename Mat::row_nonzero_descriptor row_nonzero_descriptor;

		typedef typename Mat::size_type nz_index_type;
		
		typedef typename Mat::column_iterator column_iterator;
		//typedef typename Mat::column_nonzero_iterator column_nonzero_iterator;

		typedef typename Mat::symmetry_category symmetry_category;
	};
}

#endif // YASMIC_SMATRIX_TRAITS

