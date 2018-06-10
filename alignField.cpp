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
//  7) pointer to a location of type size_t where we return the length of the data that user 
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
#include "alignField.hpp"
using namespace std;
//char *alignCenter(char, int, BaseClass *, int, int, char *, int=ASSUMEDBUFFERSIZE);
char *Align::alignCenter(int columnNumber,  char *data,  int assumedBufferSize) {
    this->colWidth = *(this->colWidthArray + columnNumber);
    this->workBuffer = this->originalBufferLocation;
    if (data !=nullptr ) {
        this->dataSize = strlen(data); //If this instruction fails, then assume it's because we're pointing to a NULL field.
    } else {
         while (this->colWidth--) {
            *(this->workBuffer++) = '*'; // a colWidth's worth of `*` characters (FORTRAN-like format overflow indicator).
         }
        this->dataSize=0; // Do this for the benefit of the next `if` instruction to force a return to caller with field filled with `*` characters.
        this->rc = DATAPASSEDISOFSIZEZERO;
        return this->originalBufferLocation;
    }


    

//    if ( this->colWidth != *(this->colWidthArray + columnNumber) ) cout << "Given column width is " << colWidth << ", while constructor's computed column width is " << this->colWidth << "!! GO FIGURE! " << endl;


    if (this->dataSize > assumedBufferSize  || this->dataSize > colWidth) {  //Is the width of the data > size of the field where we wish to place it?
//        memset(this->workBufferToBeReturnedToCaller, '*', colWidth-1);
            this->dataSize = -2 + colWidth; //Reduce the size of the Valid data so it will fit within the column with two spaces remaining.
//        *(this->workBufferToBeReturnedToCaller + colWidth) = NULL;  //Place a NULL at the end of the string to terminate the string of all `*`
//        return this->workBufferToBeReturnedToCaller;
    }
    this->totalWidthOfPadding = colWidth - this->dataSize;
    this->leftWidthOfPadding = (colWidth - this->dataSize)>>1;
    this->rightWidthOfPadding = this->totalWidthOfPadding - this->leftWidthOfPadding;
    this->workBuffer = this->originalBufferLocation; //Make sure we start at the beginning of the buffer.
    //Now fill the workBuffer with: a) the left padding, b) the data, and c) the right padding.
    while(this->leftWidthOfPadding--) { //Do left padding.
        *(this->workBuffer++) = this->fill; //Move the fill character into the workBuffer one-fillcharacter-at-a-time;
    }
    while (this->dataSize--) {
        *(this->workBuffer++) = *(data++); //Move the data into the workBuffer one-characcter at a time.
    }
    while(this->rightWidthOfPadding--) { //Move the right padding into the buffer
        *(this->workBuffer++) = this->fill;
    }
    *(this->workBuffer) = NULL; //Terminate the field of padding characters and field data with a NULL, thus terminating the string.
    
    return this->originalBufferLocation;
}      
Align::Align( int assumedBufferSize) {
    
    this->originalBufferLocation = new char[assumedBufferSize];
    //    this->leftPaddingBuffer = new char[assumedBufferSize/2];
    //    this->rightPaddingBuffer = new char[assumedBufferSize/2];
    this->workBufferToBeReturnedToCaller = this->originalBufferLocation;
    this->fill = FILLCHARACTER;
    this->loopCounter=0;
    this->numberOfFields = 0;
}
void Align::alignSetup( const char *headerRowSpacerPattern) {
//Create an array of field widths based upon the string provided by the caller.
// String looks like:
//----------------+-------------+------------+----------------+-------------+----------------+------------+------+------- +------- +---------+------- +- ------+---------+---------------
//With the `+` signs are the places where the field to the left of the `+` ends and the field to the right of the `+` sign begins.
//We call the above string the headerRowSpacerPattern.
//Doing the code, below, saves us the tedius, and error prone, procedure of manually counting the number of characters between fields.
    while( *(headerRowSpacerPattern + this->loopCounter)   ) {
        this->testPattern = *(headerRowSpacerPattern + this->loopCounter);
        if (*(headerRowSpacerPattern + this->loopCounter) == '+') {
            this->numberOfFields++;
        }
        this->loopCounter++;
    }
    this->colWidthArray = new int[this->numberOfFields];
    this->loopCounter=0; //Reset the loop counter.
    this->numberOfFields=0; //Reset this, too.
    this->numberOfCharacters = 0; //And don't forget to reset this guy.
    do {
        if (*(headerRowSpacerPattern + this->loopCounter) == '+') {
            *(this->colWidthArray + this->numberOfFields) = this->numberOfCharacters;
             this->numberOfFields++;
             this->numberOfCharacters = 0;
        } else {
            this->numberOfCharacters++; //Bump up the width of this field by 1
        }
    }     while( *(headerRowSpacerPattern + this->loopCounter++) );
    *(this->colWidthArray + this->numberOfFields) = (this->numberOfCharacters-1); //Put the field width of the last (right-most) field into the array, less the space occupied by the string-ending NULL character.
    /*
    for (this->loopCounter=0; this->loopCounter<this->numberOfFields; this->loopCounter++ ) {
        cout << this->loopCounter <<". " << "Field Width " << *(this->colWidthArray + this->loopCounter) << endl;
    }
     */
}
Align::~Align() {
    /*
    if (this->colWidthArray !=nullptr) delete this->colWidthArray;
    if (this->originalBufferLocation != nullptr) delete(this->originalBufferLocation);
     */
    
}
