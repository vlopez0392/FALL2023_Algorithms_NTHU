/*
    FALL 2023 CS431102 Bonus programming Assignment 4
    Student: Victor D. Lopez 
    ID: 110062426
    
    //COMPILE WITH: g++ -Wall -std=c++11 -o bonus4 kruskal.cpp

    //EXECUTE WITH: ./bonus4 
*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <limits>
#include <map>

//A map (int to string) to pretty print edges to the console
const std::map<int, std::string> edgeNames = {
    {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {4, "e"}, 
    {5, "f"}, {6, "g"}, {7, "h"}, {8, "i"}
};

class Edge{
    public:
        int u;
        int v;
        int w; //u and v are vertices and w is the edge weight
        Edge(int u, int v, int w){
            this->u = u;
            this->v = v;
            this->w = w;
        }
};

//Pretty preting edges
void prettyPrintEdge(std::vector<Edge *>& edges, std::map<int, std::string> edgeNames){
    for(Edge * e: edges){
        std::cout << " Edge: " << edgeNames[e->u] << " --->  " << edgeNames[e->v] 
        << " || edge weight: " << e->w <<std::endl; 
    }
}

void cycleEdge(Edge * e, std::map<int, std::string> edgeNames){
    std::cout << " Edge: " << edgeNames[e->u] << " --->  " << edgeNames[e->v] 
    << " discarded due to cycle formation \n" ; 
}

void treeEdge(Edge * e, std::map<int, std::string> edgeNames){
    std::cout << " Edge: " << edgeNames[e->u] << " --->  " << edgeNames[e->v] 
    << " with edge weight: " << e->w << " chosen \n" ; 
}

//Disjoint set and Union-find
void makeSet(int v, std::vector<int>& parent, std::vector<int>& rank){
    parent[v] = v;
    rank[v] = 0;
}

int find(int v, std::vector<int>& parent){
    if(v == parent[v]){
        return v;
    }else{
        int res = find(parent[v], parent);
        parent[v] = res;
        return res;
    }
}

void Union(int u, int v, std::vector<int>& parent, std::vector<int>& rank){
    int a = find(u, parent);
    int b = find(v, parent);
    if (a != b) { 
        if (rank[a] < rank[b]) { 
            parent[a] = b; 
        } 
        else if (rank[a] > rank[b]) { 
            parent[b] = a; 
        } 
        else { 
            parent[b] = a; 
            rank[a] += 1; 
        } 
    } 
}

int computeMSTcost(std::vector<Edge *>& MST){

    int cost = 0;
    for(Edge * e: MST){
        cost += e->w;
    }
    return cost;
}

int main(int argc, char *argv[]){
    int n = 14; //Number of edges   
    
    //Add the edges to the edge vector and create graph
    std::vector<Edge *> edges;

    edges.emplace_back(new Edge(0,1,4)); // vertex a, vertex b, weight
    edges.emplace_back(new Edge(1,2,8)); // b,c 
    edges.emplace_back(new Edge(2,3,7)); // c,d
    edges.emplace_back(new Edge(3,4,9)); // d,e
    edges.emplace_back(new Edge(4,5,10)); //e,f
    edges.emplace_back(new Edge(5,6,2));  //f,g
    edges.emplace_back(new Edge(6,7,1));  //g,h
    edges.emplace_back(new Edge(7,8,7));  //h,i

    edges.emplace_back(new Edge(0,7,8));  //a,h
    edges.emplace_back(new Edge(1,7,11)); //b,h
    edges.emplace_back(new Edge(2,5,4));  //c,f
    edges.emplace_back(new Edge(2,8,2));  //c,i
    edges.emplace_back(new Edge(3,5,14)); //d,f
    edges.emplace_back(new Edge(6,8,6));  //g,i

    //Begin Kruskals algorithm to find MST 
    std::vector<int> parent(n);
    std::vector<int> rank(n);

    for(int i = 0 ; i < n; i++){
        makeSet(i, parent, rank);
    };

    //Sort edges by increasing weight
    std::cout << "Input graph edges: " << std::endl;
    prettyPrintEdge(edges, edgeNames);
    std::cout << "---------------------------------" << '\n';

    //Kruskals
    std::sort(edges.begin(), edges.end(),[](Edge * a, Edge * b){
                    return a->w < b->w;});
    std::vector<Edge * > MST; 
    for(Edge * e: edges){
        if(find(e->u, parent) != find(e->v, parent)){
            MST.emplace_back(e);
            Union(e->u, e->v,parent, rank);
            treeEdge(e, edgeNames);
        }else{
            cycleEdge(e, edgeNames);
        }
    }
    std::cout << "---------------------------------" << '\n';
    int cost = computeMSTcost(MST);
    
    std::cout << "Edges that belong to the MST : " << std::endl;
    prettyPrintEdge(MST, edgeNames);
    std::cout << "---------------------------------" << '\n';
    std::cout << "Cost of MST is: " << cost << std::endl;
}




