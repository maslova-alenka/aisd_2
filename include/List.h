#ifndef LIST_INCLUDE_LIST_H
#define LIST_INCLUDE_LIST_H

#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <cmath>

#include "Random.h"

template <typename T>
struct Node {
	Node* next, * prev;
	T data;

	Node(T data) : data(data), prev(nullptr), next(nullptr) {};

    //Node(Node* next, Node* prev, T data) :next(next), prev(prev), data(data) {};
    //Node(const Node<T>& other) {
    //    next = nullptr;
    //    prev = nullptr;
    //    data = new T(other._data);
    //}
};

template <typename T>
class CyclicList {
	Node<T>* _head, * _tail;
	size_t _size;
public:
	CyclicList() : _head(nullptr), _tail(nullptr), _size(0) {};

    //CyclicList(size_t size, T min_value, T max_value) {
    //    for (size_t i = 0; i < size; i++) {
    //        T value = static_cast<T>(rand()) / RAND_MAX * (max_value - min_value) + min_value;
    //        this->push_tail(value);
    //    }
    //}

    //CyclicList(size_t size) {
    //    _head = nullptr;
    //    _tail = nullptr;
    //    for (size_t i = 0; i < size; i++) {
    //        push_tail(new Node<T>(new T(random<T>())));
    //    }
    //}

    /*(new Node<T>(new T(random<T>())));*/

/* (Node<T>(T(random<T>())))*/


    CyclicList(size_t size, T lower_bound, T upper_bound) :CyclicList() {
        std::random_device rd;
        std::mt19937 gen(rd());

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(lower_bound, upper_bound);

            for (size_t i = 0; i < size; ++i) {
                T data = dist(gen);
                push_tail(data);
            }
            _size = size;
        }
        else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(lower_bound, upper_bound);

            for (size_t i = 0; i < size; ++i) {
                T data = dist(gen);
                push_tail(data);
            }
            _size = size;
        }
    }

    CyclicList(const CyclicList& other) {
        _head = nullptr;
        _tail = nullptr;
        Node<T>* tmp = other._head;
        this->push_tail(*tmp);
        tmp = tmp->next;
        while (tmp != other._head) {
            Node<T>* new_tmp = new Node<T>(*tmp);

            this->push_tail(*new_tmp);
            tmp = tmp->next;
        }
    }


    CyclicList& operator=(const CyclicList& other) {
        if (this != &other) {
            clear();
            Node<T>* tmp = other._head;
            while (tmp != nullptr) {
                push_tail(tmp->data);
                tmp = tmp->next;
            }
        }
        return *this;
    }

    size_t size() const {
        return _size;
    }

    void push_head(T data) {
        Node<T>* tmp = new Node<T>(data);
        if (_head == nullptr) {
            _head = tmp;
            _tail = tmp;
            tmp->next = tmp;
            tmp->prev = tmp;
        }
        else {
            tmp->prev = _tail;
            tmp->next = _head;
            _tail->next = tmp;
            _head->prev = tmp;
            _head = tmp;
        }
        _size++;
    }

    void push_tail(T data) {
        Node<T>* tmp = new Node<T>(data);
        if (_head == nullptr) {
            _head = tmp;
            _tail = tmp;
            tmp->next = tmp;
            tmp->prev = tmp;
        }
        else {
            tmp->prev = _tail;
            tmp->next = _head;
            _tail->next = tmp;
            _head->prev = tmp;
            _tail = tmp;
        }
        _size++;
    }

    void push_head(const CyclicList<T>& other) {
        Node<T>* other_tail_next = other._tail->next;
        Node<T>* head_prev = _head->prev;

        other._tail->next = _head;
        _head->prev = other._tail;
        other._head->prev = _tail;
        _tail->next = other._head;

        _head = other._head;
        _tail = other_tail_next->prev;

        _head->prev = head_prev;
        head_prev->next = _head;
    }

    void push_tail(const CyclicList& other) {
        if (other.empty()) {
            return;
        }
        if (empty()) {
            *this = other;
            return;
        }
        Node<T>* other_head = other._head;
        while(other_head) {
            push_tail(other_head->data);
            other_head = other_head->next;
        }

        //other._head->prev = _tail; 
        //_tail->next = other._head;
        //other._tail->next = _head;
        //_head->prev = other._tail;
        //_tail = other._tail;
        //_size += other._size;
    }

    void delete_node(T data) {
        Node <T>* tmp = _head;
        while (tmp != nullptr) {
            if (tmp->data == data) {
                if (tmp == _head)
                {
                    Node<T>* new_head = _head->next;
                    new_head->prev = nullptr;
                    delete _head;
                    _head = new_head;
                }
                else if (tmp == _tail)
                {
                    Node<T>* new_tail = _tail->prev;
                    new_tail->next = nullptr;
                    delete _tail;
                    _tail = new_tail;
                }
                else
                {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                    delete tmp;
                }
            }
            tmp = tmp->next;
        }
    }

    void pop_head() {
        if (_head == nullptr) {
            return;
        }
        if (_head == _tail) {
            delete _head;
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            Node<T>* new_head = _head->next;
            new_head->prev = _tail;
            _tail->next = new_head;
            delete _head;
            _head = new_head;
        }
        _size--;
    }

    void pop_tail() {
        if (_tail == nullptr) {
            return;
        }
        if (_head == _tail) {
            delete _tail;
            _head = nullptr;
            _tail = nullptr;
        }
        else {
            Node<T>* new_tail = _tail->prev;
            new_tail->next = _head;
            _head->prev = new_tail;
            delete _tail;
            _tail = new_tail;
        }
        _size--;
    }

    Node<T>* operator[](int index) {
        if (index < 0 || index >= _size) {
            throw std::out_of_range("In operator[]: index is out of range");
        }

        Node<T>* head = _head;
        for (int i = 0; i < index; i++) {
            head = head->next;
        }

        return head;
    }

    Node<T>* operator[](int index) const {
        if (index < 0 || index >= _size) {
            throw std::out_of_range("In operator[]: index is out of range");
        }

        Node<T>* head = _head;
        for (int i = 0; i < index; i++) {
            head = head->next;
        }

        return head;
    }


    T& operator[](size_t index){
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = _head;

        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = _head;

        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }

    void reverse() {
        if (_head == nullptr || _head == _tail) {
            return; // Nothing to reverse
        }

        Node<T>* current = _head;

        do {
            Node<T>* temp_next = current->next;

            current->next = current->prev;
            current->prev = temp_next;

            current = temp_next;
        } while (current != _head);

        // Swap head and tail
        Node<T>* temp_head = _head;
        _head = _tail;
        _tail = temp_head;
    }

    void clear() {
        while (!empty()) {
            pop_head();
        }
    }

    ~CyclicList() {
        clear();
    }

    bool empty() const {
        return (_size == 0);
    }

};
#endif