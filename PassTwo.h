#ifndef PASSTWO_H
#define PASSTWO_H

#include "FileHandler.h"
#include <bitset>
using namespace std;

class PassTwo
{
    public:
        PassTwo();
        PassTwo(string);
        ~PassTwo();

        void setDirectory(string);
        void readIntFile(string);


        string firstTwoHex(ProgLine);
        string last4to6Hex(ProgLine);
        string directiveOpCode(ProgLine);
        string calcObjCode(ProgLine);

        void saveDefandRef(ProgLine);
        void addToRecord(ProgLine);
        void writeObjFile();

        string HexToBin(string);
        string BinToHex(string);
        string findAddress(string);
        string iFormat(ProgLine);

        vector<string> HeadR, EndR, TextR, DefR, RefR, ModR;
        vector<string> Extdef, Extref;

        string directory;
        
        FileHandler f;
    private:
};

#endif