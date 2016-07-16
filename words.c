//
//  words.c
//  
//
//  Created by Josh Chavez on 4/7/16.
//
//

#include "words.h"

Word * buildWord(char * word) {
    char * c = (char *)calloc(strlen(word) + 1, sizeof(char));
    strcpy(c, word);
    
    Word * s = (Word *)calloc(1, sizeof(Word));
    
    s->word = (char *)calloc(strlen(c) + 1, sizeof(char));
    strcpy(s->word, c);
    
    s->occurences = 1;
    
    free(c);
    return s;
}
