#include <iostream>

using namespace std;

struct node
{
	int a;
	node *l, *r;
};

void del(node *&tree)
{
	if (tree != NULL)
	{
		del(tree->l);
		del(tree->r);
		delete(tree);
		tree = NULL;
	}
}

int height(node *tree)
{
	int h1 = 0, h2 = 0;
	if (tree == NULL)
	{
		return 0;
	}
	if (tree->l)
	{
		h1 = height(tree->l);
	}
	if (tree->r)
	{
		h2 = height(tree->r);
	}
	return (max(h1,h2) + 1);
}

void add_node(int a, node *&tree)
{
	if (tree == NULL)
	{
		tree = new node;
		tree->a = a;
		tree->l = tree->r = NULL;
		return;
	}
	if (tree->a == a)
	{
		return;
	}
	if (a < tree->a)
	{
		add_node(a, tree->l);
	}
	else
	{
		add_node(a, tree->r);
	}
}	

int main()
{
	int a;
	node *tree = NULL;
	for(;;)
	{
		cin >> a;
		if (a == 0)
		{
			break;
		}
		else
		{
			add_node(a, tree);
		}
	}
	cout << height(tree) << endl;
	del(tree);
	return 0;
}
