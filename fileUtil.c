//
//  fileUtil.c
//  
//
//  Created by Josh Chavez on 10/28/14.
//
//

#include "fileUtil.h"

char * readFileName() {
    printf("Please enter an input file name. ");
    char temp[100];
    char * fp = temp;
    fgets(fp,100,stdin);
    strip(fp);

    char * fn = NULL;
    
    fn = (char *)calloc(strlen(fp) + 1, sizeof(char));
    strcpy(fn, fp);
    
    return fn;
    
}
FILE * openFile() {
    char * fileName = readFileName();
    
    FILE * fin = fopen(fileName, "r");
    while(fin == NULL) {
        char temp[100];
        printf("cleaning input buffer you many need to press enter\n");
        while(fgetc(stdin)!= '\n');
        
        printf("file did not open\n");
        printf("please enter the name of an input file ");
        fgets(temp, 100, stdin);
        strip(temp);
        fin = fopen(temp, "r");
        
        
    }
    
    
    free(fileName);
    fileName = NULL;
    return fin;
    
}











