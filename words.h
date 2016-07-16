//
//  words.h
//  
//
//  Created by Josh Chavez on 4/7/16.
//
//

#ifndef words_h
#define words_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word {
    char * word;
    int occurences;
};

typedef struct word Word;

Word * buildWord(char * word);

#endif /* words_h */
