//
// Created by amina on 10/3/2021.
//

#ifndef R1_LIST_H
#define R1_LIST_H

#include <iostream>

//generical data type
typedef int TElem;

typedef struct Node{
    TElem elem;
    Node* next;
} SLLNode;

class List {

public:

    SLLNode* head;

    List();
    SLLNode* getHead();
    void setHead(SLLNode* new_head);
    void Print();
    void Print_Rec(SLLNode* node);
    SLLNode* create_rec();
    void destroy_rec(SLLNode* node);
    ~List();



};


#endif //R1_LIST_H
