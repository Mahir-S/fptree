#include <vector>
#include <map>
#include <iostream>
using namespace std;

int MINSUP;

struct Fpnode
{
	int itemid,count;
	std::vector<Fpnode*> children;
	Fpnode * parent;
	Fpnode * auxiliary;
	Fpnode * next;

};
struct Fptree
{
	std::map<int,pair<Fpnode *,Fpnode*>> table;//map of item to its head and tail
	Fpnode * root;
	//map<int,int> itemcount //map item to its count

};


void print_tree(Fpnode * node)//helper function for debugging
{
	if(node->itemid != 0)
		cout << node->itemid << " " << node->parent->itemid << " "  << node->auxiliary << endl;
	for(int j = 0;j < node->children.size();j++)
		print_tree(node->children[j]);
}

void insert_transaction(Fptree * tree,vector<int> &transaction)
{
	Fpnode * cur = tree->root;
	for(int i = 0;i < transaction.size();i++)
	{
		bool already_present = 0;
		for(int j = 0;j < cur->children.size();j++)
		{
			if(cur->children[j]->itemid == transaction[i])
			{
				cur = cur->children[j];
				cur->count++;
				already_present = 1;
				break;
			}
		}
		if(!already_present)
		{
			Fpnode * node = new Fpnode();
			node->parent = cur;
			node->itemid = transaction[i];
			node->auxiliary = NULL;
			node->next = NULL;
			cur->children.push_back(node);
			node->count = 1;
			//updating table and next pointers
			if(tree->table[transaction[i]].first == NULL)
			{
				tree->table[transaction[i]] = {node,node};
			}
			else
			{
				tree->table[transaction[i]].second->next = node;
				tree->table[transaction[i]].second = node;
			}
			cur = node;
		}
	}
}

vector<vector<int>> mine_frequent_itemsets(Fptree * tree);

void traverse_upwards(Fptree * tree,Fpnode * start)
{
	Fpnode * cur = start;
	int prevcount = cur->count;//count of every node will be increased by this

	cur = cur -> parent;
	Fpnode * prevnode = NULL;
	bool on_existing_path = 0;

	while(cur!=NULL)
	{
		if(cur->auxiliary == NULL)
		{
			Fpnode * node;
			if(cur->parent == NULL)
			{
				node = tree->root;
				if(prevnode != NULL)
				{
					prevnode->parent = node;
					node->children.push_back(prevnode);
				}
			}
			else
			{

				node = new Fpnode();
				node->auxiliary = NULL;
				node->next = NULL;
				
				node->itemid = cur->itemid;
				node->count = prevcount;
				//setting parent and children
				if(prevnode != NULL)
				{
					prevnode->parent = node;
					node->children.push_back(prevnode);
				}
				//updating table of new tree and next pointers
				if(tree->table[cur->itemid].first == NULL)
				{
					tree->table[cur->itemid] = {node,node};
				}
				else
				{
					tree->table[cur->itemid].second->next = node;
					tree->table[cur->itemid].second = node;
				}	
			}
			
			//setting auxiliary pointers
			cur->auxiliary = node;

			prevnode = node;
		}
		else
		{
			if(on_existing_path == 0 and prevnode!=NULL)
			{

				cur->auxiliary->children.push_back(prevnode);
				prevnode->parent = cur->auxiliary;

			}
			on_existing_path = 1;
			cur->auxiliary->count += prevcount;
		}

		cur = cur->parent;
	}
}

void clear_auxiliary_pointers(Fpnode * node)
{
	Fpnode * cur = node;
	cur = cur->parent;
	while(cur!=NULL)
	{
		if(cur->auxiliary == NULL)
			break;
		cur->auxiliary = NULL;
		cur = cur->parent;
	}
	return;
}

vector<vector<int>> build_conditional_fptree(Fptree * tree,int item)
{
	Fptree  * newtree = new Fptree();
	newtree->root = new Fpnode();
	newtree->root->itemid = 0;
	newtree->root->auxiliary = NULL;
	newtree->root->parent = NULL;


	Fpnode* cur = tree->table[item].first;//head
	int cnt = 0;
	//count the frequency of item 
	while(cur != NULL)
	{
		cnt += cur->count;
		cur = cur->next;
	}

	if(cnt < MINSUP)
	{
		return vector<vector<int>>();//return empty vector of vector;
	}
	//actual construction
	
	cur = tree->table[item].first;
	while(cur != NULL)
	{
		traverse_upwards(newtree,cur);
		cur = cur->next;
	}

	//clearing the auxiliary pointers
	cur = tree->table[item].first;
	while(cur != NULL)
	{
		clear_auxiliary_pointers(cur);
		cur = cur->next;
	}

	vector<vector<int>> pattern = mine_frequent_itemsets(newtree);
	
	for(auto &v : pattern)
		v.push_back(item);
	pattern.push_back(vector<int>(1,item));
	
	return pattern;
}




vector<vector<int>> mine_frequent_itemsets(Fptree * tree)
{
	vector<vector<int>> pattern;
	for(auto it : tree->table)
	{
		int item = it.first;//key
		vector<vector<int>> patterns = build_conditional_fptree(tree,item);
		for(auto &v:patterns)
			pattern.push_back(v);
	}
	return pattern;
}

void set_minsupport(int num)
{
	MINSUP = num;
	return;
}