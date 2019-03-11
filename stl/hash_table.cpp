#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <iterator>
#include <algorithm>

const int n = 100000, p = 32;

using namespace std;

class hash_table
{
	vector <list <string>> table;
	int hash(string str)
	{
		size_t s = 0, t = 1;
		for (size_t i = 0; i < str.size(); i++)
		{
			s = s + str[i] * t;
			t = t * p;
		}
		return s % n;
	}
public:
	hash_table(): table(n) {}
	bool check(string s)
	{
		auto tmp = table[hash(s)];
		if (find(tmp.begin(), tmp.end(), s) != tmp.end())
		{
			return true;
		}
		return false;
	}
	void push(string s)
	{
		int cur_hash = hash(s);
		table[cur_hash].push_back(s);
		table[cur_hash].unique();
	}
	void del(string s)
	{
		int cur_hash = hash(s);
		auto tmp = table[cur_hash];
		auto it = find(tmp.begin(), tmp.end(), s);
		if (it == tmp.end())
		{
			return;
		}
		auto l = table[cur_hash];
		table[cur_hash].clear();
		for (auto i = l.begin(); i != l.end(); i++)
		{
			if (*i != s)
			{
				table[cur_hash].push_back(*i);
			}
		}
	}
};

int main()
{
	string s1, s2;
	hash_table set;
	vector <pair <string, string>> s;

	for (;;)
	{
		getline(cin, s1);
		if (s1 == "#")
		{
			break;
		}
		if (s1.size() < 3)
		{
			s.push_back(make_pair(s1, ""));
		}
		else
		{
			s2 = s1;
			s1.erase();
			s1.assign(s2, 0, 1);
			s2.erase(0, 2);
			s.push_back(make_pair(s1, s2));
		}
	}
	
	for (size_t  i = 0; i < s.size(); i++)
	{
		int c = s[i].first[0];
		switch (c)
		{
			case '+':
				set.push(s[i].second);
				break;
			case '-':
				set.del(s[i].second);
				break;
			case '?':
				if (set.check(s[i].second))
				{
					cout << "YES" << endl;
				}
				else
				{
					cout << "NO" << endl;
				}
				break;
			default:
				break;
		}
	}

	return 0;
}
