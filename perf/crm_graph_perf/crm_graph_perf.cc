
/*
 * David Gleich
 * yasmic
 * 2005
 */
 
/**
 * @file crm_graph_perf.cc
 * Performance test of the yasmic library and crm_graph adapter.
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm>

#include <numeric>

//#include <yasmic/ifstream_as_matrix.hpp>
#include <yasmic/ifstream_matrix.hpp>
#include <yasmic/binary_ifstream_matrix.hpp>
#include <yasmic/compressed_row_matrix.hpp>
#include <yasmic/compressed_row_matrix_graph.hpp>
#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/boost_mod/kruskal_min_spanning_tree.hpp>

#include <yasmic/iterator_utility.hpp>

#include <boost/timer.hpp>

/*
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/detail/edge.hpp>
*/

/*
*/
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

#define MAX_TRY 10

int main(int argc, char **argv)
{
    using namespace std;
    
    if (argc != 2)
    {
        return (-1);
    }
    
    string filename = argv[1];

	cout << "matrix filename: " << filename << endl;

	using namespace yasmic;
	using namespace boost;

	boost::timer t0;
    
    typedef binary_ifstream_matrix<> simple_matrix;
    typedef smatrix_traits<simple_matrix>::size_type size_type;

	
    
    /*typedef compressed_row_matrix<
        vector<int>::iterator, vector<int>::iterator, constant_iterator<double>  >
        crs_matrix;       */
	/*typedef compressed_row_matrix<
        vector<int>::iterator, vector<int>::iterator, counting_iterator<int>  >
        crs_matrix;*/       
	typedef compressed_row_matrix<
        vector<int>::iterator, vector<int>::iterator, constant_iterator<double> >
        crs_matrix;
    typedef simple_csr_matrix<int, double> csr_matrix;
    
    ifstream fs(filename.c_str(), ios::binary);
	simple_matrix m(fs);
    
    size_type nr = nrows(m);
    size_type nc = ncols(m);

    vector<int> rows(nr+1);
    vector<int> cols(nnz(m));
    vector<double> vals(nnz(m));
    
    {
    	vector<int> cur(nr);
		size_type nzcount = 0;
    	
    	smatrix_traits<simple_matrix>::nonzero_iterator nzi, nzend;
    	tie(nzi, nzend) = nonzeros(m);
    	for (; nzi != nzend; ++nzi)
    	{
            ++rows[row(*nzi, m)+1];
    	}
    	
    	// compute the reduction
    	partial_sum(rows.begin(), rows.end(), rows.begin());
    	
    	tie(nzi, nzend) = nonzeros(m);
    	for (; nzi != nzend; ++nzi)
    	{
            cols[rows[row(*nzi, m)]+cur[row(*nzi, m)]] = column(*nzi, m);
            vals[rows[row(*nzi, m)]+cur[row(*nzi, m)]] = value(*nzi, m);
            ++cur[row(*nzi, m)];
    	}
    }


	double val = 1.0;
	constant_iterator<double>  cvals(&val), cvals_end(&val);
	//constant_iterator<double>  cvals(1.0), cvals_end(1.0, nnz(m)-1);
    //counting_iterator<int> cvals(0), cvals_end(nnz(m)-1);
	//double *cvals = NULL, *cvals_end = (double*)(NULL+nnz(m)-1);

    crs_matrix crm(rows.begin(), rows.end(), cols.begin(), cols.end(),
            cvals, cvals_end, nr, nc, nnz(m));

	cout << "matrix load time: " << t0.elapsed() << " seconds" << endl;

	/*t0.restart();

	int num_components = 0;

	for (int ntry = 0; ntry < MAX_TRY; ntry++)
	{
		vector<int> component_map(num_vertices(crm));
		// compute connected components
		num_components = strong_components(crm, 
			make_iterator_property_map(component_map.begin(), 
			get(vertex_index, crm)));
	}

	cout << "connected components time: " << t0.elapsed() << " seconds" << endl;
	cout << "components: " << num_components << endl;


    csr_matrix csr;
    csr.ai = &rows[0];
    csr.aj = &cols[0];
    csr.a = &vals[0];
    csr.nrows = nr;
    csr.ncols = nc;
    csr.nnz = nnz(m);

	t0.restart();

	num_components = 0;

	for (int ntry = 0; ntry < MAX_TRY; ntry++)
	{
		vector<int> component_map(num_vertices(csr));
		// compute connected components
		num_components = strong_components(csr, 
			make_iterator_property_map(component_map.begin(), 
			get(vertex_index, csr)));
	}

	cout << "connected components time: " << t0.elapsed() << " seconds" << endl;
	cout << "components: " << num_components << endl;


	t0.restart();

    typedef adjacency_list<vecS, vecS, directedS> Graph;

    Graph g(nrows(crm));

	{
		smatrix_traits<crs_matrix>::nonzero_iterator nzi, nzend;
	    
    	tie(nzi, nzend) = nonzeros(crm);
	    	
    	for (; nzi != nzend; ++nzi)
    	{
			add_edge(row(*nzi, crm), column(*nzi, crm), g);
    	}
	}

	cout << "convert to boost time: " << t0.elapsed() << " seconds" << endl;

	t0.restart();

	for (int ntry = 0; ntry < MAX_TRY; ntry++)
	{
		vector<int> component_map(num_vertices(g));
		// compute connected components
		num_components = strong_components(g, 
			make_iterator_property_map(component_map.begin(), get(vertex_index, g)));
	}

	cout << "connected components time: " << t0.elapsed() << " seconds" << endl;
	cout << "components: " << num_components << endl;

	t0.restart();*/

    {
        for (int ntry = 0; ntry < MAX_TRY; ntry++) {
            typedef simple_csr_matrix<int,double> crs_weighted_graph;
            crs_weighted_graph g(nr, nr, nnz(m), &rows[0], &cols[0], &vals[0]);
            std::vector<graph_traits<crs_weighted_graph>::edge_descriptor> 
                oi(nr-1);
            std::vector<graph_traits<crs_weighted_graph>::edge_descriptor>::iterator
                oi_end = kruskal_minimum_spanning_tree(g,oi.begin());
        }
    }
}

