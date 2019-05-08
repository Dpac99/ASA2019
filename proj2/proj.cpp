#include <iostream>
#include <vector>
#include <algorithm>

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
    int _maxCap;
    int _id;
    int _reverse;
public:
    Edge(int c, int i, int rev, int max){
        _cap = c;
        _maxCap = max;
        _id = i;
        _reverse = rev;
    }
    inline int getCapacity(){
        return _cap;
    }
    inline int getId(){
        return _id;
    }
    inline int getReverse(){
        return _reverse;
    }
    inline int getMaxCap(){
        return _maxCap;
    }
    inline void setCapacity(int c){
        _cap = c;
    }
    inline bool isFull(){
        return _cap == _maxCap; 
    }
};

inline bool compareE(int* a, int*b){
        if(a[0] == b[0]){
            return a[1] < b[1];
        }
        else{
            return a[0] < b[0];
        }
    }

class Graph{
private:
    int _v;
    int _e;
    int _f;
    vector<vector<Edge*>> _conections;
    int* heights;
    int* excess;
    vector<int> q;
    int * current;
    bool* in_t_cut;
    vector<int*> t_cut_E;
    vector<int> t_cut_V;
    vector<int>pre_cut;
    int* d;
    int *f;
    int _time=1;
public:
    Graph(int sup,int sta, int e){
        _f = sup;
        _v=2*(sup+sta)+ 2;
        heights = new int[_v];
        heights[0] = _v;
        heights[1] = 0;
        excess = new int[_v];
        excess[0] = 0;
        excess[1] = 0;
        d= new int[_v];
        d[0] = -1;
        d[1] = -1;
        f = new int[_v];
        f[0] = -1;
        f[1] = -1;
        in_t_cut = new bool[_v];
        in_t_cut[0] = false;
        in_t_cut[1] = false;
        q = vector<int>();
        _e =e + 2*sup + sta ;
        _conections = vector<vector<Edge*>>(_v);
        for(int i=_v - 1; i>1; i--){
            q.push_back(i);
            heights[i] = 0;
            excess[i] = 0;
            d[i]=-1;
            f[i]=-1;
            in_t_cut[i] = false;
        }
    }

    Graph(int v, int e){
        _v = v;
        _e = e;
        _conections = vector<vector<Edge*>>(_v);
        d= new int[_v];
        f = new int[_v];
        in_t_cut = new bool[_v];
        for(int i=0; i<_v; i++){
            d[i]=-1;
            f[i]=-1;
            in_t_cut[i] = false;
        }
    }

    void addSupplier(int id, int cap){
        int in = getEntryV(id);
        int out = getExitV(id); 
        int sourceSize = _conections.at(0).size();
        int inSize = _conections.at(in).size();
        int outSize = _conections.at(out).size();
        _conections.at(0).push_back(new Edge(cap, in,inSize, cap));
        _conections.at(in).push_back(new Edge(0, 0, sourceSize, cap));
        _conections.at(in).push_back(new Edge(cap, out, outSize, cap));
        _conections.at(out).push_back(new Edge(0,in, _conections.at(in).size()-1, cap));
    }

