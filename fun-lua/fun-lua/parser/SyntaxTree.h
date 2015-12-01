//
//  SyntaxTree.h
//  fun-lua
//
//  Created by neo on 15-11-30.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__SyntaxTree__
#define __fun_lua__SyntaxTree__
#include <vector>
#include <memory>
namespace Parser {
    class SyntaxTree{
    public:
        virtual ~SyntaxTree(){};
        
    };
    
    class Statement : public SyntaxTree{
        
    };
    
    
    class Block : public SyntaxTree{
        std::vector<std::unique_ptr<Statement>> _stmts;
        std::unique_ptr<Statement> _retStmt;
    };
    
    class NameList : public SyntaxTree
    {
        
    };
    
    class Var : public SyntaxTree
    {
        
    };
    
    class VarList : public SyntaxTree
    {
        
    };
    
    class Exp : public SyntaxTree
    {
        
    };
    
    class TableConstructor : public Exp
    {
        
    };
    
    class BinOpExp : public Exp
    {
        
    };
    class UnopExp : public Exp
    {
        
    };
    class PrefixExp : public Exp
    {
        
    };
    
    class ExpList : public SyntaxTree
    {
        
    };
    
    
    class AssignStatment : public Statement
    {
        
    };
    
    class LocalAssignStatement : public Statement
    {
        
    };
    
    class FunCallStatment : public Statement
    {
        
    };
    
    class DoStatement : public Statement
    {
        
    };
    
    class WhileStatement : public Statement
    {
        
    };
    
    class RepeateStatement : public Statement
    {
        
    };
    
    class ForStatement : public Statement
    {
        
    };
    
    class ForInStatement : public Statement
    {
        
    };
    
    class IfStatement : public Statement
    {
        
    };
    
    class FunStatement : public Statement
    {
        
    };
    
    class LocalFunStatement : public Statement
    {
        
    };
    
    
}

#endif /* defined(__fun_lua__SyntaxTree__) */
