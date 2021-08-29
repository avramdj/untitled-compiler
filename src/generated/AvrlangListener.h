
    #include <iostream>
    #include <ast.hpp>
    using namespace std;
    using namespace llvm;


// Generated from Avrlang.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "AvrlangParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by AvrlangParser.
 */
class  AvrlangListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterFile(AvrlangParser::FileContext *ctx) = 0;
  virtual void exitFile(AvrlangParser::FileContext *ctx) = 0;

  virtual void enterGlobalstmt(AvrlangParser::GlobalstmtContext *ctx) = 0;
  virtual void exitGlobalstmt(AvrlangParser::GlobalstmtContext *ctx) = 0;

  virtual void enterFunctiondef(AvrlangParser::FunctiondefContext *ctx) = 0;
  virtual void exitFunctiondef(AvrlangParser::FunctiondefContext *ctx) = 0;

  virtual void enterParamlist(AvrlangParser::ParamlistContext *ctx) = 0;
  virtual void exitParamlist(AvrlangParser::ParamlistContext *ctx) = 0;

  virtual void enterBlock(AvrlangParser::BlockContext *ctx) = 0;
  virtual void exitBlock(AvrlangParser::BlockContext *ctx) = 0;

  virtual void enterStmt(AvrlangParser::StmtContext *ctx) = 0;
  virtual void exitStmt(AvrlangParser::StmtContext *ctx) = 0;

  virtual void enterAssign(AvrlangParser::AssignContext *ctx) = 0;
  virtual void exitAssign(AvrlangParser::AssignContext *ctx) = 0;

  virtual void enterRet(AvrlangParser::RetContext *ctx) = 0;
  virtual void exitRet(AvrlangParser::RetContext *ctx) = 0;

  virtual void enterWhileloop(AvrlangParser::WhileloopContext *ctx) = 0;
  virtual void exitWhileloop(AvrlangParser::WhileloopContext *ctx) = 0;

  virtual void enterIf_expr(AvrlangParser::If_exprContext *ctx) = 0;
  virtual void exitIf_expr(AvrlangParser::If_exprContext *ctx) = 0;

  virtual void enterIfelse_expr(AvrlangParser::Ifelse_exprContext *ctx) = 0;
  virtual void exitIfelse_expr(AvrlangParser::Ifelse_exprContext *ctx) = 0;

  virtual void enterExpr(AvrlangParser::ExprContext *ctx) = 0;
  virtual void exitExpr(AvrlangParser::ExprContext *ctx) = 0;

  virtual void enterArglist(AvrlangParser::ArglistContext *ctx) = 0;
  virtual void exitArglist(AvrlangParser::ArglistContext *ctx) = 0;

  virtual void enterNeg(AvrlangParser::NegContext *ctx) = 0;
  virtual void exitNeg(AvrlangParser::NegContext *ctx) = 0;

  virtual void enterBinoperator(AvrlangParser::BinoperatorContext *ctx) = 0;
  virtual void exitBinoperator(AvrlangParser::BinoperatorContext *ctx) = 0;


};

