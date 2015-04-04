/*
Otimização em grafos
Trabalho extra 1 - data de entrega: 22/09/2014 data limite: 15/10/2014
Julio Cesar Cavalcanti Carvalho
mat. 201010102211
*/

#include <iostream>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <vector>
#include <queue>
#include <set>
#define MAX 800

using namespace std;

struct vertice {
    int filho_e, filho_d, pai; // numero da linha...
    vertice() {}
    vertice(int fe, int fd, int pai) : filho_e(fe), filho_d(fd), pai(pai) {}
};


struct group { // guarda os parametros da melhor sequencia encontrada
    int ends[int (MAX/2+1)];
    std::set <int> cobertura;
    std::set <int> ids;
    group() {}
};

vertice Grafo[MAX];
group resp;
int id = 1, endId = 1, tempEnd=0;

void inserir(int nid, int pai) {
char e, d;
int eId = endId, dId = endId;
    cout <<  "o vertice " << nid << " possui filho 'esquerdo' ? (S/N) "; cin >> e;
    cout <<  "o vertice " << nid << " possui filho 'direito' ? (S/N) "; cin >> d; cout << endl;
    if (e == 'S' || e == 's') {
        Grafo[nid].filho_e = (++eId,eId);
        Grafo[eId] = vertice(0,0,nid);
    }
    if (d == 'S' || d == 's') {
        Grafo[nid].filho_d = (dId = eId+1, dId);
        Grafo[dId] = vertice(0,0,nid);
    }
    endId = ( eId == dId ? (resp.ends[tempEnd++] = nid, max(endId,max(dId, eId))): max(dId, eId));
}

int main() {
    std::set<int>::iterator it;
    int i, j, pid = 0;
    memset(resp.ends,0,sizeof(resp.ends));

    Grafo[id] = vertice(0,0,0);
    while (id <= endId) {
        inserir(id,Grafo[id].pai);
        id++;
    }

    // coloca o pai de todos os filhos (folhas) do final do grafo já que só são acessíveis pelos seus respectivos pais
    for (i=0; i < tempEnd ;i++) {
        pid = Grafo[resp.ends[i]].pai;
        pid = (pid==0 ? resp.ends[i] : pid);
        resp.cobertura.insert(Grafo[pid].pai);
        resp.cobertura.insert(Grafo[pid].filho_e);
        resp.cobertura.insert(Grafo[pid].filho_d);
        resp.cobertura.insert(pid);
        resp.ids.insert(pid);

        if (*resp.cobertura.begin() == 0) resp.cobertura.erase(0);
    }

    // testa se a cobertura esta completa ou se precisamos adicionar cabines ao longo do caminho
    if (resp.cobertura.size() < endId)
        for (i = endId; i >= 1 ;i--)
            if (!resp.cobertura.count(i)) {
                pid = Grafo[i].pai;
                pid = (pid==0 ? i : pid);
                resp.cobertura.insert(Grafo[pid].pai);
                resp.cobertura.insert(Grafo[pid].filho_e);
                resp.cobertura.insert(Grafo[pid].filho_d);
                resp.cobertura.insert(pid);
                resp.ids.insert(pid);
            if (*resp.cobertura.begin() == 0) resp.cobertura.erase(0);
            }

    // cobertura no set
    cout << "Numero minimo de vertices = " << resp.ids.size() << endl;
    cout << "numero de vertices cobertos = " << resp.cobertura.size() << endl;
    cout << "vertices cabinados:";
    for (it=resp.ids.begin(); it!=resp.ids.end(); ++it) std::cout << ' ' << *it; cout << endl << endl;

    return 0;
}
