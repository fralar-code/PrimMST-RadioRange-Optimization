#ifndef PRIORITY_QUEUE_H  
#define PRIORITY_QUEUE_H

#include <iostream>
#define INF 2147483647
using namespace std;

/*the PriorityQueue class was specifically created for the problem at hand
and is implemented using a heap*/
class PriorityQueue{
private:
    int num;                //represents the number of elements stored in the heap
    int *pq;                //max or min heap
    int *qp;                //array that keeps track of the position of vertices in the heap
    double *key;            //array to store edge costs
    bool max;               //boolean variable that if set to true allows managing the max heap, otherwise the min heap

public:
    PriorityQueue() {}
    /*initializes a priority queue to contain V elements
    the second parameter of the constructor is a boolean used to specify max heap or min heap*/
    PriorityQueue(int V, bool max){
        num = 0;
        pq = new int[V + 1];          //the first useful position of pq is index 1, to exploit the parent-child relationship: 2i and 2i+1
        qp = new int[V];              //qp[i] indicates the position of vertex i in pq
        key = new double[V];          //key[i] indicates the cost to reach vertex i
        this->max=max;
        for(int i=0;i<V;i++){         //loop to initialize qp to inf useful for the contains method
            qp[i]=INF;
        }
    }



    void insert(int i, double key){
        this->num++;
        this->qp[i] = this->num;        //vertex i will be in position num
        this->pq[this->num] = i;        //besides indicating the number of elements num also acts as an index for insertion in pq
        this->key[i] = key;
        bubbleUp(num);                  //calls bubbleUP to maintain the min/max heap property
    }

    void decreaseKey(int i, double key){
        if (this->key[i] > key)
            this->key[i] = key;
        bubbleUp(qp[i]);
    }

    void bubbleUp(int num){
        /*num is the index of the vertex on which we are performing the bubbleUp operation
        parent is the index of the parent*/
        int parent = num / 2;
        /*in the case of min heap if the key of the child is less than that of the parent
        inside the if we exchange the two values and vice versa for max heap*/
        if (num != 1 && comparePriority(this->key[this->pq[num]],this->key[this->pq[parent]]) )
        {
            //swap vertices in pq using xor
            this->pq[num]= this->pq[num]^this->pq[parent];
            this->pq[parent]=this->pq[parent]^this->pq[num];
            this->pq[num]= this->pq[num]^this->pq[parent];

            //swap vertex positions in qp using xor
            this->qp[pq[num]]=this->qp[pq[num]]^this->qp[pq[parent]];
            this->qp[pq[parent]]=this->qp[pq[parent]]^this->qp[pq[num]];
            this->qp[pq[num]]=this->qp[pq[num]]^this->qp[pq[parent]];

            //recursively goes up towards the root
            bubbleUp(parent);
        }
    }

//deletion of max/min
    int deleteM(){
        int r = this->pq[1];                //the max/min is at the root of the heap
        this->pq[1] = this->pq[num];        //once extracted, puts the last node of the array at the root
        this->qp[pq[1]] = 1;
        this->qp[r] = INF;                  //we keep track of deleted vertices by assigning them inf as position in the heap
        this->num--;
        bubbleDown(1);                      //calls bubbleDown to maintain the min/max heap property
        return r;
    }

//returns the vertex at the root without deleting it
    int getRoot(){
        return this->pq[1];
    }

    void bubbleDown(int i){
        //sx represents the index of the left child of i and dx vice versa
        //m contains the index of the child with the smaller value in the case of min heap and vice versa for max heap
        int m = 0, sx = 2 * i, dx = sx + 1;
        //checks if the left child exists
        if (sx <= this->num){
            m = sx;
        }
        //the right child can only exist if the left child exists
        if (dx <= this->num){
            //by comparing the keys of the children we can understand which of the two is the min/max
            if ( comparePriority(this->key[pq[sx]],this->key[pq[dx]]))
                m = sx;
            else
                m = dx;
        }
        //if m has the value 0 the vertex of index i has no children, base case
        if(m==0)
            return;
        //we compare the key of vertex i with the child with the minimum key in the case of min heap
        if (comparePriority(this->key[pq[m]],this->key[pq[i]])){
            //swap as in bubbleUp to maintain the heap property
            this->pq[i]=this->pq[i]^this->pq[m];
            this->pq[m]=this->pq[m]^this->pq[i];
            this->pq[i]=this->pq[i]^this->pq[m];

            this->qp[pq[i]]=this->qp[pq[i]]^this->qp[pq[m]];
            this->qp[pq[m]]=this->qp[pq[m]]^this->qp[pq[i]];
            this->qp[pq[i]]=this->qp[pq[i]]^this->qp[pq[m]];

            //recursively goes down
            bubbleDown(m);
        }
    }

    bool isEmpty(){
        return this->num==0;
    }

    bool contains(int v){
        return !(this->qp[v] == INF);   //if qp[v]==INF the vertex v is not contained in the heap
    }

    /*operation is used to replace comparisons in all functions
    of the priority queue, appropriately to manage max or min heap*/
    bool comparePriority(double a, double b){
        if(this->max) return a > b;
        return a < b;
    }

    double getValue(int v){
        return this->key[v];
    }

};

#endif // PRIORITY_QUEUE_H