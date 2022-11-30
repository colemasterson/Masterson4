/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
***  DESCRIPTION : LiteralTable.cpp is where the LiteralTable is
implemented. 
********************************************************************/

#include "LiteralTable.h"

using namespace std;

/********************************************************************
*** FUNCTION : LiteralTable()                                     ***
*********************************************************************
*** DESCRIPTION : creates a LiteralTable.                         ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
LiteralTable::LiteralTable()
{

}
/********************************************************************
*** FUNCTION : LiteralTable()                                     ***
*********************************************************************
*** DESCRIPTION : destroys a LiteralTable.                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
LiteralTable::~LiteralTable()
{

}

/********************************************************************
*** FUNCTION : insertLit(string in)                               ***
*********************************************************************
*** DESCRIPTION : inserts literals into the table that are valid
and are not duplicates of another literal.                        ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void LiteralTable::insertLit(string in)
{
    if(e.isValidLit(in) && !checkDuplicate(in))
    {
        Literal lit;
        lit.name = in;
        lit.opVal = assignValue(lit.name);
        lit.length = assignLength(lit.name);
        lit.address = assignAddress(lit.name);

        lits.push_back(lit);
    }
    else
    {
        if(e.errorMessage != "")
            displayError();
    }
}

/********************************************************************
*** FUNCTION : displayError()                                     ***
*********************************************************************
*** DESCRIPTION : prints an error to the terminal.                ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void LiteralTable::displayError()
{
    cout << e.errorMessage << endl;
}


/********************************************************************
*** FUNCTION : assignValue(string in)                             ***
*********************************************************************
*** DESCRIPTION : takes in the contents of a literal as a string,
converts it to the proper value, and returns the value.           ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string LiteralTable::assignValue(string in)
{
    string val, contents;
    int hex, ascii = 0;
    char t = in.at(1);
    char d = in.at(0);
    bool inContent = false;
    stringstream stream;
    //get literal contents
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

    if(t == 'X' || t == 'x'|| d == 'X' || d == 'x')
    {
        //Literal is byte, convert from hex to decimal and return number
        if(contents.size() % 2 != 0)
        {
            string resize;
            resize += "0";
            resize += contents;
            contents = resize;
        }
        return contents;
    }

    else if(t == 'C'|| t == 'c'|| d == 'C' || d == 'c')
    {
        //Literal is character, convert to ASCII and concatnate values for return
        string cHex;

        for(int i = 0; i < contents.size(); i++)
        {
            //get ascii character value from contents
            ascii = contents.at(i);
            //convert from decimal to hex
            stream << std::hex << ascii;
            //convert from hex to string
            stream >> cHex;
            //append to total
            val += cHex;
            //clear the string stream
            stream.clear();
        }

        return (val);
    }
    return "NULL";

}

/********************************************************************
*** FUNCTION : checkDuplicate(string in)                          ***
*********************************************************************
*** DESCRIPTION : loops through the literal table looking for
duplicate symbols. If a dublicate is found, the function returns
true. Else, the function returns false.                           ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool LiteralTable::checkDuplicate(string in)
{
    //get literal header and determine if it is a char or byte literal
    bool isByte = false;
    string head, contents, twinLit;

    head = in.substr(0,2);

    if(head == "=X" || head == "=x")
        isByte = true;
    
    //loop over literals and check for literals containing both variants of headers
    
    if(isByte)
    {
        if(head == "=X")
            twinLit += "=x" + in.substr(2, in.size());
        else
            twinLit += "=X" + in.substr(2, in.size());
       
    }
    else
    {
        if(head == "=C")
            twinLit += "=c" + in.substr(2, in.size());
        else
            twinLit += "=C" + in.substr(2, in.size());
       
    }

    for(Literal lit : lits)
    {
        if(lit.name == in || lit.name == twinLit)
            return true;
    }

    return false;
}

/********************************************************************
*** FUNCTION : assignLength(string in)                          ***
*********************************************************************
*** DESCRIPTION : Assigns and returns the length of a literal based
on the literal contents.                                          ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : int                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
int LiteralTable::assignLength(string in)
{
    bool isChar, inContent = false;
    string contents;
    string head = in.substr(0,2);

    if(head == "=C" || head == "C'")
        isChar = true;
    
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

    if(isChar)
    {
        return contents.size();
    }
    else
    {
        float ans = (contents.size()+1)/2;
        return ceil(ans);
    }
       
}

/********************************************************************
*** FUNCTION : assignAddress(string in)                           ***
*********************************************************************
*** DESCRIPTION : returns the size of the list, which corresponds to
the address of a newly inserted literal.                          ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
int LiteralTable::assignAddress(string in)
{
    return lits.size();
}

/********************************************************************
*** FUNCTION : view(Literal tLit)                                 ***
*********************************************************************
*** DESCRIPTION : displays all member attributes of a Literal struct.

*** INPUT ARGS : Literal tLit                                     ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void LiteralTable::view(Literal tLit)
{
    cout << setw(20) << tLit.name << setw(15) << tLit.opVal << setw(15) << tLit.length << setw(15) << tLit.intAddress << endl;
}

/********************************************************************
*** FUNCTION : printLitTable()                                    ***
*********************************************************************
*** DESCRIPTION : loops through the literal table and displays all
member attributes for every literal.                              ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void LiteralTable::printLitTable()
{
    cout << "Literal Table" << endl;
    cout << setw(20) << "Name" << setw(15) << "Value" << setw(15) << "Length" << setw(15) << "Address"<< endl;
    for(Literal t : lits)
    {
        view(t);
    }
}
