/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
***  DESCRIPTION : Expressions.h is where the Expression class is 
defined. An Expression object has 2 member values: a SymbolTable and    
LiteralTable object. Expressions.h contains a struct, 2 constructors, 
a destructor, 10 public methods, and a string to display errors.        
********************************************************************/
#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "SymbolTable.h"
#include "LiteralTable.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <vector>

struct Expression
{
    string name, rString;
    int value;
    bool N, I, X;
};

class Expressions
{
    public:
        string userMessage;
        Expressions();
        ~Expressions();

        string getPath();
        void readSymbols(string);
        void readExpressions(string);
        string readSingleExp(string);
        
        Expression evaluate(string);
        string calcRFlag(string, string, bool);
        bool validExpression(string); 
        string parseRFlag(bool);

        void printSTable();
        void printLTable();
        void view(Expression);

        void pause() const;

        SymbolTable symbols;
        LiteralTable literals;
    private:

};
#endif