#ifndef STARTUP_H
#define STARTUP_H
#include "comm_headers.h"

class Graph_Node{
  private:
    string Node_Name;  // Variable name
    vector<int> Children; // Children of a particular node - these are index of nodes in graph.
    vector<string> Parents; // Parents of a particular node- note these are names of parents
    int nvalues;  // Number of categories a variable represented by this node can take
    vector<string> values; // Categories of possible values
    vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    vector<float> org_CPT;
  public:
    Graph_Node(string name,int n,vector<string> vals);
    string get_name();
    vector<int> get_children();
    vector<string>  get_Parents();
  	vector<float> get_CPT();
    vector<float> get_org_CPT();

  	int  get_nvalues();
    void  set_CPT(vector<float> new_CPT);
    void  set_org_CPT(vector<float> new_CPT);
    void  set_Parents(vector<string> Parent_Nodes);
    int add_child(int new_child_index );
  	vector<string> get_values();

};


class network{
  public:
      list <Graph_Node> Pres_Graph;
      int addNode(Graph_Node node);
    	int netSize();
      int get_index(string val_name);
      list<Graph_Node>::iterator get_nth_node(int n);
      list<Graph_Node>::iterator search_node(string val_name);
};

network read_network();


#endif
