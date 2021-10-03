//
// Created by amina on 10/3/2021.
//

#include "List.h"

List::List() {
    this->head = this->create_rec();

}

void List::Print() {
    this->Print_Rec(this->head);
    std::cout<<std::endl;
}


SLLNode *List::create_rec() {
    TElem x;
    std::cout<<"x = ";
    std::cin>>x;

    if(x==0)
        return NULL;
    else{
        SLLNode* node = new SLLNode;
        node->elem = x;
        node->next = create_rec();
        return node;
    }
}

void List::Print_Rec(SLLNode* node) {
    if(node!=NULL){
        std::cout<<node->elem<<" ";
        this->Print_Rec(node->next);
    }
}

void List::destroy_rec(SLLNode *node) {
    if(node != NULL){
        this->destroy_rec(node->next);
        delete node;
    }
}

List::~List() {
    //this->destroy_rec(this->head);
}

SLLNode *List::getHead() {
    return this->head;
}

void List::setHead(SLLNode *new_head) {
    this->head = new_head;

}
