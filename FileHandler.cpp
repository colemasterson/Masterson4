/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
***  DESCRIPTION : FileHandler.cpp is where the FileHeader class
 is implemented.
********************************************************************/
#include "FileHandler.h"

/********************************************************************
*** FUNCTION : FileHandler()                                      ***
*********************************************************************
*** DESCRIPTION : creates a FileHandler.                          ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
FileHandler::FileHandler()
{
    
}
/********************************************************************
*** FUNCTION : FileHandler(string in)                             ***
*********************************************************************
*** DESCRIPTION : creates a FileHandler, gets the name for the
intermediate file and processes a file in.                        ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
FileHandler::FileHandler(string in)
{
    path = in;
    intName = getIntName(in);
    readProgram(path);

}

/********************************************************************
*** FUNCTION : ~FileHandler()                                     ***
*********************************************************************
*** DESCRIPTION : destroys a LiteralTable.                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
FileHandler::~FileHandler()
{

}

/********************************************************************
*** FUNCTION : getPath()                                          ***
*********************************************************************
*** DESCRIPTION : prompts user and returns a file path            ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::getPath()
{
    string s;
    cin >> s;
    cin.ignore();
    return s;
}

/********************************************************************
*** FUNCTION : readProgram(string path)                           ***
*********************************************************************
*** DESCRIPTION : completes the first pass of SIC assembly.
Calculates the location counter values, stores symbols and literals,
and writes these to an intermediate file.                         ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void FileHandler::readProgram(string path)
{
    vector<ProgLine> arguments;
    ifstream inFile;
    ofstream outFile;
    string tLine;
    ErrorCheck e;
    string startAddr, endAddr;

    // used to get current line and location counter
    int lineCounter = 1;
    string nextLoc = "0";

    inFile.open(path);

    if(!inFile)
    {
        cout << "Error: File not found. Please re-enter file path." << endl;
        readProgram(getPath());
    }    

    outFile.open(intName);
    outFile << setw(5) << "LINE#" << setw(10) << "LOCCTR" << setw(15) << "LABEL" << setw(15) << "Operation" << setw(20) << "Operand" << endl;

    while(getline(inFile, tLine))
    {
        ProgLine p;

        if(tLine.find(';') != string::npos)
        {
            // if comment is a partial line comment, remove it from the string
            int idx = tLine.find(';');            
            p.comment = tLine.substr(idx, tLine.size());
            tLine = tLine.substr(0, idx - 1);



            //if comment is a full line comment, continue to next line
            if(idx == 0)
                continue;
        }

        istringstream ss(tLine);
        string arg;

        while(ss >> arg)
        {
           // arg.erase(std::remove_if(arg.begin(), arg.end(), ::isspace), arg.end());
            p.contents.push_back(arg);
        }

        p.locctr = nextLoc;

        if(p.contents.size() == 3)
        {
            p.label = p.contents.at(0);
            p.operation = p.contents.at(1);
            p.operand = p.contents.at(2);
           // p.locctr = evalLOCCTR(nextLoc, p);
        }
        else if(p.contents.size() == 2)
        {
            if(p.contents.at(0).find(':') != string::npos)
            {
                p.label = p.contents.at(0);
                p.operation = p.contents.at(1);
               // p.locctr = evalLOCCTR(nextLoc, p);

            }
            else
            {
                p.operation = p.contents.at(0);
                p.operand = p.contents.at(1);
                //p.locctr = evalLOCCTR(nextLoc, p);
            }
        }
        else
            p.comment = p.contents.at(0);

        p.LineNum = lineCounter;
        
    if(p.operation == "START")
    {
        startAddr = p.operand;
        p.locctr = addHex(nextLoc, toHex(p.operand));
    }
    if(p.operation == "EQU")
    {
        if(e.isNum(p.operand))
            p.locctr = toHex(p.operand);
        else if(p.operand.at(0) == '*')
        {
            p.locctr = nextLoc;
        }
        else if(expressions.validExpression(p.operand))
        {
            p.locctr = expressions.readSingleExp(p.operand);
        }
    }
    else if(p.operation == "RESB")
    {
        p.locctr = addHex(nextLoc, toHex(p.operand));
        nextLoc = addHex(p.locctr, "3");
    }
    else if(p.operation == "RESW")
    {
        int value = stoi(p.operand);
        value = value * 3;
        string res = to_string(value);

        p.locctr = nextLoc;
        nextLoc = addHex(nextLoc, "3");
    }
    else if(p.operation == "BYTE")
    {
        int l = expressions.literals.assignLength(p.operand);
        nextLoc = addHex(nextLoc, to_string(l));
    }
    else if(p.operation == "END")
    {
        p.locctr = nextLoc;
        endAddr = p.locctr;
    }



    if(!p.operand.empty() && p.operand.at(0) == '=')
        expressions.literals.insertLit(p.operand);

    if(p.label != "")
        insertLabel(p);

    if(!isADirective(p))
        nextLoc = evalLOCCTR(nextLoc, p);
        
      //  p.locctr = evalLOCCTR(currLoc, p.operand);
        arguments.push_back(p);

        outFile << setw(2) << setfill('0') << (p.LineNum) << "\t\t  " << setfill(' ') << setw(5) << setfill('0') << p.locctr  << setfill(' ') << setw(15) << p.label << setw(15) << p.operation << setw(20) << p.operand << endl;
        
        lineCounter++;
        
    }

    string litAddr = nextLoc;
    int k = 0;
    for(Literal lit : expressions.literals.lits)
    {
        
        outFile <<  setw(2) << setfill('0') <<
        lineCounter << "\t\t  "<< setfill(' ') << setw(5) << setfill('0') <<
        litAddr << setfill(' ') << setw(15) << 
        "*" << setw(15) << 
        lit.name << endl;

        expressions.literals.lits.at(k).intAddress = litAddr;
        litAddr = addHex(litAddr, toHex(to_string(lit.length)));
        lineCounter++;
        k++;
    }

    //outFile << calcProgLength(startAddr, endAddr) << endl;

    inFile.close();
    outFile.close();

    cout << setw(5) << "LINE#" << setw(10) << "LOCCTR" << setw(15) << "LABEL" << setw(15) << "OPERATION" << setw(20) << "OPERAND" << endl;

    for(int i = 0; i < arguments.size(); i++)
    {
        cout << setw(2) << setfill('0') << 
        arguments.at(i).LineNum << "\t  " << setfill(' ') << setw(5) << setfill('0') <<
        arguments.at(i).locctr  << setfill(' ') << setw(15) << 
        arguments.at(i).label << setw(15) << 
        arguments.at(i).operation << setw(20) << 
        arguments.at(i).operand << endl;     
    }

    litAddr = nextLoc;
    for(Literal lit : expressions.literals.lits)
    {
        cout <<  setw(2) << setfill('0') <<
        lineCounter << "\t  "<< setfill(' ') << setw(5) << setfill('0') <<
        litAddr << setfill(' ') << setw(15) << 
        "*" << setw(15) << 
        lit.name << endl;

        litAddr = addHex(litAddr, toHex(to_string(lit.length)));
        lineCounter++;
    }
    cout << calcProgLength(startAddr, endAddr) << endl;


    return;

}

void FileHandler::passOne(string path)
{
    ifstream inFile;
    ofstream outFile;
    ErrorCheck e;
    string tempLine, startAddr, endAddr;

    int lineCounter = 1;
    string nextLoc = 0;

    inFile.open(path);

    if(!inFile)
    {
        cout << "Error: File not found. Please re-enter file path." << endl;
        passOne(getPath());
    }

    outFile.open(intName);
    outFile << setw(5) << "LINE#" << setw(10) << "LOCCTR" << setw(15) << "LABEL" << setw(15) << "Operation" << setw(20) << "Operand" << endl;


}

// 1. get line from the file and pass into string stream

// 2. remove comments from the line

// 3. while ss >> args push them onto a stack

// 4. sort and populate into 2, 3, or 4 arguments

// 5. determine if ProgLine contains directive

    // 6a. if true, determine locctr in directiveLogic()

    // 6b. if false, determine locctr in instructionLogic()

// 7. check for literals



/********************************************************************
*** FUNCTION : getIntName(string in)                              ***
*********************************************************************
*** DESCRIPTION : Takes file name and creates an intermediate file
with the same name.                                               ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::getIntName(string in)  
{    
    string newPath = in.substr(0, in.size()-4);
    newPath += ".int";
    return newPath;   
}

ProgLine FileHandler::instantiatePL(string tLine, string loc)
{
    ProgLine p;
    istringstream ss(tLine);
        string arg;

        while(ss >> arg)
        {
           // arg.erase(std::remove_if(arg.begin(), arg.end(), ::isspace), arg.end());
            p.contents.push_back(arg);
        }

        p.locctr = loc;

        if(p.contents.size() == 3)
        {
            p.label = p.contents.at(0);
            p.operation = p.contents.at(1);
            p.operand = p.contents.at(2);
           // p.locctr = evalLOCCTR(nextLoc, p);
        }
        else if(p.contents.size() == 2)
        {
            if(p.contents.at(0).find(':') != string::npos)
            {
                p.label = p.contents.at(0);
                p.operation = p.contents.at(1);
               // p.locctr = evalLOCCTR(nextLoc, p);

            }
            else
            {
                p.operation = p.contents.at(0);
                p.operand = p.contents.at(1);
                //p.locctr = evalLOCCTR(nextLoc, p);
            }
        }
        else
            p.comment = p.contents.at(0);


    return p;
}
        
/********************************************************************
*** FUNCTION : insertLabel(ProgLine p)                            ***
*********************************************************************
*** DESCRIPTION : populates the value for a symbol based on the
given assembly line and adds the label to the symbol table.       ***

*** INPUT ARGS : ProgLine                                         ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void FileHandler::insertLabel(ProgLine p)
{
    SymbolNode temp;

    //initalize temp symbol name and iFlag from the progline
    temp.symbol = e.nameCheck(p.label);
    temp.iFlag = true;
    temp.hexValue = p.locctr;
    temp.value = toDec(temp.hexValue);

    if(!p.operand.empty())
    {
    if(p.operation == "EQU" && (e.isNum(p.operand) || p.operand != "*"))
        temp.rFlag = false;
    else
        temp.rFlag = true;

        expressions.symbols.insert(temp);
        return;
    }
    temp.rFlag = true;
    expressions.symbols.insert(temp);

    return;
}

/********************************************************************
*** FUNCTION : toDec(string hexIn)                                ***
*********************************************************************
*** DESCRIPTION : converts a hexadecimal string to decimal.       ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : int                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
int FileHandler::toDec(string hexIn)
{
    stringstream ss;
    int x;
    ss << hex << hexIn;
    ss >> x;
    return x;
}

/********************************************************************
*** FUNCTION : calcProgLength(string first, string last)          ***
*********************************************************************
*** DESCRIPTION : calculates the lengths of a program based on the
starting and final program counter, as well as the total length
of stored literals.                                               ***

*** INPUT ARGS : string, string                                   ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::calcProgLength(string first, string last)
{
    int length;

    length = toDec(last) - toDec(first) + stoi(totalLitLength());

    string result = "\nProgram Length = (" + ((last)) + " - " + ((first)) + ") + " + (totalLitLength()) + " = " + toHex((length)); 

    return result;
}

/********************************************************************
*** FUNCTION : totalLitLength()                                   ***
*********************************************************************
*** DESCRIPTION : Sums the length values of all literals stored
in the literal table.                                             ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : int                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::totalLitLength()
{
    int sum = 0;
        for(Literal lit : expressions.literals.lits)
            sum += lit.length;

    return to_string(sum);
}

/********************************************************************
*** FUNCTION : setIType(ProgLine in)                              ***
*********************************************************************
*** DESCRIPTION : returns a string containing the instruction format
of an instruction found in a given line.                          ***

*** INPUT ARGS : Progline                                         ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::setIType(ProgLine in)
{
    string IType;
    if(in.operation.at(0) == '+')
        return "4";

    InstPtr ptr = instructions.find(in.operation);

    if(ptr == nullptr)
    {
        for(int i = 0; i < dir.size(); i++)
        {
            if(in.operand == dir.at(i))
            {
                //operation is a directive, do directive logic
               IType = "DR";
               return IType;
            }
        }
        // invalid instruction
        IType = "ER";
        return IType;
    }
    // valid instruction
    IType = ptr->data.iType;
    return IType;
}

/********************************************************************
*** FUNCTION : evalOpCode(string in)                              ***
*********************************************************************
*** DESCRIPTION : finds the opcode of a given instruction from the 
Instructions search tree.                                         ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::evalOpCode(string in)
{
    string opVal;
    Instruction temp;
    temp.name = in;
    InstPtr ptr = instructions.find(temp);

    if(ptr == nullptr)
    {
        for(int i = 0; i < dir.size(); i++)
        {
            if(in == dir.at(i))
            {
                //operation is a directive, do directive logic
               opVal = "DD";
               return opVal;
            }
        }
        // invalid instruction
        opVal = "EE";
        return opVal;
    }
    // valid instruction
    opVal = ptr->data.opcode;
    return opVal;    
}

/********************************************************************
*** FUNCTION : evalLOCCTR(string currLoc, ProgLine p)             ***
*********************************************************************
*** DESCRIPTION : calculates the next location counter value based
on the operation, operands given, previous location counter, and
instruction format.                                               ***

*** INPUT ARGS : Progline, string                                 ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::evalLOCCTR(string currLoc, ProgLine p)
{

    Instruction temp;
    temp.name = p.operation;
    InstPtr ptr = instructions.find(temp);

        if(p.operation.at(0) == '+')
            return addHex(currLoc, "4");
        
        if(ptr != nullptr)
        {
            string type = ptr->data.iType;

            if(type == "1")
            {
                //cout << addHex(currLoc, "1") << endl;
                return addHex(currLoc, "1");
            }
            else if(type == "2")
            {
                //cout << addHex(currLoc, "2") << endl;
                return addHex(currLoc, "2");
            }
            else if(type == "34")
            {
                if((e.isNum(p.operand) && isOutOfRange(p.operand)) || p.operation.at(0) == '+')
                {
                    //cout << addHex(currLoc, "4") << endl;
                    return addHex(currLoc, "4");
                }
                //cout << addHex(currLoc, "3") << endl;
                return addHex(currLoc, "3");
            } 
            else if(type =="4")
                return addHex(currLoc, "4");
          
        }
        if(p.operand.at(0) == '@' || p.operand.at(0) == '#')
        {
            p.operand = p.operand.substr(1, p.operand.size());
            return evalLOCCTR(currLoc, p);
        }
        return "NFIND";
}

/********************************************************************
*** FUNCTION : isOutOfRange(string operand)                       ***
*********************************************************************
*** DESCRIPTION : returns true if a decimal string is greater than
2048.                                                             ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool FileHandler::isOutOfRange(string operand)
{
    stringstream ss;
    int decimal;

    ss >> decimal;

    if(decimal < 2048)
        return false;
    
    return true;

}

/********************************************************************
*** FUNCTION : toHex(string in)                                   ***
*********************************************************************
*** DESCRIPTION : converts a decimal string to hexadecimal.       ***

*** INPUT ARGS : string                                           ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::toHex(string in)
{

    int i = stoi(in);

    stringstream ss(in);
    ss << hex << i;
    string res = ss.str();  

    return res;
}

/********************************************************************
*** FUNCTION : toHex(int in)                                      ***
*********************************************************************
*** DESCRIPTION : converts an integer to a hexadecimal string.    ***

*** INPUT ARGS : int                                              ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::toHex(int in)
{
    stringstream ss;

    ss << hex << in;
    string res = ss.str();

    return res;
}

/********************************************************************
*** FUNCTION : addHex(string currL, string incr)                  ***
*********************************************************************
*** DESCRIPTION : returns the sum of two hexadecimal strings      ***

*** INPUT ARGS : string, string                                   ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string FileHandler::addHex(string currL, string incr)
{
    stringstream ss1, ss2, ss3;
    int cDec, iDec, sDec;
    string result;

    ss1 << hex << currL;
    ss1 >> cDec;

    //cout << cDec << endl;
    ss1.clear();

    ss2 << hex << incr;
    ss2 >> iDec;

    //cout << iDec << endl;
    ss2.clear();

    sDec = cDec + iDec;

    ss3 << hex << sDec;
    result = ss3.str();

    ss3.clear();
    //cout << "Result: " << sDec << endl;
    return result;
}

/********************************************************************
*** FUNCTION : isADirective(ProgLine p)                           ***
*********************************************************************
*** DESCRIPTION : returns true if a given line's operand is found
in the SIC directives list.                                       ***

*** INPUT ARGS : Progline                                         ***

*** OUTPUT ARGS : bool                                            ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
bool FileHandler::isADirective(ProgLine p)
{
    for(string i : dir)
    {
        if(p.operation == i)
            return true;
    }
    return false;
}

string FileHandler::displayObjCode(string in)
{
    if(in == "NULL")
        return "";
    else
        return in;
}

void FileHandler::printProg(ProgLine p)
{
    cout << setw(2) << setfill('0') << 
    (p.LineNum) << "\t  " << setfill(' ') << setw(5) << setfill('0') <<
    p.locctr  << setfill(' ') << setw(15) << 
    p.label << setw(15) << 
    p.operation << setw(20) << 
    p.operand << setw(15) << displayObjCode(p.objCode) << endl;

    return;
}

string FileHandler::subHex(string currL, string decr)
{
    stringstream ss1, ss2, ss3;
    int cDec, iDec, sDec;
    string result;

    ss1 << hex << currL;
    ss1 >> cDec;

    //cout << cDec << endl;
    ss1.clear();

    ss2 << hex << decr;
    ss2 >> iDec;

    //cout << iDec << endl;
    ss2.clear();

    sDec = cDec - iDec;

    ss3 << hex << sDec;
    result = ss3.str();

    ss3.clear();
    //cout << "Result: " << sDec << endl;
    return result;
}
