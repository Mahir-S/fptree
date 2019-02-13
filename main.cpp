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
int percent = 33;
bool comp(int x,int y)
{
	if(freq[x] != freq[y])
		return freq[x] > freq[y];
	else
		return x < y;
}
vector<string> tokenize(string &line,int sz)
{
	vector<string> v;
	string temp = "";
	for(int i = 0;i < sz ;i++)
	{
		
		if(line[i] == ',')
		{

			v.push_back(temp);
			temp = "";
		}
		else
			temp += line[i];
	}

	if(temp != "")
	{
		v.push_back(temp);
	}
	return v;
}
int main(int argc,char *argv[])
{
	memset(freq,0,sizeof(freq));

	if(argc < 2)
	{
		cout << "Please specify dataset path as a command line paramter.\n";
		return 0;
	}
	if(argc == 3)
	{
		int z = atoi(argv[2]);
		percent = z;

	}


	freopen(argv[1],"r",stdin);

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

		vector<string> itemset =  tokenize(line,sz) ;
		for(auto item : itemset)
		{
			
			item_mapping[item]++;
		}
		
		transactions.push_back(itemset);	
		number_of_transactions++;
	}
	int minsupport = (percent*number_of_transactions)/100; 
	set_minsupport(minsupport);
	
	cout << minsupport << endl;
	
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
			if(freq[item_mapping[s]] >= minsupport)
			local.push_back(item_mapping[s]);
		}
		//IMPORTANT
		//sort(local.begin(),local.end(),comp);
		
		if(local.size() > 0)
			insert_transaction(tree,local);
	}

	
	vector<vector<int>> patterns = mine_frequent_itemsets(tree);

	freopen("frequent_itemsets.txt","w",stdout);
	for(auto pattern : patterns)
	{
		for(auto item : pattern)
			cout << reverse_map[ item ]<< " " ;
		cout << endl;
	}


	return 0;
}