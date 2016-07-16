//
//  linkedList.c
//  
//
//  Created by Josh Chavez on 11/9/14.
//
//

#include "linkedList.h"

LinkedList * linkedList() {
    return (LinkedList *)calloc(1, sizeof(LinkedList));
}
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
    if(a.data->occurences < b.data->occurences) {
        return 1;
    }
    else {
        return 0;
    }
}
void MergeSort(Node** headRef, int(*f)(void * one, void * two))
{
    Node* head = *headRef;
    Node* a;
    Node* b;
    
    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }
    
    FrontBackSplit(head, &a, &b); //splits head into sublists
    
    MergeSort(&a, f);
    MergeSort(&b, f); //sorts sublists
    
    *headRef = SortedMerge(a, b, f); //merges the two lists
}

struct node* SortedMerge(Node* a, Node* b, int(*f)(void * one, void * two))
{
    Node* result = NULL;
    
    if (a == NULL)
        return(b);
    else if (b==NULL)
        return(a);
    
    /* Pick either a or b, and recur */
    
    //if (a->data->occurences >= b->data->occurences)
    if(f(a,b) == 1)
    {
        result = a;
        result->next = SortedMerge(a->next, b, f);
    }
    else
    {
        result = b;
        result->next = SortedMerge(a, b->next, f);
    }
    return(result);
}

void FrontBackSplit(Node* source,
                    Node** front, Node** back)
{
    Node* fast;
    Node* slow;
    if (source==NULL || source->next==NULL)
    {
        /* length < 2 cases */
        *front = source;
        *back = NULL;
    }
    else
    {
        slow = source;
        fast = source->next;
        
        /* Advance 'fast' two nodes, and advance 'slow' one node */
        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }
        
        /* 'slow' is before the midpoint in the list, so split it in two
         at that point. */
        *front = source;
        *back = slow->next;
        slow->next = NULL;
    }
}








void addInOrder(Word * s, LinkedList * myList) {
    if(myList->size == 0) {
        addFirst(s, myList);
    }
    else if(myList->size == 1) {
        if(strcasecmp(s->word, myList->head->data->word) == 0) {
            myList->head->data->occurences = myList->head->data->occurences + 1;
            free(s->word);
            free(s);
        }
        else if(strcasecmp(s->word, myList->head->data->word) < 0) {
            addFirst(s, myList);
        }
        else {
            Node * nn = (Node *)calloc(1, sizeof(Node));
            nn->data = s;
            nn->next = NULL;
            myList->head->next = nn;
            myList->size = myList->size + 1;
            
        }
    }
    else {
        Node * cur = myList->head;
        if(strcasecmp(s->word, myList->head->data->word) == 0) {
            myList->head->data->occurences = myList->head->data->occurences + 1;
            free(s->word);
            free(s);
        }
        else if(strcasecmp(s->word, myList->head->data->word) < 0) {
            addFirst(s, myList);
        }
        else {
            while(cur->next != NULL) {
                if(strcasecmp(s->word, cur->next->data->word) == 0) {
                    cur->next->data->occurences = cur->next->data->occurences + 1;
                    free(s->word);
                    free(s);
                    break;
                }
                else if(strcasecmp(s->word, cur->next->data->word) < 0) {
                    Node * nn = (Node *)calloc(1, sizeof(Node));
                    nn->data = s;
                    nn->next = cur->next;
                    cur->next = nn;
                    myList->size = myList->size + 1;
                    //add
                    break;
                }
                else {
                    cur = cur->next;
                    if(cur->next == NULL) {
                        Node * nn = (Node *)calloc(1, sizeof(Node));
                        nn->data = s;
                        nn->next = NULL;
                        cur->next = nn;
                        myList->size = myList->size + 1;
                        break;
                    }
                }
            }
        }
    }
    
}
void addFirst(void * s, LinkedList * myList) {
    Node * nn = (Node *)calloc(1, sizeof(Node));
    nn->data = s;
    nn->next = NULL;
    nn->next = myList->head;
    myList->head = nn;
    myList->size = myList->size + 1;
}
void saveList(void(*f)(Node * cur, FILE * fout), LinkedList * myList, FILE * fout) {
    fprintf(fout, "|--------------------|---------|\n");
    fprintf(fout, "|English Word        |Count    |\n");
    fprintf(fout, "|--------------------|---------|\n");
    if(myList->size == 0) {
        printf("The list is empty\n");
    }
    else {
        Node * cur = myList->head;
        while(cur != NULL) {
            f(cur, fout);
            cur = cur->next;
        }
    }
}
void printList(void(*f)(Node * cur), LinkedList * myList) {
    printf("|--------------------|---------|\n");
    printf("|English Word        |Count    |\n");
    printf("|--------------------|---------|\n");
    if(myList->size == 0) {
        printf("The list is empty\n");
    }
    else {
        Node * cur = myList->head;
        while(cur != NULL) {
            f(cur);
            cur = cur->next;
        }
    }
}
void cleanUp(void(*f)(Node *n), LinkedList *myList) {
    Node * cur = myList->head;
    while(cur != NULL) {
        myList->head = cur->next;
        f(cur);
        cur = myList->head;
    }
}



