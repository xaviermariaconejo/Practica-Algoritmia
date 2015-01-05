#include <iostream>
#include <vector>
#include <stack>
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

stack<int> compatible1(int i, const vector<Vuelo>& v)
{
	stack<int> s;
	for (int j = 0; j < v.size(); ++j)
	{
		if ((i != j) and (v[i].d == v[j].o) and ((v[i].td + 15) >= v[j].to))
			q.push(2*j);
	}
	return s;
}

queue<int> compatible2(int i, const vector<Vuelo>& v)
{
	stack<int> r, s;
	s.push(i);
	while (!s.empty())
	{
		int d = s.top().d;
		for (int j = 0; j < v.size(); ++j)
		{
			if ((i != j) and (v[i].d == v[j].o) and (v[i].td >= v[j].to))
			{
				s.push(j);
				if ((v[i].td + 15) >= v[j].to)
					r.push(2*j);
			}
		}
		s.pop();
	}
	return r;
}

vector< vector<int> > grafo(const vector<Vuelo>& v, bool x)
{
	int n = 2*v.size();
	vector< vector< pair<int, int> > > mat(n + 2, vector< pair<int, int> > (n + 2, pair<int, int>(0, 0)));
	//mat es el grafo que representa el conjunto de vuelos: first -> lower bound; second -> capacity
	for (int i = 0; i < v.size(); ++i)
	{
		mat[2*i][2*i + 1] = pair<int, int>(1, 1);
		mat[n][2*i] = pair<int, int>(0, 1);
		mat[2*i + 1][n + 1] = pair<int, int>(0, 1);
		stack<int> s;
		if (x) s = compatible1(i, v);
		else s = compatible2(i, v);
		while (!s.empty())
		{
			mat[2*i + 1][s.top()] = pair<int, int>(0, 1);
			s.pop();
		}
	}

	n = mat.size();
	vector< vector<int> > m(n + 2, vector<int> (n + 2, 0));
	//m es el grafo que ya se han quitado los lower bound y a la 
	//vez se ha hecho la transformacion a circulacion de demandas
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

void BFS(const vector< vector<int> >& mat, int i)
{
	vector<bool> visitats(mat.size(), false);
	queue<int> q;
	q.push(i);
	visitats[i] = true;
	while (!q.empty())
	{
		int x = q.front();
		for (int j = 0; j < mat.size(); ++j)
		{
			if (!visitats[j] and mat[i][j] > 0)
			{
				q.push(j);
				visitats[j] = true;
			}
		}
		q.pop();
	}
}

void EdmonsKarp(vector< vector<int> >& mat)
{

}

vector< list<int> > resolver(const vector< vector<int> >& mat)
{
	for (int k = 1; k <= (mat.size() - 4)/2; ++k)
	{
		mat[n][n - 1] = m[n - 1][n + 1] = k;
		EdmonsKarp(mat);		
	}
}

int main()
{
	vector<Vuelo> v = leer();
	vector< vector<int> > mat = grafo(v, true);
	vector< list<int> > sol = resolver(mat);
	escriure(sol);
}