    void addStation(int id, int cap){
        int in = getEntryV(id);
        int out = getExitV(id);
        _conections.at(in).push_back(new Edge(cap, out, _conections.at(out).size(), cap));
        _conections.at(out).push_back(new Edge(0, in, _conections.at(in).size() -1, cap ));
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
        _conections.at(origOut).push_back(new Edge(cap, destIn, _conections.at(destIn).size(), cap));
        _conections.at(destIn).push_back(new Edge(0, origOut, _conections.at(origOut).size() - 1, cap));
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

    int findMinHeight(int v){
        int h=0, u=0;
        int i=0;
        int size = _conections.at(v).size();
        for(; i< size; i++){
            if(_conections.at(v).at(i)->getCapacity() > 0){
                h = heights[_conections.at(v).at(i)->getId()];
                u = _conections.at(v).at(i)->getId();
                break;
            }
        }
        for (; i< size;  i++){
            if(heights[_conections.at(v).at(i)->getId()] < h && _conections.at(v).at(i)->getCapacity() > 0){
                h=heights[_conections.at(v).at(i)->getId()];
                u = _conections.at(v).at(i)->getId();
            }
        }
        return u;
    }

    void relabel(int v){
        heights[v] = 1 + heights[findMinHeight(v)];
    }

    inline int min(int a, int b){
        return a<b? a : b;
    }

    void push(int v, Edge *e){
        int total = min(excess[v], e->getCapacity());
        Edge* revE = _conections.at(e->getId()).at(e->getReverse());
        e->setCapacity( e->getCapacity() - total);
        revE->setCapacity(revE->getCapacity() + total);
        excess[v]-=total;
        excess[e->getId()]+=total;
    }

    void discharge(int v){
        long unsigned int i=0;
        Edge* e;
        while(excess[v] > 0){
            if(i == _conections.at(v).size()){
                relabel(v);
                i=0;
            }
            else{
                e = _conections.at(v).at(i);
                if(e->getCapacity() > 0 && heights[v] == heights[e->getId()] + 1){
                    push(v, e);
                }
                else{
                    i++;
                }
            }
        }
    }

    void moveToFront(int v){
        long unsigned int i;
        for(i=0; q.at(i) != v; i++);
        q.erase(q.begin() + i);
        q.push_back(v);
    }

    void relabelToFront(){
        int v, oldh;
        for(int i = q.size() - 1; i>=0; i--){
            v = q.at(i);
            oldh = heights[v];
            discharge(v);
            if(heights[v] > oldh){
                moveToFront(v);
                i=q.size()-1;
            }
        }
    }

    void printGraph(){
        for (int i=0; i<_v; i++){
            for(long unsigned int j=0; j<_conections.at(i).size(); j++){
                printf("From %d (excess = %d, height = %d) to %d (excess = %d, height = %d) with capacity %d\n", i,excess[i], heights[i], _conections.at(i).at(j)->getId(),excess[_conections.at(i).at(j)->getId()], heights[_conections.at(i).at(j)->getId()], _conections.at(i).at(j)->getCapacity());
            }
        }
    }

    void printList(){
        for(int i=0; i<_v; i++){
            printf("%d | ", i);
            for(long unsigned int j = 0; j<_conections.at(i).size(); j++){
                printf("{ %d %d %d} ", _conections.at(i).at(j)->getId(),_conections.at(i).at(j)->getCapacity(), _conections.at(i).at(j)->getReverse());
            }
            printf("\n");
        }
    }

    void printMinCut(){
        printf("Cut vertixes: {");
        for(int i=0; i<_v; i++){
            if(in_t_cut[i]){
                printf("%d, ", i);
            }
        }
    }

    Graph* invertGraph(){
        Graph* g = new Graph(_v, _e);
        for(int i=0; i<_v; i++){
            int size = _conections.at(i).size();
            for(int j=0; j<size; j++){
                if(_conections.at(i).at(j)->getCapacity() > 0){
                    g->addConnection(j,i,1);
                }
            }
        }
        return g;
    }

    void findMinCut(int v){ // vector<Edge*> t_cut
        //printf("{%d}", v);
        d[v] = _time++;
        in_t_cut[v] = true;
        int size = _conections.at(v).size();
        for(int i=0; i<size; i++){
            if(d[_conections.at(v).at(i)->getId()] == -1 && !(_conections.at(v).at(i)->isFull()) ){
                findMinCut(_conections.at(v).at(i)->getId());
            }
        }
        f[v] = _time++;
        pre_cut.push_back(v);
    }

    void filterEdges(){
        int size = pre_cut.size();
        for(int i=0; i<size; i++){
            int v = pre_cut.at(i);
            int adjS = _conections.at(v).size();
            for(int j=0; j<adjS; j++){
                Edge* e = _conections.at(v).at(j);
                int orig  = e->getId();
                int v = _conections.at(orig).at(e->getReverse())->getId();
                if(getParentV(v) == getParentV(orig) && !in_t_cut[orig] && getParentV(v) > _f+1){
                    t_cut_V.push_back(getParentV(v));
                }
                else if(getParentV(v) != getParentV(orig) && !in_t_cut[orig] && !in_t_cut[getExitV(getParentV(orig))] && (e->isFull())){
                    int * a = new int[2];
                    a[0] = getParentV(orig);
                    a[1] = getParentV(v);
                    t_cut_E.push_back(a);
                }
            }
        }
    }

    void printResult(){
        /*printf("92|");
        for(int i=0; i<_conections.at(92).size(); i++){
            printf("{%d}", _conections.at(92).at(i)->getId());
        }
        printf("\n");
        printf("93|");
        for(int i=0; i<_conections.at(93).size(); i++){
            printf("{%d}", _conections.at(93).at(i)->getId());
        }
        printf("\n");
        printf("110|");
        for(int i=0; i<_conections.at(110).size(); i++){
            printf("{%d}", _conections.at(110).at(i)->getId());
        }
        printf("\n");
        printf("111|");
        for(int i=0; i<_conections.at(111).size(); i++){
            printf("{%d}", _conections.at(111).at(i)->getId());
        }
        printf("\n");
        printf("92: %d, 93: %d, 110: %d, 111: %d\n", in_t_cut[92], in_t_cut[93], in_t_cut[110], in_t_cut[111]);*/
        sort(t_cut_V.begin(), t_cut_V.end());
        sort(t_cut_E.begin(), t_cut_E.end(), compareE);
        printf("%d\n", excess[1]);
        if(t_cut_V.size() > 0){
            printf("%d", t_cut_V.at(0));
        }
        for( long unsigned int i =1; i<t_cut_V.size(); i++){
            printf(" %d", t_cut_V.at(i));
        }
        printf("\n");
        for( long unsigned int i=0; i<t_cut_E.size(); i++){
            printf("%d %d\n", t_cut_E.at(i)[0], t_cut_E.at(i)[1]);
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
    //g->printGraph();
    //g->printList();
    //printf("\n\n");
    g->relabelToFront();
    //g->printGraph();
    //g->printList();
    //printf("cut|");
    g->findMinCut(1);
    g->filterEdges();
    //printf("\n");
    //g->printMinCut();
    g->printResult();
    //g->printList();
}


