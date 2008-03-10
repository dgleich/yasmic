
/**
 * @file read_matrix.cc
 * Check the performance of all the algorithms to read a matrix in yasmic
 * against the performance of native algorithms to accomplish
 * the same thing.
 */

/*
 * David Gleich
 * Stanford University
 * 11 November 2006
 * Copyright Stanford University
 *
 * Last modified: $Date: $
 * $Revision: $
 */
 
 
/* includes from load_crm_matrix.hpp */ 
#ifdef BOOST_MSVC
#if _MSC_VER >= 1400
	// disable the warning for ifstream::read
    #pragma warning( push )
	#pragma warning( disable : 4996 )
#endif // _MSC_VER >= 1400
#endif // BOOST_MSVC

#include <cctype>
#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>

#include <string>
#include <algorithm>

#include <vector>

#include <yasmic/verbose_util.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#define YASMIC_UTIL_LOAD_GZIP

#include <yasmic/ifstream_matrix.hpp>
#include <yasmic/binary_ifstream_matrix.hpp>
#include <yasmic/cluto_ifstream_matrix.hpp>
#include <yasmic/graph_ifstream_matrix.hpp>

#ifdef YASMIC_UTIL_LOAD_GZIP

// include the boost code to do the gzip files.
#define BOOST_IOSTREAMS_NO_LIB
#include <boost/iostreams/filtering_stream.hpp>
#include <yasmic/boost_mod/gzip.hpp>
// just directly include the BOOST zlib code
#include <yasmic/boost_mod/zlib.cpp>

#endif // YASMIC_UTIL_LOAD_GZIP

/* end includes from load_crm_matrix.hpp */

/* includes for this file */
#include <zlib.h>

#include <util/timer.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <boost/lexical_cast.hpp>
/* end includes from this file */


/**
 * This function does the yasmic matrix read.
 */
template <class InputMatrix, class Index, class Value>
bool read_yasmic_matrix_degs(InputMatrix& m, std::string filename,
						 Index* degs, int num_tries, Value v)
{
	using namespace yasmic;
	using namespace std;
	
	int nr = nrows(m);
	
	typename smatrix_traits<InputMatrix>::nz_index_type nzcount = 0;
	
	typename smatrix_traits<InputMatrix>::nonzero_iterator nzi, nzend;

	int warning = 0;

    typename smatrix_traits<InputMatrix>::index_type last_good_r = 0;
    typename smatrix_traits<InputMatrix>::index_type last_good_c = 0;
	typename smatrix_traits<InputMatrix>::nz_index_type last_good_nz = 0;
	
	while (num_tries > 0)
	{
		// reset degrees
		for (int i = 0; i < nr; i++)
		{
			degs[i] = 0;
		}
		
		boost::tie(nzi, nzend) = nonzeros(m);
		for (; nzi != nzend; ++nzi)
		{
			/*warning = warning | (row(*nzi, m) >= nr) | (column(*nzi, m) >= nc);
            last_good_r = (!warning)*row(*nzi,m);
            last_good_c = (!warning)*column(*nzi,m);
            last_good_nz = (!warning)*nzcount;
			++degs[row(*nzi, m)*(!warning)];
			++nzcount;*/
			
			++degs[row(*nzi, m)];
		}
		
		/*if (warning)
		{
			using namespace std;
			cerr << "error: invalid matrix data, nrows or ncols exceeded (" 
                << last_good_r << "," << last_good_c << "," << last_good_nz << ")"
                << endl;
			return (false);
		}

		if (nzcount != nnz(m))
		{
			using namespace std;
			cerr << "error: number of nonzeros do not match nnz" << endl;
			return (false);
		}*/
		
		num_tries--;
	}
}

/** 
 * Test if a file with a .graph extension is a smat or not.
 */

template <class Index, class InputStream>
bool load_crm_matrix_graph_test(InputStream& ifs)
{
    using namespace std;

    string line;
    getline(ifs,line);
    istringstream iss(line);

    Index i;
    iss >> i;
    iss >> i;
    iss >> i;

    if (iss.fail())
    {
        return (false);
    }
    else
    {
        return (true);
    }
}


void read_clib_smat_degs(std::string filename, int* degs, int num_tries)
{
	using namespace std;
	
	while (num_tries > 0)
	{
		FILE *f = fopen(filename.c_str(), "rt");
		
		if (!f)
		{
			cerr << "err reading " << filename << endl; 
			return;
		}
		
		int nr, nc, nnz;
		fscanf(f, "%i %i %i", &nr, &nc, &nnz);
		
		int r, c;
		double v;
		
		while (nnz > 0)
		{
			fscanf(f, "%i %i %lf", &r, &c, &v);
			++degs[r];
			
			--nnz;
		}
		
		fclose(f);
		
		num_tries--;
	}
}


