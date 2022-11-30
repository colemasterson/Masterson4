#include "PassTwo.h"

PassTwo::PassTwo()
{
    // prompt user to enter program file name, store in FileHandler object f
    cout << "ENTER PROGRAM FILE:\n";
    string datPath = f.getPath();

    // complete pass one
    f.intName = f.getIntName(datPath);          // remove the program file type extension (.dat), append ".int" to the end
    f.readProgram(datPath);                     // complete pass one of assembly
    f.expressions.symbols.inOrder();            // print symbol table tree using in order traversal to command line
    f.expressions.literals.printLitTable();     // print literal table to command line
       
    // complete pass two
    setDirectory(f.intName);
    readIntFile(f.intName);
}

PassTwo::PassTwo(string datPath)
{
    setDirectory(datPath);

    f.intName = f.getIntName(datPath);
    f.readProgram(directory + ".dat");
    f.expressions.symbols.inOrder();
    f.expressions.literals.printLitTable();

    readIntFile(f.intName);
}

PassTwo::~PassTwo()
{
    
}

void PassTwo::setDirectory(string path)
{
    if(path.find('.') != string::npos)
    {
        int idx = path.find('.');
        path = path.substr(0, idx);

        directory = path;
    }

    return;
}


void PassTwo::readIntFile(string path)
{
    HeadR.push_back("H^");
    EndR.push_back("E^");

    ifstream inFile;
    ofstream outFileTxt, outFileObj;
    string tLine;
    string baseLoc;
    bool isEOP = false;

    inFile.open(path);
    outFileTxt.open(directory + ".txt");
    outFileObj.open(directory + ".obj");

    if(!inFile)
    {
        cout << "ERROR: INTERMEDIATE FILE NOT FOUND. " << path << endl;
        exit(102);
    }

    outFileTxt << setw(2) << 
    "LINE#" << setw(5)  <<
    "LOCCTER" << setw(15) << 
    "LABEL" << setw(15) << 
    "OPERATION" << setw(20) << 
    "OPERAND" << setw(15) << "OBJECT CODE" << endl;

    while(getline(inFile, tLine))
    {
        ProgLine p;
        string tArg;
        stringstream ss(tLine);

        //not all intermediate lines have the same number of arguments, store all line arguments into allArgs and determine ProgLine values based on size.
        while(ss >> tArg)
            p.contents.push_back(tArg);
       

        if(isEOP)
        {
            for(string ar : p.contents)
            {   
                if(ar.at(0) == '=')
                {
                    for(Literal lit : f.expressions.literals.lits)
                    {
                        if(ar == lit.name)
                            p.objCode = lit.opVal;
                    }
                }
                continue;
            }
        } 
        //remove the header of the intermediate file from file processing
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

        if(p.operation == "BASE")
        {
            SymbolNode temp;
            temp.symbol = p.operand;
            if(f.expressions.symbols.find(temp) != nullptr)
                baseLoc = f.expressions.symbols.find(temp)->data.hexValue;
        }

    //pop back colon from label 
        if(p.label.back() == ':')
            p.label = p.label.substr(0, p.label.size()-1);

        saveDefandRef(p);

        p.objCode = calcObjCode(p);  
        f.printProg(p);
        addToRecord(p);

    outFileTxt << setw(2) << setfill('0') << 
    (p.LineNum) << "\t  " << setfill(' ') << setw(5) << setfill('0') <<
    p.locctr  << setfill(' ') << setw(15) << 
    p.label << setw(15) << 
    p.operation << setw(20) << 
    p.operand << setw(15) << f.displayObjCode(p.objCode) << endl;
        

    }
    inFile.close();

    outFileTxt << "Literal Table" << endl;
    outFileTxt << setw(20) << "Name" << setw(15) << "Value" << setw(15) << "Length" << setw(15) << "Address"<< endl;
    for(Literal tLit : f.expressions.literals.lits)
    {
        outFileTxt << setw(20) << tLit.name << setw(15) << tLit.opVal << setw(15) << tLit.length << setw(15) << tLit.intAddress << endl;
    }
    
    
    f.expressions.symbols.inOrderWrite(directory + ".txt");

    outFileTxt.close();

    writeObjFile();

    
    return;
}

