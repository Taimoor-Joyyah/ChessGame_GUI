//
// Created by J-Bros on 12/17/2022.
//

#ifndef CHESSGAME_LINKEDLIST_H
#define CHESSGAME_LINKEDLIST_H

template<typename T>
class LinkedList {
public:
    void insert(T value);
    void remove(T value);
    bool contains(T value) const;
    bool isEmpty() const;
    T removeFirst();
    T get(int index) const;
    int size() const;

private:
    struct Node;

    Node *head;
    Node *tail;
    int count;
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

    while (current->value != value) {
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
    delete current;
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
        if (current->value == value)
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

#endif //CHESSGAME_LINKEDLIST_H
