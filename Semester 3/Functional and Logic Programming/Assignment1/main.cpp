#include <iostream>
#include "D:\Uni documents\Sem3\Functional and logic programming\R1\List.h"
#include <crtdbg.h>

/* R1
6. a. Add an element at the end of a list.
   b. Concatenate two lists.
 */



SLLNode* create_node(TElem e){
    SLLNode* new_node = new SLLNode;
    new_node->elem = e;
    new_node->next = NULL;

    return new_node;
}

//void addElem(SLLNode* node, TElem e){
//    if(node->next == NULL){
//        SLLNode* new_node = create_node(e);
//        node->next = new_node;
//    }
//    else{
//       return addElem(node->next, e);
//    }
//}

/*
 * Mathematical model for
 * a. Add an element at the end of a list
 *
 * addElem(l1l2...ln, e) = [e], n=0
 *                          l1 + addElem(l2l3...ln, e), otherwise
 *
 * addElemMain(l1l2...ln) = addElem(l1l2...ln, e)
 */

SLLNode* addElemv2(SLLNode* node, TElem  e){
    if(node == NULL){
        SLLNode* new_node = create_node(e);
        return new_node;
    }
    else{
        node->next = addElemv2(node->next,e);
        return node;
    }
}

SLLNode* addElemMain (List l){
    TElem e;
    std::cout<<"Element to be added: ";
    std::cin>>e;

    return addElemv2(l.getHead(), e);
    //return l.head;
}

void r1a(){
    List l;
    l.Print();
    auto list = addElemMain(l);
    //std::cout<<list->elem<<std::endl;
    std::cout<<"List is: ";
    l.setHead(list);
    l.Print();
//    while(list){
//        std::cout<<list->elem<<" ";
//        list = list->next;
//    }
    l.destroy_rec(l.getHead());
}

/*
 * Mathematical model for
 * b. Concatenate two lists
 *
 * concatenate(l1l2...ln,s1s2...sm) = empty list, m = 0
 *                                    s1 + concatenate(l1l2...ln, s2s3...sm), n = 0
 *                                    l1 + concatenate(l2l3...ln, s1s2...sm), otherwise
 */

void concatenate(SLLNode*& l, SLLNode* s){
    if(l  == NULL) //if the first list is empty
        l = s;
    else
    if(l->next == NULL)
        l->next = s;
    else
        concatenate(l->next, s);
}

void r1b(){
    std::cout<<"First list >>";
    List l1;
    std::cout<<"\nSecond list >>";
    List l2;
    concatenate(l1.head, l2.head);
    l1.Print();

}

int main() {
  bool over = false;

  while(not over){
      int choice;
      std::cout<<"0. Exit.\n"
                 "1. a. Add an element at the end of a list.\n"
                 "2. b. Concatenate two lists.\n";
      std::cin>>choice;

      if(choice == 1)
          r1a();
      else if(choice == 2)
          r1b();
      else if(choice == 0)
          over = true;
      else
          std::cout<<"Invalid choice. Please try again!\n";
  }

  std::cout<<"Goodbye!";

//    int r = _CrtDumpMemoryLeaks();
//
//    printf("\n Memory leaks: %d",r);
    return 0;
}
