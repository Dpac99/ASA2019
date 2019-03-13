#include <iostream>
#include <string>
#include <vector>
#include<stack>

#define UNVISITED -1
#define MIN(a, b) (a<b? a : b)

static int d_time = 0;


using namespace std;

class SCC{
private:
    int _id = UNVISITED;
    vector<int> _nodes;
public:
    void addNode(int n){
        if(n > _id){
            _id = n;
        }
        _nodes.push_back(n);
    }

    int size(){
        return _nodes.size();
    }

    int id(){
        return _id;
    }

    void print(){
        cout << "SCC id:" << _id << endl;
        for(int i =0; i<_nodes.size(); i++){
            cout << _nodes.at(i) << endl;
        }
    }

};

class Graph{
private:
    vector<vector<int>> _matrix;
    int _n_nodes;
    int _n_networks;
    vector<SCC*> _networks;
public:
    Graph(int n){
        _n_nodes = n;
        _matrix = vector<vector<int>>(n+1);
    }

    void insert_edge(int v1, int v2){
        _matrix.at(v1).push_back(v2);
        _matrix.at(v2).push_back(v1);
    }

    void visit(int* d, int* low, bool* on_stack, stack<int> *visited, int pos);

    void tarjan_scc(){
        int *d = new int[_n_nodes];
        int *low = new int[_n_nodes];
        bool *on_stack = new bool[_n_nodes];
        stack<int> *visited = new stack<int>();

        for(int i =0; i< _n_nodes; i++){
            d[i]=UNVISITED;
            low[i] = UNVISITED;
            on_stack[i] = false;
        }
        for(int i=0; i<_n_nodes; i++){
            if(d[i] == UNVISITED){
                visit(d, low, on_stack, visited, i);
            }
        }
    }

    /* DEBUG START */
    void print_graph(){
        cout << _n_nodes << endl;
        for(int i =0; i< _n_nodes; i++){
            cout << i << " : ";
            for (int j=0; j< _matrix.at(i).size(); j++){
                cout << _matrix.at(i).at(j) << "  ";
            }
            cout << endl;
        }
        cout <<endl; 
        for(int i =0; i<_networks.size(); i++){
                _networks.at(i)->print();
            }
    }

    /* DEBUG END */
};

void Graph::visit(int* d, int* low, bool* on_stack, stack<int> *visited, int pos){
    d[pos] = low[pos] = d_time++;
    visited->push(pos);
    on_stack[pos] = true;
    for(int i = 0; i< _matrix.at(pos).size(); i++){
        int adj = _matrix.at(pos).at(i);
        if(d[i] == UNVISITED){
            visit(d, low, on_stack, visited, adj);
            low[pos] = MIN(low[pos], low[adj]);
        }
        else if(on_stack[adj]){
            low[pos] = MIN(low[pos], d[adj]);
        }
    }
    int curr;
    if(low[pos] == d[pos]){
        SCC* scc = new SCC();
        while(visited->top() != pos){
            curr = visited->top();
            cout << curr << endl ;
            on_stack[curr] = false;
            scc->addNode(curr);
            visited->pop();
        }
        on_stack[pos] = false;
        scc->addNode(pos);
        visited->pop();
        _networks.push_back(scc);
    }
}

Graph* parse(){
    int n_vert, n_con, r1, r2;
    Graph *g;
    scanf("%d", &n_vert);
    g = new Graph(n_vert); // n^2
    scanf("%d", &n_con);
    for(int i =0; i<n_con; i++){
        scanf("%d %d", &r1, &r2);
        g->insert_edge(r1-1,r2-1);
    }
    return g;
}

int main(){
    Graph* g = parse();
    g->tarjan_scc();
    g->print_graph();
    return 0;
}