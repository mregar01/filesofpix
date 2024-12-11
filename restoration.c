/*
 *     restoration.c
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01), 
 *     1/30/22
 *     
 *     filesofpix
 *
 *     The restoration file is the most important part of the filesofpix 
 *     assignment. This file either takes in a file from standard input or
 *     takes in the name of a file from the command line. After processing the
 *     lines of the file using readaline.c this file finds the infusion
 *     sequences of each line. If the infusion sequences are repeated across 
 *     the text file, the address of the line is saved in a list. Then, each
 *     line stored in the list has its non digit characters stripped and then
 *     printed out according to the spec. Finally all allocated memory is freed
 *     
 */

#include <stdlib.h>
#include <stdio.h>
#include <readaline.h>
#include <except.h>
#include <atom.h>
#include <list.h>
#include <table.h>
#include <assert.h>
#include <stdbool.h>

/*****************************************************************
 *                  Data formatting functions
 *****************************************************************/

/**********infusionSequence********
 *
 * This function finds the infusion sequence of an array
 * Inputs:
 *              char *str: array of characters of a line
 *              size_t arrayLength
 * Return: a Hanson Atom of the infusion sequence
 * Expects
 *      str to be nonnull and array length > 0
 * Notes:
 *              Loops through array and checks if each character is an integer
                or not, if it is not, adds to infusion sequence array

                Not checking array length as it is nonzero if str is nonnull.
 ************************/
const char *infusionSequence(char *str, size_t arrayLength)
{
        char *tempArray = malloc(arrayLength);
        size_t pos = 0;
        for (size_t i = 0; i < arrayLength; i++) {
                /*Decimal values less than 48 and greater than 57 are non-digit
                values that will be recorded in the infusion sequence*/
                if (str[i] < 48 || str[i] > 57) {
                        tempArray[pos] = str[i];
                        pos++;
                }
        }
        
        /*The infusion sequence is going to be stored in an Atom, and thus
          must be nonnull.*/
        assert(tempArray != NULL);
        const char *toReturn = Atom_new(tempArray, pos);
        free(tempArray);
        return toReturn;
}

/**********lineCleaner********
 *
 * Takes in a real line of the pgm and adds all numbers to *datapp and ignores
 * all non integer characters. Also gets the number of integers in the array
 * which will be the width of the pgm
 * Inputs:
 *              char *currline: line that needs to be cleaned
 *              int **datapp: double pointer to store address of int array 
 *              storing the characters of a line
 * Return: the width of the pgm
 * Expects
 *      currLine is a real line
 * Notes:
 *              iterates through every character in the line. If it is an
                integer, if the next one or two characters are integers 
                essentialy concatenates them by multiplying by 10 and
                then adding. This is so integers stored continuously are 
                represented as one integer, instead of two or three. (i.e 123 
                instead of 1, 2, 3)
 ************************/
int lineCleaner(char *currLine, int **datapp, size_t arraySize)
{
        int width = 0;
        int i = 0;
        int currNum = 0;
        bool notAdded = false;
        int *lineCharstoInt;
        lineCharstoInt = malloc(2 *arraySize);
        /*Loop iterates through a character array storing a line upto the
          point of a newline*/
        while (currLine[i] != '\n') {
                /*Essentially the opposite of infusion sequence, where we only
                  add digit characters, instead of nondigit characters*/
                if (currLine[i] > 47 && currLine[i] < 58) {
                        currNum *= 10;
                        currNum += currLine[i] - '0';
                        notAdded = true;
                        if (currLine[i + 1] == '\n') {
                                lineCharstoInt[width] = currNum;
                                width++;
                        }
                }
                else {
                        /*As soon as the character is a non digit, the current
                          number is added to the array. For consecutive
                          nondigit characters, the notAdded bool helps indicate
                          if the current number is already added to the array*/
                        if (notAdded) {
                                lineCharstoInt[width] = currNum;
                                notAdded = false;
                                width++;
                        }
                        currNum = 0;
                }
                i++;
        }
        *datapp = lineCharstoInt;
        return width;
}

/*****************************************************************
 *                  Other helper functions
 *****************************************************************/

/*This helper function is used to free the character arrays pointed to in
the Hanson List that stores it*/

