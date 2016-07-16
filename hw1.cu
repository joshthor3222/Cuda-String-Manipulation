//
//  hw1.c
//
//
//  Created by Josh Chavez on 4/7/16.
//
//

#include <math.h>
#include <stdio.h>

extern "C" {
#include "hw1funs.h"
#include "timing.h"
}


__global__ void kernel(char *words, char *finalwords, unsigned long long int *ret, int *occurences, int numWords,
                       int wordSize, int loopNum) {

    extern __shared__ char s[];
    __shared__ int hash;
    __shared__ int occur;

    int ix = threadIdx.x;
    int idx = blockIdx.x*wordSize + threadIdx.x + (blockIdx.y * 65535 * wordSize); //CORRECT IDX!!


    if (idx < (numWords * wordSize)) {
        char a = words[idx];
        if(a >= 'a' && a <= 'z') {
            s[ix] = a;
        }
        else if(a >= 'A' && a <= 'Z') {
            a = a + 32;
            s[ix] = a;
        }
        else
            s[ix] = ' ';

        __syncthreads();


        if(ix > 0) {
            if((s[ix] == ' ' || s[ix - 1] == ' ') && ix < wordSize) {
                s[ix] = ' ';
                s[ix + 1] = ' ';
            }
        }

        //below this works!!
        if (threadIdx.x == 0) { //this thread adjusts the shared memory
            occur = 0;
            unsigned long long int res = 1;
            for (int i = 0; (s[i] != '\0' && s[i] != ' '); i++) {
                res = res * 31 + s[i];
            }
            hash = res % numWords;

            unsigned long long int old = atomicCAS(&ret[hash], 0, res); //global - slow, 1 call.
            while(old != 0) {
                if(old == res)
                    break;
                else {
                    if(hash < numWords)
                        hash++;
                    else
                        hash = 0;
                    old = atomicCAS(&ret[hash], 0, res);
                }
            }
            occur = atomicAdd(&occurences[hash], 1);
        }
        __syncthreads();
        if(occur == 0) {
            finalwords[(hash * wordSize) + ix] = s[ix];
        }
    }
}

//this will get the words in a array to be used by a cuda kernel.
void getCount(FILE *fin, int *words, int *wordLength) {
    char temp[100];
    int i = 0;
    while (fscanf(fin, "%s", temp) == 1) {
        if (isalpha(temp[0])) {
            i++;
            int m = strlen(temp);
            if (m > *wordLength) {
                *wordLength = m;
            }
        }
    }
    *words = i;
    rewind(fin);
}

void getWordsStart(FILE *fin, char **wordList) {
    char temp[100];
    int i = 0;
    while (fscanf(fin, "%s", temp) == 1) {
        if (isalpha(temp[0])) {
            int m = 0;
            while (isalpha(temp[m])) {
                temp[m] = tolower(temp[m]);
                m++;
            }
            temp[m] = '\0';
            wordList[i] = (char *) malloc((strlen(temp) + 1) * sizeof(char));
            strcpy(wordList[i], temp);
            i++;
        }
    }
}

void sequential(char **words, char **finalwords, unsigned long long int *ret, int *occurences, int numWords,
                int wordSize) {
    for (int j = 0; j < numWords; j++) {
        char newWord[wordSize];
        strcpy(newWord, words[j]);
        unsigned long long int res = 1; // Initialize result
        for (int i = 0; (newWord[i] != '\0' && newWord[i] != ' '); i++) {
            res = res * 31 + newWord[i];
        }


        //res is full value we are saving.

        int hash = res % numWords;
        while (ret[hash] != 0) {// && ret[hash] != res) {
            if (ret[hash] == res) {
                if (strcmp(finalwords[hash], newWord) == 0) {
                    occurences[hash] = occurences[hash] + 1;
                    break;
                }
            }
            if (hash < numWords)
                hash++;
            else
                hash = 0;
        }
        if (ret[hash] == 0) {
            ret[hash] = res;
            occurences[hash] = occurences[hash] + 1;
            finalwords[hash] = words[j];
        }
    }
}

