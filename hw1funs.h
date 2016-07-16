//
//  hw1funs.h
//  
//
//  Created by Josh Chavez on 4/7/16.
//
//

#ifndef hw1funs_h
#define hw1funs_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "linkedList.h"
#include "words.h"
#include "fileUtil.h"

void getWords(FILE * fin, LinkedList * myList);
void convertWord(char * str);
void strip(char * str);
void cleanWord(Node * cur);
void wordPrint(Node * cur);
void sort(int(*f)(void * one, void * two), LinkedList * myList);
void wordSave(Node * cur, FILE * fout);
int partitionOccur( int * a, char ** b, int l, int r);
void quickSortOccur( int * a, char ** b, int l, int r);
void quick_sort (char ** array, int * occur, int left, int right);
void swap (char ** array, int * occur, int a, int b);
void cudaWordPrint(char * word, int occur, FILE * fout);
void cudaSaveList(char ** words, int * occurs, int size, FILE * fout);
#endif /* hw1funs_h */
