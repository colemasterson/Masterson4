#include "PassTwo.h"

PassTwo::PassTwo()
{
    //string path = f.getPath();

    cout << "ENTER PROGRAM FILE:\n";
    string e = f.getPath();

    f.readProgram(e);
    f.expressions.symbols.inOrder();
    f.expressions.literals.printLitTable();

    string path = "sic2.int";
    setDirectory(path);
    readIntFile(path);
}

PassTwo::PassTwo(string path)
{
    setDirectory(path);

    f.readProgram(directory + ".dat");
    f.expressions.symbols.inOrder();
    f.expressions.literals.printLitTable();

    readIntFile(path);
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
    ModR.push_back("M^");
    RefR.push_back("R^");
    TextR.push_back("T^");
    DefR.push_back("D^");

    ifstream inFile;
    string tLine;
    string baseLoc;
    bool isEOP = false;

    inFile.open(path);
    //outFile.open(directory + ".obj");

    if(!inFile)
    {
        cout << "ERROR: INTERMEDIATE FILE NOT FOUND." << endl;
        exit(102);
    }

    f.expressions.symbols.inOrder();

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
            p.operand = baseLoc;

    //pop back colon from label 
        if(p.label.back() == ':')
            p.label = p.label.substr(0, p.label.size()-1);

        p.objCode = calcObjCode(p);  
        f.printProg(p);
        addToRecord(p);

        //write LR line
    
    //write HR to obj program
    //initialize first TR
    
    //while opcode != END


    //need an expressions handler to handle the operand values: SECOND-FIRST, @RED, etc.

    //determine the addressing type of the instruction



    }
    inFile.close();

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
            string temp = "=" + p.operand;
            //operation is a directive, eval opcode
            if(instr == "BYTE" || instr == "WORD")
            {
                result = f.expressions.literals.assignValue(temp);
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
        else if(isF4)
            tOpFormat = "4";
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

    //if certain directive, return nothing
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
        opAddress = f.expressions.readSingleExp(p.operand);
        isExp = true;
    }
    else
    {   //check if operand is a symbol
        tSym.symbol = tOperand;

        if(f.expressions.symbols.find(tSym) != nullptr)
            isSym = true;
        
        NodePtr realSymbol = f.expressions.symbols.find(tSym);
        opAddress = realSymbol->data.hexValue;
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
    }
    else if(isSym)
    {

        NodePtr s = f.expressions.symbols.find(tSym);
        string symVal, nextLoc;
        symVal = s->data.hexValue;
        nextLoc = f.addHex(p.locctr, iFormat(p));

        disp = f.subHex(s->data.hexValue, (f.addHex(p.locctr, iFormat(p))));
    }

    if(isF4)
        dLength = 5;
    

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
            Extdef.push_back(reg);
    }

    if(p.operation == "EXTREF")
    {
        string values = p.operand;
        string reg;

        istringstream ss(values);

        while(getline(ss, reg, ','))
            RefR.push_back(reg + "^");
    }
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
            TextR.push_back(p.operand + "^");
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

    // conditions for Def record
    if(Extdef.size() > 0)
    {
        for(int i = 0; i < Extdef.size(); i++)
        {
            if(p.label == Extdef.at(i))
            {
                DefR.push_back(Extdef.at(i) + "^");
                DefR.push_back(p.locctr + "^");
                Extdef.erase(Extdef.begin() + i);
            }
        }
    }

    // conditions for Ref record

    //Conditions for Textfile
    if(p.objCode != "" && p.objCode != "NULL")
        TextR.push_back(p.objCode + "^");

    //Conditions for Mod Record

    return;
}

void PassTwo::writeObjFile()
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

    if(DefR.size() > 1)
    {
        for(string s: DefR)
            cout << s;
    
        cout << endl;
    }

    if(RefR.size() > 1)
    {
        for(string s: RefR)
            cout << s;
    
        cout << endl;
    }

    if(TextR.size() > 1)
    {
        for(string s: TextR)
            cout << s;
    
        cout << endl;
    }

    if(ModR.size() > 1)
    {
        for(string s: ModR)
            cout << s;
    
        cout << endl;
    }

    if(EndR.size() > 1)
    {
        for(string s: EndR)
            cout << s;
    
        cout << endl;
    }

    return;
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
        
        if(f.instructions.find(p.operation) != nullptr)
        {
            InstPtr tempInst = f.instructions.find(p.operation);

            if(tempInst->data.iType == "34")
                return "3";
            else
                return tempInst->data.iType;
        }       
    }

    return "";
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