void read_clib_smat_gz_degs(std::string filename, int* degs, int num_tries)
{
	char line[1024];
	using namespace std;
	
	while (num_tries > 0)
	{
		gzFile f = gzopen(filename.c_str(), "rt");
		
		if (!f)
		{
			cerr << "err reading " << filename << endl; 
			return;
		}
		
		gzgets(f, line, 1024);
		
		int nr, nc, nnz;
		sscanf(line, "%i %i %i", &nr, &nc, &nnz);
		
		int r, c;
		double v;
		
		while (nnz > 0)
		{
			gzgets(f, line, 1024);
			
			if (sscanf(line, "%i %i %lf", &r, &c, &v) != 3)
			{
				continue;
			}
			
			
			
			++degs[r];
			
			--nnz;
		}
		
		gzclose(f);
		
		num_tries--;
	}
}

void read_clib_bsmat_degs(std::string filename, int* degs, int num_tries)
{
	using namespace std;
	
	while (num_tries > 0)
	{
		FILE *f = fopen(filename.c_str(), "r");
		
		if (!f)
		{
			cerr << "err reading " << filename << endl; 
			return;
		}
		
		int nr, nc, nnz;
		fread(&nr, sizeof(int), 1, f);
		fread(&nc, sizeof(int), 1, f);
		fread(&nnz, sizeof(int), 1, f);
		
		int r, c;
		double v;
		
		while (nnz > 0)
		{
			fread(&r, sizeof(int), 1, f);
			fread(&c, sizeof(int), 1, f);
			fread(&v, sizeof(double), 1, f);
			
			++degs[r];
			
			--nnz;
		}
		
		fclose(f);
		
		num_tries--;
	}
}
void read_clib_bsmat_gz_degs(std::string filename, int* degs, int num_tries)
{
	char line[1024];
	using namespace std;
	
	while (num_tries > 0)
	{
		gzFile f = gzopen(filename.c_str(), "rb");
		
		if (!f)
		{
			cerr << "err reading " << filename << endl; 
			return;
		}
		
		
		
		int nr, nc, nnz;
		gzread(f, &nr, sizeof(int)*1);
		gzread(f, &nc, sizeof(int)*1);
		gzread(f, &nnz, sizeof(int)*1);
		
		
		
		int r, c;
		double v;
		
		while (nnz > 0)
		{
			gzread(f, &r, sizeof(int)*1);
			gzread(f, &c, sizeof(int)*1);
			gzread(f, &v, sizeof(double)*1);
			
			++degs[r];
			
			--nnz;
		}
		
		gzclose(f);
		
		num_tries--;
	}
}

 
 
