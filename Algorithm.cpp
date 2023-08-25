#include "./List.h"
#include "./Deque.h"
#include "./Pair.h"



template<typename T>
int _partition(List<T>& list, int low, int high, bool reverse = false){

    T pivot = list[low];
    int i = low - 1; 
    int j = high + 1;
    T tmp_val;

    while(true){
        do{
            i++;
        } while( reverse? (list[i] > pivot) : (list[i] < pivot) );
        do {
            j--;
        } while( reverse? (list[j] < pivot) : (list[j] > pivot) );

        if( i >= j ){
            return j;
        }

        //swap
        tmp_val = list[i];
        list[i] = list[j];
        list[j] = tmp_val;
    }
}


template<typename T>
void sort(List<T>& list, bool reverse = false){
    List<Pair<int,int>> que;
    que.push({0,list.length()-1});
    int low, high, pivot;

    while(!que.empty()){
        low = que.top().key;
        high = que.top().value;
        que.pop();

        if(low < high){
            pivot = _partition(list, low, high, reverse);
            que.push({low, pivot});
            que.push({pivot + 1, high});
        } 
    }

}


int main(){
    List<int> lst = { 10, 7, 8, 9, 1, 5 };
    sort(lst);
    // BaseComparator<int> ;
    lst.print();
}
