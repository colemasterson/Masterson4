/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
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

string getPath();
void testFunction();
void testFunction2(FileHandler);

int main(int argc, char* argv[])
{

    if(argc == 1)
    {

/*         cout << "ENTER PROGRAM FILE:\n";
        string e = getPath();

        FileHandler prog(e);
        prog.expressions.symbols.inOrder();
        prog.expressions.literals.printLitTable();
 */

        PassTwo p;

        //testFunction();
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

string getPath()
{
    string s;
    cin >> s;
    cin.ignore();
    return s;
}

void testFunction()
{
    PassTwo p2Object;
    vector<string> test = {"0000f", "A13", "00004", "00013", "0C"};

    for(string s : test)
    {
        string og, bin, hex;
        og = s;
        bin = p2Object.HexToBin(og);
        hex = p2Object.BinToHex(bin);
        cout << setw(15) << og << setw(15) << bin << setw(15) << hex << endl;
    }

    return;
}

void testFunction2(FileHandler passOne)
{
    ifstream inFile;
    string tLine;

    inFile.open("sic2.int");

    if(!inFile)
    {
        cout << "BRUHHH";
        exit(102);
    }

    while(getline(inFile, tLine))
    {
        stringstream ss(tLine);
        string tArg;
        ProgLine p;

        while(ss >> tArg)
            p.contents.push_back(tArg);

        if(p.contents.at(0) == "LINE#")
            continue;

        if(p.contents.size() == 4)
        {
            p.LineNum = stoi(p.contents.at(0));
            p.locctr = p.contents.at(1);
            p.operation = p.contents.at(2);
            p.operand = p.contents.at(3);
            
        }
        else if(p.contents.size() == 5)
        {

            p.LineNum = stoi(p.contents.at(0));
            p.locctr = p.contents.at(1);
            p.label = p.contents.at(2);
            p.operation = p.contents.at(3);
            p.operand = p.contents.at(4);            
        }

        passOne.printProg(p);
    }
    inFile.close();

    return;
}
