//
// Created by Matthew Swigart on 3/23/22.
//

#ifndef INC_22S_FLIGHT_PLANNER_LINKEDLIST_H
#define INC_22S_FLIGHT_PLANNER_LINKEDLIST_H
#include "DSString.h"

template <class T> struct Node//Should I use a struct?
{
public:
    T data;
    Node* next; // Pointer to next node in DLL
    Node* prev; // Pointer to previous node in DLL
    Node(){
        //data = 0;
        next = nullptr;
        prev = nullptr;
    }
};

template <class T> struct DSLinkedList {
public:

    void push_back(T);//push onto linked list

    void pop_back();//pop off of linked list

    T back();//move back on linked list

    bool isEmpty();//check if empty

    DSLinkedList(){
        first = nullptr;
        last = nullptr;
    }

    struct iterator {
        Node<T> *ptr;

        iterator operator++() {
            ptr = ptr->next;
            return *this;
        }

        T operator*() {
            return ptr->data;
        }

        bool operator==(iterator other) {
            return ptr == other->ptr;
        }

        bool operator!=(iterator other) {
            return ptr != other->ptr;
        }

        T* operator->() {
            return &ptr->data;
        }
    };

    iterator begin(){
        return iterator({first});
    }

    iterator end(){
        return iterator({nullptr});
    }

private:

    Node<T> *first, *last;
};

template <class T> bool DSLinkedList<T>::isEmpty(){
    if (last == nullptr){
        return true;
    }
    return false;
}

template <class T> T DSLinkedList<T>::back(){
    if (isEmpty()){
        throw std::logic_error("Empty list.");
    }
    return last->data;
}

template <class T> void DSLinkedList<T>::push_back(T flight) {
    if (isEmpty()){
        first = new Node<T>;
        first -> data = flight;
        last = first;
    }
    else{
        Node<T> *temp = last;
        last = new Node<T>;
        last -> data = flight;
        temp -> next = last;
        last -> prev = temp;
        }
}

template <class T> void DSLinkedList<T>::pop_back() {
    if (isEmpty()){
        throw std::logic_error("Empty List.");
    }
    else if (first == last){
        delete first;
        first = nullptr;
        last = nullptr;
    }
    else{
        last = last->prev;
        delete last->next;
        last -> next = nullptr;
    }
};


#endif //INC_22S_FLIGHT_PLANNER_LINKEDLIST_H
