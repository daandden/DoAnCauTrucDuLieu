#include "list.h"
#include <iostream>
#include <string>
using namespace std;

List::List()
    : length{0}, headNode{nullptr}, currentNode{nullptr}, tempNode{nullptr }
{
}

List::~List()
{
}

void List::pushNode(int value)
{
    node newNode = new LinkedList;
    newNode->data = value;
    newNode->next = headNode; //chi den Node dau tien
    headNode = newNode; // chuyen Node moi thanh Node dau tien
    /*khong can check headNode
     * vi neu headNode == null thi newNode->next = headNode = null
     * headNode = newNode => headNode->next = null
     */
    length++;
}

void List::appendNode(int value)
{
    node newNode = new LinkedList;
    newNode->next = nullptr;
    newNode->data = value;
    currentNode = headNode;

    if (headNode != nullptr)
        /*kiem tra co Node dau hay chua
         *neu co thi duyet toi Node cuoi roi tro next den newNode
         *neu khong thi Node dau la newNode
         *neu khong kiem tra thi vdu headNode = null
         *,check currentNode->next = headNode->next = null->next k co => loi
         */
    {
        while (currentNode->next != nullptr)
        {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
    else
    {
        headNode = newNode;
    }
    length++;
}

void List::addNode(int value, int position)
{
    if (position < 1 || position > length + 1)
    {
        cout << "invalid position" << endl;
    }
    else if (position == 1)
    {
        pushNode(value);
    }
    else if (position == length + 1)
    {
        appendNode(value);
    }
    else
    {
        node newNode = new LinkedList;
        newNode->data = value;
        currentNode = headNode;
        int i = 1;
        /*Tim den Node dung truoc vi tri position roi them newNode sau Node do
         *->position -1
         */
        while (i != position - 1)
        {
            currentNode = currentNode->next;
            i++;
        }
        tempNode = currentNode->next;
        currentNode->next = newNode;
        newNode->next = tempNode;
        length++;
    }
}

void List::deleteFirstNode()
{
    currentNode = headNode;
    headNode = headNode->next;
    delete currentNode;
    length--;
}

void List::deleteNode(int position)
{
    if (position < 1 || position > length)
    {
        cout << "Invalid position" << endl;
    }
    else
    {
        if (headNode != nullptr) //check co Node nao chua
        {
            if (position == 1)// xoa Node dau tien
            {
                deleteFirstNode();
            }
            else
            {
                currentNode = headNode;
                int i = 1;
                while (i != position - 1) //Tim den Node dung truoc Node can xoa
                {
                    currentNode = currentNode->next;
                    i++;
                }
                tempNode = currentNode->next; //tempNode la Node can xoa
                currentNode->next = tempNode->next;
                delete tempNode;
                length--;
            }
        }
        else
        {
            cout << "Empty list" << endl;
        }
    }
}

int List::getNode(int position)
{
    if (position < 1 || position > length)
    {
        return -1;
    }
    else
    {
        int i = 1;
        currentNode = headNode;
        while (i != position)
        {
            currentNode = currentNode->next;
            i++;
        }
        return currentNode->data;
    }
}

int List::searchNode(int value)
{
    int position = 1;
    for (currentNode = headNode; currentNode != nullptr; currentNode = currentNode->next)
    {
        if (currentNode->data == value)
        {
            return position;
        }
        position++;
    }
    return -1; //khong tim thay
}

int List::getLength()
{
    return length;
}


