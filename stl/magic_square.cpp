#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

#define n 4

using namespace std;

bool check_lines(const vector <vector <int>> &magic, int sum)
{
	bool b = false;
	for (int i = 0; i < n; i++)
	{
		int s = 0;
		for (int  j = 0; j < n; j++)
		{
			if (magic[j][i] == 0)
			{
				b = true;
			}
			s += magic[j][i];
		}
		if (s != sum / 4 && !b)
		{
			return false;
		}
		b = false;
	}
	return true;;
}

bool check_columns(const vector <vector <int>> &magic, int sum)
{
	bool b = false;
	for (int i = 0; i < n; i++)
	{
		int s = 0;
		for (int  j = 0; j < n; j++)
		{
			if (magic[i][j] == 0)
			{
				b = true;
			}
			s += magic[i][j];
		}
		if (s != sum / 4 && !b)
		{
			return false;
		}
		b = false;
	}
	return true;
}

void print(const vector <vector <int>> &magic)
{
	for (int i = 0; i < n; i++)
	{
		for (int  j = 0; j < n; j++)
		{
			cout << magic[i][j] << " ";
		}
		cout << endl;
	}
}

void find(const int i, const int j, int sum, vector <vector <int>> &magic, list <int> l, const bool b, bool &finish)
{
	if (l.size() == 0)
	{
		print(magic);
		finish = true;
		return;
	}

	if (j == n)
	{
		if (check_columns(magic, sum))
		{
			find(i + 1, i + 1, sum, magic, l, !b, finish);
		}
		return;
	}

	if (i == n)
	{
		if (check_lines(magic, sum))
		{
			find(j, j + 1, sum, magic, l, !b, finish);
		}
		return;
	}
	
	for (auto it = l.begin(); it != l.end();)
	{
		int val = *it;
		magic[i][j] = *it;
		auto it1 = l.erase(it);
		if (b)
		{
			find(i + 1, j, sum, magic, l, b, finish);
		}
		else
		{
			find(i, j + 1, sum, magic, l, b, finish);
		}
		magic[i][j] = 0;
		l.insert(it1, val);
		it = it1;
		if (finish)
		{
			return;
		}
	}
}

int main()
{
	int a, sum = 0;
	list <int> l;
	vector <vector <int>> magic(n);

	for (int i = 0; i < n; i++)
	{
		magic[i].assign(n, 0);
	}

	for (int i = 0; i < n * n; i++)
	{
		cin >> a;
		l.push_back(a);
		sum += a;
	}
	
	if (sum % n != 0)
	{
		cout << "NO SOLUTION" << endl;
		return 0;
	}
	
	bool finish = false;
	find(0, 0, sum, magic, l, true, finish);
	if (!finish)
	{
		cout << "NO SOLUTION" << endl;
	}
	return 0;
}
