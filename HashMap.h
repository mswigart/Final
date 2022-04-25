//
// Created by Matthew Swigart on 4/6/22.
//

#ifndef UNTITLED24_HASHMAP_H
#define UNTITLED24_HASHMAP_H

#include"LinkedList.h"//will I need this or just AVL Tree
#include "AVLTree.h"

//when do I use std::hash?

template <class T> class HashTable{ //Can put mult classes and const after template, allows mult tupes

private:

    std::hash<T> hash;//call something else

    AvlTree<T>* data;//How do I implement Linked List with AVL Tree here

    int capacity = 0;



public:

    HashTable(int size = 100){/*Constructor, does this belong here or another function outside
        of function*/

        capacity = size;/*Unsure how to update values as insertion happens,
        should this be lower?*/

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


    void remove(const T& word){//remove index
        int index = hash(word);
        if(data[index].contains(word)){
            data[index].remove(word);
        }
    }

    bool contains(const T& word){
        int index = hash(word);
        return data[index].contains(word);
    }

    T* get(const T& word){
        int index = hash(word);
        return data[index].get(word);
    }

    //declare insert data[index].insert(word)
    void insert(const T& word){

    }
};

#endif //UNTITLED24_HASHMAP_H


