/******************************************************************** 
***  NAME        : Cole Masterson                                 *** 
***  CLASS       : CSC 354                                        *** 
***  ASSIGNMENT  : 3                                              *** 
***  DUE DATE    : 11/2/2022                                     *** 
***  INSTRUCTOR  : GAMRADT                                        *** 
********************************************************************* 
DESCRIPTION : Instructions.h is where the Instructions class is defined.
A Instructions object has 2 member values: a string path, and a  
InstPtr root. Instructions.h contains 2 structs, 3 constructors, a  
destructor, 9 public methods, and 8 private methods.         
********************************************************************/
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

struct InstNode;
typedef InstNode* InstPtr;

//struct that holds instruction name, format type, and opcode values
struct Instruction
{
    string name;
    string iType;
    string opcode;
};

//node pointing to instruction for use in binary tree
struct InstNode
{
    Instruction data;
    InstPtr left, right;
};

class Instructions
{
    public:
        Instructions();
        Instructions(string);
        ~Instructions();

        void insert(const Instruction);
        void remove(const Instruction);
        InstPtr find(const Instruction) const;
        InstPtr find(const string) const;

        void inOrder() const;
        void view(const InstPtr) const;
        void pause() const;
        
        string getPath();
        void readFile(string);
        
    private:
        InstPtr root;
        string path = "";

        void copy(InstPtr &);
        void destroy(InstPtr &);
        void deleteNode(InstPtr &);
        void inOrder(const InstPtr) const;

        void findMaxNode(InstPtr &, InstPtr &);
        void insert(const Instruction, InstPtr &);
        void remove(const Instruction, InstPtr &);
        InstPtr find(const Instruction, const InstPtr) const;
        
};
#endif