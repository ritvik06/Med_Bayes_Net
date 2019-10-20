// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
#include "startup_code.h"
// Our graph consists of a list of nodes where each node is represented as follows:

	// Constructor- a node is initialised with its name and its categories
  Graph_Node::Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
		nvalues=n;
		values=vals;
	}
	string Graph_Node::get_name()
	{
		return Node_Name;
	}
	vector<int>  Graph_Node::get_children()
	{
		return Children;
	}
	vector<string>  Graph_Node::get_Parents()
	{
		return Parents;
	}
	vector<float>Graph_Node:: get_CPT()
	{
		return CPT;
	}
  vector<float>Graph_Node:: get_org_CPT()
  {
    return org_CPT;
  }

	int Graph_Node:: get_nvalues()
	{
		return nvalues;
	}
	vector<string>Graph_Node:: get_values()
	{
		return values;
	}
	void Graph_Node:: set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
  void Graph_Node:: set_org_CPT(vector<float> new_CPT)
	{
		org_CPT.clear();
		org_CPT=new_CPT;
	}
  void Graph_Node:: set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
  int Graph_Node::add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
    int Graph_Node::find_index(string str){
    	for(int i=0;i<nvalues;i++){
    		if(values[i].compare(str)==0){
    			return i;
    		}
    	}
    	return -1;
    }


 // The whole network represted as a list of nodes


	int network::addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}


	int network::netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int network::get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator network::get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt;
    }

    int network::num_vars(){
    	return Pres_Graph.size();
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator network::search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }

            cout<<"node not found\n";
        return listIt;
    }



// unordered_map<string,Graph_Node> variables;
// DATABASE data_values;
vector<int> ques_pos;
// network Alarm;

network read_network(string filename)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(filename);
  	string temp;
  	string name;
  	vector<string> values;

    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);


      		ss.str(line);
     		ss>>temp;


     		if(temp.compare("variable")==0)
     		{

     				ss>>name;
     				getline (myfile,line);

     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{

     					ss2>>temp;


     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);

     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);
     				// variables[name]=new_node;
     				// count_var++;
     		}
     		else if(temp.compare("probability")==0)
     		{

     				ss>>temp;
     				ss>>temp;

                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);

     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;

     				ss2.str(line);
     				ss2>> temp;

     				ss2>> temp;

     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{

     					curr_CPT.push_back(atof(temp.c_str()));
     					ss2>>temp;

    				}

                    listIt->set_CPT(curr_CPT);
                    listIt->set_org_CPT(curr_CPT);
     		}
            else
            {

            }
    	}

    	if(find==1)
    	myfile.close();
  	}

  	return Alarm;
}

DATABASE dat_reader(string filename,network n)
{
    DATABASE data_values;
	string val;
	ifstream myfile;
	myfile.open(filename);
	vector<int> row;

	if(myfile.is_open()){
		// cout << "entered" << endl;
		// cout << count_var;
		// cout << Alarm.num_vars();
		while(true){
			for(int i=0;i<n.num_vars();i++){
				myfile >> val;
				if(myfile.eof()) break;

				else if(val.compare("\"?\"")==0){
					// cout << "Entered " << i << endl;
                    ques_pos.push_back(i);
					row.push_back(-1);
				}
				else{
					int index = (*(n.get_nth_node(i))).find_index(val);
					row.push_back(index);
				}

			}

			// for(int i=0;i<count;i++){
			// 	cout << row[i] << " ";
			// }

			if(myfile.eof()) break;

			data_values.push_back(row);
			row.clear();

		}
	}

    return data_values;

}



// int main()
// {
// 	network Alarm;
// 	Alarm=read_network();
//
// // Example: to do something
// 	cout<<"Perfect! Hurrah! \n";
//
// }
