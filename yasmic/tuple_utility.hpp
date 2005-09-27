#ifndef YASMIC_TUPLE_UTILITY
#define YASMIC_TUPLE_UTILITY

#include <boost/tuple/tuple.hpp>

namespace yasmic
{
	template <int N1, int N2, class Tuple>
	boost::tuple<
        typename boost::tuples::element<N1, Tuple>::type,
        typename boost::tuples::element<N2, Tuple>::type >
    tuple_get_2(Tuple& t) 
    {
    	return ( boost::make_tuple(boost::tuples::get<N1>(t), 
            boost::tuples::get<N2>(t)) );
    }
        
}

#endif // YASMIC_TUPLE_UTILITY