int main() {
    cudaSetDevice(0);

    double time, then, timea, timeb;

    //Sequential time, currently commented out.

    FILE *fin = NULL;
    FILE *fout;
    char **wordList;
    char **seqFinalList;


    int *wordsA, *wordLengthA;
    int aa = 0, ba = 0;
    wordsA = &aa;
    wordLengthA = &ba;

    fin = openFile();

    time = currentTime();

    getCount(fin, wordsA, wordLengthA); //get words


    wordList = (char **) malloc(*wordsA * sizeof(char *));
    getWordsStart(fin, wordList);

    seqFinalList = (char **) malloc(*wordsA * sizeof(char *));

    unsigned long long int *hashVals = (unsigned long long int *) malloc(*wordsA * sizeof(unsigned long long int));
    int *occurVals = (int *) malloc(*wordsA * sizeof(int));

    sequential(wordList, seqFinalList, hashVals, occurVals, *wordsA, *wordLengthA);
    //old wordlist goes into new seqfinallist so there is only one occurence of each word
    //hashvals is temporary for indexing, occurvals has number of occurences

    free(hashVals); //no longer needed, lose it.

    int newNum = 0;
    for (int z = 0; z < *wordsA; z++) {
        if (occurVals[z] != 0) {
            newNum++;
        }
    }
    //gets new size for everything

    int *seqOccurences = (int *) malloc(newNum * sizeof(int));
    char **seqWords = (char **) malloc(newNum * sizeof(char *));
    int next = 0;
    for (int z = 0; z < *wordsA; z++) {
        if (occurVals[z] != 0) {
            seqOccurences[next] = occurVals[z];
            seqWords[next] = seqFinalList[z];
            next++;
        }
    }
    free(occurVals);
    //puts occurences in new minimal array

    quick_sort(seqWords, seqOccurences, 0, newNum - 1); //works!


    fout = fopen("alphaOutput.txt", "w");
    cudaSaveList(seqWords, seqOccurences, newNum, fout);
    fclose(fout);
    printf("Alphabetical order saved in alphaOutput.txt\n");

    quickSortOccur(seqOccurences, seqWords, 0, newNum - 1); //works!

    FILE *fout1;
    fout1 = fopen("occurOutput.txt", "w");
    cudaSaveList(seqWords, seqOccurences, newNum, fout1);
    fclose(fout1);
    printf("Order of occurences saved in occurOutput.txt\n");


    for (int z = 0; z < *wordsA; z++) {
        free(wordList[z]); //gets rid of all words
    }
    free(wordList);
    free(seqOccurences);
    free(seqWords);
    free(seqFinalList);

    then = currentTime();

    printf("Sequential time spent: %lf seconds\n", then - time);
    //*/
    //Parallel execution







    timea = currentTime();
    rewind(fin);
    int *words;
    int a = 0;
    words = &a;
    int *wordLength;
    int b = 0;
    wordLength = &b;

    getCount(fin, words, wordLength); //gets total amount of words, and max word length;
    *wordLength = *wordLength + 1;

    int pitchSize = (*words * *wordLength);
    //do chunks
    char *list = (char *) malloc(pitchSize * sizeof(char));


    printf("\namount of words: %d; max length of a word: %d\n\n", *words, *wordLength);


    char temp[*wordLength];
    int i = 0;
    while (i < pitchSize) {
        if (fscanf(fin, "%s", temp) == 1) {
            if (isalpha(temp[0])) {
                for (int z = 0; z < *wordLength; z++) {
                    if (temp[z] != '\0') {
                        list[i] = temp[z];
                    }
                    else {
                        list[i] = ' ';
                    }
                    i++;

                }
            }
        }
    }
    char *dev_words;
    int *ret;
    unsigned long long *numbers;
    char *finalList = (char *) malloc(pitchSize * sizeof(char));
    char *final_words;

    cudaMalloc((void **) &dev_words, pitchSize * sizeof(char));
    cudaMemcpy(dev_words, list, pitchSize * sizeof(char), cudaMemcpyHostToDevice);
    cudaMalloc((void **) &ret, (*words) * sizeof(int));
    cudaMemset(ret, 0, *words * sizeof(int));
    cudaMalloc((void **) &numbers, (*words) * sizeof(unsigned long long int));
    cudaMemset(numbers, 0, *words * sizeof(unsigned long long int));
    cudaMalloc((void **) &final_words, pitchSize * sizeof(char));
    cudaMemset(final_words, ' ', pitchSize * sizeof(char));

    free(list);

    int dimx;
    int dimy;
    if (pitchSize < 65535) {
        dimx = 1;
        dimy = *words; //wordsize
    }
    else {
        dimx = ceil(*words /(float)65535);
        dimy = 65535;
    }

    dim3 grid;
    grid.x = dimy;
    grid.y = dimx;

    kernel <<< grid, *wordLength, (*wordLength) * sizeof(char) >>>(dev_words, final_words, numbers, ret, *words, *wordLength, i);


    int * newArray = (int *)malloc(*words * sizeof(int));
    //int newArray[*words];
    cudaFree(dev_words);
    cudaMemcpy(newArray, ret, (*words) * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(ret);
    cudaFree(numbers);
    cudaMemcpy(finalList, final_words, pitchSize * sizeof(char), cudaMemcpyDeviceToHost);
    cudaFree(final_words);




    int k = 0;
    for(i = 0; i < *words; i++) {
        if(newArray[i] != 0) {
            k++;
        }
    }

    //build arrays
    int *finalOccur = (int *) malloc(k * sizeof(int));

    char **finalWords;
    finalWords = (char **) malloc(k * sizeof(char *));

    k = 0;
    for(i = 0; i < *words; i++) {
        if(newArray[i] != 0) {
            finalOccur[k] = newArray[i];

            memset(temp, 0, sizeof(temp));
            for(int j = 0; j < *wordLength; j++) {
                if(finalList[i*(*wordLength) + j] == ' ')
                    break;
                temp[j] = finalList[i*(*wordLength) + j];
            }

            finalWords[k] = (char *) malloc((strlen(temp) + 1) * sizeof(char));
            strcpy(finalWords[k], temp);

            k++;
        }
    }
    free(finalList);



    quick_sort(finalWords, finalOccur, 0, k - 1); //works!

    FILE *fout4;
    fout4 = fopen("cudaAlphaOutput.txt", "w");
    cudaSaveList(finalWords, finalOccur, k, fout4);
    fclose(fout4);

    quickSortOccur(finalOccur, finalWords, 0, k - 1); //works!

    FILE *fout3;
    fout3 = fopen("cudaOccurOutput.txt", "w");
    cudaSaveList(finalWords, finalOccur, k, fout3);
    fclose(fout3);



    //free last arrays
    free(finalOccur);
    for (int j = 0; j < k; j++) {
        free(finalWords[j]);
    }
    free(finalWords);

    timeb = currentTime();
    printf("\n\nParallel time spent: %lf seconds\n", timeb - timea);


    return 0;


}