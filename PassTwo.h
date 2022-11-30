#ifndef PASSTWO_H
#define PASSTWO_H

#include "FileHandler.h"
#include <bitset>
using namespace std;

struct DefStruct
{
    string name, 
        address;
};

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