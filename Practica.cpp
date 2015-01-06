#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <limits>
#include <stdio.h>
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
	vector<Vuelo> vuelos;
	Vuelo v;
	while (cin >> v.o >> v.d >> v.to >> v.td)
		vuelos.push_back(v);
	return vuelos;
}

stack<int> compatible1(int i, const vector<Vuelo>& v)
{
	stack<int> s;
	for (int j = 0; j < v.size(); ++j)
	{
		if ((i != j) and (v[i].d == v[j].o) and ((v[i].td + 15) <= v[j].to))
			s.push(2*j);
	}
	return s;
}

stack<int> compatible2(int i, const vector<Vuelo>& v)
{
	stack<int> r, s;
	s.push(i);
	while (!s.empty())
	{
		i = s.top();
		s.pop();
		for (int j = 0; j < v.size(); ++j)
		{
			if ((i != j) and (v[i].d == v[j].o) and ((v[i].td + 15) <= v[j].to))
			{
				s.push(j);
				r.push(2*j);
			}
		}
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
			m[i][n + 1] = m[i][n + 1] + mat[i][j].first;
			m[n][j] = m[n][j] + mat[i][j].first;
		}
	}
	m[n][n - 2] = m[n - 1][n + 1] = -1;
	return m;
}

vector< list<int> > calculaViajes(const vector< vector<int> > &res, int a, int b, int k)
{
	vector< list<int> > vl(k);
	stack<int> s;
	s.push(a);
	int aux = 0;
	while (!s.empty())
	{
		a = s.top();
		s.pop();
		for (int i = 0; i < res[a].size(); ++i)
		{
			if (res[a][i] > 0)
			{
				if (i != b)
				{
					s.push(i + 1)
					vl[aux].push_back(i/2);
				}
				else
				{
					++aux;
				}
			}
		}
	}
}

void BFS(const vector< vector<int> >& mat, int s, int t, vector<int>& p, vector< vector<int> >& res)
{
	vector<int> f(mat.size()); //capacitat del cami. Necesaria per calcular el flux que queda en l'aresta i quant torna.

    p = vector<int>(mat.size(), -1);
    p[s] = s; //indicamos que es el inicio.
    f[s] = numeric_limits<int>::max();

    queue<int> q;
    q.push(s);

    while (not q.empty()) {
        
    	int u = q.front();
        q.pop();

    	for(int v = 0; v < mat[u].size(); ++v) {

    		if (mat[u][v]-res[u][v] > 0 and p[v] == -1) { //Capacitat-Fluxe es > 0 i no s'ha visitat encara.
    			p[v] = u;
    			if (f[u] <= mat[u][v]-res[u][v]) f[v] = f[u];
    			else f[v] = mat[u][v]-res[u][v];
    			//agafem el minim.

    			if(v != t) q.push(v); //sino hem arribat al final seguim buscant el cami.

    			else { //recorrem el cami al reves i anem calculant el nou graf residu:
    				while(p[v] != v) { //l'unic que el seu pare es ell mateix es s.
    					u = p[v];
    					res[u][v] += f[t];
    					res[v][u] -= f[t];
    					v = u;
    				}
    				return; //TODO: si ya hemos encontrado un camino que llega a t salimos de la funcion.
    			}
    		}
    	}
    }
}

int EdmonsKarp(vector< vector<int> >& mat, int s, int t, vector< vector<int> >& res)
{
	res = vector< vector<int> >(mat.size(),vector<int>(mat.size(),0)); //graf residual
	vector<int> p; //taula dels "pares", contindra el cami.

    do {
    	BFS(mat,s,t,p,res);
    } while (p[t] != -1);
    
    //no existeix cap cami desde s fins a t, no es pot augmentar mes:
	int suma = 0;
	for (int j = 0; j < res[s].size(); ++j) //calculem el fluxe que surt de s, que sera el que arribara a t, per tant el fluxe maxim.
		suma = suma + res[s][j];

	return suma;	
}

vector< list<int> > resolver(vector< vector<int> >& mat)
{
	bool b = false;
	int k, s, t;
	s = mat.size() -2;
	t = mat.size() -1;
	vector< vector<int> > res; //esto contendra el grafo residual para calcular que vuelo hace cada piloto una vez hallada la k optima.

	for (k = 1; k <= (mat.size() - 4)/2 and !b; ++k)
	{
		int cont;	
		mat[mat.size() - 2][mat.size() - 4] = mat[mat.size() - 3][mat.size() - 1] = k;
		b = ((mat.size() - 4)/2 + k) == EdmonsKarp(mat,s,t,res);
	}
	//EdmonsKarp(mat,s,t,res);//recalculamos el optimo.

	return calculaViajes(res, mat.size()-4, mat.size()-3, k - 1);
	//sacar la lista de viajes q hace cada piloto
}

void escriure(vector< list<int> > sol, string s) {
	FILE * pFile;
	pFile = fopen ("Resultado" + s + ".txt","w");
	if (pFile != NULL)
	{
		fputs (sol.size(),pFile);
		for (int i = 0; i < sol.size(); ++i)
		{
			for (list<int>::iterator it = sol[i].begin(); it != sol[i].end(); ++it)
				fputs(*it, pFile);
			fputs("\n", pFile)
		}
		fclose (pFile);
	}
}

int main()
{
	vector<Vuelo> v = leer();
	vector< vector<int> > mat = grafo(v, true);
	vector< list<int> > sol = resolver(mat);
	escriure(sol, "1");
	vector< vector<int> > mat = grafo(v, false);
	vector< list<int> > sol = resolver(mat);
	escriure(sol, "2");
}