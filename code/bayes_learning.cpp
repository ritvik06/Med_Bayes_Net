#include "bayes_learning.h"

//Need to make it plug and play for different intialization strategies
/*
Current Strategy:
 If a node has no parents, then check its column in the DB. Remove all '-1' and
 get probability on the remaining values. WONT WORK IF EMPTY

If it has parents, Get the remaining probability from the known data. \
Then divide it uniformly

*/

void initialize_probability(network &n, DATABASE &db){
  list<Graph_Node> g_l = n.Pres_Graph;
  list <Graph_Node>::iterator it;
  int ind=0;
  for (it = g_l.begin();it!=g_l.end();it++){
    Graph_Node curr_node = *(n.get_nth_node(ind));
    vector<float> curr_table = curr_node.get_CPT();
    if (curr_node.get_Parents().size()!=0){ //If not a starting node
      float filled=0;
      int num=0;
      for (int i = 0; i< curr_table.size();i++){
        if (curr_table[i]>=0){
          filled+=curr_table[i];
        }else{
          num++;
        }
      }
      float left = (1-filled)/num;
      for (int i = 0; i< curr_table.size();i++){
        if (curr_table[i]<0){
          curr_table[i] = left;
        }
    }
    curr_node.set_CPT(curr_table);
  }else{
    int n = db.size();
    int m = db[0].size();
    int all_v = curr_node.get_nvalues();
    vector<float> freq(all_v,0);
    int all_sum=0;
    for (int i = 0; i< n;i++){
      int val = db[i][ind];
      if (val >= 0){
        freq[val]++;
        all_sum++;
      }
    }
    for (int i = 0; i<all_v;i++){
      freq[i]/=all_sum;
    }
    curr_node.set_CPT(freq);
  }
  ind++;
}
}

// Function for filling the database from the CPT, E Step
DATABASE modify_database(DATABASE db, network n){
  list<Graph_Node> g_l = n.Pres_Graph;
  vector<string> parents;
  vector<float> CPT_node;
  vector<int> CPT_num;
  vector<float> prob_list;
  // int alpha = 0.5;
  int first_rank;
  int second_rank;
  int max_product,sum;

  for(int i=0;i<db.size();i++){
    int index = ques_pos[i];
    Graph_Node ques_node = *(n.get_nth_node(index));
    parents = ques_node.get_Parents();
    CPT_node = ques_node.get_CPT();
    sum=0;
    for(int j=(parents.size()-1);j>=0;j--){
      if(j==parents.size()){
        sum+=db[i][n.get_index(parents[j])];  
        max_product=*
      }
      else{
        sum+=(parents[j]*max_product);
      }


    }

    for(int j=0;j<ques_node.get_nvalues();j++){
      second_rank*=first_rank;
      prob_list.push_back(CPT_node[second_rank-1]);
    }

    db[i][index] = distance(prob_list.begin(),max_element(prob_list.begin(),prob_list.end()));
  }

  return db;

}

// DATABASE modify_database(DATABASE db, network n){
//   DATABASE new_db = db;
//   int n,m;
//   n = db.size();
//   m = db[0].size();
//   for (int i=0;i<n;i++){
//     vector<int>row = db[i];
//     vector<int> p_val;
//     for(int j = 0; j<m;j++){
//       if (row[j]==-1){
//         Graph_Node g = *(n.get_nth_node(j));
//
//
//
//       }
//     }
//
//
//
//   }
// }



int main(int argc, char const *argv[]) {
    network Alarm;
    vector<vector<int> > 
    Alarm = read_network();
    dat_reader(Alarm);

    initialize_probability(Alarm,)
    // Graph_Node g = *(Alarm.get_nth_node(0));

    // cout<<it.begin()<<" "<<it++<<" "<<endl;
  return 0;
}
