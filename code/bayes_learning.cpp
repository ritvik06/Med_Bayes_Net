#include "bayes_learning.h"

//Need to make it plug and play for different intialization strategies
/*
Current Strategy:
 If a node has no parents, then check its column in the DB. Remove all '-1' and
 get probability on the remaining values. WONT WORK IF EMPTY

If it has parents, Get the remaining probability from the known data. \
Then divide it uniformly

*/
void initialize_probability(network n, DATABASE db){
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

void m_step(network n, DATABASE db){
    list<Graph_Node> g_l = n.Pres_Graph;
    list<Graph_Node>::iterator it;
    int ind = 0;
    int n_r = db.size();
    int n_c = db[0].size();

    for (it = g_l.begin();it!=g_l.end();it++){
      Graph_Node curr_node = *(n.get_nth_node(ind));
      vector<float> curr_table = curr_node.get_CPT();
      vector<float> org_table = curr_node.get_org_CPT();
      int np = curr_node.get_nvalues();

      vector<string> parents = curr_node.get_Parents();
      vector<int> index_list,max_poss_list;
      for (int i = 0; i<parents.size();i++){
        int curr_ind = n.get_index(parents[i]);
        Graph_Node p_node = n.get_nth_node(curr_ind);
        max_poss_list.push_back(p_node.get_nvalues());
        index_list.push_back(curr_ind);
      }

      vector<float> final_table;
      for (int i = 0; i<org_table.size();i++){
        if (org_table[i] == -1){
          vector<int>prob;
          int cur_p = i/np;
          int rem =  i%np;
          prob.push_back(cur_p);
          for (int t = 0; t<max_poss_list.size();t++){
            cur_p = rem/max_poss_list[t];
            rem = rem%max_poss_list[t];
            prob.push_back(cur_p);
          }

          // for (int t = 0; t<par)
          // int exp = power(2,np+1);
          // int rem = i%exp;
          // for (int j = 0; j<np;j++){
          //   if (rem&(1<<(np-j-1)) != 0){
          //     prob.push_back(0);
          //   }else{
          //     prob.push_back(1);
          //   }
          // }
          //WRONG!!!


          int count=0;
          for (int j=0;j<n_r;j++){
            bool okRow=true;
            for (int l = 0; l<index_list.size();l++){
              if (db[j][l] == prob[l]){
                continue;
              }else{
                okRow=false;
                break;
              }
            }
            if (okRow){
              count++;
            }
          }
          float ans = count/n_r;
          final_table.push_back(ans);
        }else{
          final_table.push_back(curr_table[i]);
        }
      }
      curr_node.set_CPT(final_table);
      ind++;
    }

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
    Alarm = read_network();

    // Graph_Node g = *(Alarm.get_nth_node(0));

    // cout<<it.begin()<<" "<<it++<<" "<<endl;
  return 0;
}
