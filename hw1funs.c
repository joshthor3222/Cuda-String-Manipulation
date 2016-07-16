//
//  hw1funs.c
//  
//
//  Created by Josh Chavez on 4/7/16.
//
//

#include "hw1funs.h"

void getWords(FILE * fin, LinkedList * myList) {
    char temp[100];
    while(fscanf(fin, "%s", temp) == 1){
        if(isalpha(temp[0])) {
            int i = 0;
            while(i < strlen(temp)) {
                temp[i] = tolower(temp[i]);
                i++;
            }
            convertWord(temp);
            addInOrder(buildWord(temp), myList);
        }
    }
}
void sortOccurances(LinkedList * myList) {
    int i;
    for(i = 0; i < myList->size; i++) {
        
    }
}
void cleanWord(Node * cur) {
    free(cur->data->word);
    free(cur->data);
    free(cur);
}
void wordSave(Node * cur, FILE * fout) {
    fprintf(fout, "|%-20s|%-9d|\n", cur->data->word, cur->data->occurences);
    fprintf(fout, "|--------------------|---------|\n");
}
void wordPrint(Node * cur) {
    printf("|%-20s|%-9d|\n", cur->data->word, cur->data->occurences);
    printf("|--------------------|---------|\n");
}
void cudaWordPrint(char * word, int occur, FILE * fout) {
    fprintf(fout, "|%-20s|%-9d|\n", word, occur);
    fprintf(fout, "|--------------------|---------|\n");
}
void cudaSaveList(char ** words, int * occurs, int size, FILE * fout) {
    fprintf(fout, "|--------------------|---------|\n");
    fprintf(fout, "|English Word        |Count    |\n");
    fprintf(fout, "|--------------------|---------|\n");

    int i = 0;
    while(i < size) {
        cudaWordPrint(words[i], occurs[i], fout);
        i++;
    }

}
void convertWord(char * str) {
    int x = 0;
    while(str[x] != '\0') {
        if(!isalpha(str[x])) {
            while(str[x] != '\0') {
                str[x] = '\0';
                x++;
            }
        }
        else {
            x++;
        }
    }
}
void strip(char * str) {
    int x = 0;
    while(str[x] != '\0') {
        if(str[x] == '\r' || str[x] == '\n') {
            str[x] = '\0';
        }
        
        x++;
    }
}
void sort(int(*f)(void * one, void * two), LinkedList * myList) {
    int total; total = myList->size;
    int x, y, i, j;
    Node temp;
    Node *cur, *cur2;
    for(x = 0; x < total; x++){
        cur = myList->head;
        for(i = 0; i < x; i++) {
            cur = cur->next;
        }
        temp = *cur;
        for(y = 0; y < total; y++){
            cur2 = myList->head;
            for(j = 0; j < y; j++) {
                cur2 = cur2->next;
            }
            if(f(cur, cur2) == 1) {
                temp.data = cur2->data;
                cur2->data = cur->data;
                cur->data = temp.data;
                
                temp = *cur;
            }
        }
    }
}
/*
int wordSort(void * one, void * two) {
    Node a = *((Node *) one);
    Node b = *((Node *) two);
    if(strcasecmp(a.data->word, b.data->word) < 0) {
        return 1;
    }
    else {
        return 0;
    }
}
int occurSort(void * one, void * two) {
    Node a = *((Node *) one);
    Node b = *((Node *) two);
    if(a.data->occurences > b.data->occurences) {
        return 1;
    }
    else {
        return 0;
    }
}
 */
void fixList(LinkedList * myList) { //corrects occurances after the array has been sorted by words
    Node *a = myList->head;
    Node *b = a->next;
    Node *temp;
    //if a->word == b->word, combine occurances, remove b.
    while(b != NULL) {
        if(strcasecmp(a->data->word, b->data->word) == 0) {
            a->data->occurences = a->data->occurences + b->data->occurences;
            temp = b->next;
            a->next = temp;
            free(b->data->word);
            free(b->data);
            free(b);
            b = a->next;
            myList->size = myList->size - 1;
        
        }
        else {
            a = b;
            b = b->next;
        }
    }
    //if a->word != b->word, incriment a and b.
    
    
}

void quickSortOccur( int * a, char ** b, int l, int r)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = partitionOccur( a, b, l, r);
        quickSortOccur( a, b, l, j-1);
        quickSortOccur( a, b, j+1, r);
    }

}



int partitionOccur( int * a, char ** b, int l, int r) {
    int pivot, i, j, t;
    char * temp;
    pivot = a[l];
    i = l; j = r+1;

    while( 1)
    {
        do ++i; while( a[i] <= pivot && i <= r );
        do --j; while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
        temp = b[i]; b[i] = b[j]; b[j] = temp;
    }
    t = a[l]; a[l] = a[j]; a[j] = t;
    temp = b[l]; b[l] = b[j]; b[j] = temp;
    return j;
}



void swap (char ** array, int * occur, int a, int b)
{
    char * holder;
    int t;

    holder = array[a];
    t = occur[a];
    array[a] = array[b];
    occur[a] = occur[b];
    array[b] = holder;
    occur[b] = t;
}

void quick_sort (char ** array, int * occur, int left, int right)
{
    int pivot;
    int i;
    int j;
    const char * key;

    /* Catch the trivial case. */

    if (right - left == 1) {
        if (strcmp (array[left], array[right]) > 0) {
            swap (array, occur, left, right);
        }
        return;
    }


    pivot = (left + right) / 2;
    key = array[pivot];

    /* Put the pivot key at the left of the list. */

    swap (array, occur, left, pivot);

    i = left + 1;
    j = right;

    while (i < j) {
        while (i <= right && strcmp (array[i], key) < 0) {
            i++;
        }
        while (j >= left && strcmp (array[j], key) > 0) {
            j--;
        }
        if (i < j) {
            swap (array, occur, i, j);
        }
    }
    swap (array, occur, left, j);

    if (left < j - 1) {
        quick_sort (array, occur, left, j - 1);
    }
    if (j + 1 < right) {
        quick_sort (array, occur, j + 1, right);
    }
}
