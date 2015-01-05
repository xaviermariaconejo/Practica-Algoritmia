#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;


void BFS(const vector< vector<int> >& mat, int s, int t, vector<int>& p, vector< vector<int> >& res)
{
	vector<int> f = vector<int>(mat.size()); //capacitat del cami. Necesaria per calcular el flux que queda en l'aresta i quant torna.

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

int EdmonsKarp(vector< vector<int> >& mat, int s, int t)
{
	vector< vector<int> > res (mat.size(),vector<int>(mat.size(),0)); //graf residual
	vector<int> p; //taula dels "pares", contindra el cami.

    do {
    	BFS(mat,s,t,p,res);
    } while (p[t] != -1);
    
    //no existeix cap cami desde s fins a t, no es pot augmentar mes:
	int suma = 0;
	for (int j = 0; j < res[s].size(); ++j) //calculem el fluxe que surt de s, que sera el que arribara a t, per tant el fluxe maxim.
	{
		suma = suma + res[s][j];
	}
	return suma;
	
}

int main() {
    vector< vector<int> > graf = vector< vector<int> >(6,vector<int>(6,0));
    int u,v,c;

    for(int i = 0; i < 8; ++i) {
        cin >> u >> v >> c;
        graf[u][v] = c;
    }


    cout << "Max Flow: " << EdmonsKarp(graf, 0, 5) << endl;
}