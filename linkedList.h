//
//  linkedList.h
//  
//
//  Created by Josh Chavez on 11/9/14.
//  Updated 4/7/16 for hw1
//
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "words.h"


struct node { //node structure
    Word * data;
    struct node * next;
};
typedef struct node Node;

struct linkedList { //linkedlist structure
    Node * head;
    int size;
};

typedef struct linkedList LinkedList;
int occurSort(void * one, void * two);
int wordSort(void * one, void * two);
void MergeSort(struct node** headRef, int(*f)(void * one, void * two));
struct node* SortedMerge(struct node* a, struct node* b, int(*f)(void * one, void * two));
void FrontBackSplit(struct node* source,struct node** frontRef, struct node** backRef);
LinkedList * linkedList();
void fixList(LinkedList * myList);
void addFirst(void * s, LinkedList * myList);
void addInOrder(Word * s, LinkedList * myList);
void saveList(void(*f)(Node * cur, FILE * fout), LinkedList * myList, FILE * fout);
void printList(void(*f)(Node * cur), LinkedList * myList);
void cleanUp(void(*f)(Node * n), LinkedList *myList);


#endif /* defined(____linkedList__) */
