
/*
 * David Gleich
 * yasmic
 * 2005
 */
 
/**
 * @file ifstream_as_matrix.cc
 * Test file for using an ifstream as a matrix.
 */

#include <iostream>
#include <fstream>
#include <string>

//#include <yasmic/ifstream_as_matrix.hpp>
#include <yasmic/ifstream_matrix.hpp>
#include <yasmic/binary_ifstream_matrix.hpp>
#include <yasmic/cluto_ifstream_matrix.hpp>

#include <yasmic/transpose_matrix.hpp>
#include <yasmic/nonzero_union.hpp>

template <class Matrix>
void dump_matrix(Matrix& m)
{
	using namespace yasmic;
	using namespace std;

	typedef smatrix_traits<Matrix>::size_type size_type;

		size_type nr = nrows(m);
		size_type nc = ncols(m);
		cout << "nrows: " << nr << endl;
		cout << "ncols: " << nc << endl;
	    
		{
    		smatrix_traits<Matrix>::nonzero_iterator nzi, nzend;
	    
    		tie(nzi, nzend) = nonzeros(m);
	    	
    		for (; nzi != nzend; ++nzi)
    		{
				cout << row(*nzi, m) << " " << column(*nzi, m) << " " << value(*nzi, m) << endl;
    		}

		}
}


int main(int argc, char **argv)
{
    using namespace std;
    
    if (argc != 4)
    {
        return (-1);
    }
    
    string filename_txt = argv[1];
	string filename_bin = argv[2];
	string filename_cluto = argv[3];
    
    using namespace yasmic;
    
    typedef ifstream_matrix<> simple_matrix;
    typedef smatrix_traits<simple_matrix>::size_type size_type;

	typedef binary_ifstream_matrix<> simple_bin_matrix;
	typedef cluto_ifstream_matrix<> cluto_matrix;

	//typedef add_symmetric<simple_matrix> symmetric_matrix;

    
    ifstream f(filename_txt.c_str());
    //symmetric_matrix m = static_cast<symmetric_matrix>(simple_matrix(f));
	simple_matrix m(f);

	size_type nr, nc;
	boost::tie(nr, nc) = dimensions(m);

	ifstream b(filename_bin.c_str(), ios::binary);
	simple_bin_matrix m2(b);

	ifstream b2(filename_bin.c_str(), ios::binary);
	simple_bin_matrix m22(b2);

	ifstream c(filename_cluto.c_str());
	cluto_matrix m3(c);
    

	dump_matrix(m);
	dump_matrix(m2);

	istringstream _line("12 15 1\n5 6 8\n");

	simple_matrix mtest(_line);
	
	dump_matrix(mtest);
    dump_matrix(m3);

	

	typedef transpose_matrix<simple_bin_matrix> tmatrix;

	typedef nonzero_union<simple_bin_matrix, tmatrix> nzu_matrix;

	tmatrix trans_m(m2);

	nzu_matrix nzu(m22, trans_m);

	//cout << "nrows: " << nrows(trans_m) << " ncols: " << ncols(trans_m) << endl;

	dump_matrix(nzu);
    
    return (0);
}


