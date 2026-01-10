#ifndef STACK_H
#define STACK_H
template <typename T>
class Node
{
private:
    T value;
    Node<T> *prev;
    Node<T> *getNode();

public:
    T getValue();
    Node(T aValue);
    Node(T aValue, Node *node);
    Node *getPrev();
    void setPrev(Node *node);
};

template <typename T>
class Stack
{
private:
    Node<T> *root;

public:
    Stack();
    ~Stack();
    void push(T push);
    T pop();
    T peek();
    bool isEmpty();
};

#include "stack.cpp"

#endif