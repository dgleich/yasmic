
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

int main(int argc, char **argv)
{
    using namespace std;
    
    if (argc != 3)
    {
        return (-1);
    }
    
    string filename_txt = argv[1];
	string filename_bin = argv[2];
    
    using namespace yasmic;
    
    typedef ifstream_matrix<> simple_matrix;
    typedef smatrix_traits<simple_matrix>::size_type size_type;

	typedef binary_ifstream_matrix<> simple_bin_matrix;

    
    ifstream f(filename_txt.c_str());
    simple_matrix m(f);

	ifstream b(filename_bin.c_str(), ios::binary);
	simple_bin_matrix m2(b);
    
    
	{
		size_type nr = nrows(m);
		size_type nc = ncols(m);
		cout << "nrows: " << nr << endl;
		cout << "ncols: " << nc << endl;
	    
		{
    		smatrix_traits<simple_matrix>::nonzero_iterator nzi, nzend;
	    
    		tie(nzi, nzend) = nonzeros(m);
	    	
    		for (; nzi != nzend; ++nzi)
    		{
				cout << row(*nzi, m) << " " << column(*nzi, m) << " " << value(*nzi, m) << endl;
    		}

		}
	}

	{
		size_type nr = nrows(m2);
		size_type nc = ncols(m2);
		cout << "nrows: " << nr << endl;
		cout << "ncols: " << nc << endl;

		cout << "nnz: " << nnz(m2) << endl;
	    
		{
    		smatrix_traits<simple_bin_matrix>::nonzero_iterator nzi, nzend;
	    
    		tie(nzi, nzend) = nonzeros(m2);
	    	
    		for (; nzi != nzend; ++nzi)
    		{
				cout << row(*nzi, m2) << " " << column(*nzi, m2) << " " << value(*nzi, m2) << endl;
    		}

		}
	}
    
    
    return (0);
}