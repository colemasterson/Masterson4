/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
***  DESCRIPTION : FileHandler.cpp is where the FileHandler program 
is defined. The FileHandler class has 1 struct, 2 constructors, a 
destructor, 17 public methods, a string for the program and 
intermediate file paths, an Expression object, Instruction object,
,ErrorCheck object, and a vector containing SIC directives.
********************************************************************/
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Instructions.h"
#include "Expressions.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

struct ProgLine
{
    //locations to store line values before processing
    vector<string> contents;

    //processed values
    string  label, 
            operation, 
            operand,
            locctr,
            comment,
            iType,
            objCode;

    //line number
    int LineNum;

    //set if label is relative
    bool isR;

};

class FileHandler
{

    public:
        FileHandler();
        FileHandler(string);
        ~FileHandler();

        string getPath();
        void readProgram(string);
        void passOne(string);
        string getIntName(string);

        string addHex(string, string);
        string subHex(string, string);

        string toHex(string);
        string toHex(int);
        int toDec(string);

        string setIType(ProgLine);
        void insertLabel(ProgLine);
        void insertLiteral(string);
        void printProg(ProgLine);

        ProgLine instantiatePL(string, string);
        void directiveLogic(ProgLine);
        void instructionLogic(ProgLine);
        bool isOutOfRange(string);
        bool isADirective(ProgLine);

        string displayObjCode(string);

        string calcProgLength(string, string);
        string totalLitLength();

        string evalOpCode(string);
        string evalLOCCTR(string, ProgLine);

        string path;
        string intName;

        Instructions instructions;
        Expressions expressions;
        ErrorCheck e;

        //list of SIC directives
        vector<string> dir = {"BASE", "BYTE", "END", "EQU", "EXTDEF", "EXTREF", "RESB", "RESW", "START", "WORD"};

    private:



};

#endif