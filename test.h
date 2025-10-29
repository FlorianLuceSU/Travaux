#include <stdbool.h>
#include <stddef.h> 


template < typename T >
struct Queue {
size_t front ; // index of the first element in the queue
size_t length ; // number of items presently in the queue
size_t capacity ; // capacity of the queue ( in nbr of items )
T * data ; // address of the array
};


int main(){
    return 0;
}