string PassTwo::calcObjCode(ProgLine p)
{
    string first3, last3;
    first3 = firstTwoHex(p);
    last3 = last4to6Hex(p);
    return first3 + last3;
}
string PassTwo::firstTwoHex(ProgLine p)
{
    bool isF4;  //set to true if instruction is format 4
    string instr = p.operation;
    string result = "";
    string tOpCode, binOp, tOpFormat;
    
    //if theres an operand, find the opcode from the tree

    //populate obj code for end of int file literals

    if(p.operation == "*")
    {
        for(Literal lit: f.expressions.literals.lits)
        {
            if(p.operand == lit.name)
                return lit.opVal;
        }
    }

    if(p.operation == "START" ||p.operation == "END" || p.operation == "EXTDEF" ||p.operation == "EXTREF" || p.operation == "BASE" || p.operation == "EQU")
        return "";

    if(!instr.empty())
    {
        if(instr.at(0) == '+')
        {
            isF4 = true;
            instr = p.operation.substr(1, p.operation.size());
        }

        InstPtr currInst = f.instructions.find(instr);
        if(currInst != nullptr)
        {
            tOpCode = currInst->data.opcode;
        }
        else if(f.isADirective(p))
        {
            //operation is a directive, eval opcode
            if(instr == "BYTE" || instr == "WORD")
            {
                if(p.operand.back() == '\'')
                    result = f.expressions.literals.assignValue(p.operand);
                else
                    result = f.expressions.readSingleExp(p.operand);

                while(result.size() < 6)
                    result = "0" + result;
                return result;
            }

            if(instr == "RESW" || instr == "RESB")
            {
                //check if value is to large to hold in a format 3 instruction
                return "";
            }

        }

        

        //determine if instruction is simple, immediate or indirect

        if(p.operand.at(0) == '@')
            tOpFormat = "2";
        else if(p.operand.at(0) == '#')
            tOpFormat = "1";
        else if(p.operand.find(",T") != string::npos || p.operand == "F")
            tOpFormat = "0";
        else
            tOpFormat = "3";

        
        //convert the opcode to binary and keep the first 6 bits
        binOp = HexToBin(tOpCode);
        binOp = binOp.substr(0,6);
        binOp += "00";

        tOpCode = BinToHex(binOp);
        // return first three hex
        if(!f.isADirective(p))
            result = f.addHex(tOpCode, tOpFormat);

        // determine if it is base relative, program counter relative, or instruction 4
        
        // concatenate the relative hex number to the first two hex numbers
        
    }
    //if the final result is 1 digit, append a zero to the front
    if(result.size() <= 1)
        result = "0" + result;

    // return first two hex
    return result;
}


