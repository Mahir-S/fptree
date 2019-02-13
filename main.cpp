#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <assert.h>
#include <cstring>
#include "fptree.hpp"
using namespace std;
#define MAXN 500

int freq[MAXN];
int percent = 60;
bool comp(int x,int y)
{
	if(freq[x] != freq[y])
		return freq[x] > freq[y];
	else
		return x < y;
}
int main(int argc,char *argv[])
{
	memset(freq,0,sizeof(freq));

	if(argc < 2)
	{
		cout << "Please specify dataset path as a command line paramter.\n";
		return 0;
	}



	freopen(argv[1],"r",stdin);
	//freopen("out.txt","w",stdout);

	map<string,int> item_mapping;
	map<int,string> reverse_map;

	string line;
	vector<vector<string>> transactions;
	int number_of_transactions = 0;
	while(getline(cin,line))
	{
		string temp = "";
		vector<string> v;
		int sz = line.size();
		if((int)line[sz-1] == 13)
			sz--;
		for(int i = 0;i < sz ;i++)
		{
			
			if(line[i] == ',')
			{

				item_mapping[temp]++;
				v.push_back(temp);
				temp = "";
			}
			else
				temp += line[i];
		}

		if(temp != "")
		{
			v.push_back(temp);
			item_mapping[temp]++;
		}
		transactions.push_back(v);
		number_of_transactions++;
	}
	set_minsupport(3);

//	set_minsupport((percent*number_of_transactions)/100);
	
	vector<pair<int,int>> frequencies;
	int itemid = 1;

	for(auto &it : item_mapping)
	{
		//it.second is the frequency

		freq[itemid] = it.second;
		
		reverse_map[itemid] = it.first;
		it.second = itemid;
		itemid++;
	} 
	
	sort(frequencies.rbegin(),frequencies.rend());

	Fptree * tree = new Fptree();
	tree->root = new Fpnode();
	tree->root->parent = NULL;
	tree->root->itemid = 0;
	for(auto t : transactions)
	{
		vector<int> local;
		for(auto s : t)
		{
			local.push_back(item_mapping[s]);
		}

		//sort(local.begin(),local.end(),comp);

		insert_transaction(tree,local);
	}

	//print_tree(tree->root);
	//print auxiliary pointers
	vector<vector<int>> patterns = mine_frequent_itemsets(tree);
	for(auto pattern : patterns)
	{
		for(auto item : pattern)
			cout << item << " " ;
		cout << endl;
	}
	cout << "end";

	return 0;
}