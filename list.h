#pragma once
#include <string>
class List
{
private:
    typedef struct LinkedList {
        int data;
        LinkedList* next;
    }* node;

    int length;
    node headNode;
    node currentNode;
    node tempNode;

public:
    List();
    ~List();
    void pushNode(int value); //them Node vao dau day
    void appendNode(int value); //them Node vao cuoi day
    void addNode(int value, int position); //them Node o vi tri position
    void deleteFirstNode();
    void deleteNode(int position); //xoa Node o vi tri position
    int getNode(int position); //lay data o vi tri position
    int searchNode(int value); //tim gia tri data trong list
    int getLength(); //kiem tra do dai cua List
};
