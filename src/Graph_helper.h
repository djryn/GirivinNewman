//
// Created by Daniel Ryan on 4/5/22.
//

#ifndef INC_22S_PA03_GRAPH_HELPER_H
#define INC_22S_PA03_GRAPH_HELPER_H

#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>
#include <typeinfo>
#include <cxxabi.h>
#include <fstream>
#include <queue>



struct VertexProperty {
    int community = 0;
    bool used = false;
    int distance = -1;
    int origDegree = 0;
    int newDegree = 0;
};
struct EdgeProperty {
    std::string Name = "m";
    int count = 0;
};

struct component {
    int compEdges = 0;
    int totalEdges = 0;
};

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty> Graph;
typedef boost::range_detail::integer_iterator<unsigned long> vertexIt;
typedef boost::graph_traits<Graph>::vertex_descriptor vd;


class Graph_helper {
private:
    Graph graph;
    int numNodes;
    int numEdges;
    std::map<vd, int> max_comp;
    double best_mod;
    int num_communities;

public:
    // functions for graph reading and testing
    void print_graph();
    void print_edges();
    void print_vertices();

    /**
     * Reads the graphml into an adjacency list. Also reads one property for nodes
     * @param file the name of the graphml file
     */
    void read_graphml(const char* file);

    /**
     *  sets the newDegree vertex attribute for every vertex in the current graph
     */
    void set_degree();

    /**
     *  Calculates the modularity of the current graph and components
     * @return the modularity
     */
    double get_modularity();

    /**
     *  The master function for the Girvan-Newman community detection algorithm. Uses
     *  Breadth First Search to find all shortest paths, finds edge betweeness centrality to remove
     *  the highest used edge. For every new partition of the graph a modularity is calculated to determine
     *  the graph's component density and outputs the most optimal cut.
     */
    void girvan_newman();

    /**
     *  Loop for Girvan-Newman algorithm. Removes edges until new component is created, leading to a new
     *  partition
     */
    void girvan_newman_helper();

    /**
     * Implementation of Breadth First Search in order to visit the graph by visiting each adjacent node
     * before moving further. Keeps track of each previously visited node for every node.
     * @param prev The list of each node's previously visited node
     * @param iter The vertex iterator for the current node
     */
    void breadth_first_search(std::map<vd, vd>& prev, vertexIt iter);

    /**
     * Uses prev list from BFS in order to reconstruct the shortest path for the current node to every other
     * node. Also counts the usage of each edge
     * @param prev The list of each node's previously visited node
     * @param iter The vertex iterator for the current node
     */
    void reconstruct_paths(std::map<vd, vd>& prev, vertexIt iter);

    /**
     * Resets data used for tracking usage of vertices
     * @param iter The vertex iterator for the current node
     */
    void reset_tracking_data(vertexIt iter);

    /**
     * Creates output file with community report for the graph
     * @param report the list of vertices in each component
     */
    void print_report(std::vector<std::string> report) const;

};

#endif //INC_22S_PA03_GRAPH_HELPER_H