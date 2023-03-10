//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_LINKEDLIST_H
#define CHESSGAME_LINKEDLIST_H

#include <stdexcept>

using namespace std;

template<typename T>
class LinkedList {
public:
    void insert(T value);
    void remove(T value);
    bool contains(T value) const;
    bool isEmpty() const;
    T removeFirst();
    T get(int index) const;
    T iteratorNext();
    void iteratorReset();
    bool isIteratorEnd() const;
    int size() const;
    void merge(LinkedList<T> &list);
    void intersect(LinkedList<T> &list);
    void subtract(LinkedList<T> &list);
    void clear();

private:
    struct Node;

    Node *head;
    Node *tail;
    int count;

    Node *iterator;
};

template<typename T>
struct LinkedList<T>::Node {
    T value;
    Node *next;
};

template<typename T>
void LinkedList<T>::insert(T value) {
    tail = new Node{value, nullptr};

    if (head == nullptr) {
        head = tail;
        count = 1;
        return;
    }

    Node *current = head;

    while (current->next != nullptr)
        current = current->next;
    current->next = tail;
    ++count;
}

template<typename T>
void LinkedList<T>::remove(T value) {
    if (head == nullptr)
        return;

    Node *current = head;
    Node *previous;

    while (*current->value != *value) {
        if (current->next == nullptr)
            return;
        previous = current;
        current = current->next;
    }

    if (current == head)
        head = current->next;
    else {
        if (current == tail)
            tail = previous;
        previous->next = current->next;
    }
    --count;
//    delete current;
}

template<typename T>
T LinkedList<T>::get(int index) const {
    if (index >= count)
        return nullptr;

    Node *current = head;

    while (index != 0) {
        current = current->next;
        --index;
    }
    return current->value;
}

template<typename T>
bool LinkedList<T>::contains(T value) const {
    Node *current = head;
    while(current != nullptr) {
        if (*(current->value) == *value)
            return true;
        current = current->next;
    }
    return false;
}

template<typename T>
int LinkedList<T>::size() const {
    return count;
}

template<typename T>
bool LinkedList<T>::isEmpty() const {
    return count == 0;
}

template<typename T>
T LinkedList<T>::removeFirst() {
    T node = head->value;
    remove(node);
    return node;
}

template<typename T>
T LinkedList<T>::iteratorNext() {
    if (iterator == nullptr)
        throw out_of_range("No more Object");
    Node *node = iterator;
    iterator = iterator->next;
    return node->value;
}

template<typename T>
void LinkedList<T>::iteratorReset() {
    iterator = head;
}

template<typename T>
bool LinkedList<T>::isIteratorEnd() const {
    return iterator == nullptr;
}

template<typename T>
void LinkedList<T>::merge(LinkedList<T> &list) {
    Node *current = list.head;
    while (current != nullptr) {
        if (!contains(current->value))
            insert(current->value);
        current = current->next;
    }
}

template<typename T>
void LinkedList<T>::intersect(LinkedList<T> &list) {
    Node *current = head;
    while (current != nullptr) {
        if (!list.contains(current->value))
            remove(current->value);
        current = current->next;
    }
}

template<typename T>
void LinkedList<T>::subtract(LinkedList<T> &list) {
    Node *current = list.head;
    while (current != nullptr) {
        if (contains(current->value))
            remove(current->value);
        current = current->next;
    }
}

template<typename T>
void LinkedList<T>::clear() {
    while (!isEmpty())
        removeFirst();
}

#endif //CHESSGAME_LINKEDLIST_H
