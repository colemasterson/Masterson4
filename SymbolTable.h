/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                      *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
DESCRIPTION : SymbolTable.h is where the SymbolTable class is defined.
A SymbolTable object has 2 member values: a string path, and a  
NodePtr root. SymbolTable.h contains 3 structs, 3 constructors, a  
destructor, 11 public methods, and 8 private methods.         
********************************************************************/

#ifndef SymbolTable_H
#define SymbolTable_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;


struct Node;
typedef Node* NodePtr;

// Contains data to be inserted into a Node struct
struct SymbolNode
{
    string symbol, hexValue;
    int value;
    bool rFlag, iFlag, mFlag;
};

// Contains data read from file that will be converted into SymbolNode format
struct Line
{
    string name, flag, val;
    bool rFlag;
    int value;
};

// Struct used in the binary tree
struct Node
{
    SymbolNode data;
    NodePtr left, right;
};

class SymbolTable
{
    public:
        SymbolTable();
        SymbolTable(string);
        SymbolTable(const SymbolTable &);
        ~SymbolTable();

        void insert(const SymbolNode);
        void remove(const SymbolNode);
        NodePtr find(const SymbolNode) const;

        void inOrder() const;
        void inOrderWrite(string);
        void view(const NodePtr) const;
        void pause() const;
        
        string getPath();
        void readFile(string);
        void readSearch();
        void readExpression();
        string flagText(bool);
        
    private:
        NodePtr root;
        string path = "";

        void copy(NodePtr &);
        void destroy(NodePtr &);
        void deleteNode(NodePtr &);
        void inOrder(const NodePtr) const;
        void inOrderWrite(const NodePtr, string) const;

        void findMaxNode(NodePtr &, NodePtr &);
        void insert(const SymbolNode, NodePtr &);
        void remove(const SymbolNode, NodePtr &);
        NodePtr find(const SymbolNode, const NodePtr) const;
        
};

#endif
