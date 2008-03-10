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
#ifndef BOOST_GRAPH_CLUSTERING_COEFFICIENTS_HPP
#define BOOST_GRAPH_CLUSTERING_COEFFICIENTS_HPP

namespace boost {
    template <class Visitor, class Graph>
    class DFSVisitorConcept {
    public:
        void constraints() {
            function_requires< CopyConstructibleConcept<Visitor> >();
            vis.triangle(x,y,z,g);
        }
    private:
        Visitor vis;
        Graph g;
        typename graph_traits<Graph>::vertex_descriptor x,y,z;
        typename graph_traits<Graph>::edge_descriptor e;
    };

    namespace detail {
        /*template <class Vertex, class IndMap>
        struct in_indicator_pred
	        : public std::unary_function<Vertex, bool>
        {
	        in_indicator_pred(IndMap indmap, Vertex src)
		        : i(indmap), u(src)
	        {}
	        IndMap i;
            Vertex u;
	        bool operator() (const Vertex &v) const
	        {   
		        return (i[v] > 0) && (u != v);
	        }
        };*/

        template <class Graph, class CCMap, class CCVisitor, class ColorMap>
        clustering_coefficients_impl(
            const Graph& g, CCMap cc, CCVisitor vis, ColorMap color_map)
        {
            function_requires<AdjacencyGraphConcept<Graph> >();
            function_requires<CCVisitorConcept<CCVisitor, Graph> >();

            typedef typename graph_traits<Graph>::vertex_descriptor vertex;
            typename property_traits<CCMap>::value_type cc_type;

            typename graph_traits<Graph>::vertex_iterator vi,vi_end;
            typename graph_traits<Graph>::adjacency_iterator ai,ai_end;
            typedef color_traits<ColorValue> Color;
            for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
                put(color_map, *vi, Color::white());
            }
            for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
                vertex v = *vi;
                for (tie(ai,ai_end)=adjacacent_vertices(v,g);ai!=ai_end;++ai) {
                    put(color_map, *ai, Color::black());
                }
                typename graph_traits<Graph>::degree_size_type d = out_degree(v,g);
                cc_type cur_cc = 0;
                for (tie(ai,ai_end)=adjacacent_vertices(v,g);ai!=ai_end;++ai) {
                    vertex w = *ai;
                    // don't count self-loops
                    if (v == w) { --d; continue; }
                    typename graph_traits<Graph>::adjacency_iterator ai2,ai2_end;
                    for (tie(ai2,ai2_end) = adjacenct_vertices(w,g);ai2!=ai2_end;++ai2) {
                        if (get(color_map, *ai2) == Color::black()) {
                            vis.triangle(v,w,*ai2);
                            ++cur_cc;
                        }
                    }
                }
                if (d>1) {  
                    cc[v] = cur_cc/(cc_type)(d*(d-1));
                } else {
                    cc[v] = (cc_type)0;
                }

                // reset the colors
                for (tie(ai,ai_end)=adjacacent_vertices(v,g);ai!=ai_end;++ai) {
                    put(color_map, *ai, Color::white());
                }
            }
        }

        template <typename WeightMap>
        struct clustering_coefficients_dispatch1 {
            template <class Graph, class CCMap, class VertexIndexMap>
            static void run(const Graph& g, CCMap cc, 
                VertexIndexMap vind, WeightMap wm)
            {}
        };
        template <>
        struct clustering_coefficients_dispatch1<detail::error_property_not_found> {
            template <class Graph, class CCMap, class VertexIndexMap>
            static void run(const Graph& g, CCMap cc, 
                VertexIndexMap vind, detail::error_property_not_found)
            {
            }
        };

    } // namespace detail

    template <class Visitors = null_visitor>
    class clustering_coefficients_visitor {
    public:
        clustering_coefficients_visitor() {}
        clustering_coefficients_visitor(Visitors vis) : m_vis(vis) {}

        template <class Vertex, class Graph>
        void triangle(Vertex x, Vertex y, Vertex z, const Graph& g) {
            invoke_visitors(m_vis, x, y, z, g, ::boost::on_triangle());
        }

        BOOST_GRAPH_EVENT_STUB(on_triangle,clustering_coefficients)

    protected:
        Visitors m_vis;
    };
    template <class Visitors>
    clustering_coefficients_visitor<Visitors>
    make_clustering_coefficients_visitor(Visitors vis) {
        return clustering_coefficients_visitor<Visitors>(vis);
    }
    typedef clustering_coefficients_visitor<> default_clustering_coefficients_visitor;

    template <class Graph, class CCMap, class P, class T, class R>
    void clustering_coefficients(const Graph& g, CCMap cc,
        const bgl_named_params<P,T,R>& params)
    {
        typedef bgl_named_params<P,T,R> named_params;
        typedef typename property_value<named_params, edge_weight_t>::type ew;
        detail::clustering_coefficients_dispatch1<ew>::run(
            g, cc,
            choose_const_pmap(get_param(params, vertex_index), g, vertex_index),
            get_param(params, edge_weight)
            );
    }
    
    template <class Graph, class CCMap>
    void clustering_coefficients(const Graph& g, CCMap cc)
    {
    }

    template <class Graph, class CCMap, class CCVisitor>
    void clustering_coefficients(const Graph& g, CCMap cc,
        )
    {
    }
} // namespace boost

#endif // BOOST_GRAPH_CLUSTERING_COEFFICIENTS_HPP

