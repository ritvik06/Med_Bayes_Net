#include "bayes_learning.h"

//Need to make it plug and play for different intialization strategies
/*
Current Strategy:
 If a node has no parents, then check its column in the DB. Remove all '-1' and
 get probability on the remaining values. WONT WORK IF EMPTY

If it has parents, Get the remaining probability from the known data. \
Then divide it uniformly

*/
void initialize_probability(network* n, DATABASE db){

  list<Graph_Node> g_l = (*n).Pres_Graph;
  list <Graph_Node>::iterator it;
  int ind=0;
  for (it = g_l.begin();it!=g_l.end();it++){
    Graph_Node* curr_node = &(*((*n).get_nth_node(ind)));
    vector<float> curr_table = (*curr_node).get_CPT();
    if ((*curr_node).get_Parents().size()!=0){ //If not a starting node
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
    (*curr_node).set_CPT(curr_table);
  }else{
    int n_r = db.size();
    int n_c = db[0].size();
    int all_v = (*curr_node).get_nvalues();
    vector<float> freq(all_v,0);
    int all_sum=0;
    for (int i = 0; i< n_r;i++){
      int val = db[i][ind];
      if (val >= 0){
        freq[val]++;
        all_sum++;
      }
    }
    for (int i = 0; i<all_v;i++){
      freq[i]/=all_sum;
    }
    (*curr_node).set_CPT(freq);
  }
  // cout<<curr_node.get_CPT()[0]<<endl;
  ind++;
  // cout<<ind<<endl;
}

}

// Function for filling the database from the CPT, E Step
DATABASE modify_database(DATABASE db, network &n){
  vector<string> parents;
  vector<float> CPT_node;
  vector<int> CPT_num;
  vector<float> prob_list;
  vector<float>::iterator x;
  // int alpha = 0.5;
  int first_rank;
  int second_rank;
  int max_product,sum,temp_sum;

  for(int i=0;i<db.size();i++){
    int index = ques_pos[i];
    Graph_Node ques_node = (*n.get_nth_node(index));
    parents = ques_node.get_Parents();
    CPT_node = ques_node.get_CPT();
    sum=0,temp_sum=0;
    max_product=1;
    for(int j=(parents.size()-1);j>=0;j--){
      if(j==parents.size()){
        sum+=db[i][n.get_index(parents[j])];  
        max_product=(*n.search_node(parents[j])).get_nvalues();
      }
      else{
        sum+=(db[i][n.get_index(parents[j])]*max_product);
        max_product*=(*n.search_node(parents[j])).get_nvalues();
      }

    }

    for(int j=0;j<ques_node.get_nvalues();j++){
      temp_sum=0;
      temp_sum=sum+(j*max_product);
      prob_list.push_back(CPT_node[temp_sum]);
      // cout << CPT_node[temp_sum] << " ";
    }
    // cout << endl;

    float random = ((float) rand()/RAND_MAX);
    // cout << random;
    sort(prob_list.begin(),prob_list.end());
    for(x = prob_list.begin();x!=prob_list.end();x++){
      if(random>(*x)) break;
    }

    
    db[i][index] = distance(prob_list.begin(),x);

    prob_list.clear();
  }

  return db;

}

void m_step(network* n, DATABASE db){
    list<Graph_Node> g_l = (*n).Pres_Graph;
    list<Graph_Node>::iterator it;
    int ind = 0;
    int n_r = db.size();
    int n_c = db[0].size();

    for (it = g_l.begin();it!=g_l.end();it++){
      Graph_Node* curr_node = &(*((*n).get_nth_node(ind)));
      vector<float> curr_table = (*curr_node).get_CPT();
      vector<float> org_table = (*curr_node).get_org_CPT();
      int np = (*curr_node).get_nvalues();

      vector<string> parents = (*curr_node).get_Parents();
      vector<int> index_list,max_poss_list;
      for (int i = 0; i<parents.size();i++){
        int curr_ind = (*n).get_index(parents[i]);
        Graph_Node *p_node = &(*((*n).get_nth_node(curr_ind)));
        max_poss_list.push_back((*p_node).get_nvalues());
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
      (*curr_node).set_CPT(final_table);
      ind++;
    }

}




int main(int argc, char const *argv[]) {
    network Alarm;

// <<<<<<< HEAD
//     vector<vector<int> > 
//     Alarm = read_network();
//     dat_reader(Alarm);

//     initialize_probability(Alarm,)
//     // Graph_Node g = *(Alarm.get_nth_node(0));
// =======
    (Alarm) = read_network();
    // cout<<"Test"<<endl;
    DATABASE d = dat_reader(Alarm);
    initialize_probability(&Alarm,d);
    DATABASE new_db = modify_database(d,Alarm);

    for(int i=0;i<5;i++){
      for(int j=0;j<37;j++){
        cout << d[i][j] << " ";
      }
      cout << endl;
    }
      cout << endl;
      cout << endl;
      cout << endl;
      cout << endl;


    for(int i=0;i<5;i++){
      for(int j=0;j<37;j++){
        cout << new_db[i][j] << " ";
      }
      cout << endl;
    }

    m_step(&Alarm,new_db);

    // for (int j = 0;j<37;j++){
    //   cout<<"------"<<j+1<<"-----"<<endl;
    //   list<Graph_Node>::iterator g = (Alarm.get_nth_node(j));
    //   cout<<"Num of Parents: "<<(*g).get_Parents().size()<<endl;
    //   for (int i = 0; i< (*g).get_CPT().size();i++){
    //     cout<<((*g).get_CPT())[i]<<endl;
    //   }
    //   // cout<<"------"<<endl;
    // }

    // Graph_Node g = *(Alarm.get_nth_node())
    // cout<<it.begin()<<" "<<it++<<" "<<endl;
  return 0;
}
