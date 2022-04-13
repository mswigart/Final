//
// Created by Matthew Swigart on 4/6/22.
//

#ifndef UNTITLED24_HASHMAP_H
#define UNTITLED24_HASHMAP_H


template <class T> class DSVector{ //Can put mult classes and const after template, allows mult tupes

private:
    T* data;
    int size = 0;
    int capacity = 0;

public:

    DSVector(){//Constructor
        capacity = 10;
        size = 0;
        data = new T[capacity];//array alloc
    }

    DSVector(const DSVector<T>&x){//Copy Constructor
        size = x.size;
        capacity = x.capacity;
        data = new T[capacity];
        std::copy(x.data, x.data + x.size, data);//Three points, start, size, and one past
    }

    DSVector<T>& operator=(const DSVector<T>&y) {//OA Operator, replacing an object that  exists
        delete [] data;//free up data
        size = y.size;//make copy again
        capacity = y.capacity;
        data = new T[capacity];
        std::copy(y.data, y.data + y.size, data);
        return *this;//return ourselves

    }

    ~DSVector(){
        delete [] data;
    }//Destructor

    void resize(int newSize){//resize pages

        T* resize = new T[newSize];
        std::copy(data, data+size, resize);//copies to array
        delete [] data;
        data = resize;
        capacity = newSize;
        size = newSize;
    }


    void push_back(const T& z){//Push back function declared within class
        if(capacity == size){//allocate more space
            T* resize = new T[capacity*2];
            std::copy(data, data+size, resize);//copies to array
            delete [] data;
            data = resize;
            capacity = capacity*2;
        }
        data[size] = z;//Calls assignment operator
        size++;
    }

    T& operator[](int index){//T& instead of void
        assert(index >= 0 && index < size);//checks and stops program
        return data[index];
    }

    int find(const T& word){//Finds words in book
        for(int i = 0; i<size; i++){
            if(data[i] == word){
                return i;
            }
        }
        return -1;
    }

    void remove(int index){//remove index
        assert(index >= 0 && index < size);//checks and stops program
        for (int i = index; i < size-1; i++) {
            data[index] = data[index+1];
        }
        size = size-1;
    }

    void remove(const T& word) {//remove word
        int index = find(word);
        if(index == -1){
            return;
        }
        remove(index);//uses remove function
    }

    typedef T* iterator;//first type, then name

    iterator begin(){
        return data;
    }

    iterator end(){
        return data +size;//one past the end of book
    }

    int length(){
        return size;
    }

};

#endif //UNTITLED24_HASHMAP_H


