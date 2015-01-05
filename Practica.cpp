#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <limits>
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
		if ((i != j) and (v[i].d == v[j].o) and ((v[i].td + 15) >= v[j].to))
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
		int d = v[s.top()].d;
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

void BFS(const vector< vector<int> >& mat, int s, int t, vector<int>& p, vector< vector<int> >& res)
{
	vector<int> f = vector<int>(mat.size()); //capacitat del cami. Necesaria per calcular el flux que queda en l'aresta i quant torna.

    p[s] = s; //indicamos que es el inicio.
    f[s] = numeric_limits<int>::max();

    queue<int> q;
    q.push(s);

    while (not q.empty()) {
    	int u = q.front();
    	for(int j = 0; j < mat[u].size(); ++j) {
    		int v = mat[u][j];
    		if (mat[u][v] > 0 and p[v] == -1) {
    		//Capacitat-Fluxe es > 0 i no s'ha visitat encara.
    			p[v] = u;
    			if (f[u] <= mat[u][v]) f[v] = f[u];
    			else f[v] = mat[u][v];
    			//agafem el minim.

    			if(v != t) q.push(v);
    			//sino hem arribat al final seguim buscant el cami.
    			else {
    			//recorrem el cami al reves i anem calculant el nou graf residu:
    				while(p[v] != v) {
    				//l'unic que el seu pare es ell mateix es s.
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

int EdmonsKarp(vector< vector<int> >& mat, int s, int t)
{
	vector< vector<int> > res = vector< vector<int> >(mat.size()); //graf residual
	vector<int> p = vector<int>(mat.size(), -1); //taula dels "pares", contindra el cami.

	BFS(mat,s,t,p,res);
	
    while (p[t] != -1) {
    	BFS(mat,s,t,p,res);
    }
    
    //no existeix cap cami desde s fins a t, no es pot augmentar mes:
	int suma = 0;
	for (int j = 0; j < res[s].size(); ++j) //calculem el fluxe que surt de s, que sera el que arribara a t, per tant el fluxe maxim.
	{
		suma = suma + res[s][j];
	}
	return suma;
	
}

int otroAlgoritmo(vector< vector<int> >& mat)
{

}

vector< list<int> > resolver(const vector< vector<int> >& mat, bool x)
{
	bool b = false;
	int k, s, t;
	s = mat[mat.size() -2][0];
	t = mat[mat.size() -1][0];

	for (k = 1; k <= (mat.size() - 4)/2 and !b; ++k)
	{
		int cont;
		mat[mat.size() - 2][mat.size() - 3] = mat[mat.size() - 3][mat.size() - 1] = k;
		vector< vector<int> > aux = mat;
		if (x) cont = EdmonsKarp(aux,s,t);
		else cont = otroAlgoritmo(aux);
		b = ((mat.size() - 4)/2 + k) == cont;
	}
	vector< list<int> > v(k);
	//sacar la lista de viajes q hace cada piloto
	return v;
}

void escriure(vector< list<int> > sol) {
	cout << sol.size() << endl;
	//esto es el numero de pilotos que seria la k que resuelve el problema.

	for(int i = 0; i < sol.size(); ++i) {
		for(list<int>::iterator j = sol[i].begin(); j != sol[i].end(); ++j) {
			cout << *j << "";
			//TODO: No se si funciona
		}
		cout << endl;
	}
}

int main()
{
	vector<Vuelo> v = leer();
	vector< vector<int> > mat = grafo(v, true);
	vector< list<int> > sol = resolver(mat, true);
	escriure(sol);
}