string PassTwo::last4to6Hex(ProgLine p)
{
    SymbolNode tSym;

    string adrType, disp;
    string tOperand = p.operand;
    //string opValue;
    string opAddress; 
    bool isF4 = false;

    bool isConst = false;
    bool isSym = false;
    bool isExp = false;
    bool isLit = false;

    int dLength = 3;

    //if directive, return nothing

    if(f.isADirective(p))
        return "";

    //remove front addressing char and determine operand type
    if(p.operand.front() == '@' || p.operand.front() == '#')
        tOperand = tOperand.substr(1, tOperand.size());

    //remove the indexing from operand 
    if(p.operand.find(',') != string::npos)
    {
        int idx = p.operand.find(',');
        tOperand = tOperand.substr(0, idx);
    }

    //Determine if operand is a constant, literal, expression, or symbol

    if(f.e.isNum(tOperand))
    {   //check if operand is a constant
        isConst = true;
        opAddress = f.expressions.readSingleExp(p.operand);
    }
    else if(p.operand.front() == '=' && p.operation != "*")
    {   // check if operand is a literal
        isLit = true;
        opAddress = findAddress(p.operand);
    }
    else if(p.operand.front() == '=' && p.operation == "*")
    {   // check if literal at end of file
        return "";
    }
    else if(p.operand.find('-') != string::npos || p.operand.find('+') != string::npos || p.operand.find(',') != string::npos) //check if operand is an expression
    {   //check if operand is an expression
        if(p.operand.find(",T") != string::npos)
            return "45";
        opAddress = f.expressions.readSingleExp(p.operand);
        isExp = true;
    }
    else
    {   //check if operand is a symbol

        if(p.operand == "F")
            return "60";
        tSym.symbol = tOperand;

        if(f.expressions.symbols.find(tSym) != nullptr)
        {
            isSym = true;
            NodePtr realSymbol = f.expressions.symbols.find(tSym);
            opAddress = realSymbol->data.hexValue;
        }
    }
    
    if(p.operation.at(0) == '+')
    {   // format 4 condition
        adrType = "1";
        isF4 = true;
    }
    else if(f.toDec(opAddress) > 2047 || f.toDec(opAddress) < -2048)
    {   //base relative condition
        adrType = "4";

    }
    else
    {   //not BR or F4, must be PC
        adrType = "2";
    }

    if(isF4)
        dLength = 5;
    //calculate displacement based on the set operand flags

    if(isConst)
    {
        disp = f.toHex(tOperand);
    }
    else if(isLit)
    {
        for(Literal lit: f.expressions.literals.lits)
        {
            if(lit.name == tOperand)
            {
                disp = f.subHex(lit.intAddress, (f.addHex(p.locctr, iFormat(p))));
            }
        }
    }
    else if(isExp)
    {
        disp = f.subHex(f.expressions.readSingleExp(p.operand), (f.addHex(p.locctr, iFormat(p))));

        while(disp.size() > dLength)
            disp = disp.substr(1, disp.size());
    }
    else if(isSym)
    {

        NodePtr s = f.expressions.symbols.find(tSym);
        string symVal, nextLoc;
        symVal = s->data.hexValue;
        nextLoc = f.addHex(p.locctr, iFormat(p));

        disp = f.subHex(s->data.hexValue, (f.addHex(p.locctr, iFormat(p))));
        
        while(disp.size() > dLength)
            disp = disp.substr(1, disp.size());
    }
    


    while(disp.size() < dLength)
        disp = "0" + disp;

    return adrType + disp;
}

//first, determine if the operand is a literal, constant, symbol or expression

//next, calc if its Base, PC or Format 4 and add 4, 2, or 1 to the object code

//last, determine the case for calculating displacement

    //CERTAIN DIRECTIVES [START, END, EXTDEF, EXTREF, ]
    //Return nothing

    //LITERAL
    //Literal location - current locctr + instruction format

    //SYMBOL
    //Symbol location - current locctr + instruction format

    //EXPRESSION
    //Value of the expression - current locctr + instruction format

    //CONSTANT
    // Hex Value of the constant

string PassTwo::HexToBin(string hNum)
{
    int length = 0;

    int leadZeroIDX = 0;

    //loop thru input hex string, start counting after leading zeroes
    if(hNum.empty())
        return "";

    if(hNum.at(0) == '0')
    {
        for(int i = 0; i < hNum.size(); i++)
        {
            if(hNum.at(i) == '0')
                continue;
            
            leadZeroIDX = i;
            break;
        }

    }   
        //from the end of the leading zeroes to the end of the hex input, increment length
        for(int j = leadZeroIDX; j < hNum.size(); j++)
            length++;

    //multiply length by 4 to convert from hex length to binary length
    length = length * 4;

    stringstream ss;

    ss << hex << hNum;
    unsigned n;
    ss >> n;
    bitset<12> b(n);     // outputs "0000 1010"
    string result =  b.to_string();
    string revised;

    //adjust final result based on calculated length
    for(int i = result.size() - length; i < result.size(); i++)
       revised += result.at(i);
    
    return revised;
}

