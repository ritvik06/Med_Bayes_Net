#include "bayes_learning.h"

vector<float> initialize_probability(network n, vector<vector<int> > db){
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




int main(int argc, char const *argv[]) {
    network Alarm;
    Alarm = read_network();

    // Graph_Node g = *(Alarm.get_nth_node(0));

    // cout<<it.begin()<<" "<<it++<<" "<<endl;
  return 0;
}