/**********freeList********
 *
 * frees all elements in a list, then frees the list itself
 * Inputs:
 *              List_T currList: list to free
 * Return: void
 * Expects
 *      A list storing character arrays
 * Notes:
 *              pops off all elements of the list which are malloced arrays,
                frees the arrays, then frees the list
 ************************/
void freeList(List_T currList)
{
        assert(currList != NULL);
        while (List_length(currList) != 0) {
                void *currLine = NULL;

                currList = List_pop(currList, &currLine);
                free(currLine);
        }
        List_free(&currList);
}

/**********printPGM********
 *
 * Given a list of char arrays and an arraySize prints out all parts of the PGM
 * Inputs:
 *              List_T currList: list of all real lines in file
 *              size_t arraySize: length of longest array in list
 * Return: none
 * Expects
 *      none
 * Notes:
 *              Calls upon lineCleaner to clean up all lines and then
 *              prints each element of cleaned integer array as ASCII
 *              characters
 ************************/
void printPGM(List_T currList, size_t arraySize)
{

        assert(currList != NULL);
        void *currLine = NULL;
        int cleanLine[arraySize];
        int *toPrint = cleanLine;

        int height = List_length(currList);

        /*Cleans the first line before printing all the lines in order to
          obtain the width for the header*/
        currList = List_pop(currList, &currLine);
        char *temp = currLine;
        int width;
        width = lineCleaner(temp, &toPrint, arraySize);

        /*Prints header and first line*/
        printf("P5 %d %d 255\n", width, height);
        for (int i = 0; i < width; i++) {
                printf("%c", toPrint[i]);
        }
        free(toPrint);
        /*Loops using same logic as for first line to print the raw pgm rows*/
        while (List_length(currList) != 0) {
                currList = List_pop(currList, &currLine);
                temp = currLine;
                width = lineCleaner(temp, &toPrint, arraySize);
                for (int i = 0; i < width; i++) {
                        printf("%c", toPrint[i]);
                }
                free(toPrint);
        }
}
/*****************************************************************
 *                  Main function
 *****************************************************************/
int main(int argc, char *argv[])
{
        
        FILE *fp = NULL;
        Except_T wrong = {"Too many arguments\n"};
        Except_T cantopen = {"Can't open file\n"};
        
        if (argc == 1) {
                fp = stdin;
        }
        else if (argc == 2) {
                fp = fopen(argv[1], "r");
        }
        else {
                RAISE(wrong);
        }
        if (fp == NULL) {
                RAISE(cantopen);
        }
        char c = 'x';
        char *currLine = &c;

        List_T allLines = List_list(NULL);

        
        /*The hint for the table is 1000 because on average, most of the 
          corrupted files provided are less than 1000 lines long*/
        Table_T infusionTable = Table_new(1000, NULL, NULL);

        List_T realLines = List_list(NULL);

        size_t arraySize = -1;
        size_t realArraySize;
        char *currRealLine;

        /*Loop to iterate through all the lines read, in order to find their
          corresponding infusion sequences, as well as to identify real lines*/
        while (arraySize != 0) {
                arraySize = readaline(fp, &currLine);

                const char *infusion = infusionSequence(currLine, arraySize);
                allLines = List_push(allLines, currLine);

                if (arraySize != 0) {

                        char *check = Table_get(infusionTable,
                                                infusion);
                        
                        /*Real lines contain the same infusion sequence, so if
                          a table entry is found for the infusion sequence, 
                          we know that the current line is real*/
                        if (check != NULL) {
                                realLines = List_push(realLines, check);
                                realArraySize = arraySize;
                                currRealLine = currLine;
                        }
                        Table_put(infusionTable, infusion, currLine);
                }
        }
        /*Pushing the last real line into the real lines list and reversing it
          because Hanson lists push to the front, i.e., the first row is at the
          end of the list before reversal*/
        realLines = List_push(realLines, currRealLine);
        realLines = List_reverse(realLines);

        /*After all the real lines have been found, the printPGM helper 
          function is called to format and print the raw PGM*/
        printPGM(realLines, realArraySize);

        
        /*Calling the helper function to free the arrays stored in the list
          used and using the Hanson table free method to free the table*/
        freeList(allLines);
        Table_free(&infusionTable);
        
        fclose(fp);

        exit(EXIT_SUCCESS);
}
