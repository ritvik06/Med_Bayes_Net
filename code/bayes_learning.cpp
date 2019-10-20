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
    // Graph_Node* curr_node = &(*((*n).get_nth_node(ind)));
    Graph_Node* curr_node = &(*it);
    vector<float> curr_table = (*curr_node).get_CPT();
    int index_of_node =  (*n).get_index((*curr_node).get_name());
    // if ((*curr_node).get_Parents().size()!=0){ //If not a starting node
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
  // }else{
  //   int n_r = db.size();
  //   int n_c = db[0].size();
  //   int all_v = (*curr_node).get_nvalues();
  //   vector<float> freq(all_v,0);
  //   int all_sum=0;
  //   for (int i = 0; i< n_r;i++){
  //     int val = db[i][index_of_node];
  //     if (val >= 0){
  //       freq[val]++;
  //       all_sum++;
  //     }
  //   }
  //   for (int i = 0; i<all_v;i++){
  //     freq[i]/=all_sum;
  //   }
  //   (*curr_node).set_CPT(freq);
  // }
  // cout<<curr_node.get_CPT()[0]<<endl;
  // ind++;
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

    // float random = ((float) rand()/RAND_MAX);
    // cout << random;
    // sort(prob_list.begin(),prob_list.end());
    // for(x = prob_list.begin();x!=prob_list.end();x++){
    //   if(random>(*x)) break;
    // }


    db[i][index] = distance(prob_list.begin(),max_element(prob_list.begin(),prob_list.end()));

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
      int org_max_list_mul=1;
      for (int i = 0; i<parents.size();i++){
        int curr_ind = (*n).get_index(parents[i]);
        Graph_Node *p_node = &(*((*n).get_nth_node(curr_ind)));
        max_poss_list.push_back((*p_node).get_nvalues());
        index_list.push_back(curr_ind);
        org_max_list_mul*=((*p_node).get_nvalues());
      }

      index_list.push_back((*n).get_index((*curr_node).get_name()));

      // cout<< test<<endl;

      sort(index_list.begin(),index_list.end());

      // cout<<"Size of max_pos_list: "<<index_list.size()<<endl;
      // for (int w = 0; w<index_list.size();w++){
      //   cout<<max_poss_list[w]<<" ";
      // }
      // cout<<endl;
      // for (int w = 0; w<index_list.size();w++){
      //   cout<<index_list[w]<<" ";
      // }
      // cout<<endl<<"Number of elements in CPT: "<<org_table.size()<<" N values of the node: "<<np<<endl;
      // cout<<"------------"<<endl;

      vector<float> final_table;
      for (int i = 0; i<org_table.size();i++){
        if (org_table[i] == -1){
          int max_list_mul=org_max_list_mul;
          vector<int>prob;
          int cur_p = i/max_list_mul;
          int rem =  i%max_list_mul;
          max_list_mul/=np;
          if (max_list_mul==0){
            max_list_mul=1;
          }
          prob.push_back(cur_p);
          for (int t = 0; t<max_poss_list.size();t++){
            cur_p = rem/max_list_mul;
            rem = rem%max_list_mul;
            max_list_mul /=max_poss_list[t];
            if (max_list_mul == 0){
              max_list_mul=1;
            }
            prob.push_back(cur_p);
          }
          // for (int y = 0; y< prob.size();y++){
          //     cout<< prob[y]<<" ";
          // }
          // cout<<endl;
          // cout<<"Size of prob: "<<prob.size()<<endl;
          float count=0;
          for (int j=0;j<n_r;j++){
            bool okRow=true;
            for (int l = 0; l<index_list.size();l++){
              int db_col = index_list[l];
              if (db[j][db_col] == prob[l]){
                continue;
              }else{
                okRow=false;
                break;
              }
            }
            if (okRow){
              // if (index_list.size() >2){
              //   cout<<"Row: ";
              //   for (int l = 0; l<index_list.size();l++){
              //     cout<<db[j][index_list[l]]<<" ";
              //   }
              //   cout<<endl;
              // }
              count++;
            }
          }
          float ans = ((float) count/n_r);
          // cout<<count<<" "<<" "<<n_r<<" "<<ans<<" "<<endl;
          final_table.push_back(ans);
        }else{
          final_table.push_back(curr_table[i]);
        }
      }
      (*curr_node).set_CPT(final_table);
      // cout<<"Iteration finished."<<endl;

      ind++;
    }

}


float get_score(network n,network acn){
  float score = 0;
  for (int i =0; i< 37;i++){

    Graph_Node g = *(n.get_nth_node(i));
    Graph_Node acg = *(acn.get_nth_node(i));
    vector<float> g_cpt = g.get_CPT();
    vector<float> acg_cpt = acg.get_CPT();
    for (int k =0; k<g_cpt.size();k++){
      score+= abs(g_cpt[k] - acg_cpt[k]);
    }
  }

  return score;
}
//n has probability initialised

void pipeline(network *n, DATABASE d,network acn){
  float i_score = get_score(*n,acn);
  // cout<<d[0][11]<<endl;

  DATABASE n_d = modify_database(d,*n);
  // cout<<n_d[0][11]<<endl;

  m_step(n,n_d);
  float f_score = get_score(*n,acn);
  cout<<"f: "<<f_score<<" i: "<<i_score<<endl;
  while (f_score<=i_score || f_score-i_score<40){// && (f_score - i_score)>= 0){ //Change this
    i_score = f_score;
    n_d = modify_database(n_d,*n);
    // cout<<n_d[0][11]<<endl;
    m_step(n,n_d);
    f_score = get_score(*n,acn);
    cout<<"f: "<<f_score<<" i: "<<i_score<<endl;
  }
  cout<<"f: "<<f_score<<" i: "<<i_score<<endl;
}


int main(int argc, char const *argv[]) {
    network Alarm;
    string infile="alarm.bif";
    string recfile = "records.dat";
    (Alarm) = read_network(infile);
    DATABASE d = dat_reader(recfile,Alarm);
    initialize_probability(&Alarm,d);
    network g_alarm = read_network("gold_alarm.bif");
    DATABASE n_d = modify_database(d,Alarm);
    // for (int i= 0; i<11;i++){
    //   cout<<n_d[0][i]<<" ";
    // }
    // cout<<endl;
    // m_step(&Alarm,n_d);
    pipeline(&Alarm,d,g_alarm);
    // DATABASE new_db = modify_database(d,Alarm);

    // for (int j = 0;j<37;j++){
    //   cout<<"------"<<j+1<<"-----"<<endl;
    //   list<Graph_Node>::iterator g = (Alarm.get_nth_node(j));
    //   cout<<"Num of Parents: "<<(*g).get_Parents().size()<<endl;
    //   for (int i = 0; i< (*g).get_CPT().size();i++){
    //     cout<<((*g).get_CPT())[i]<<endl;
    //   }
    //   // cout<<"------"<<endl;
    // }

  return 0;
}