void PassTwo::saveDefandRef(ProgLine p)
{
    if(p.operation == "EXTDEF")
    {
        string values = p.operand;
        string reg;

        istringstream ss(values);

        while(getline(ss, reg, ','))
        {
            DefStruct temp;
            temp.name = reg;
            DefR.push_back(temp);
        }
    }

    if(p.operation == "EXTREF")
    {
        string values = p.operand;
        string reg;

        istringstream ss(values);

        while(getline(ss, reg, ','))
        {
            RefR.push_back(reg);

            ModStruct temp;
            temp.name = reg;
            ModR.push_back(temp);

            SymbolNode newSym;
            newSym.symbol = reg;
            newSym.value = 0;
            newSym.rFlag = 0;
            newSym.iFlag = 0;
            newSym.mFlag = 1;
            f.expressions.symbols.insert(newSym);
        }
    }
}

void PassTwo::checkDefandMod(ProgLine p)
{
    bool isExpression;
    bool isSub, addedFront, addedBack = false;
    int idx;
    string first, last;


    // determine if the operand is an expression
    if(p.operand.find('+') != string::npos)
    {
        isExpression = true;
        idx = p.operand.find('+');

        first = p.operand.substr(0, idx);
        last = p.operand.substr(idx + 1, p.operand.size());
    }
    else if(p.operand.find('-') != string::npos)
    {
        isExpression = true;
        isSub = true;
        idx = p.operand.find('-');

        first = p.operand.substr(0, idx);
        last = p.operand.substr(idx + 1, p.operand.size());
    }


    //if the operand is an expression...
    if(isExpression)
    {
        for(int i = 0; i < ModR.size(); i++)
        {
            if(first == ModR[i].name && addedFront == false)
            {
                if(ModR[i].address != "")
                {
                    ModStruct newMod;

                    newMod.name = first;

                    if(p.operation == "WORD" || p.operation == "BYTE")
                        newMod.HBLength = "06";
                    else
                        newMod.HBLength = "05";
                    
                    newMod.address = p.locctr;
                    newMod.ModFlag = "+";

                    ModR.push_back(newMod);
                }
                else
                {
                    ModR[i].address = p.locctr;

                    if(p.operation == "WORD" || p.operation == "BYTE")
                        ModR[i].HBLength = "06";
                    else
                        ModR[i].HBLength = "05";

                    ModR[i].ModFlag = "+";
                }
                
                addedFront = true;
            }

            if(last == ModR[i].name && addedBack == false)
            {
                if(ModR[i].address != "")
                {
                    ModStruct newMod;
                    newMod.name = last;

                    if(p.operation == "WORD" || p.operation == "BYTE")
                        newMod.HBLength = "06";
                    else
                        newMod.HBLength = "05";

                    newMod.address = p.locctr;

                    if(isSub)
                        newMod.ModFlag = "-";
                    else
                        newMod.ModFlag = "+";

                    ModR.push_back(newMod);
                }
                else
                {
                    ModR[i].address = p.locctr;

                    if(p.operation == "WORD" || p.operation == "BYTE")
                        ModR[i].HBLength = "06";
                    else
                        ModR[i].HBLength = "05";

                    if(isSub)
                        ModR[i].ModFlag = "-";
                    else
                        ModR[i].ModFlag = "+";
                }

                addedBack = true;                
            }
        }
    }
    else
    {
        //operand must be a symbol
        for(int i = 0; i < ModR.size(); i++)
        {
            if(ModR[i].name == p.operand)
            {
                if(ModR[i].address != "")
                {
                    ModStruct newMod;

                    newMod.name = p.operand;
                    newMod.address = p.locctr;
                    newMod.HBLength = "05";
                    newMod.ModFlag = "+";

                    ModR.push_back(newMod);
                }
                else
                {
                    ModR[i].address = p.locctr;
                    ModR[i].HBLength = "05";
                    ModR[i].ModFlag = "+";                    
                }
            }
        }
    }


    //check for Def Record matches
    for(int i = 0; i < DefR.size(); i++)
    {
        if(p.label.find(DefR[i].name) != string::npos)
            DefR[i].address = p.locctr;
    }


    //check for Mod Record matches

    //could be a symbol or expression

    //if an expression, split it into two symbols.
    //if either symbol has already been defined in another modr, make a new one for it
    //set both HB to 06
    //if the operation is -, set second modflag to -
}

