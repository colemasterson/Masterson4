/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
*********************************************************************
***  DESCRIPTION : main.cpp is where the main program is executed.
The main function takes in 2 arguments, which are files that are used
in popuating the symbol table and evaluating expressions based on the
symbol table values.
********************************************************************/
#include "FileHandler.h"
#include "Expressions.h"
#include "Instructions.h"
#include "PassTwo.h"

using namespace std;


int main(int argc, char* argv[])
{

    if(argc == 1)
    {
        PassTwo p;

        return 0;
    }
    else if(argc == 2)
    {
        string e(argv[1]);

        PassTwo p2(e);

        return 0;
    }
    else
        cout << "Correct Usage: ./output OR ./output programFile" << endl;

    return 1;
}


