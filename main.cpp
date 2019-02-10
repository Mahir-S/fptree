#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <assert.h>
#include <fptree.hpp>
using namespace std;

int main(int argc,char *argv[])
{
	cout << argc << endl;

	if(argc < 2)
	{
		cout << "Please specify dataset path as a command line paramter.\n";
		return 0;
	}
	freopen(argv[1],"r",stdin);
	freopen("out.txt","w",stdout);

	map<string,int> item_mapping;
	map<int,string> reverse_map;

	string line;
	vector<string> v;
	while(getline(cin,line))
	{
		string temp = "";
		v.push_back(line);
		int sz = line.size();
		for(int i = 0;i < sz - 1;i++)
		{
			
			if(line[i] == ',')
			{

				item_mapping[temp]++;

				temp = "";
			}
			else
				temp += line[i];
		}

		if(temp != "")
			item_mapping[temp]++;
	}
	vector<pair<int,int>> frequencies;
	int itemid = 0;
	for(auto it : item_mapping)
	{
		//it.second is the frequency
		frequencies.push_back({it.second,itemid});
		reverse_map[itemid] = it.first;
		itemid++;
	} 
	
	sort(frequencies.rbegin(),frequencies.rend());
	for(auto it : frequencies)
	{
		cout << reverse_map[it.second] << " " << it.first << endl;
	}

	for(auto s : v)
	{
		string temp = "";
		for(int j = 0;j < s.size() - 1;j++)
		{
			if(s[j] == ',')
			{

				insert_string(temp);
				temp = "";
			}
			else
				temp += s[j];
		}
		if(temp != "")
			insert_string(temp);
	}
	return 0;
}