void PassTwo::addToRecord(ProgLine p)
{
    //conditions for Header Record
    if(p.operation == "START")
    {
        if(!p.label.empty())
        {
            p.label = p.label.substr(0,6);
            
            if(p.label.back() == ':')
                p.label.pop_back();

            HeadR.push_back(p.label + "^");

            while(p.operand.size() < 6)
                p.operand = "0" + p.operand;
            
            HeadR.push_back(p.operand + "^");
            TextR2.push_back(p);
        }
    }
    // conditions for Header and End Record
    if(p.operation == "END")
    {
        while(p.operand.size() < 6)
            p.operand = "0" + p.operand;
            
        string totLength = f.addHex(p.locctr, f.totalLitLength());
        while(totLength.size() < 6)
            totLength = "0" + totLength;

        HeadR.push_back(totLength);
        EndR.push_back(HeadR.at(2));

        //if the last character of the last string in the end file ends in a carot, delete it.
        if(EndR.back().back() == '^')
            EndR.back().pop_back();
    }

    // conditions for Def record and Mod Record
    checkDefandMod(p);
    
    //Conditions for Textfile
    if(p.objCode != "" && p.objCode != "NULL")
    {
        TextR2.push_back(p);
    }

    return;
}

void PassTwo::writeObjFile()
{
    ofstream outFile;
    outFile.open(directory + ".obj");

    //if there are values in the record, print the record contents and a new line.

    if(HeadR.size() > 1)
    {
        for(string s: HeadR)
        {
            outFile << s;
        }
        outFile << endl;
    }

    if(DefR.size() > 0)
    {
        outFile << "D^";
        for(int i = 0; i < DefR.size(); i++)
        {
            outFile << DefR[i].name << "^";
            if(i == DefR.size() - 1)
                outFile << DefR[i].address;
            else
                outFile << DefR[i].address << "^";

        }
        outFile << endl;
    }

    if(RefR.size() > 1)
    {
        outFile << "R^";
        for(int i = 0; i < RefR.size(); i++)
        {
            if(i == RefR.size() - 1)
                outFile << RefR[i];
            else
                outFile << RefR[i] << "^";

        }
        outFile << endl;
    }

    if(TextR2.size() > 0)
    {
        // calculate a running sum of argument sizes. When sum = 60, indent and start a new text record.
        // FORM: T^(start address)^(length of TR)^(opcodes)^(opcodes)
        string startLoc = "000000";

        while(!TextR2.empty())
        {
            vector<ProgLine> tempTR;
            int count = 0;

            while(count < 30 && !TextR2.empty()) 
            {
                if(count + TextR2.front().objCode.size() > 30)   //safety check
                    break;
                
                count += TextR2.front().objCode.size();  //increment counter by the number of half bytes in opcode
                tempTR.push_back(TextR2.front()); //add the object code to the current text record
                TextR2.erase(TextR2.begin());     //remove that opcode from TextR
            }
            //calculate length
            string endPt = f.addHex(tempTR.back().locctr, iFormat(tempTR.back()));
            string length = f.subHex(endPt, tempTR.front().locctr);
            
            //print text record and indent
            outFile << "T^" + startLoc + "^" + length + "^";
            for(int i = 0; i < tempTR.size(); i++)
            {
                if(i == tempTR.size()- 1)
                    outFile << tempTR[i].objCode;
                else
                {
                    outFile << tempTR[i].objCode + "^";
                }

            }
            outFile << "\n";
            //increment starting locctr
            startLoc = f.addHex(startLoc, length);
        }    
    }

    if(ModR.size() > 0)
    {
        for(int i = 0; i < ModR.size(); i++)
        {
            outFile << "M^" + ModR[i].address + "^" + ModR[i].HBLength + "^" + ModR[i].ModFlag + ModR[i].name << endl;
        }
    }

    if(EndR.size() > 1)
    {
        for(string s: EndR)
            outFile << s;
    
        outFile << endl;
    }

    outFile.close();
    return;
}

