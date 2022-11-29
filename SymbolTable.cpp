/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
DESCRIPTION : SymbolTable.cpp is where the SymbolTable class is defined.
A SymbolTable object has 2 member values: a string path, and a     
NodePtr root. SymbolTable.cpp contains 3 structs, 3 constructors, a  
destructor, 10 public methods, and 8 private methods. 
********************************************************************/
#include "SymbolTable.h"
#include "Expressions.h"

using namespace std;

/********************************************************************
*** FUNCTION : SymbolTable() : root(NULL)                              ***
*********************************************************************
*** DESCRIPTION : creates a SymbolTable, initializes the root to null. ***
    getPath is called to initialize path, and readFile is called.
*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
SymbolTable::SymbolTable() : root(NULL)
{
    root = nullptr;
}
/********************************************************************
*** FUNCTION : SymbolTable(string in)                                  ***
*********************************************************************
*** DESCRIPTION : creates a SymbolTable, initializes the root to null. ***
    getPath is called to initialize path, and readFile is called.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
SymbolTable::SymbolTable(string in)
{
    root = nullptr;
    readFile(in);
}
/********************************************************************
*** FUNCTION : SymbolTable(const SymbolTable & inTree)                      ***
*********************************************************************
*** DESCRIPTION : copy constructor for SymbolTable.                    ***

*** INPUT ARGS : const SymbolTable & inTree                            ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
SymbolTable::SymbolTable(const SymbolTable & inTree)
{
    NodePtr newPtr = inTree.root;
    root = nullptr;
    copy(newPtr);
}
/********************************************************************
*** FUNCTION : ~SymbolTable()                                          ***
*********************************************************************
*** DESCRIPTION : Destructor for SymbolTable.                          ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
SymbolTable::~SymbolTable()
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
string SymbolTable::getPath()
{
    string s;
    cout << "Please enter file name:";
    cin >> s;
    cin.ignore();
    return s;
}
/********************************************************************
*** FUNCTION : copy(NodePtr & nodePtr)                            ***
*********************************************************************
*** DESCRIPTION : function that copies all nodes in the tree.     ***

*** INPUT ARGS : NodePtr & nodePtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::copy(NodePtr & nodePtr)
{
    if(nodePtr == nullptr)
        return;
    insert(nodePtr->data);
    copy(nodePtr->left);
    copy(nodePtr->right);

}
/********************************************************************
*** FUNCTION : destroy(NodePtr & nodePtr)                         ***
*********************************************************************
*** DESCRIPTION : deletes all the nodes in the tree.              ***

*** INPUT ARGS : NodePtr & nodePtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::destroy(NodePtr & nodePtr)
{
    if(nodePtr)
    {
        if(nodePtr->left)
        {
            destroy(nodePtr->left);
        }
        if(nodePtr->right)
        {
            destroy(nodePtr->right);
        };
        delete nodePtr;
    }
}
/********************************************************************
*** FUNCTION : string flagText(bool in)                           ***
*********************************************************************
*** DESCRIPTION : converts a boolean value to readable text.      ***

*** INPUT ARGS : bool in                                          ***

*** OUTPUT ARGS : string                                          ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
string SymbolTable::flagText(bool in)
{
    stringstream c;
    c << boolalpha << in;
    return c.str();
}
/********************************************************************
*** FUNCTION : readFile(string in)                                ***
*********************************************************************
*** DESCRIPTION : Takes a string input of the desired file. Next, ***
each line in the file is parsed by a string stream and placed into
the Line struct. Each value is cleaned up to match SymbolNode     ***
format and values are verified by an ErrorCheck object. After 
verification, Line values are assigned to a SymbolNode and        ***
inserted into the tree. Unverified lines are printed as errors.

*** INPUT ARGS : string in                                        ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::readFile(string in)
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
        Line t;
        istringstream ss(tLine);

        while(ss >> t.name >> t.flag >> t.val)
        {
            SymbolNode n;
            ErrorCheck e;

            //scrub contents for error causing values
            t.name = e.nameCheck(t.name);
            t.rFlag = e.flagCheck(t.flag);
            t.value = e.valCheck(t.val);

            // if line is a valid symbol, assign line values to symbol and check if already inserted
            if(e.isValid)
            {
                n.symbol = t.name;
                n.rFlag = t.rFlag;
                n.value = t.value;
                n.iFlag = true;
                n.mFlag = false;
                
                //if symbol is found in table, do not add and create error message
                NodePtr tNode = find(n, root);
                if(tNode == nullptr)
                    insert(n, root);
                else
                {
                    // since symbol has been defined more than once, set m flag
                    e.isValid = false;
                    tNode->data.mFlag = true;
                    e.errorMessage += " ERROR: SYMBOL (" + n.symbol + ") has been previously defined.";
                }
            }
            if(!e.isValid)
                cout << setw(7) << t.name << setw(7) <<  t.flag << setw(7) << t.val << "\t" << e.errorMessage << endl;
        }
    }
    inFile.close();
    cout << "\n\n" << endl;
}
/********************************************************************
*** FUNCTION :  void readSearch()                                 ***
*********************************************************************
*** DESCRIPTION : Asks user to input a file to be opened. Next,   ***
the file contents is read. An ErrorCheck object verifies the names
in the file. If names are valid, the name is searched in the symbol
table. If names are invalid, an error message is displayed. If a
valid symbol name is not found, an error message is displayed.

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::readSearch()
{
    fstream inFile;
    vector<string> test;
    string line = getPath();

    inFile.open(line);

    if(!inFile)
    {
        cout << "Error opening: " + line + ", please enter path again." << endl;
        readSearch();
    }

    while(getline(inFile, line))
    {
        istringstream ss(line);    
        string t;

        while(ss >> t)
            test.push_back(t);
    }
    cout << "Symbol\t" << setw(7) << "Value" << setw(7) << "RFlag" << setw(7) << "IFlag" << setw(7) << "MFlag" << endl;
    for(string i : test)
    {
        SymbolNode temp;    
        ErrorCheck e;
        string newSymbol = e.nameCheck(i);

        if(e.isValid)
        {
            temp.symbol = newSymbol;
            NodePtr node = find(temp);
            if(node == nullptr)
                cout << "Node " + temp.symbol + " not found." << endl;
            else
                view(node);
        }
        else
        {
            cout << e.errorMessage << endl;
        }

    }

    inFile.close();
}
/********************************************************************
*** FUNCTION : view(const NodePtr nodePtr) const                  ***
*********************************************************************
*** DESCRIPTION : Displays all attributes of a Nodes data.        ***

*** INPUT ARGS : const NodePtr nodePtr                            ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::view(const NodePtr nodePtr) const
{
    cout << setw(7) << nodePtr->data.symbol << setw(7) <<  nodePtr->data.hexValue << setw(7) <<
    nodePtr->data.rFlag << setw(7) << nodePtr->data.iFlag << setw(7) << nodePtr->data.mFlag << endl;
}

/********************************************************************
*** FUNCTION : Graph(const string name)                           ***
*********************************************************************
*** DESCRIPTION : creates a Graph, initializes the adjacent matrix
and the visited set array.                                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::insert(const SymbolNode in)
{
    insert(in, root);
}
/********************************************************************
*** FUNCTION : Graph(const string name)                           ***
*********************************************************************
*** DESCRIPTION : creates a Graph, initializes the adjacent matrix
and the visited set array.                                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::remove(const SymbolNode in)
{
    remove(in, root);
}
/********************************************************************
*** FUNCTION : Graph(const string name)                           ***
*********************************************************************
*** DESCRIPTION : creates a Graph, initializes the adjacent matrix
and the visited set array.                                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
NodePtr SymbolTable::find(const SymbolNode in) const
{
    return find(in, root);
}


/********************************************************************
*** FUNCTION : insert(SymbolNode in, NodePtr & nodePtr)           ***
*********************************************************************
*** DESCRIPTION : Takes in a SymbolNode and a NodePtr. The function**
traverses the tree until it finds a Node with the same symbol name
or a nullptr. If a symbol is found with the same name, an error 
message is displayed. If a nullptr is found, the SymbolNode is 
inserted into the tree.

*** INPUT ARGS : SymbolNode in, NodePtr & nodePtr                 ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::insert(SymbolNode in, NodePtr & nodePtr)
{
    if(nodePtr == nullptr)
    {
        nodePtr = new (std::nothrow) Node;

        nodePtr->data = in;
        nodePtr->left = nullptr;
        nodePtr->right = nullptr;
    }
    else if(in.symbol.compare(nodePtr->data.symbol) == 0)
    {
        cout << "ERROR: Symbol (" + nodePtr->data.symbol + ") is already in the table." << endl;
    }
    else if(in.symbol.compare(nodePtr->data.symbol) < 0)
    {
        insert(in, nodePtr->left);
    }
    else if(in.symbol.compare(nodePtr->data.symbol) > 0)
    {
        insert(in, nodePtr->right);
    }
}
/********************************************************************
*** FUNCTION : Graph(const string name)                           ***
*********************************************************************
*** DESCRIPTION : creates a Graph, initializes the adjacent matrix
and the visited set array.                                        ***

*** INPUT ARGS : n/a                                              ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::remove(const SymbolNode in, NodePtr & nodePtr)
{
    if(nodePtr == nullptr)
    {
        cout << "Node not found." << endl;
    }
    else if(in.symbol.compare(nodePtr->data.symbol) == 0)
    {
        deleteNode(nodePtr);
    }
    else if(in.symbol.compare(nodePtr->data.symbol) < 0)
        remove(in, nodePtr->left);
    else
        remove(in, nodePtr->right);
} 
/********************************************************************
*** FUNCTION : deleteNode(NodePtr & nodePtr)                      ***
*********************************************************************
*** DESCRIPTION : deletes a given node and reassigns the pointers
of the parent node to keep the tree continuous.                   ***

*** INPUT ARGS : NodePtr & nodePtr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::deleteNode(NodePtr & nodePtr)
{
    NodePtr tNodePtr = nullptr;

    if(!nodePtr->left && !nodePtr->right)
    {
        nodePtr = nullptr;
        delete nodePtr;
    }
    else if(nodePtr->left && !nodePtr->right)
    {
        tNodePtr = nodePtr;
        nodePtr = nodePtr->left;
        delete tNodePtr;
    }
    else if(!nodePtr->left && nodePtr->right)
    {
        tNodePtr = nodePtr;
        nodePtr = nodePtr->right;
        delete tNodePtr;
    }
    else
    {
        findMaxNode(nodePtr->left, tNodePtr);
        nodePtr->data = tNodePtr->data;
        tNodePtr = nullptr;
        delete tNodePtr;
    }
}
/********************************************************************
*** FUNCTION : findMaxNode(NodePtr & nodePtr, NodePtr & temp)     ***
*********************************************************************
*** DESCRIPTION : Finds the maximum node in the tree.             ***

*** INPUT ARGS : NodePtr & nodePtr, NodePtr & temp                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::findMaxNode(NodePtr & nodePtr, NodePtr & temp)
{
    if(!nodePtr->right)
    {
        temp = nodePtr;
        nodePtr = nodePtr->left;
        return;
    }
    else
    {
        findMaxNode(nodePtr->right, temp);
    }
}
/********************************************************************
*** FUNCTION : find(const SymbolNode s, NodePtr nodePtr) const    ***
*********************************************************************
*** DESCRIPTION : Traverses the tree looking for a node with the
same name as the SymbolNode input, and returns the pointer of it. ***

*** INPUT ARGS : const SymbolNode s, NodePtr nodePtr              ***

*** OUTPUT ARGS : NodePtr                                         ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
NodePtr SymbolTable::find(const SymbolNode s, NodePtr nodePtr) const
{
    if(nodePtr == nullptr)
    {
        return nullptr;
    }
    else if(nodePtr->data.symbol == s.symbol)
    {
        return(nodePtr);
    }
    else if(nodePtr->data.symbol > s.symbol)
    {
        return find(s, nodePtr->left);
    }
    else
    {
        return find(s, nodePtr->right);
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
void SymbolTable::inOrder() const
{
    inOrder(root);
}

/********************************************************************
*** FUNCTION : inOrder(const NodePtr ptr) const                   ***
*********************************************************************
*** DESCRIPTION : Performs an inorder traversal of the tree,
pausing every 20 lines to avoid data scrolling off the screen.    ***

*** INPUT ARGS : const NodePtr ptr                                ***

*** OUTPUT ARGS : n/a                                             ***

*** IN/OUT ARGS : n/a                                             ***

*** RETURN : n/a                                                  ***
********************************************************************/
void SymbolTable::inOrder(const NodePtr nodePtr) const
{
    stack<NodePtr> s;
    NodePtr tNode = nodePtr;
    int i = 0;
    while ((tNode != nullptr || s.empty() == false) && i < 20)
    {
        if(i == 0)
            cout << "LABEL\t" << setw(7) << "VALUE" << setw(7) << "RFLAG" << setw(7) << "IFLAG" << setw(7) << "MFLAG\n" << endl;


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
void SymbolTable::pause() const
{
    cout << "Enter any key to continue...";
    getchar();
}
