#include <cstddef>
#include <iostream>
#include "stack.h"
using namespace std;

template <typename T>
Node<T> *Node<T>::getNode()
{
    return this->prev;
}

template <typename T>
T Node<T>::getValue()
{
    return this->value;
}

template <typename T>
Node<T>::Node(T aValue)
{
    value = aValue;
    prev = nullptr;
}
template <typename T>
Node<T>::Node(T aValue, Node *node)
{
    value = aValue;
    prev = node;
}

template <typename T>
Node<T> *Node<T>::getPrev()
{
    return this->prev;
}

template <typename T>
void Node<T>::setPrev(Node *node)
{
    this->prev = node;
}

template <typename T>
Stack<T>::Stack()
{
    this->root = nullptr;
}

template <typename T>
Stack<T>::~Stack()
{
    // pop until its emptied
    while (this->root != nullptr)
    {
        this->pop();
    }
}

template <typename T>
void Stack<T>::push(T push)
{
    Node<T> *nextRoot = new Node<T>(push);
    if (root != nullptr)
    {
        nextRoot->setPrev(root);
    }
    root = nextRoot;
}

template <typename T>
T Stack<T>::pop()
{
    if (root == nullptr)
    {
        throw std::length_error("No data in your in the stack");
    }
    Node<T> *oldRoot = root;
    root = root->getPrev();
    T a = oldRoot->getValue();
    delete oldRoot; // destroy the object
    return a;
}

template <typename T>
T Stack<T>::peek()
{
    if (root == nullptr)
    {
        return (T)NULL;
    }
    return root->getValue();
}

template <typename T>
bool Stack<T>::isEmpty()
{
    return this->root == nullptr;
}