void PassTwo::printObjFile()
{
    //fstream outFile;
    //outFile.open(directory + ".obj");

    cout << "OBJECT RECORDS" << endl;

    //if there are values in the record, print the record contents and a new line.

    if(HeadR.size() > 1)
    {
        for(string s: HeadR)
        {
            cout << s;
        }
        cout << endl;
    }

    if(DefR.size() > 0)
    {
        cout << "D^";
        for(int i = 0; i < DefR.size(); i++)
        {
            cout << DefR[i].name << "^";
            if(i == DefR.size() - 1)
                cout << DefR[i].address;
            else
                cout << DefR[i].address << "^";

        }
        cout << endl;
    }

    if(RefR.size() > 1)
    {
        cout << "R^";
        for(int i = 0; i < RefR.size(); i++)
        {
            if(i == RefR.size() - 1)
                cout << RefR[i];
            else
                cout << RefR[i] << "^";

        }
        cout << endl;
    }

    if(TextR2.size() > 0)
    {
        // calculate a running sum of argument sizes. When sum = 60, indent and start a new text record.
        // FORM: T^(start address)^(length of TR)^(opcodes)^(opcodes)
        string startLoc = "000000";

        while(!TextR2.empty())
        {
            vector<ProgLine> tempTR;
            int count = 0;

            while(count < 30 && !TextR2.empty()) 
            {
                if(count + TextR2.front().objCode.size() > 30)   //safety check
                    break;
                
                count += TextR2.front().objCode.size();  //increment counter by the number of half bytes in opcode
                tempTR.push_back(TextR2.front()); //add the object code to the current text record
                TextR2.erase(TextR2.begin());     //remove that opcode from TextR
            }
            //calculate length
            string endPt = f.addHex(tempTR.back().locctr, iFormat(tempTR.back()));
            string length = f.subHex(endPt, tempTR.front().locctr);
            
            //print text record and indent
            cout << "T^" + startLoc + "^" + length + "^";
            for(int i = 0; i < tempTR.size(); i++)
            {
                if(i == tempTR.size()- 1)
                    cout << tempTR[i].objCode;
                else
                {
                    cout << tempTR[i].objCode + "^";
                }

            }
            cout << "\n";
            //increment starting locctr
            startLoc = f.addHex(startLoc, length);
        }    
    }

    if(ModR.size() > 0)
    {
        for(int i = 0; i < ModR.size(); i++)
        {
            cout << "M^" + ModR[i].address + "^" + ModR[i].HBLength + "^" + ModR[i].ModFlag + ModR[i].name << endl;
        }
    }

    if(EndR.size() > 1)
    {
        for(string s: EndR)
            cout << s;
    
        cout << endl;
    }

    return;
}

void PassTwo::printProgLength()
{

}


string PassTwo::BinToHex(string bin)
{
    std::stringstream reader(bin);
    std::stringstream result;

    while (reader)
    {
        std::bitset<8> digit;
        reader >> digit;
        result << std::hex << digit.to_ulong();
    }

    string sResult = result.str();
    sResult.pop_back();

    return sResult;
}

string PassTwo::iFormat(ProgLine p)
{
    if(!p.operation.empty())
    {
        if(p.operation.front() == '+')
            return "4";
        else if(f.instructions.find(p.operation) != nullptr)
        {
            InstPtr tempInst = f.instructions.find(p.operation);

            if(tempInst->data.iType == "34")
                return "3";
            else
                return tempInst->data.iType;
        }
        else if(p.operation == "WORD" || p.operation == "BYTE")
            return "3";       
    }

    return "0";
}

string PassTwo::findAddress(string litIn)
{
    for(Literal lit : f.expressions.literals.lits)
    {
        if(litIn == lit.name)
        {
            return lit.intAddress;
        }
    }

    cout << "Could not find " + litIn + "inside the symbol table." << endl;
    return "";
}


bool PassTwo::isAnExtRef(string operandIn)
{
    for(string ref: RefR)
    {
        if(operandIn.find(ref) != string::npos)
            return true;
    }
    cout << "REF NOT FOUND: " << operandIn << endl;
    return false;
}
