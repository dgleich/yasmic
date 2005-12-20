
/*
 * David Gleich
 * yasmic
 * 2005
 */
 
/**
 * @file compressed_row_matrix_test.cc
 * Try using the compressed_row_matrix.hpp header.
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm>

#include <numeric>



#include <yasmic/ifstream_as_matrix.hpp>
#include <yasmic/compressed_row_matrix.hpp>

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
    
    typedef compressed_row_matrix<
        vector<int>::iterator, vector<int>::iterator, vector<double>::iterator >
        crs_matrix;
        
    
    ifstream m(filename.c_str());
    
    size_type nr = nrows(m);
    size_type nc = ncols(m);
    cout << "nrows: " << nr << endl;
    cout << "ncols: " << nc << endl;
    cout << "nnz: " << nnz(m) << endl;
    
    {
    	smatrix_traits<simple_matrix>::nonzero_iterator nzi, nzend;
    	
    
    	tie(nzi, nzend) = nonzeros(m);
    	for (; nzi != nzend; ++nzi)
    	{
    
            cout << "ifstream: " <<  row(*nzi, m) << " " << column(*nzi, m) << " " << value(*nzi, m) << endl;
    	}
    	    	
    
    }
    
    vector<int> rows(nr+1);
    vector<int> cols(nnz(m));
    vector<double> vals(nnz(m));
    
    {
    	vector<int> cur(nr);
    	
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
    }
    
    cout << "rows: " << endl;
    copy(rows.begin(), rows.end(), ostream_iterator<int>(cout, "\n"));
    
    cout << "cols: " << endl;
    copy(cols.begin(), cols.end(), ostream_iterator<int>(cout, "\n"));

    
    crs_matrix crm(rows.begin(), rows.end(), cols.begin(), cols.end(),
            vals.begin(), vals.end(), nr, nc, nnz(m));


    {
    	smatrix_traits<crs_matrix>::nonzero_iterator nzi, nzend;
    	tie(nzi, nzend) = nonzeros(crm);
    	
		for (; nzi != nzend; ++nzi)
    	{
    
            cout << row(*nzi, crm) << " " << column(*nzi, crm) << " " << value(*nzi, crm) << endl;
    	}

		smatrix_traits<crs_matrix>::row_iterator ri, riend;
		

		ri = crm.begin_rows();
		riend = crm.end_rows();

		for (; ri != riend; ++ri)
		{
			cout << "row_start: " << *ri << endl;

			smatrix_traits<crs_matrix>::row_nonzero_iterator rnzi, rnziend;
			tie(rnzi, rnziend) = row_nonzeros(*ri, crm);
			for (; rnzi != rnziend; ++rnzi)
			{
				cout << *ri << " " << column(*rnzi, crm) << " " << row_value(*rnzi, crm) << endl;
				//cout << *rnzi << " " << column(*rnzi, crm) << endl;
			}
		}
    }
    
    cout << "nrows: " << nrows(crm) << endl;
    cout << "ncols: " << ncols(crm) << endl;
    cout << "nnz: " << nnz(crm) << endl;
    
}