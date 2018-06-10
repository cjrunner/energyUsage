//
//  align.hpp
//  Project: energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/19/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
//  Created 2018-05-18T18:58:27
//
//This function, named stickItInTheCenter:
//  1) takes a string,
//  2) determines its width,
//  3) determines if if will fit into the specified column width
//  3a) IF NOT, stickItInTheCenter, will, like FORTRAN, return a column width full of * characters
//  3b) If the string will fit then stickItInTheCenter will return to the caller with the buffer
//       provided by the caller filled with the input value padded on the left and right with the
//       caller provided fill character.
//
//  Input Parameters:
//  1) lrspaces: a buffer, assumed to be of width 80 characters, provided by the caller;
//  2) fillchar: a single character that will serve as padding to the left and to the right
//     of the value the caller desires to be in the center of the lrspaces buffer;
//  3) colxx: width of the buffer, lrbuffer. MUST BE < 80 characters (NO WCHAR, please) wide.
//  4) ptrbc: Pointer to the instance of the baseClass caller is using.
//  5) tuple: a value used by postgres PQ… routines for determining where to obrain, within the
//     current cursor, the value we wish to insert into the lrspaces buffer. Essentially, the row
//     number, or tuple, returned from the SELECT statement.
//  6) fieldid: the field number (column number) where we obtain the value to be inserted into
//     the lrbuffer.
//  7) pointer to a location of type char * where we find the character string that user
//     desires to place in the lrspaces buffer.
//  8) Assumed buffer size. Default is 80, otherwise, specify the max length of the lrspaces buffer
//     being used. Must be greater than the maximum size of the data being found in any filed
//     being retured by the SQL select. Needed only if the width of the data in the field returned
//     by your SELECT SQL statement is greater than 80 characters.
//
//  Return codes: a pointer to the lrspaces buffer filled with the fieldidcolumn that's padded on
//  the left and right with the fill character.
//  If the width of the lrspaces buffer is less than the width of the value obtained by from the
//  postgres database, then stickItInTheCenter will fill the lrspaces buffer with `*` characters.
//
//  Assumptions: Using postgreSQL, C++, BaseClass object found in file baseClass.hpp
//
//  Example of use: std::cout << stickItInTheCenter(lrspaces, ' ', ptrbc, tupleid0, fieldid0) << stickItInTheCenter(lrspaces, ' ', ptrbc, tupleid0, fieldid1) << stickItInTheCenter(lrspaces, ' ', ptrbc, tupleid0, fieldid2) << … << std::endl;
//



#ifndef __ALIGN__
#define __ALIGN__

#define ASSUMEDBUFFERSIZE 80
#define FILLCHARACTER ' '
#include <cstring>
#include <iostream>
#include "/usr/local/pgsql/pgsql101/include/libpq-fe.h"
#include "myPrototypes.hpp"
#include "baseClass.hpp"
#define DATAPASSEDISOFSIZEZERO 1
class Align {
private:
    unsigned short int totalWidthOfPadding = 0;
    unsigned short int leftWidthOfPadding = 0;
    unsigned short int rightWidthOfPadding = 0;
    size_t dataSize = 0; //Make this size_t so compiler won't throw a warning message when we do a strlen instruction.
    unsigned int loopCounter = 0;
    unsigned int bufferSize; //Size of the buffer where we will do our work.
    unsigned int numberOfFields = '0';
    unsigned int numberOfCharacters = '0';
    int *colWidthArray;
    unsigned int colWidth = 0;
    char *workBuffer; //Points to the buffer where we will do our work
    char * originalBufferLocation;
    char leftPaddingBuffer = ' ';
    char rightPaddingBuffer = ' ';
    char fill = '*' ;
    char testPattern;
    
    
public:
    int  rc;
    char *workBufferToBeReturnedToCaller;
    Align(int);
    ~Align();   //Destructor used to free dynamically acquired storage used by this class's member functions.
void    alignSetup(const char *);
char *alignLeft(int, char *, int=ASSUMEDBUFFERSIZE);
char *alignCenter(int,   char *,  int=ASSUMEDBUFFERSIZE);
char *alignRight(int, char *, char, int=ASSUMEDBUFFERSIZE);
};


#endif /* __ALIGN__ */
