#ifndef YASMIC_UTIL_WRITE_MATRIX
#define YASMIC_UTIL_WRITE_MATRIX

#include <yasmic/util/write_petsc_matrix.hpp>

namespace impl
{
    namespace write
    {
        struct smat_writer
        {
            template <class Matrix>
            bool write_matrix(std::ostream& f, Matrix& m)
            {
                return (true);
            }
        };
    }
}

typedef struct impl::write::smat_writer smat_writer;
typedef struct impl::write::bsmat_writer<int,int,double> bsmat_writer;
typedef struct impl::write::petsc_writer petsc_writer;
typedef struct impl::write::cluto_writer cluto_writer;

template <class index_type, class nz_index_type, class value_type>
struct parametrized_writers
{
    typedef struct impl::write::bsmat_writer<index_type,nz_index_type,value_type>
        bsmat_writer;
};

template <class Matrix, class Tag>
void write_matrix(std::ostream& f, Matrix& m, Tag type)
{
    type.write_matrix(f,m);
}


#endif //YASMIC_UTIL_WRITE_MATRIX

