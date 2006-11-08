#ifndef YASMIC_UTIL_WRITE_MATRIX
#define YASMIC_UTIL_WRITE_MATRIX

#include <yasmic/util/write_petsc_matrix.hpp>

namespace impl
{
    namespace write
    {
        template <class index_type, class nz_index_type, class value_type, bool header>
        struct custom_smat_writer
        {
            template <class Matrix>
            bool write_matrix(std::ostream& f, Matrix& m)
            {
                using namespace yasmic;

                if (header)
                {
                    ostream << (index_type)nrows(m) << " " 
                            << (index_type)ncols(m) << " " 
                            << (nz_index_type)nnz(m) << std::endl;
                }

                typename smatrix_traits<Matrix>::nonzero_iterator nzi, nziend;
                boost::tie(nzi,nziend) = nonzeros(m);
                for (; nzi != nziend; ++nzi)
                {
                    ostream << (index_type)row(*nzi, m) << " "
                            << (index_type)column(*nzi, m) << " "
                            << (value_type)value(*nzi, m) << st::endl;
                }
            }
        };

        template <class index_type, class nz_index_type, class value_type, bool header>
        struct custom_bsmat_writer
        {
            template <class Matrix>
            bool write_matrix(std::ostream& f, Matrix& m)
            {
                return (true);
            }
        };

        struct smat_writer
            : public custom_smat_writer<int, unsigned int, double, true>
        {
        };

        struct bsmat_writer
            : public custom_bsmat_writer<unsigned int, unsigned int, double, true>
        {
        };

        struct cluto_writer
        {
            template <class RowAccessMatrix>
            bool write_matrix(std::ostream& f, RowAccessMatrix& m)
            {
                return (true);
            }
        };

        struct petsc_writer
        {
            template <class RowAccessMatrix>
            bool write_matrix(std::ostream& f, RowAccessMatrix& m)
            {
                return (true);
            }
        };

    }
}

typedef struct impl::write::smat_writer smat_writer;
typedef struct impl::write::bsmat_writer bsmat_writer;
typedef struct impl::write::petsc_writer petsc_writer;
typedef struct impl::write::cluto_writer cluto_writer;

template <class index_type, class nz_index_type, class value_type>
struct parametrized_writers
{
    typedef struct impl::write::custom_bsmat_writer<index_type,nz_index_type,value_type,true>
        bsmat_writer;
    typedef struct impl::write::custom_smat_writer<index_type,nz_index_type,value_type,true>
        smat_writer;
};

template <class Matrix, class Tag>
void write_matrix(std::ostream& f, Matrix& m, Tag type)
{
    type.write_matrix(f,m);
}


#endif //YASMIC_UTIL_WRITE_MATRIX

