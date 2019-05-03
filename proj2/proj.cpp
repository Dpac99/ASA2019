#include <iostream>
#include <vector>
#include <stack>

using namespace std;

inline int getEntryV(int id){
    return 2*id - 2;
}

inline int getExitV(int id){
    return 2*id - 1;
}

inline int getParentV(int id){
    return (int) (id+2)/2;
}


class Edge{
private:
    int _cap;
    int _id;
public:
    Edge(int c, int i){
        _cap = c;
        _id = i;
    }
    int getCapacity(){
        return _cap;
    }
    int getId(){
        return _id;
    }
    void setCapacity(int c){
        _cap = c;
    }
};

class Graph{
private:
    int _v;
    int _e;
    vector<vector<Edge*>> _conections;
    vector<vector<int>> _paths;
    int* heights;
    int* excess;
    stack<int> *queue =  new stack<int>();
public:
    Graph(int sup,int sta, int e){
        _v=2*(sup+sta)+ 2;
        heights = new int[_v];
        heights[0] = _v;
        heights[1] = 0;
        excess = new int[_v];
        excess[0] = 0;
        excess[1] = 0;
        _e =e + 2*sup + sta ;
        _conections = vector<vector<Edge*>>(_v);
        for(int i=2; i<_v; i++){
            queue->push(i);
            heights[i] = 0;
            excess[i] = 0;
        }
    }

    void addSupplier(int id, int cap){
        int in = getEntryV(id);
        int out = getExitV(id); 
        _conections.at(0).push_back(new Edge(cap, in));
        _conections.at(in).push_back(new Edge(0, 0));
        _conections.at(in).push_back(new Edge(cap, out));
        _conections.at(out).push_back(new Edge(0,in));
    }

    void addStation(int id, int cap){
        int in = getEntryV(id);
        int out = getExitV(id);
        _conections.at(in).push_back(new Edge(cap, out));
        _conections.at(out).push_back(new Edge(0, in));
    }

    void addConnection(int origin, int destiny, int cap){
        int origOut = getExitV(origin);
        int destIn;
        if(destiny != 1){
            destIn = getEntryV(destiny);
        }
        else{
            destIn = 1;
        }
        _conections.at(origOut).push_back(new Edge(cap, destIn));
        if(destIn != 1){
            _conections.at(destIn).push_back(new Edge(0, origOut));
        }
    }

    void initializeSource(){
        int size = _conections.at(0).size();
        Edge *v;
        for(int i = 0; i<size; i++){
            v = _conections.at(0).at(i);
            excess[0] -= v->getCapacity();
            excess[v->getId()] = v->getCapacity();
            _conections.at(v->getId()).at(0)->setCapacity(v->getCapacity());
            v->setCapacity(0);
        }
    }

    void printGraph(){
        for (int i=0; i<_v; i++){
            for(int j=0; j<_conections.at(i).size(); j++){
                printf("From %d (excess = %d, height = %d) to %d (excess = %d, height = %d) with capacity %d\n", i,excess[i], heights[i], _conections.at(i).at(j)->getId(),excess[_conections.at(i).at(j)->getId()], heights[_conections.at(i).at(j)->getId()], _conections.at(i).at(j)->getCapacity());
            }
        }
    }
};

/*Input 
 *One line with f>0; e>= 0; t>=0;
 * f=number of suppliers; e=number of stations; t = number of connections
 * 
 * One line with f ints representing the capacity of each supplier
 * 
 * One line with e ints representing the capacity of each station
 * 
 * A sequence of t lines with 3 ints o>=2; d>=1; c>=1
 * o = origin id; d = destiny id; c = capacity of the edge 
 * 
 * Additional info:
 * Source id = 0;
 * Target id = 1;
 * if 2<= o <=f+1, then its a supplier, otherwise it's a station
*/

Graph* parseInput(){
    Graph *g;
    int f, e, t, c, o, d;
    scanf("%d", &f);
    scanf("%d", &e);
    scanf("%d", &t);
    g = new Graph(f ,e, t);
    for( int i = 2; i< f+2; i++){
        scanf("%d", &c);
        g->addSupplier(i, c);
    }
    for(int i = f+2; i<f+2+e; i++){
        scanf("%d", &c);
        g->addStation(i, c);
    }
    for(int i = 0; i<t; i++){
        scanf("%d %d %d", &o, &d, &c);
        g->addConnection(o,d,c);
    }

    return g;
}

int main(){
    Graph* g = parseInput();
    g->initializeSource();
    g->printGraph();
}


