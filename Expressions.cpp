/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 2                                              *** 
***  DUE DATE    : 10/15/2022                                     *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
***  DESCRIPTION : Expressions.cpp is where the Expression class is 
implemented. An Expression object has 2 member values: a SymbolTable &    
LiteralTable object. Expression.cpp contains a struct, 2 constructors, 
a destructor, 10 public methods, and a string to display errors.        
********************************************************************/

#include "Expressions.h"

/********************************************************************
*** FUNCTION : Expressions() : root(NULL)                         
*********************************************************************
*** DESCRIPTION : creates an Expressions object.
*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Expressions::Expressions()
{

}

/********************************************************************
*** FUNCTION : ~Expressions()                        
*********************************************************************
*** DESCRIPTION : destroys an Expressions object.
*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Expressions::~Expressions()
{
    
}

/********************************************************************
*** FUNCTION : readSymbols(string in)                             ***
*********************************************************************
*** DESCRIPTION : function to access private member value symbols
and enter a file to be read and processed.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::readSymbols(string in)
{
    symbols.readFile(in);
}

/********************************************************************
*** FUNCTION : readSymbols(string in)                             ***
*********************************************************************
*** DESCRIPTION : asks user to enter a string file path and returns
the input.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string Expressions::getPath()
{
    string s;
    cout << "Please enter file name:";
    cin >> s;
    cin.ignore();
    return s;
}

/********************************************************************
*** FUNCTION : validExpression(string in)                         ***
*********************************************************************
*** DESCRIPTION : determines if a string expression is valid or not.
Returns true if exp is valid.                                     ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool Expressions::validExpression(string in)
{
    if(in.find(',') != string::npos)
    {
        if(in.find(",X") == string::npos && in.find(",x") == string::npos)
        {
            cout << "ERROR: ALL EXPRESSIONS WITH INDEXED ADDRESSING MUST USE THE X REGISTER. EXP: ("+ in + ")\n";
            return false;
        } 

        if (in.find('@') != string::npos)
        {
            cout << "ERROR: CANNOT USE INDIRECT ADDRESSING WITH INDEXED ADDRESSING. EXP: ("+ in + ")\n";
            return false;
        }

        if (in.find('#') != string::npos)
        {
            cout << "ERROR: CANNOT USE IMMEDIATE ADDRESSING WITH INDEXED ADDRESSING. EXP: ("+ in + ")\n";
            return false;
        }
    }
    return true;
}
/********************************************************************
*** FUNCTION : readExpressions(string line)                       ***
*********************************************************************
*** DESCRIPTION : reads expressions from a file entered via command
line. The expression is validated and if it is valid, is processed***
and sent to evaluate() to assign values and rflags.
*** INPUT ARGS : string line                                      ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::readExpressions(string line)
{
    fstream inFile;
    ErrorCheck e;
    cout << "Expression File:" << endl;
    string tempExp;
    bool RF, isLit, hasIdx = false;
    int i = 0;

    //open the file, if file is not found prompt user to enter file path
    inFile.open(line);
    if(!inFile)
    {
        cout << "Invalid file name. Please enter file name again." << endl;
        string in = getPath();
        readExpressions(in);
    }

    cout << setw(15) << "Expression:" << setw(10) << "Value:" << setw(15) << "Relocatable:" << setw(10) << "N-Bit:" << setw(10) << "I-Bit:" << setw(10) << "X-Bit:" << endl;

    //get each line of the symbol table and add to expression vector
    while(!inFile.eof())
    {
        i++;
        inFile >> tempExp;
        if(validExpression(tempExp) == false)
            continue;
        Expression result;

        if(tempExp.find(",") != string::npos || tempExp.find(",") != string::npos)
            hasIdx = true;

        if(tempExp.at(0) == '=')
        { 
            literals.insertLit(tempExp);
            continue;
        }
        else if(tempExp.at(0) == '@')
        {
            tempExp.erase(0,1);
            result = evaluate(tempExp);
            result.name = '@' + tempExp;
            result.N = 1;
            result.I = 0;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(tempExp.at(0) == '#')
        {
            tempExp.erase(0,1);
            result = evaluate(tempExp);
            result.name = '#' + tempExp;
            result.N = 0;
            result.I = 1;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(isdigit(tempExp.front()) && isdigit(tempExp.back()))
        {
            result = evaluate(tempExp);
            result.name = tempExp;
            result.N = 0;
            result.I = 1;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(hasIdx)
        {
            tempExp.pop_back();
            tempExp.pop_back();

            result = evaluate(tempExp);
            result.name = tempExp + ",X";
            result.N = 1;
            result.I = 1;
            result.X = 1;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else
        {
            result = evaluate(tempExp);
            result.name = tempExp;
            result.N = 1;
            result.I = 1;
            result.X = 0;
            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        if(!isLit && !RF && result.name.at(0) != '-')
            view(result);
        hasIdx = false;

        if(i == 20)
        {
            pause();
            i=0;
        }
    }

    inFile.close();

    // 0 = Absolute
    // 1 = Relative

    /*for all expressions in exp, if expression starts with:


        =, try to insert into symbol table
            insert into Line struct
            test Line struct and try to insert
            
        @, indirect addressing - use RFLAG value
        #, immediate addressing - use RFLAG value
        0-9, immediate addressing - Absolute RFLAG

     if the expression contains:

        + or -, check RFLAG compatability and assign new RFLAG

    */ 



};

