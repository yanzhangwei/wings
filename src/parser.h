#ifndef _PARSER_H
#define _PARSER_H
#pragma once

#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "ast_expr_literal.h"
#include <vector>
#include <optional>

class Parser {
public:
    Parser(Scanner* sc);
    ~Parser();

    ASTProgram* parseProgram();
private:
    Expr* parseSourceElement();
    Expr* parseStatement();
    Expr* parseBlock();
    Expr* parseVariableSatement();
    Expr* parseEmptyStatement();
    Expr* parseExpressionStatement();
    Expr* parseIfStatement();
    Expr* parseForStatement();
    Expr* parseDoWhileStatement();
    Expr* parseWhileStatement();
    Expr* parseContinueStatement();
    Expr* parseBreakStatement();
    Expr* parseReturnStatement();
    Expr* parseWithStatement();
    // Expr* parseLabelledStatement();
    Expr* parseSwitchStatement();
    Expr* parseThrowStatement();
    Expr* parseTryStatement();
    Expr* parseDebuggerStatement();

    Expr* parsePrimaryExpression();
    Identifier* parseIdentifier();
    Literal* parseLiteral();

    Expr* parseArrayLiteral();
    Expr* parseElementList();
    Expr* parseElision();

    Expr* parseObjectLiteral();

    Expr* parseExpression();

    Expr* parseMemberExpression();
    Expr* parseNewExpression();
    Expr* parseCallExpression(MemberExpr* e);
    ExprPtrVec parseArguments();
    ExprPtrVec parseArgumentList();
    Expr* parseLeftHandSideExpression();
    Expr* parsePostfixExpression();
    Expr* parseUnaryExpression();
    Expr* parseMultiplicativeExpression();
    Expr* parseAdditiveExpression();
    Expr* parseShiftExpression();
    Expr* parseRelationalExpression(bool noIn = false);
    Expr* parseRelationalExpressionNoIn();
    Expr* parseEqualityExpression(bool noIn = false);
    Expr* parseEqualityExpressionNoIn();
    Expr* parseBitwiseANDExpression(bool noIn = false);
    Expr* parseBitwiseANDExpressionNoIn();
    Expr* parseBitwiseXORExpression(bool noIn = false);
    Expr* parseBitwiseXORExpressionNoIn();
    Expr* parseBitwiseORExpression(bool noIn = false);
    Expr* parseBitwiseORExpressionNoIn();
    Expr* parseLogicalANDExpression(bool noIn = false);
    Expr* parseLogicalANDExpressionNoIn();
    Expr* parseLogicalORExpression(bool noIn = false);
    Expr* parseLogicalORExpressionNoIn();
    Expr* parseConditionalExpression(bool noIn = false);
    Expr* parseConditionalExpressionNoIn();
    Expr* parseAssignmentExpression(bool noIn = false);
    Expr* parseAssignmentExpressionNoIn();
    
    Expr* parseVariableDeclarationList(bool noIn = false);
    Expr* parseVariableDeclarationListNoIn();
    Expr* parseVariableDeclaration(bool noIn = false);
    Expr* parseVariableDeclarationNoIn();
    Expr* parseInitialiser(bool noIn = false);
    Expr* parseInitialiserNoIn();

    Expr* parsePropertyNameAndValueList();
    Expr* parsePropertyAssignment();
    Expr* parsePropertyName();

    Expr* parseFunctionDeclaration();
    Expr* parseFunctionExpression();
    std::vector<Identifier*> parseFormalParameterList();
    Expr* parseFunctionBody();

    void next();
    Token getCurrentToken();
    bool match(const TokenType& t, bool next = true);

    bool isAssignmentOperator();
    bool isLiteral();
    bool isPropertyName();
    bool isUnaryOperator();
private:
    Token token;
    Scanner *scanner;
    bool rollback;
    bool record;
    TokenSet tokenBuffer;
};
#endif