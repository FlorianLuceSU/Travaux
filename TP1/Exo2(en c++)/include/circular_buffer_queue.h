#pragma once
#include <stddef.h> 

template <typename T>
struct Queue {
	size_t front;
	size_t length;
	size_t capacity; 
	T *data;
};


bool is_empty(const struct Queue *q);