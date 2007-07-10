//
//=======================================================================
// Copyright 2007 Stanford University
// Authors: David Gleich
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
#ifndef BOOST_GRAPH_CORE_NUMBERS_HPP
#define BOOST_GRAPH_CORE_NUMBERS_HPP

/*
 *KCore
 *
 *Requirement:
 *      IncidenceGraph
 */

namespace boost {

    // A linear time O(m) algorithm to compute the indegree core number 
    // of a graph.
    //
    // The algorithm comes from:
    // Vladimir Batagelj and Matjaz Zaversnik, "An O(m) Algorithm for Cores 
    // Decomposition of Networks."  Sept. 1 2002.

    namespace detail {
        template <typename Graph, typename CoreMap>
        void compute_in_degree_map(Graph& g, CoreMap d)
        {
            typename graph_traits<Graph>::vertex_iterator vi,vi_end;
            typename graph_traits<Graph>::out_edge_iterator ei,ei_end;
            for (tie(vi,vi_end) = vertices(g); vi!=vi_end; ++vi) { 
                put(d,*vi,0);
            }
            for (tie(vi,vi_end) = vertices(g); vi!=vi_end; ++vi) {
                for (tie(ei,ei_end) = out_edges(*vi,g); ei!=ei_end; ++ei) {
                    put(d,target(*ei,g),get(d,target(*ei,g))+1);
                }
            }
        }

        // for this functions CoreMap must be initialized
        // with the in degree of each vertex
        template <typename Graph, typename CoreMap,
            typename PositionMap>
        typename property_traits<CoreMap>::value_type
        core_numbers_dispatch(Graph& g, CoreMap c, PositionMap pos)
        {
            typedef typename graph_traits<Graph>::vertices_size_type size_type;
            typedef typename graph_traits<Graph>::degree_size_type degree_type;
            typedef typename graph_traits<Graph>::vertex_descriptor vertex;
            typename graph_traits<Graph>::vertex_iterator vi,vi_end;

		    // compute the maximum degree (degrees are in the coremap)
            typename graph_traits<Graph>::degree_size_type max_deg = 0;
		    for (tie(vi,vi_end) = vertices(g); vi!=vi_end; ++vi) { 
                max_deg = (std::max)(max_deg, get(c,*vi));
            }
            // store the vertices in bins by their degree
            // allocate two extra locations to ease boundary cases
            std::vector<size_type> bin(max_deg+2);
            for (tie(vi,vi_end) = vertices(g); vi!=vi_end; ++vi) {  
                ++bin[get(c,*vi)];
            }
            // this loop sets bin[d] to the starting position of vertices
		    // with degree d in the vert array for the bucket sort
            size_type cur_pos = 0;
		    for (degree_type cur_deg = 0; cur_deg < max_deg+2; ++cur_deg) {
			    degree_type tmp = bin[cur_deg];
			    bin[cur_deg] = cur_pos;
			    cur_pos += tmp;
		    }
            // perform the bucket sort with pos and vert so that
            // pos[0] is the vertex of smallest degree
            std::vector<vertex> vert(num_vertices(g));
            for (tie(vi,vi_end) = vertices(g); vi!=vi_end; ++vi) { 
                vertex v=*vi; 
                size_type p=bin[c[v]];
			    put(pos,v,p);
			    vert[p]=v;
			    ++bin[get(c,v)];
		    }
            // we ``abused'' bin while placing the vertices, now, 
		    // we need to restore it
		    std::copy(boost::make_reverse_iterator(bin.end()-2),
			    boost::make_reverse_iterator(bin.begin()+1), 
			    boost::make_reverse_iterator(bin.end()-1));
            // now simulate removing the vertices
            for (size_type i=0; i < num_vertices(g); ++i) {
			    vertex v = vert[i];
                typename graph_traits<Graph>::out_edge_iterator oi,oi_end;
                for (tie(oi,oi_end) = out_edges(v,g); oi!=oi_end; ++oi) {
                    vertex u = target(*oi,g);
                    // if c[u] > c[v], then u is still in the graph,
				    // if c[u] = c[v], then we'll remove u soon, and
				    // it's core number is the same as v.
				    // if c[u] < c[v], we've already removed u.
                    if (get(c,u) > get(c,v)) {
                        degree_type deg_u = get(c,u);
                        degree_type pos_u = get(pos,u);
                        // w is the first vertex with the same degree as u
					    // (this is the resort operation!)
					    degree_type pos_w = bin[deg_u];
					    vertex w = vert[pos_w];
                        if (u!=v) {
                    	    // swap u and w
						    pos[u] = pos_w;
						    pos[w] = pos_u;
						    vert[pos_w] = u;
						    vert[pos_u] = w;
                        }
                        // now, the vertices array is sorted assuming
					    // we perform the following step
					    // start the set of vertices with degree of u 
					    // one into the future (this now points at vertex 
					    // w which we swapped with u).
					    ++bin[deg_u];
					    // we are removing v from the graph, so u's degree
					    // decreases
					    put(c,u,get(c,u)-1);
                    }
                }
            }
            return get(c,vert[num_vertices(g)-1]);
        }

    } // namespace detail

    /*template <typename Graph, typename CoreMap, class P, class T, class R>
    typename property_traits<CoreMap>::value_type
    core_numbers(Graph& g, CoreMap c, 
                 const bgl_named_params<P, T, R>& params)
    {
        typedef bgl_named_params<P,T,R> named_params;
        typedef typename property_value<named_params, degree_map_t>::type ew;

    }*/

    template <typename Graph, typename CoreMap>
    typename property_traits<CoreMap>::value_type
    core_numbers(Graph& g, CoreMap c)
    {
        typedef typename graph_traits<Graph>::vertices_size_type size_type;
        detail::compute_in_degree_map(g,c);
        return detail::core_numbers_dispatch(g,c,
            make_iterator_property_map(
                std::vector<size_type>(num_vertices(g)).begin(),get(vertex_index, g))
        );
    }

} // namespace boost

#endif // BOOST_GRAPH_CORE_NUMBERS_HPP

