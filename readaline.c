/*
 *     readaline.c
 *     by Prithviraj Singh Shahani (pshaha01) and Max Regardie (mregar01),
 *     1/30/22
 *     
 *     filesofpix
 *
 *     The readaline file contains a  function that takes in an input file
 *     and a reference to char array and returns the number of bytes and stores
 *     the contents of a single line in the char array. This file is used in
 *     the restoration program to read a single line of a corrupted image file
 */

#include <readaline.h>
#include <stdio.h>
#include <stdlib.h>

#include <except.h>
#include <assert.h>


/**********readaline********
 *
 * Checks if a number is under a given limit
 * Inputs:
 *              FILE *inputfd: pointer to the file stream
 *              char **datapp: double pointer to store address of the array 
 *              storing the characters of a line
 * Return: the number of bytes on a given line
 * Expects
 *      inputfd and *datapp to be nonnull
 * Notes:
 *              Uses realloc to dynamically allocate memory to the array 
 *              storing characters, just in case it goes over 1000 characters
 * 
 *              Intended to use a do while loop but it wasn't working, 
 *              so used a break statement in a while loop instead
 ************************/

size_t readaline(FILE *inputfd, char **datapp)
{
        /*Asserts to raise the checked runtime error if input is NULL*/
        assert(inputfd != NULL);
        assert(*datapp != NULL);
        assert(datapp != NULL);

        /*Array size initially set to 1000 since limited implementation
        recommends using size 1000*/
        size_t length = 1000;

        /*Position keeps track of current index of character array and is used
        to return the number of bytes in the line since each char is 1 byte*/
        size_t pos = 0;

        /*A character array is created and stores the characters of a line*/
        char *lineChars;
        lineChars = malloc(length);
        assert(lineChars != NULL);

        /*c is used to retrieve the decimal value of the current character*/
        int c = fgetc(inputfd);

        /*If c is -1, that means that it is reading EOF and in accordance with
        spec, datapp is set to NULL and the returned value is 0*/
        if (c == -1) {
                *datapp = NULL;
                free(lineChars);
                return 0;
        }

        /*Looping through the line and adding the characters to the array
        storing the line one-by-one. It only stops looping
        once it reads a newline*/
        while (c != -1) {

                /*If clause is used to dynamically reallocate memory for the
                array when it reaches capacity. Using pos + 1 because the
                newline character is added to the array as well*/
                if ((pos + 1) == length)
                {
                        length *= 2;
                        lineChars = (char *)realloc(lineChars, length);
                }
                
                lineChars[pos] = c;
                pos++;
                
                /*Intended to use a Do While loop where this was the stop
                condition, however that was causing an infinite loop so a
                break statement is used instead in a regular while loop*/
                if (c == '\n') {
                        break;
                }

                c = fgetc(inputfd);
        }

        /*datapp points to the array that stores the characters of
        the current line and the number of bytes is returned*/
        *datapp = lineChars;
        return pos;
}