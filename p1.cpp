#include <iostream>
#include <string>
#include <vector>
#include<stack>

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
public:
    Graph(int n){
        _n_nodes = n;
        _matrix = vector<vector<int>>(_n_nodes);
    }

    void insert_edge(int v1, int v2){
        _matrix.at(v1).push_back(v2);
        _matrix.at(v2).push_back(v1);
    }

    void visit(int* d, int* low, bool* on_stack, stack<int> *visited, int* parent, int pos);

    void tarjan_scc(){
        int *d = new int[_n_nodes];
        int *low = new int[_n_nodes];
        int *parent = new int[_n_nodes];
        bool *on_stack = new bool[_n_nodes];
        stack<int> *visited = new stack<int>();

        for(int i =0; i< _n_nodes; i++){
            d[i]=UNVISITED;
            low[i] = UNVISITED;
            parent[i] = NIL;
            on_stack[i] = false;
        }
        for(int i=0; i<_n_nodes; i++){
            if(d[i] == UNVISITED){
                visit(d, low, on_stack, visited, parent, i);
            }
        }
    }

    /* DEBUG START */
    void print_graph(){
        cout << _n_nodes << endl;
        for(int i =0; i< _n_nodes; i++){
            cout << i +1<< " : ";
            for (int j=0; j< _matrix.at(i).size(); j++){
                cout << _matrix.at(i).at(j) +1 << "  ";
            }
            cout << endl;
        }
        cout <<endl; 
        cout<<"Networks: "<<endl;
        for(int i =0; i<_networks.size(); i++){
            cout << _networks.at(i)  + 1<< endl;
        }
        cout << "Aps: "<< endl;
        for(int i =0; i<_aps.size(); i++){
                cout << _aps.at(i) + 1<< endl;
            }
    }

    /* DEBUG END */
};

void Graph::visit(int* d, int* low, bool* on_stack, stack<int> *visited, int *parent, int pos){
    int children = 0;
    d[pos] = low[pos] = d_time++;
    visited->push(pos);
    on_stack[pos] = true;
    for(int i = 0; i< _matrix.at(pos).size(); i++){
        int adj = _matrix.at(pos).at(i);
        if(d[adj] == UNVISITED){
            children++;
            parent[adj] = pos;
            visit(d, low, on_stack, visited, parent,adj);
            low[pos] = MIN(low[pos], low[adj]);

            if(parent[pos] == NIL && children >=2){
                _aps.push_back(pos);
            }
            else if (parent[pos] != NIL && low[adj] >= d[pos]){
                _aps.push_back(pos);
            }
        }
        else if(on_stack[adj]){
            low[pos] = MIN(low[pos], d[adj]);
        }
    }
    int curr;
    if(low[pos] == d[pos]){
        int max = NIL;
        while(visited->top() != pos){
            curr = visited->top();
            on_stack[curr] = false;
            if(curr > max){max = curr;}
            visited->pop();
        }
        on_stack[pos] = false;
        if(pos>max){max=pos;}
        visited->pop();
        _networks.push_back(max);
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