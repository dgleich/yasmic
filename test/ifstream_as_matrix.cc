
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

#include <yasmic/ifstream_as_matrix.hpp>

int main(int argc, char **argv)
{
    using namespace std;
    
    if (argc != 2)
    {
        return (-1);
    }
    
    string filename = argv[1];
    
    using namespace yasmic;
    
    typedef ifstream simple_matrix;
    typedef smatrix_traits<simple_matrix>::size_type size_type;
    
    ifstream m(filename.c_str());
    
    
    
    size_type nr = nrows(m);
    size_type nc = ncols(m);
    cout << "nrows: " << nr << endl;
    cout << "ncols: " << nc << endl;
    
    {
    	smatrix_traits<simple_matrix>::nonzero_iterator nzi, nzend;
    
    	

    	
    	tie(nzi, nzend) = nonzeros(m);
    	
    	/*nz = istream_iterator<smatrix_traits<simple_matrix>::nonzero_descriptor>(m);
    	nzend = istream_iterator<smatrix_traits<simple_matrix>::nonzero_descriptor > ();*/
    	
    	
    	
    	/*cout << "nz: " << nz << endl;
    	cout << "nzend: " << nzend << endl;*/
    	
    	for (; nzi != nzend; ++nzi)
    	{
    
            cout << row(*nzi, m) << " " << column(*nzi, m) << " " << value(*nzi, m) << endl;
    	}
    	
    	/*m.seekg(0, std::ios_base::beg);
    	
    	while (m)
    	{
    		m >> nz;
    		
    		cout << nz << endl;
    	}*/
    }
    
    
    return (0);
}