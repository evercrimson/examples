#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

bool dfs(int v, vector <bool> &visited, const vector <vector <int>> &g, vector <int> &mt)
{
	if (visited[v])
	{
		return false;
	}
	visited[v] = true;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (mt[to] == -1 || dfs(mt[to], visited, g, mt))
		{
			mt[to] = v;
			return true;
		}
	}
	return false;
}

bool operator <(const set <string> &st1, const set <string> &st2)
{
	return includes(st2.begin(), st2.end(), st1.begin(), st1.end());
}

int main()
{
	int n;
	cin >> n;
	vector <set <string>> juice(n);

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;
		for (int j = 0; j < a; j++)
		{
			string s;
			cin >> s;
			juice[i].insert(s);
		}
	}

	vector <vector <int>> g(n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i != j && juice[i] < juice[j])
			{
				g[i].push_back(j);
			}
		}
	}
	
	vector <bool> visited;
	vector <int> mt(n, -1);
	int count = 0;

	for (int i = 0; i < n; i++)
	{
		visited.assign(n, false);
		dfs(i, visited, g, mt);
	}

	for (int i = 0; i < n; i++)
	{
		if (mt[i] != -1)
		{
			count++;
		}
	}

	cout << n - count << endl;

	return 0;
}
