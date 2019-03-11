#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

void build(vector <long long> &a, vector <long long> &tree, long long v, long long tl, long long tr)
{
	if (tl == tr)
	{
		tree[v] = a[tl];
	}
	else
	{
		long long tm = (tl + tr) / 2;
		build(a, tree, v * 2, tl, tm);
		build(a, tree, v * 2 + 1, tm + 1, tr);
		tree[v] = max(tree[v * 2], tree[v * 2 + 1]);
	}
}

long long get_max(vector <long long> &tree, long long v, long long tl, long long tr, long long l, long long r)
{
	if (tr < l || r < tl)
	{
		return 0;
	}

	if (l <= tl && tr <= r)
	{
		return tree[v];
	}

	long long tm = (tl + tr) / 2;
	return max(get_max(tree, v * 2, tl, tm, l, r), get_max(tree, v * 2 + 1, tm + 1, tr, l, r));
}

int main()
{
	long long n, m, x, y;
	cin >> n;
	vector <long long> a(n);
	vector <long long> tree(4 * n);

	for (long long i = 0; i < n; i++)
	{
		cin >> x;
		a[i] = x;
	}
	
	build(a, tree, 1, 0, n - 1);
	cin >> m;
	vector <long long> res;

	for (long long i = 0; i < m; i++)
	{
		cin >> x >> y;
		res.push_back(get_max(tree, 1, 0, n - 1, x - 1, y - 1));
	}

	for (size_t i = 0; i < res.size() ; i++)
	{
		cout << res[i] << " ";
	}

	cout << endl;
	return 0;
}