string Expressions::readSingleExp(string tempExp)
{
    bool hasIdx = false;
    bool RF = false;
    bool isLit = false;

    Expression result;
    
        

        if(tempExp.find(",X") != string::npos || tempExp.find(",x") != string::npos)
            hasIdx = true;


        if(tempExp.at(0) == '=')
        { 
            literals.insertLit(tempExp);
            return "";
        }
        else if(tempExp.at(0) == '@')
        {
            tempExp.erase(0,1);
            result = evaluate(tempExp);
            result.name = '@' + tempExp;
            result.N = 1;
            result.I = 0;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(tempExp.at(0) == '#')
        {
            tempExp.erase(0,1);
            result = evaluate(tempExp);
            result.name = '#' + tempExp;
            result.N = 0;
            result.I = 1;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(isdigit(tempExp.front()) && isdigit(tempExp.back()))
        {
            result = evaluate(tempExp);
            result.name = tempExp;
            result.N = 0;
            result.I = 1;
            result.X = 0;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else if(hasIdx)
        {
            tempExp.pop_back();
            tempExp.pop_back();

            result = evaluate(tempExp);
            result.name = tempExp + ",X";
            result.N = 1;
            result.I = 1;
            result.X = 1;

            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        else
        {
            result = evaluate(tempExp);
            result.name = tempExp;
            result.N = 1;
            result.I = 1;
            result.X = 0;
            if(result.rString == "ERROR")
            {
                cout << "ERROR: INCOMPATABLE RFLAGS BETWEEN THE ARGUMENTS IN ("+result.name+")\n";
                RF = true;
            }
        }
        if(!isLit && !RF && result.name.at(0) != '-')
            hasIdx = false;

        stringstream ss;
        ss << hex << result.value;
        string res = ss.str();

        return res;

}

/********************************************************************
*** FUNCTION : printLTable()                                      ***
*********************************************************************
*** DESCRIPTION : accesses the private member literals and prints
the literal table.                                                ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::printLTable()
{
    literals.printLitTable();
}

/********************************************************************
*** FUNCTION : parseRFlag(bool flag)                              ***
*********************************************************************
*** DESCRIPTION : Takes in a bool flag value and converts it to text
used by the expression evaluator.                                 ***

*** INPUT ARGS : bool flag                                        ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string Expressions::parseRFlag(bool flag)
{
    if(flag)
        return "RELATIVE";
    else
        return "ABSOLUTE";
}

/********************************************************************
*** FUNCTION : pause() const                                      ***
*********************************************************************
*** DESCRIPTION : pause function used to prevent expression output
from rolling off the screen.                                      ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::pause() const
{
    cout << "Enter any key to continue...";
    getchar();
}

/********************************************************************
*** FUNCTION : view(Expression ex)                                ***
*********************************************************************
*** DESCRIPTION : displays all member values of an Expression.    ***

*** INPUT ARGS : Expression ex                                    ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::view(Expression ex)
{
    cout << setw(15) << ex.name << setw(10) << ex.value << setw(15) << ex.rString << setw(10) << ex.N << setw(10) << ex.I << setw(10) << ex.X << endl;
}

/********************************************************************
*** FUNCTION : printSTable()                                      ***
*********************************************************************
*** DESCRIPTION : accesses the private member value symbols and 
prints the symbol table with in-order traversal.                  ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Expressions::printSTable()
{
    symbols.inOrder();
}

/********************************************************************
*** FUNCTION : calcRFlag(string r1, string r2, bool sub)          ***
*********************************************************************
*** DESCRIPTION : calculates the new RFlag value for an expression
based off of the two operands RFlag values and the operation being
applied.                                                          ***

*** INPUT ARGS : string r1, r2, bool sub                          ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string Expressions::calcRFlag(string r1, string r2, bool sub)
{
    if(sub)
    {
        if(r1 == "ABSOLUTE")
        {
            if(r2 == "ABSOLUTE")
                return "ABSOLUTE";
            else
                return "ERROR";
        }
        else
        {
            if(r2 == "ABSOLUTE")
                return "RELATIVE";
            else
                return "ABSOLUTE";
        }
    }
    else
    {
        if(r1 == "ABSOLUTE" && r2 == "ABSOLUTE")
            return "ABSOLUTE";
        else if(r1 == "ABSOLUTE" && r2 == "RELATIVE")
            return "RELATIVE";
        else if(r1 == "RELATIVE" && r2 == "ABSOLUTE")
            return "RELATIVE";
        else
            return "ERROR";
    }
    return "ERROR";
}

/********************************************************************
*** FUNCTION : evaluate(string in)                                ***
*********************************************************************
*** DESCRIPTION : receives a valid operand and parses it to find 
operators. After all information is found, the values and rflags are 
set and the Expression is returned.                               ***

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : Expression                                      ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Expression Expressions::evaluate(string in)
{   
    Expression res;
    ErrorCheck e;

    // if expression is not an addition or subtraction...
    if(in.find('+') == string::npos && in.find("-") == string::npos)
    {
        //add first 6 characters of the expression to symbol node and search for it in the table.
        SymbolNode temp;

        if(in.length() > 6)
            temp.symbol = in.substr(0,6);
        else
            temp.symbol = in;
        
        //if symbol could not be found...
        if(!symbols.find(temp))
        {
            //expression could be a number. if its not a number...
            if(!e.isNum(in))
            {
                //cout << "ERROR1: INVALID EXPRESSION ("+in+") COULD NOT BE INTERPRETED. \t "+ temp.symbol + "\n";
                return res;
            }

            //expression is a number; set name and value;
            res.value = stoi(in);
            res.rString = "ABSOLUTE";
            return res;
        }

        //symbol is found and no other arguments are present
        NodePtr t = symbols.find(temp);
        res.value = t->data.value;
        res.rString = parseRFlag(t->data.rFlag);
        return res;
    }

    //expression contains addition or subtraction

    //if expression contains addition...
    if(in.find('+') != string::npos || in.find('-') != string::npos)
    {
        Expression e1, e2;
        SymbolNode o1, o2;
        string op1, op2;
        NodePtr opPtr;
        int split;
        //flag to determine if expression involves addition(false) or subtraction(true)
        bool isSub;

        if(in.find('+') != string::npos)
        {
            split = in.find('+');
            isSub = false;
        }
        else
        {
            split = in.find('-');
            isSub = true;
        }

        op1 = in.substr(0, split);
        op2 = in.substr(split+1, in.size());


        o1.symbol = op1;
        o2.symbol = op2;

        if(!symbols.find(o1))
        {
            if(!e.isNum(op1))
            {
                //cout << "ERROR1: INVALID EXPRESSION ("+in+") COULD NOT BE INTERPRETED. \t "+ o1.symbol + "\n";
                return res;
            }

            //expression is a number; set name and value;
            e1.value = stoi(op1);
            e1.rString = "ABSOLUTE";
        }
        else
        {
            opPtr = symbols.find(o1);
            e1.value = opPtr->data.value;
            e1.rString = parseRFlag(opPtr->data.rFlag);
        }

        if(!symbols.find(o2))
        {
            if(!e.isNum(op2))
            {
                cout << "ERROR2: INVALID EXPRESSION ("+in+") COULD NOT BE INTERPRETED. \t "+ o2.symbol + "\n";
                return res;
            }

            //expression is a number; set name and value;
            e2.value = stoi(op2);
            e2.rString = "ABSOLUTE";
        }
        else
        {
            opPtr = symbols.find(o2);
            e2.value = opPtr->data.value;
            e2.rString = parseRFlag(opPtr->data.rFlag);
        }

        if(isSub)
        {
            res.value = e1.value - e2.value;
            res.rString = calcRFlag(e1.rString, e2.rString, isSub);
            return res;
        }

        //must be addition

        res.value = e1.value + e2.value;
        res.rString = calcRFlag(e1.rString, e2.rString, isSub);
        return res;


    }
    return res;
}