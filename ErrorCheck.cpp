/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                     *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
DESCRIPTION : ErrorCheck.cpp is where the ErrorCheck class functions
are implemented. 

********************************************************************/
#include "ErrorCheck.h"

using namespace std;

/********************************************************************
*** FUNCTION : ErrorCheck()                           ***
*********************************************************************
*** DESCRIPTION : creates a ErrorCheck, initializes the Valid flag
to true and the error message to an empty string.                 ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
ErrorCheck::ErrorCheck()
{
    isValid = true;
    errorMessage = "";
}
/********************************************************************
*** FUNCTION : ~ErrorCheck()                                      ***
*********************************************************************
*** DESCRIPTION : destructor for an ErrorCheck object.            ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
ErrorCheck::~ErrorCheck()
{
    
}
/********************************************************************
*** FUNCTION : nameCheck(string in)                           ***
*********************************************************************
*** DESCRIPTION : Takes in a raw symbol name from the file. Next, ***
it removes the semi colon from the end of the symbol and ensures
the symbol is in valid format to be inserted into the symbol table.
If the symbol is invalid, the isValid flag is set to false and the
proper error message is displayed. If the symbol is valid, the new,
refined symbol is returned.
*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string ErrorCheck::nameCheck(string in)
{
    string out = in;

    //check to see if string is alphanumeric
    if(in.back() == ':')
        out.pop_back();
    
    if(!validSymbol(out))
        return out;
    
    if(out.length() > 6)
        out = out.substr(0, 6);

    if(isdigit(in.front()))
    {
        isValid = false;
        errorMessage += " ERROR: INVALID SYMBOL NAME, NAME MUST START WITH A LETTER.";
    }
    return out;

}
/********************************************************************
*** FUNCTION : flagCheck(string s)                                ***
*********************************************************************
*** DESCRIPTION : Takes in a string s from the file contents.     ***
If the string matches input requirements, the proper rFlag boolean
is returned. If invalid, the isValid flag is set to false and the 
proper error message is printed.
*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool ErrorCheck::flagCheck(string s)
{
    if(s == "false"|| s == "0")
        return false;
    if(s == "true" || s == "1")
        return true;

    errorMessage += " ERROR: INVALID RFLAG FORMAT: ( " + s + " ). Must be true, false, 0, or 1.";
    isValid = false;
    return false;
}
/********************************************************************
*** FUNCTION : valCheck(float in)                                 ***
*********************************************************************
*** DESCRIPTION : takes in a raw float value from the file. If the***
float is an integer, an integer copy is returned. If invalid, the
isValid flag is set to false, the proper error message is printed,
and INT_MAX is returned.
*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : int                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
int ErrorCheck::valCheck(string in)
{
    //if value is integer, return value
    //if value is not integer, cout error and return max int
    stringstream s(in);
    float f;

    if(s >> f)
    {
        //string is a float, check if integer
        if(ceil(f) == f)
            return f;
        else
        {
            //string is a float but not integer
            errorMessage += " ERROR: VALUE ("+ in +") MUST BE INTEGER.";
            isValid = false;
            return INT_MAX;
        }

    }
    else
    {
        //string is not a number
        errorMessage += " ERROR: VALUE ("+ in +") MUST BE INTEGER.";
        isValid = false;
        return INT_MAX;    
    }
}
/********************************************************************
*** FUNCTION : validSymbol(string in)                             ***
*********************************************************************
*** DESCRIPTION : Ensures a string symbol is alphanumeric. If     ***
the string contains special characters, the function returns false,
the isValid flag is set to false, and the proper error message is
printed. If valid, the function returns true.
*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool ErrorCheck::validSymbol(string in)
{
    for(int i = 0; i < in.size(); i++)
    {
        if(isalnum(in[i]) == 0) 
        {
            isValid = false;
            return false;
        }
    }
    return true;
}

/********************************************************************
*** FUNCTION : isNum(string in)                                   ***
*********************************************************************
*** DESCRIPTION : determines if a string is a number. Returns true
if string is a number.                                            ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool ErrorCheck::isNum(string in)
{
    if(in.size() < 1)
        return false;
    for(int i = 0; i < in.size(); i++)
    {
       if(!isdigit(in.at(i)))
       {
            return false; 
       }
    }
    return true;
}

/********************************************************************
*** FUNCTION : isValidLit(string in)                              ***
*********************************************************************
*** DESCRIPTION : determines if a literal is valid. Returns true if
literal is valid, and false otherwise.                            ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool ErrorCheck::isValidLit(string in)
{
    bool inContent = false;
    string contents;

    if(in.size() < 5)  
    {     // smallest literal would look like this: " =A'B' " so length must be at least 5
        errorMessage += "ERROR: INVALID SIZE FOR LITERAL DEFINITION. (" + in + ")\n";
        return false;
    }

    string head = in.substr(0,3);
    if(head != "=C'" && head != "=X'" && head != "=c'" && head != "=x'" )
    {
        head = in.substr(0,2);
        if(head != "C'" && head != "X'" && head != "c'" && head != "x'")
        {
            errorMessage += "ERROR: INVALID SYNTAX FOR LITERAL TYPE DECLARATION. (" + in + ")\n";
            return false;
        }
    }
    
    // at this point the string must look like this =A'XX
    // check last character is ' and characters between are hex

    if(in.back() != '\'')
    {
        errorMessage += "ERROR: INVALID SYNTAX FOR LITERAL DEFINITION. (" + in + ")\n ";
        return false;     
    }
    // string must look like =A'X', just confirm X is hex and return true
    // grab literal contents and store in string
    for(int i = 0; i < in.size(); i++)
    {
        if(in.at(i) == '\'')
        {
            inContent = !inContent;
            continue;
        }

        if(inContent)
            contents += in.at(i);
    }
        //if a byte literal is not hexadecimal or a literal doesn't contain alphanumeric characters... 
    if(((head == "=X'" || head == "X'") && !isHex(contents)) || !validSymbol(contents))
    {
        errorMessage += "ERROR: LITERAL CONTENTS CONTAINS NON-HEX CHARACTERS (" + in + ")\n";
        return false;
    }

    // literal is valid! add to literal struct and append to literal table
    return true;
}

/********************************************************************
*** FUNCTION : isHex(string str1)                                 ***
*********************************************************************
*** DESCRIPTION : determines if a string is a valid hexadecimal
number. Returns true if valid, or false otherwise.                ***

*** INPUT ARGS : string str1                                      ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool ErrorCheck::isHex(string str)
{
    for(int i = 0; i < str.size(); i++)
    {
        char t = str.at(i);

        if(!isxdigit(t))
            return false;
    }

    return true;
}