int main(int argc, char **argv)
{
	using namespace std;
	
	util::timer::multi_timer mt;
	util::timer::timer_pointer t_yasmic, t_c;
	
	t_yasmic = mt.add("yasmic reads");
	t_c = mt.add("clib reads");
	
	if (argc < 2)
	{
		cerr << "usage: read_matrix <num_tries> matrixfile [matrixfile ...]";
		return (-1);
	}	
	
	double v = 0.0;
	
	int num_tries = boost::lexical_cast<int>(argv[1]);
	
	for (int matrix_filename_arg_num = 2; matrix_filename_arg_num < argc; ++matrix_filename_arg_num)
	{
		char *matrix_filename = argv[matrix_filename_arg_num];
		
		int *degs = NULL;
		
		cout << "reading " << matrix_filename << "..." << endl;
		
		std::string filename = matrix_filename;

		//
		// stolen from load_crm_matrix.hpp
		//
		
		// look at the extension...
		typedef string::size_type position;
		
		position dot = filename.find_last_of(".");
		
		if (dot != string::npos)
		{
			string ext = filename.substr(dot+1);
			transform(ext.begin(), ext.end(), ext.begin(), (int(*)(int))tolower);	
	
	        // handle the filtering of the iostream
	        bool ios_filter = false;
	
	#ifdef YASMIC_UTIL_LOAD_GZIP
	        typedef boost::iostreams::filtering_stream<boost::iostreams::input_seekable> filtered_ifstream;
	        filtered_ifstream ios_fifs;
	
	        if (ext.compare("gz") == 0)
	        {
	            position dot2 = filename.find_last_of(".",dot-1);
	            if (dot == string::npos)
	            {
	                cerr << "Error: matrix type indeterminate." << endl;
	                return (false);
	            }
	
	            string ext2 = filename.substr(dot2+1, (dot)-(dot2+1));
			    transform(ext2.begin(), ext2.end(), ext2.begin(), (int(*)(int))tolower);	
	
	            if (ext.compare("gz") == 0)
	            {
	                ios_filter = true;
	                ios_fifs.push(boost::iostreams::gzip_decompressor());
	            }
	
	            ext = ext2;
	        }
	        else
	        {
	        }
	
	#endif // YASMIC_UTIL_LOAD_GZIP
	
	        bool smat_graph = false;
	
	        if (ext.compare("graph") == 0)
	        {
	            // if the first line of a .graph file has 3 entries, 
	            if (ios_filter)
	            {
	                ifstream ifs(filename.c_str());
	                ios_fifs.push(ifs);
	                smat_graph = load_crm_matrix_graph_test<int>(ios_fifs);
	                ios_fifs.pop();
	            }
	            else
	            {
	                ifstream ifs(filename.c_str());
	                smat_graph = load_crm_matrix_graph_test<int>(ifs);
	            }            
	        }
	
			if (ext.compare("smat") == 0 || smat_graph)
			{
				YASMIC_VERBOSE( std::cerr << "using smat loader..." << std::endl; )
	
	            if (ios_filter)
	            {
	                ifstream ifs(filename.c_str(), ios_base::in | ios_base::binary);
	                ios_fifs.push(ifs);
				    yasmic::ifstream_matrix<> m(ios_fifs);
				    
				    degs = new int[nrows(m)];				    
				    
				    mt.push("[yasmic] reading " + filename);
				    t_yasmic->start();
				    read_yasmic_matrix_degs(m, filename, degs, num_tries, v);
				    t_yasmic->pause();
				    mt.pop();
				    
				    mt.push("[clib] reading " + filename);
				    t_c->start();
				    read_clib_smat_gz_degs(filename, degs, num_tries);
				    t_c->pause();
				    mt.pop();
	            }
	            else
	            {
	                ifstream ifs(filename.c_str());
	                ios_fifs.push(ifs);
	                yasmic::ifstream_matrix<> m(ios_fifs);
	
					degs = new int[nrows(m)];
				    
				    mt.push("[yasmic] reading " + filename);
				    t_yasmic->start();
				    read_yasmic_matrix_degs(m, filename, degs, num_tries, v);
				    t_yasmic->pause();
				    mt.pop();
				    
				    mt.push("[clib] reading " + filename);
				    t_c->start();
				    read_clib_smat_degs(filename, degs, num_tries);
				    t_c->pause();
				    mt.pop();
	            }
			}
			else if (ext.compare("bsmat") == 0)
			{
				YASMIC_VERBOSE( std::cerr << "using bsmat loader..." << std::endl; )
			
	
				ifstream ifs(filename.c_str(), ios::binary);
				yasmic::binary_ifstream_matrix<> m(ifs);
				
				degs = new int[nrows(m)];
				
				mt.push("[yasmic] reading " + filename);
			    t_yasmic->start();
			    read_yasmic_matrix_degs(m, filename, degs, num_tries, v);
			    t_yasmic->pause();
			    mt.pop();
			    
			    
			    if (ios_filter)
			    {
				    mt.push("[clib] reading " + filename);
				    t_c->start();
				    read_clib_bsmat_gz_degs(filename, degs, num_tries);
				    t_c->pause();
				    mt.pop();
			    }
			    else
			    {
			    	mt.push("[clib] reading " + filename);
				    t_c->start();
				    read_clib_bsmat_degs(filename, degs, num_tries);
				    t_c->pause();
				    mt.pop();
			    }
			}
	        /*else if (ext.compare("mat") == 0 || ext.compare("cmat") == 0 
	                 || ext.compare("cgraph") == 0)
			{
				YASMIC_VERBOSE( std::cerr << "using cluto loader..." << std::endl; )
	
				ifstream ifs(filename.c_str());
				yasmic::cluto_ifstream_matrix<> m(ifs);
				return (read_yasmic_matrix_degs(m, filename, degs, num_tries));
			}
	        else if (ext.compare("graph") == 0)
	        {
	            YASMIC_VERBOSE( std::cerr << "using graph loader..." << std::endl; )
	            ifstream ifs(filename.c_str());
				yasmic::graph_ifstream_matrix<> m(ifs);
				return (read_yasmic_matrix_degs(m, filename, degs, num_tries));
	        }*/
			else
			{
				cerr << "Error: matrix type unknown." << endl;
			}
		}
		else
		{
			cerr << "Error: matrix type indeterminate." << endl;
			return (false);
		}
		
		if (degs)
		{
			delete[] degs;
		}
	}
	
	mt.dump();
}

