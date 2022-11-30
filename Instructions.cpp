/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 4                                              *** 
***  DUE DATE    : 11/30/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
*********************************************************************
DESCRIPTION : Instructions.cpp is where the Instructions class is defined.
A Instructions object has 2 member values: a string path, and a     
InstPtr root. Instructions.cpp contains 3 structs, 3 constructors, a  
destructor, 10 public methods, and 8 private methods. 
********************************************************************/
#include "Instructions.h"
#include "Expressions.h"

using namespace std;

/********************************************************************
*** FUNCTION : Instructions() : root(NULL)                              ***
*********************************************************************
*** DESCRIPTION : creates a Instructions, initializes the root to null. ***
    getPath is called to initialize path, and readFile is called.
*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Instructions::Instructions() : root(NULL)
{
    root = nullptr;
    readFile("instructions.dat");
}
/********************************************************************
*** FUNCTION : Instructions(string in)                                  ***
*********************************************************************
*** DESCRIPTION : creates a Instructions, initializes the root to null. ***
    getPath is called to initialize path, and readFile is called.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Instructions::Instructions(string in)
{
    root = nullptr;
    readFile(in);
}


/********************************************************************
*** FUNCTION : ~Instructions()                                          ***
*********************************************************************
*** DESCRIPTION : Destructor for Instructions.                          ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
Instructions::~Instructions()
{

}
/********************************************************************
*** FUNCTION : string getPath()                                   ***
*********************************************************************
*** DESCRIPTION : asks user for file name and outputs user entry. ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string Instructions::getPath()
{
    string s;
    cout << "Please enter file name:";
    cin >> s;
    cin.ignore();
    return s;
}
/********************************************************************
*** FUNCTION : copy(InstPtr & InstPtr)                            ***
*********************************************************************
*** DESCRIPTION : function that copies all nodes in the tree.     ***

*** INPUT ARGS : InstPtr & InstPtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::copy(InstPtr & InstPtr)
{
    if(InstPtr == nullptr)
        return;
    insert(InstPtr->data);
    copy(InstPtr->left);
    copy(InstPtr->right);

}
/********************************************************************
*** FUNCTION : destroy(InstPtr & InstPtr)                         ***
*********************************************************************
*** DESCRIPTION : deletes all the nodes in the tree.              ***

*** INPUT ARGS : InstPtr & InstPtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::destroy(InstPtr & InstPtr)
{
    if(InstPtr)
    {
        if(InstPtr->left)
        {
            destroy(InstPtr->left);
        }
        if(InstPtr->right)
        {
            destroy(InstPtr->right);
        };
        delete InstPtr;
    }
}


/********************************************************************
*** FUNCTION : readFile(string in)                                ***
*********************************************************************
*** DESCRIPTION : Takes a string input of the desired file. Next, ***
each line in the file is parsed by a string stream and placed into
the Line struct. Each value is cleaned up to match Instruction     ***
format and values are verified by an ErrorCheck object. After 
verification, Line values are assigned to a Instruction and        ***
inserted into the tree. Unverified lines are printed as errors.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::readFile(string in)
{
    // file stream for opening symbol file, vector to store file contents before errorchecking,
    // string to ensure line is not EOF, ErrorCheck to verify content quality before adding to tree.

    fstream inFile;
    string tLine;

    // open file
    inFile.open(in);

    // if file doesn't exist, output error to user and ask for user to input file name
    if(!inFile)
    {
        cout << "File: " + in + " was not found. Please enter file name again:" << endl;
        getline(cin, tLine);
        readFile(tLine);
    }

    // while file is open..
    //cout << setw(7) << "Symbol:" << setw(7) << "RFlag:" << setw(7) << "Value:"  << endl;
    while(getline(inFile, tLine))
    {
        //get line contents and insert into line struct
        Instruction t;
        ErrorCheck e;
        istringstream ss(tLine);

        while(ss >> t.name >> t.iType >> t.opcode)
        {
            //if symbol is found in table, do not add and create error message
            InstPtr tNode = find(t, root);
            if(tNode == nullptr)
                insert(t, root);
            else
                e.errorMessage += " ERROR: Instruction (" + t.name + ") has been previously defined.";

        }
    }
    inFile.close();
    cout << "\n\n" << endl;
}


/********************************************************************
*** FUNCTION : view(const InstPtr InstPtr) const                  ***
*********************************************************************
*** DESCRIPTION : Displays all attributes of a Nodes data.        ***

*** INPUT ARGS : const InstPtr InstPtr                            ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::view(const InstPtr InstPtr) const
{
    cout << setw(7) << InstPtr->data.name << setw(7) <<  InstPtr->data.iType << setw(7) << InstPtr->data.opcode << endl;
}

/********************************************************************
*** FUNCTION : insert(const Instruction in)                        ***
*********************************************************************
*** DESCRIPTION : public method to insert an instruction into the list.

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::insert(const Instruction in)
{
    insert(in, root);
}
/********************************************************************
*** FUNCTION : remove(const Instruction in)                       ***
*********************************************************************
*** DESCRIPTION : public method to remove an instruction from the list.

*** INPUT ARGS : Instruction                                      ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::remove(const Instruction in)
{
    remove(in, root);
}
/********************************************************************
*** FUNCTION : find(const Instruction in) const                   ***
*********************************************************************
*** DESCRIPTION : finds an instruction in the tree.               ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
InstPtr Instructions::find(const Instruction in) const
{
    return find(in, root);
}

InstPtr Instructions::find(const string in) const
{
    Instruction i;
    i.name = in;

    return find(i, root);
}


/********************************************************************
*** FUNCTION : insert(Instruction in, InstPtr & InstPtr)           ***
*********************************************************************
*** DESCRIPTION : Takes in a Instruction and a InstPtr. The function**
traverses the tree until it finds a Node with the same symbol name
or a nullptr. If a symbol is found with the same name, an error 
message is displayed. If a nullptr is found, the Instruction is 
inserted into the tree.

*** INPUT ARGS : Instruction in, InstPtr & InstPtr                 ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::insert(Instruction in, InstPtr & InstPtr)
{
    if(InstPtr == nullptr)
    {
        InstPtr = new (std::nothrow) InstNode;

        InstPtr->data = in;
        InstPtr->left = nullptr;
        InstPtr->right = nullptr;
    }
    else if(in.name.compare(InstPtr->data.name) == 0)
    {
        cout << "ERROR: Instruction (" + InstPtr->data.name + ") is already defined." << endl;
    }
    else if(in.name.compare(InstPtr->data.name) < 0)
    {
        insert(in, InstPtr->left);
    }
    else if(in.name.compare(InstPtr->data.name) > 0)
    {
        insert(in, InstPtr->right);
    }
}
/********************************************************************
*** FUNCTION : remove(const Instruction in, InstPtr & InstPtr)    ***
*********************************************************************
*** DESCRIPTION : removes an instruction from the tree.           ***

*** INPUT ARGS : Instruction, InstPtr                             ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::remove(const Instruction in, InstPtr & InstPtr)
{
    if(InstPtr == nullptr)
    {
        cout << "Node not found." << endl;
    }
    else if(in.name.compare(InstPtr->data.name) == 0)
    {
        deleteNode(InstPtr);
    }
    else if(in.name.compare(InstPtr->data.name) < 0)
        remove(in, InstPtr->left);
    else
        remove(in, InstPtr->right);
} 
/********************************************************************
*** FUNCTION : deleteNode(InstPtr & InstPtr)                      ***
*********************************************************************
*** DESCRIPTION : deletes a given node and reassigns the pointers
of the parent node to keep the tree continuous.                   ***

*** INPUT ARGS : InstPtr & InstPtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::deleteNode(InstPtr & instPtr)
{
    InstPtr tInstPtr = nullptr;

    if(!instPtr->left && !instPtr->right)
    {
        instPtr = nullptr;
        delete instPtr;
    }
    else if(instPtr->left && !instPtr->right)
    {
        tInstPtr = instPtr;
        instPtr = instPtr->left;
        delete tInstPtr;
    }
    else if(!instPtr->left && instPtr->right)
    {
        tInstPtr = instPtr;
        instPtr = instPtr->right;
        delete tInstPtr;
    }
    else
    {
        findMaxNode(instPtr->left, tInstPtr);
        instPtr->data = tInstPtr->data;
        tInstPtr = nullptr;
        delete tInstPtr;
    }
}
/********************************************************************
*** FUNCTION : findMaxNode(InstPtr & InstPtr, InstPtr & temp)     ***
*********************************************************************
*** DESCRIPTION : Finds the maximum node in the tree.             ***

*** INPUT ARGS : InstPtr & InstPtr, InstPtr & temp                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::findMaxNode(InstPtr & instPtr, InstPtr & temp)
{
    if(!instPtr->right)
    {
        temp = instPtr;
        instPtr = instPtr->left;
        return;
    }
    else
    {
        findMaxNode(instPtr->right, temp);
    }
}
/********************************************************************
*** FUNCTION : find(const Instruction s, InstPtr InstPtr) const    ***
*********************************************************************
*** DESCRIPTION : Traverses the tree looking for a node with the
same name as the Instruction input, and returns the pointer of it. ***

*** INPUT ARGS : const Instruction s, InstPtr InstPtr              ***

*** OUTPUT ARGS : InstPtr                                         ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
InstPtr Instructions::find(const Instruction s, InstPtr InstPtr) const
{
    if(InstPtr == nullptr)
    {
        return nullptr;
    }
    else if(InstPtr->data.name == s.name)
    {
        return(InstPtr);
    }
    else if(InstPtr->data.name > s.name)
    {
        return find(s, InstPtr->left);
    }
    else
    {
        return find(s, InstPtr->right);
    }
}
/********************************************************************
*** FUNCTION : inOrder() const                                    ***
*********************************************************************
*** DESCRIPTION : Public method of the in order traversal method. ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::inOrder() const
{
    cout << "MNEMONIC" << setw(7) << "FORMAT" << setw(7) << "OPCODE (HEX)" << endl;
    inOrder(root);
}

/********************************************************************
*** FUNCTION : inOrder(const InstPtr ptr) const                   ***
*********************************************************************
*** DESCRIPTION : Performs an inorder traversal of the tree,
pausing every 20 lines to avoid data scrolling off the screen.    ***

*** INPUT ARGS : const InstPtr ptr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::inOrder(const InstPtr instPtr) const
{
    stack<InstPtr> s;
    InstPtr tNode = instPtr;
    int i = 0;
    while ((tNode != nullptr || s.empty() == false) && i < 20)
    {
        if(i == 0)
                cout << "Symbol\t" << setw(7) << "Value" << setw(7) << "RFlag" << setw(7) << "IFlag" << setw(7) << "MFlag\n" << endl;


        while (tNode != nullptr)
        {
            s.push(tNode);
            tNode = tNode->left;
        }        

        tNode = s.top();
        s.pop();
        view(tNode);

        tNode = tNode->right;
        i++;

        if(i == 20)
        {
            pause();
            i = 0;
        }
    }
}
/********************************************************************
*** FUNCTION : pause() const                                      ***
*********************************************************************
*** DESCRIPTION : Halts the program until the enter key is pressed.**

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void Instructions::pause() const
{
    cout << "Enter any key to continue...";
    getchar();
}
