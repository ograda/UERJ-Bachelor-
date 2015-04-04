/*
Otimização em grafos
Trabalho extra 1 (3ª versão busca em profundidade) - data de entrega: 13/10/2014 data limite: 15/10/2014
Julio Cesar Cavalcanti Carvalho
mat. 201010102211
Cobertura minima de uma árvore
*/

#include <iostream>
#include<string.h>
#include <set>
using namespace std;
const int NVM=1001;


set <int> ans; // vector para guardar a resposta.
int n, m, cpre, E[NVM][NVM],  pre[NVM][2]; // adicionada uma casa no pré para determinar se o vértice está coberto.
int i, j, u, w;

void Inicializa(){
    memset(E, 0, sizeof(E));
    memset(pre, 0, sizeof(pre));
	cpre = 0;
	ans.erase(ans.begin(),ans.end()); // apagar o vector de resposta.
}

void BP(int u,int v) {
	pre[v][0] = ++cpre;
	for(int w=1; w<=n; w++)
		if( (E[v][w] == 1) && (pre[w][0] == 0))
            BP(v,w);

        // 1 = marcado ( visto )
        // 2 = cabinado

        if((pre[v][1] == 0)) { // verifica se o vértice está marcado
            pre[v][1] = 1; // marca o filho
            if ((pre[u][1] != 2)) {  // pai não cabinado cabinado
                pre[u][1] = 2; // cabina o pai
                ans.insert(u); // inseri o pai na resposta
            }
        }

        if((pre[v][1] == 2)) // verifica se o vértice está cabinado
            pre[u][1] = 1; // marca o pai
}

int main(){
    std::set<int>::iterator it;

	while(true) {
        Inicializa();
        cout << "n m = "; cin >> n >> m;
	    cout << "Arestas:" << endl;

	    for(i=1; i<=m; i++) {
		    cin >> u >> w;
		    E[u][w] = E[w][u] = 1;
	    } BP(1,1); // inicia a busca em profundidade no 1 vértice

        cout << endl << "Vertices Cabinados: ";
               for (it=ans.begin(); it!=ans.end(); ++it)
                std::cout << ' ' << *it; cout << endl << endl;
    }
	return 0;
}

