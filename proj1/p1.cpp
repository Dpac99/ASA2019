#include <vector>
#include <stack>
#include <algorithm>
#include <cstdio>

#define UNVISITED -1
#define NIL -1
#define MIN(a, b) (a<b? a : b)

static int d_time = 0;

using namespace std;

class Graph{
private:
    vector<vector<int>> _matrix;
    int _n_nodes;
    int _n_networks;
    vector<int> _networks;
    vector<int> _aps;
    int _biggest_scc, _preemptive_biggest = 0;
    stack<int> *visited = new stack<int>();
public:
    Graph(int n){
        _n_nodes = n;
        _matrix = vector<vector<int>>(_n_nodes); //O(V)
    }

    void insert_edge(int v1, int v2){
        _matrix.at(v1).push_back(v2); //Amortized constant. Como definimos o vetor com n size, podemos
        _matrix.at(v2).push_back(v1);  //assumir que push_back é O(1) porque nao ha redefinicoes de tamanho
    }

    void visit(int* d, int* low, bool* on_stack, stack<int> *visited, int* parent,  bool *is_ap, int pos);
    //Tarjan. Segundo a nossa implementação (adaptado do pseudocodigo da aula), é O(V+E)

    void find_biggest_scc(int* d, int* low, bool* on_stack, stack<int> *visited, bool *is_ap, int pos);
    // Igual ao anterior

    void tarjan_scc(){ //Todas as inicializacoes sao O(V)
        int *d = new int[_n_nodes];
        int *low = new int[_n_nodes];
        int *parent = new int[_n_nodes];
        bool *on_stack = new bool[_n_nodes];
        bool *is_ap = new bool[_n_nodes];

        for(int i =0; i< _n_nodes; i++){ //O(V)
            d[i]=UNVISITED;
            low[i] = UNVISITED;
            parent[i] = NIL;
            on_stack[i] = false;
            is_ap[i] = false;
        }
        for(int i=0; i<_n_nodes; i++){
            if(d[i] == UNVISITED){ //Executar tarjan_visit para cada V nao visitado. Logo, O(V+E)
                visit(d, low, on_stack, visited, parent,is_ap, i);
            }
        }
        if(_aps.size() != 0){ //O(V). Reinicializacao dos vetores para a segunda procura DFS
            for(int i =0; i< _n_nodes; i++){
                d[i]=UNVISITED;
                low[i] = UNVISITED;
                on_stack[i] = false;
            } 
            d_time = 0;
            for(int i=0; i<_n_nodes; i++){
                if(d[i] == UNVISITED){ //O(V+E)
                    find_biggest_scc(d, low, on_stack, visited,is_ap, i);
                }
            }
        }
        else{
            _biggest_scc = _preemptive_biggest;
        }
        delete[] d;
        delete[] low;
        delete[] on_stack;
        delete[] parent;
        delete[] is_ap;
    }

    /* DEBUG START */
    void print_graph(){
        printf("%d\n", _n_networks);
        sort(_networks.begin(), _networks.end()); //Segundo o manual de cpp, O(N log(N)), em que N é o numero de subredes 
        unsigned int i;
        for(i =0; i<_networks.size() - 1; i++){ //O(N)
            printf("%d ", _networks.at(i) + 1);
        }
        printf("%d", _networks.at(i) + 1);
        printf("\n%lu\n", _aps.size());
        printf("%d\n", _biggest_scc);
    }

    /* DEBUG END */
};

void Graph::visit(int* d, int* low, bool* on_stack, stack<int> *visited, int *parent,  bool *is_ap, int pos){
    int children = 0;
    d[pos] = low[pos] = d_time++;
    visited->push(pos);
    on_stack[pos] = true;
    for(unsigned int i = 0; i< _matrix.at(pos).size(); i++){
        int adj = _matrix.at(pos).at(i);
        if(d[adj] == UNVISITED){
            children++;
            parent[adj] = pos;
            visit(d, low, on_stack, visited, parent, is_ap, adj);
            low[pos] = MIN(low[pos], low[adj]);

            if(parent[pos] == NIL && children >=2 && !is_ap[pos]){
                _aps.push_back(pos);
                is_ap[pos] = true;
            }
            else if (parent[pos] != NIL && low[adj] >= d[pos] && !is_ap[pos]){
                _aps.push_back(pos);
                is_ap[pos] = true;
            }
        }
        else if(on_stack[adj]){
            low[pos] = MIN(low[pos], d[adj]);
        }
    }
    int curr;
    if(low[pos] == d[pos]){
        int max = NIL;
        int size = 0;
        while(visited->top() != pos){
            curr = visited->top();
            on_stack[curr] = false;
            if(curr > max){max = curr;}
            visited->pop();
            size++;
        }
        on_stack[pos] = false;
        size++;
        if(pos>max){max=pos;}
        if(size > _preemptive_biggest){ _preemptive_biggest = size;}
        visited->pop();
        _networks.push_back(max);
        _n_networks++;
    }
}

void Graph::find_biggest_scc(int* d, int* low, bool* on_stack, stack<int> *visited,  bool *is_ap, int pos){
    d[pos] = low[pos] = d_time++;
    visited->push(pos);
    on_stack[pos] = true;
    for(unsigned int i = 0; i< _matrix.at(pos).size(); i++){
        int adj = _matrix.at(pos).at(i);
        if(d[adj] == UNVISITED && !is_ap[adj]){
            find_biggest_scc(d, low, on_stack, visited, is_ap, adj);
            low[pos] = MIN(low[pos], low[adj]);
        }
        else if(on_stack[adj] && !is_ap[adj]){
            low[pos] = MIN(low[pos], d[adj]);
        }
    }
    int size=0;
    int curr;
    if(low[pos] == d[pos]){
        while(visited->top() != pos){
            curr = visited->top();
            on_stack[curr] = false;
            visited->pop();
            size++;
        }
        on_stack[pos] = false;
        visited->pop();
        size++;
        if(size > _biggest_scc){ _biggest_scc = size;}
    }
}

Graph* parse(){
    int n_vert, n_con, r1, r2;
    Graph *g;
    scanf("%d", &n_vert);
    g = new Graph(n_vert); //O(V.E)
    scanf("%d", &n_con);
    for(int i =0; i<n_con; i++){
        scanf("%d %d", &r1, &r2);
        g->insert_edge(r1-1,r2-1);//O(1)
    }
    return g;
}

int main(){
    Graph* g = parse();
    g->tarjan_scc();
    g->print_graph();
    delete g;
    return 0;
}
