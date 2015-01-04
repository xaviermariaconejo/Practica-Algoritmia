#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Vuelo
{
	int o;
	int d;
	int to;
	int td;
};

vector<Vuelo> leer()
{
	vector<Vuelo> v;
	Vuelo v;
	while (cin >> v.o >> v.d >> v.to >> v.td)
		v.push_back(v);
	return v
}

void concatena(queue<int>& a, queue<int>& b)
{
	while (!b.empty())
	{
		a.push(b.front());
		b.pop();
	}
}

queue<int> compatible1(int i, const vector<Vuelo>& v)
{

}

queue<int> compatible1(int i, const vector<Vuelo>& v)
{
	
}

vector< vector<int> > grafo(const vector<Vuelo>& v, bool x)
{
	int n = 2*v.size();
	vector< vector< pair<int, int> > > mat(n + 2, vector< pair<int, int> > (n + 2, pair<int, int>(0, 0)));
	for (int i = 0; i < v.size(); ++i)
	{
		mat[i][i + 1] = pair<int, int>(1, 1);
		mat[n][i] = pair<int, int>(0, 1);
		mat[i + 1][n + 1] = pair<int, int>(0, 1);
		queue<int> q = compatible1(i, v);
		if (x)
			concatena(q, compatible2(i, v));
		while (!q.empty())
		{
			mat[i + 1][q.front()] = pair<int, int>(0, 1);
			q.pop();
		}
	}

	n = mat.size();
	vector< vector<int> > m(n + 2, vector<int> (n + 2, 0));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			m[i][j] = mat[i][j].second - mat[i][j].first;
			m[i][n + 1] = m[n][i] + mat[i][j].first;
			m[n][j] = m[n][j] + mat[i][j].first;
		}
	}
	m[n][n - 2] = m[n - 1][n + 1] = -1;
	return m;
}

int main()
{
	vector<Vuelo> v = leer();
	vector< vector<int> > mat = grafo(v, false);
	vector< list<int> > sol = EdmonsKarp(mat);
	escriure(sol);
}