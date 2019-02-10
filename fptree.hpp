#include <vector>
#include <map>
using namespace std;

struct Fpnode
{
	int itemid,p;
	std::vector<Fpnode*> children;

	Fpnode * parent;
	Fpnode * auxiliary;
	Fpnode * next;

};
struct Fptree
{
	std::map<int,pair<Fpnode *,Fpnode*>> table;//map of item to its head and tail
	Fpnode * root;
};


void print_tree(Fpnode * node)
{
	if(node->itemid != 0)
		cout << node->itemid << " " << node->parent->itemid << endl;
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
				cur->p++;
				already_present = 1;
				break;
			}
		}
		if(!already_present)
		{
			Fpnode * node = new Fpnode();
			node->parent = cur;
			node->itemid = transaction[i];
			cur->children.push_back(node);
			node->p = 1;
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
