#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include<algorithm>

using namespace std;

class Variable{
	private:
		string name;
		short int type;

	public:
		Variable(string str, short int t1){
			name = str;
			type = t1;
		}
		string get_name(){
			return name;
		}
		short int get_type(){
			return type;
		}
};

class Table{
	private:
		vector<vector<float> > table;
	public:
		public Table(int type, int size, vector<float> str){
			table.resize(type,vector<float>(size,-1));
			for(int i=0;i<type;i++){
				for(int j=0;j<size;j++){
					table[i][j]= str[size*i+j];
				}
			}
		}

		vector<vector<float> > get_table(){
			return table;
		}
};

unordered_map<string,variable> parent;



