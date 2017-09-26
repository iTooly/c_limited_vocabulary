//
//  main.c
//  Limited Vocabulary
//
//  Created by Gabriel Kapach on 16/09/2017.
//  Copyright © 2017 Gabriel Kapach. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "list.h"

#define WORD_MAX_SIZE 16

#define WRONG_ARGUMENTS                 (-1);
#define CANNOT_OPEN_INPUT_FILE          (-2);
#define CANNOT_OPEN_VOCABULARY_FILE     (-3);
#define CANNOT_OPEN_TEMP_FILE           (-4);
#define CANNOT_CLOSE_INPUT_FILE         (-5);
#define CANNOT_CLOSE_VOCABULARY_FILE    (-6);
#define CANNOT_CLOSE_TEMP_FILE          (-7);
#define WORD_TO_BIG                     (-8);

list* find_synonyms(FILE* file, char word[]);
char* get_synonym(FILE* file, char word[]);

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    int i = 0;
    char c = 0, word[WORD_MAX_SIZE] = {0};

    FILE * fp_input = NULL;
    FILE * fp_vocabulary = NULL;
    FILE * fp_temp = NULL;
    
    if (argc != 3) {
        printf("Input and output file path are required\n");
        return WRONG_ARGUMENTS;
    }
    
    fp_input = fopen(argv[1], "r");
    if (fp_input == NULL) {
        printf("Couldn't open file at path \"%s\"\n", argv[1]);
        return CANNOT_OPEN_INPUT_FILE
    }
    
    fp_vocabulary = fopen(argv[2], "r");
    if (fp_vocabulary == NULL) {
        printf("Couldn't open file at path \"%s\"\n", argv[2]);
        return CANNOT_OPEN_VOCABULARY_FILE
    }
    
    fp_temp = fopen("temp.txt", "w");
    if (fp_temp == NULL) {
        printf("Couldn't create a new file\n");
        return CANNOT_OPEN_TEMP_FILE
    }
    
    
    while (EOF != (c = fgetc(fp_input))) {
        if (c == ' ' || c == '\n') {
            word[i] = '\0';
            //Get synonym for the word that was found
            memcpy(word, get_synonym(fp_vocabulary, word), WORD_MAX_SIZE);
            
            //Write the word into the temp file
            int k = 0;
            while (word[k] != '\0') {
                fputc(word[k++], fp_temp);
            }
            
            fputc(c, fp_temp);
            i = 0;
        } else {
            if (i >= WORD_MAX_SIZE-1) {
                printf("Out of memory\n");
                return WORD_TO_BIG;
            }
            
            word[i++] = c;
        }
    }
    
    if (0 != fclose(fp_input)) {
        printf("Couldn't close file \"%s\"", argv[1]);
        return CANNOT_CLOSE_TEMP_FILE;
    }
    
    if (0 != fclose(fp_temp)) {
        printf("Couldn't close file \"temp.txt\"\n");
        return CANNOT_CLOSE_TEMP_FILE;
    }
    
    fp_input = fopen(argv[1], "w");
    if (fp_input == NULL) {
        printf("Couldn't open file at path \"%s\"\n", argv[1]);
        return CANNOT_OPEN_INPUT_FILE
    }
    
    fp_temp = fopen("temp.txt", "r");
    if (fp_temp == NULL) {
        printf("Couldn't read the temporary file \"temp.txt\"\n");
        return CANNOT_OPEN_TEMP_FILE
    }
    
    //Rewrite the input file from the temp file
    while (EOF != (c = fgetc(fp_temp))) {
        fputc(c, fp_input);
    }
    
    if (0 != fclose(fp_temp)) {
        printf("Couldn't close file \"temp.txt\"\n");
        return CANNOT_CLOSE_TEMP_FILE;
    }
    
    if (0 != remove("temp.txt")) {
        printf("Couldn't remove the temporary file \"temp.txt\"\n");
        return CANNOT_CLOSE_TEMP_FILE;
    }
    
    return 0;
}

/*
 * Function: find_synonyms
 * -----------------------
 * Returns list of synonyms found in the vocabulary file
 *
 * file: file pointer of the vocabulary file
 * word: the word we are serching synonyms for
 *
 */

list* find_synonyms(FILE* file, char word[]) {
    int line = 0, i = 0, count = 0;
    char c = 0, synonym[WORD_MAX_SIZE] = {0};
    
    //Jump to the beginning of the vocabulary file
    fseek(file, 0, SEEK_SET);
    //Go over the entire file
    while (EOF != (c = fgetc(file))) {
        //Check if reached the end of the synonym
        if (c == ',' || c == '\n') {
            synonym[i] = '\0';
            
            //Check if the synonym equal to the original word
            if (strcmp(word, synonym) == 0) {
                //Jump to the beginning of the row with the synonyms for the original word
                fseek(file, line, SEEK_SET);
                
                list* synonyms = list_init_empty();
                
                //Go over the row and separate all synonyms
                i = 0;
                while (EOF != (c = fgetc(file))) {
                    if (c == ',' || c == '\n') {
                        synonym[i] = '\0';
                        
                        //Append the new synonym into the list
                        if (synonyms->count == 0) {
                            synonyms->head = create_node(synonym);
                            synonyms->count = 1;
                            synonyms->tail = synonyms->head;
                        } else {
                            append_node(synonyms, synonym);
                        }
                        
                        //Check if reached the end of the line
                        if (c == '\n') {
                            break;
                        }
                        
                        i = 0;
                        continue;
                    }
                    
                    if (i >= WORD_MAX_SIZE-1) {
                        printf("Out of memory\n");
                        int status = WORD_TO_BIG;
                        exit(status);
                    }
                    
                    synonym[i++] = c;
                }
                
                return synonyms;
            }
            
            i = 0;
            
            //Keep which line we are working on
            if (c == '\n') {
                line = count+1;
            }
        } else {
            if (i >= WORD_MAX_SIZE-1) {
                printf("Out of memory\n");
                int status = WORD_TO_BIG;
                exit(status);
            }
            
            synonym[i++] = c;
        }
        
        count++;
    }
    
    return NULL;
}

/*
 * Function: get_synonym
 * ---------------------
 * Returns 1 random synonym from the list
 *
 * file: file pointer of the vocabulary file
 * word: the word we are serching synonyms for
 *
 */

char* get_synonym(FILE* file, char word[]) {
    list* synonyms = find_synonyms(file, word);
    
    if (synonyms != NULL) {
        node* synonym = synonyms->head;
        
        //Get random index for the synonym
        int i, index = rand() % synonyms->count;
        for (i = 0; i < index; i++) {
            synonym = synonym->next;
        }

        return synonym->value;
    }

    //If no synonyms return the original word
    return word;
}
