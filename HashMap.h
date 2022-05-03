//
// Created by Matthew Swigart on 4/6/22.
//

#ifndef UNTITLED24_HASHMAP_H
#define UNTITLED24_HASHMAP_H
#include "AVLTree.h"

//when do I use std::hash?

template <class T> class HashTable{ //Can put mult classes and const after template, allows mult tupes

private:

    std::hash<T> hash;//function object that computes hash

    AvlTree<T>* data;//array of buckets

    unsigned int capacity = 0;



public:

    HashTable(int size = 100){//Constructor

        capacity = size;//Do not need to resize

        data = new AvlTree<T>[capacity];
    }

    HashTable(const HashTable<T>&x){//Copy Constructor

        capacity = x.capacity;
        data = new AvlTree<T>[capacity];

        std::copy(x.data, x.data + x.size, data);//Three points, start, size, and one past
    }


    ~HashTable(){//clears memory
        delete []data;//anything allocated with new needs[]
    }//Destructor



   T& operator[](const T& word){//T& instead of void
        int index = hash(word);
        return data[index].get(word);
    }

    unsigned int bucket(const T& size){

        return hash(size)%capacity;//function to make sure it fits in a "bucket"
    }

    void remove(const T& word){//remove index
        int index = bucket(word);
        if(data[index].contains(word)){
            data[index].remove(word);
        }
    }

    bool contains(const T& word){
        int index = bucket(word);
        return data[index].contains(word);
    }

    T* get(const T& word){
        int index = bucket(word);
        return data[index].get(word);
    }

    //declare insert
    //

    void insert(const T& word){

        int index = bucket(word);
        data[index].insert(word);

    }
};

#endif //UNTITLED24_HASHMAP_H


