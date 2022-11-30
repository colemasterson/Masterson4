/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
* This is where the PassTwo class is defined. This class contains 2
structs, 6 object code records, 21 public methods, two constructors,
and a destructor.
*********************************************************************/

#ifndef PASSTWO_H
#define PASSTWO_H

#include "FileHandler.h"
#include <bitset>
using namespace std;


//used to hold DR values to make it easier to print
struct DefStruct
{
    string name, 
        address;
};

//used to hold MR values to make it easier to print
struct ModStruct
{
    string address,
            HBLength,
            ModFlag,
            name;
}; 

class PassTwo
{
    public:
        PassTwo();
        PassTwo(string);
        ~PassTwo();

        void setDirectory(string);
        void readIntFile(string);
        void runPassTwo(string);


        string firstTwoHex(ProgLine);
        string last4to6Hex(ProgLine);
        string directiveOpCode(ProgLine);
        string calcObjCode(ProgLine);

        void saveDefandRef(ProgLine);
        void checkDefandMod(ProgLine);
        void addToRecord(ProgLine);

        void printObjFile();
        void writeObjFile();

        void printProgLength();
        void writeProgLength();

        void writeSymbolTable();
        void writeLiteralTable();

        string HexToBin(string);
        string BinToHex(string);
        string findAddress(string);
        string iFormat(ProgLine);
        bool isAnExtRef(string);

        vector<string> HeadR, EndR, RefR;
        vector<ProgLine> TextR2;
        vector<DefStruct> DefR;
        vector<ModStruct> ModR;

        string directory;
        
        FileHandler f;
    private:
};

#endif