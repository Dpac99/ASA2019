#include <vector>
#include <iostream>

class heap {
private:
    int _size;
     std::vector<int> _elements = std::vector<int>();
public:
    int left(int pos){ return 2*pos + 1; } // O(1)
    int right(int pos){ return 2*pos +2;}// O(2)
    int parent(int pos){ return (int)((pos-1)/2);}

    void heapify(int pos){
        int largest = pos;
        int r = right(pos);
        int l = left(pos);
        if( l < _size && _elements.at(l) > _elements.at(pos)){
            largest = l;
        }
        if(r < _size && _elements.at(r) > _elements.at(largest)){
            largest = r;
        }

        if(largest != pos){
            int temp = _elements.at(pos);
            _elements.at(pos) = _elements.at(largest);
            _elements.at(largest) = temp;
            heapify(largest);
            heapify(pos);
        }
        if(l < _size){
            heapify(l);
        }
        if(r < _size){
            heapify(r);
        }
    }
    heap(): _size(0){}
    heap(std::vector<int> vec){
        _size = vec.size();
        for(int i=0; i<_size; i++){
            _elements.push_back(vec.at(i));
        }
        heapify(0);
    }

    void pushUp(int pos){
        int p = parent(pos);
        if( p >= 0 && _elements.at(pos) > _elements.at(p)){
            int temp = _elements.at(p);
            _elements.at(p) = _elements.at(pos);
            _elements.at(pos) = temp;
            pushUp(p);
        }
    }

    void push(int el){
        _elements.push_back(el);
        pushUp(_size++);
    }

    int getSize(){return _size;}

    int getElement(int pos){ return _elements.at(pos);}
};

int main(){
    std::vector<int> v = std::vector<int>();
    for(int i = 1; i<8 ; i++){
        v.push_back(i);
    }

    heap *h = new heap(v);

    for(int i=0; i< h->getSize(); i++){
        std::cout <<  h->getElement(i) << std::endl;
    }

    return 0;
}