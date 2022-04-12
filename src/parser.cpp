#include "parser.h"
#include "ast.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "token.h"
#include <cstdlib>
#include <cstring>
#include <optional>
#include <variant>
#include <vector>

Parser::Parser(Scanner* sc): scanner(sc), rollback(false),
        record(false), token(std::move(sc->scanToken())) {
}

Parser::~Parser() { delete scanner; }

ASTProgram* Parser::parseProgram() {
    ASTProgram* root = new ASTProgram();
    while (!match(TokenType::TOKEN_EOF)) {
        root->push_back(parseSourceElement());
    }
    return root;
}

Expr* Parser::parseSourceElement() {
    if (match(TokenType::TOKEN_FUNCTION)) {
        return parseFunctionDeclaration();
    } else {
        return parseStatement();
    }
}

Expr* Parser::parseStatement()  {
    if (match(TokenType::TOKEN_LEFT_BRACE)) {
        return parseBlock();
    } else if (match(TokenType::TOKEN_VAR, false)) {
        return parseVariableSatement();
    } else if (match(TokenType::TOKEN_IF)) {
        return parseIfStatement();
    } else if (match(TokenType::TOKEN_DO)) {
        return parseDoWhileStatement();
    } else if (match(TokenType::TOKEN_WHILE)) {
        return parseWhileStatement();
    } else if (match(TokenType::TOKEN_FOR)) {
        return parseForStatement();
    } else if (match(TokenType::TOKEN_CONTINUE)) {
        return parseContinueStatement();
    } else if (match(TokenType::TOKEN_BREAK)) {
        return parseBreakStatement();
    } else if (match(TokenType::TOKEN_RETURN)) {
        return parseReturnStatement();
    } else if (match(TokenType::TOKEN_WITH)) {
        return parseWithStatement();
    } /*else if (match(TokenType::TOKEN_IDENTIFIER)) {
        return parseLabelledStatement();
    }*/ else if (match(TokenType::TOKEN_SWITCH)) {
        return parseSwitchStatement();
    } else if (match(TokenType::TOKEN_THROW)) {
        return parseThrowStatement();
    } else if (match(TokenType::TOKEN_TRY)) {
        return parseTryStatement();
    } else if (match(TokenType::TOKEN_DEBUGGER)) {
        return parseDebuggerStatement();
    } else {
        return parseExpressionStatement();
    }
}

Expr* Parser::parseBlock() {
    BlockStmt* s = new BlockStmt;
    while (!match(TokenType::TOKEN_RIGHT_BRACE))
        s->push_back(parseStatement());
    match(TokenType::TOKEN_RIGHT_BRACE);
    return s;
}

Expr* Parser::parseVariableSatement() {
    Expr* e = parseVariableDeclarationList();
    match(TokenType::TOKEN_SEMICOLON);;
    return e;
}

Expr* Parser::parseEmptyStatement() {
    match(TokenType::TOKEN_SEMICOLON);
    return nullptr;
}

Expr* Parser::parseExpressionStatement() {
    Expr* e = parseExpression();
    if (match(TokenType::TOKEN_COLON)) {
        return new LabelStmt(e, parseStatement());
    }
    match(TokenType::TOKEN_SEMICOLON);
    return e;
}

Expr* Parser::parseIfStatement() {
    IfStmt* s = new IfStmt;
    match(TokenType::TOKEN_LEFT_PAREN);
    s->setTest(parseExpression());
    match(TokenType::TOKEN_RIGHT_PAREN);
    s->setConsequent(parseStatement());
    if (match(TokenType::TOKEN_ELSE)) {
        s->setAlternate(parseStatement());
    }
    return s;
}

Expr* Parser::parseForStatement() {
    match(TokenType::TOKEN_LEFT_PAREN);
    if (match(TokenType::TOKEN_VAR, false)) {
        Expr* e = parseVariableDeclarationList(true);
        if (match(TokenType::TOKEN_IN)) {
            Expr* e1 = parseExpression();
            if (match(TokenType::TOKEN_RIGHT_PAREN)) {
                return new ForInStmt(e, e1, parseStatement());
            }
        } else if (match(TokenType::TOKEN_SEMICOLON)) {
            Expr* e1 = parseExpression();
            match(TokenType::TOKEN_SEMICOLON);
            Expr* e2 = parseExpression();
            match(TokenType::TOKEN_RIGHT_PAREN);
            return new ForStmt(e, e1, e2, parseStatement());
        }
    }

    Expr* e = parseExpression();
    if (match(TokenType::TOKEN_SEMICOLON)) {
        Expr* e1 = parseExpression();
        match(TokenType::TOKEN_SEMICOLON);
        Expr* e2 = parseExpression();
        match(TokenType::TOKEN_RIGHT_PAREN);
        return new ForStmt(e, e1, e2, parseStatement());
    } else if (match(TokenType::TOKEN_IN)) {
        Expr* e1 = parseExpression();
        match(TokenType::TOKEN_RIGHT_PAREN);
        return new ForInStmt(e, e1, parseStatement());
    }
}

Expr* Parser::parseDoWhileStatement() {
    match(TokenType::TOKEN_LEFT_BRACE);
    Expr* e = parseStatement();
    match(TokenType::TOKEN_RIGHT_BRACE);
    match(TokenType::TOKEN_WHILE);
    match(TokenType::TOKEN_LEFT_PAREN);
    Expr* e1 = parseExpression();
    match(TokenType::TOKEN_RIGHT_PAREN);
    match(TokenType::TOKEN_SEMICOLON);
    return new DoWhileStmt(e1, e);
}

Expr* Parser::parseWhileStatement() {
    match(TokenType::TOKEN_LEFT_PAREN);
    Expr* e = parseExpression();
    match(TokenType::TOKEN_RIGHT_PAREN);
    return new WhileStmt(e, parseStatement());
}

Expr* Parser::parseContinueStatement() {
    if (match(TokenType::TOKEN_SEMICOLON)) {
        Expr* e = new ContinueStmt();
        match(TokenType::TOKEN_SEMICOLON);
        return e; 
    }
    Expr* e = new ContinueStmt(parseIdentifier());
    match(TokenType::TOKEN_SEMICOLON);
    return e; 
}
Expr* Parser::parseBreakStatement() {
    if (match(TokenType::TOKEN_SEMICOLON)) {
        Expr* e = new BreakStmt();
        match(TokenType::TOKEN_SEMICOLON);
        return e; 
    }
    Expr* e = new BreakStmt(parseIdentifier());
    match(TokenType::TOKEN_SEMICOLON);
    return e;
}

Expr* Parser::parseReturnStatement() {
    ReturnStmt* rs = new ReturnStmt;
    if (!match(TokenType::TOKEN_SEMICOLON)) 
        rs->setExpr(parseExpression());
    match(TokenType::TOKEN_SEMICOLON);
    return rs;
}

Expr* Parser::parseWithStatement() {

}
// Expr* Parser::parseLabelledStatement() {

// }
Expr* Parser::parseSwitchStatement() {

}
Expr* Parser::parseThrowStatement() {

}
Expr* Parser::parseTryStatement() {

}
Expr* Parser::parseDebuggerStatement() {

}

Expr* Parser::parsePrimaryExpression() {
    /*if (match(TokenType::TOKEN_THIS, false)) {

    } else*/ if (match(TokenType::TOKEN_LEFT_BRACKET)) {
        return parseArrayLiteral();
    } else if (match(TokenType::TOKEN_LEFT_BRACE)) {
        return parseObjectLiteral();
    } else if (match(TokenType::TOKEN_LEFT_PAREN)) {
        return parseExpression();
    } else if (match(TokenType::TOKEN_IDENTIFIER, false)) {
        return parseIdentifier();
    } else if (isLiteral()) {
        return parseLiteral();
    }
}

Identifier* Parser::parseIdentifier() {
    Identifier* tmp = new Identifier(token.getString());
    next();
    return tmp;
}

Literal* Parser::parseLiteral() {
    Literal* tmp = new Literal(std::strtod(token.getStart(), nullptr));
    next();
    return tmp;
}

Expr* Parser::parseArrayLiteral() {
    ArrayExpr* e = new ArrayExpr;
    while (!match(TokenType::TOKEN_RIGHT_BRACKET)) {
        while (match(TokenType::TOKEN_COMMA)) e->push_back(new Literal(std::monostate()));
        e->push_back(parseAssignmentExpression());
    }
    return e;
}

Expr* Parser::parseObjectLiteral() {
    ObjectExpr* e;
    if (match(TokenType::TOKEN_RIGHT_BRACE)) return e;
    // if (isPropertyName()) {
    //     Property* p = new Property;
        
    // }
    parsePropertyNameAndValueList();
    match(TokenType::TOKEN_COMMA);
    match(TokenType::TOKEN_RIGHT_BRACE);
    return e;
}

Expr* Parser::parseExpression() {
    return parseAssignmentExpression();
    while (match(TokenType::TOKEN_COMMA)) {
        parseAssignmentExpression();
    }
}

Expr* Parser::parseMemberExpression() {
    Expr* e = parsePrimaryExpression();
    MemberExpr* res = new MemberExpr;
    bool first = true;
    while (match(TokenType::TOKEN_DOT, false) ||
           match(TokenType::TOKEN_LEFT_BRACKET, false)) {
        MemberExpr* me = new MemberExpr;
        if (!first) {
            me->setObject(res);
        } else {
            me->setObject(e);
            first = false;
        }
        if (match(TokenType::TOKEN_DOT))
            me->setProperty(parseIdentifier());
        if (match(TokenType::TOKEN_LEFT_BRACKET)) {
            me->setProperty(parseExpression());
            match(TokenType::TOKEN_RIGHT_BRACKET);
        }
        res = me;
    }
    return first ? e : res;
}

Expr* Parser::parseNewExpression() {
    while (match(TokenType::TOKEN_NEW)) ;
    return parseMemberExpression();
}

Expr* Parser::parseCallExpression(Expr* e) {
    CallExpr* call = new CallExpr;
    call->setCallee(e);
    call->setArgs(parseArguments());
    Expr* res = call;
    while (match(TokenType::TOKEN_DOT, false) ||
           match(TokenType::TOKEN_LEFT_PAREN, false) ||
           match(TokenType::TOKEN_LEFT_BRACKET, false)) {
        if (match(TokenType::TOKEN_LEFT_PAREN)) {
            CallExpr* e = new CallExpr;
            e->setCallee(res);
            e->setArgs(parseArguments());
            res = e;
        }
        if (match(TokenType::TOKEN_DOT)) {
            MemberExpr* e = new MemberExpr;
            e->setObject(res);
            e->setProperty(parseIdentifier());
            res = e;
        }
        if (match(TokenType::TOKEN_LEFT_BRACKET)) {
            MemberExpr* e = new MemberExpr;
            e->setObject(res);
            e->setProperty(parseExpression());
            match(TokenType::TOKEN_RIGHT_BRACKET);
            res = e;
        }
    }
    return res;
}

ExprPtrVec Parser::parseArguments() {
    match(TokenType::TOKEN_LEFT_PAREN);
    ExprPtrVec vec;
    if (match(TokenType::TOKEN_RIGHT_PAREN)) return vec;
    vec = parseArgumentList();
    match(TokenType::TOKEN_RIGHT_PAREN);
    return vec;
}

ExprPtrVec Parser::parseArgumentList() {
    ExprPtrVec vec;
    vec.emplace_back(parseAssignmentExpression());
    while (match(TokenType::TOKEN_COMMA)) {
        vec.emplace_back(parseAssignmentExpression());
    }
    return vec;
}

Expr* Parser::parseLeftHandSideExpression() {
    Expr* e = parseNewExpression();
    if (match(TokenType::TOKEN_LEFT_PAREN, false)) {
        return parseCallExpression(e);
    } 
    return e;
}

Expr* Parser::parsePostfixExpression() {
    Expr* e = parseLeftHandSideExpression();
    if (match(TokenType::TOKEN_DPLUS, false) ||
        match(TokenType::TOKEN_DMINUS, false)) {
        e = new UnaryExpr(getCurrentToken(), e, false);
        next();
        return e;
    }
    return e;
}

Expr* Parser::parseUnaryExpression() {
    if (!isUnaryOperator())
        return parsePostfixExpression();
    Token t = getCurrentToken();
    next();
    Expr* e = new UnaryExpr(t, parsePostfixExpression());
    while (isUnaryOperator()) {
        t = getCurrentToken();
        next();
        e = new UnaryExpr(t, e);
    }
    return e;
}

Expr* Parser::parseMultiplicativeExpression() {
    Expr* LHS = parseUnaryExpression();
    while (match(TokenType::TOKEN_MUL, false) ||
           match(TokenType::TOKEN_DIV, false) ||
           match(TokenType::TOKEN_MOD, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseUnaryExpression();
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseAdditiveExpression() {
    Expr* LHS = parseMultiplicativeExpression();
    while (match(TokenType::TOKEN_PLUS, false) ||
           match(TokenType::TOKEN_MINUS, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseMultiplicativeExpression();
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseShiftExpression() {
    Expr* LHS = parseAdditiveExpression();
    while (match(TokenType::TOKEN_DLESS, false) ||
           match(TokenType::TOKEN_DGREATER, false) ||
           match(TokenType::TOKEN_TGREATER, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseAdditiveExpression();
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseRelationalExpression(bool noIn) {
    Expr* LHS = parseShiftExpression();
    while (match(TokenType::TOKEN_LESS, false) ||
           match(TokenType::TOKEN_GREATER, false) ||
           match(TokenType::TOKEN_GREATER_EQUAL, false) ||
           match(TokenType::TOKEN_LESS_EQUAL, false) ||
           match(TokenType::TOKEN_IN) ||
           match(TokenType::TOKEN_INSTANCEOF)) {
        if (noIn && match(TokenType::TOKEN_IN)) continue;
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseShiftExpression();
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseRelationalExpressionNoIn() {}

Expr* Parser::parseEqualityExpression(bool noIn) {
    Expr* LHS = parseRelationalExpression(noIn);
    while (match(TokenType::TOKEN_EQUALITY, false) ||
           match(TokenType::TOKEN_STRICT_EQUALITY, false) ||
           match(TokenType::TOKEN_INEQUALITY, false) ||
           match(TokenType::TOKEN_STRICT_INEQUALITY, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseRelationalExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseEqualityExpressionNoIn() {}

Expr* Parser::parseBitwiseANDExpression(bool noIn) {
    Expr* LHS = parseEqualityExpression(noIn);
    while (match(TokenType::TOKEN_BITWISE_AND, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseEqualityExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseBitwiseANDExpressionNoIn() {}

Expr* Parser::parseBitwiseXORExpression(bool noIn) {
    Expr* LHS = parseBitwiseANDExpression(noIn);
    while (match(TokenType::TOKEN_BITWISE_XOR, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseBitwiseANDExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseBitwiseXORExpressionNoIn() {}

Expr* Parser::parseBitwiseORExpression(bool noIn) {
    Expr* LHS = parseBitwiseXORExpression(noIn);
    while (match(TokenType::TOKEN_BITWISE_OR, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseBitwiseXORExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseBitwiseORExpressionNoIn() {}

Expr* Parser::parseLogicalANDExpression(bool noIn) {
    Expr* LHS = parseBitwiseORExpression(noIn);
    while (match(TokenType::TOKEN_LOGICAL_AND, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseBitwiseORExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseLogicalANDExpressionNoIn() {}

Expr* Parser::parseLogicalORExpression(bool noIn) {
    Expr* LHS = parseLogicalANDExpression(noIn);
    while (match(TokenType::TOKEN_LOGICAL_OR, false)) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseLogicalANDExpression(noIn);
        LHS = new BinaryExpr(LHS, t, RHS);
    }
    return LHS;
}

Expr* Parser::parseLogicalORExpressionNoIn() {}

Expr* Parser::parseConditionalExpression(bool noIn) {
    return parseLogicalORExpression(noIn);
    if (match(TokenType::TOKEN_CONDITIONAL)) {
        parseAssignmentExpression();
        if (match(TokenType::TOKEN_COLON))
            parseAssignmentExpression(noIn);
    }
}
Expr* Parser::parseConditionalExpressionNoIn() {}

Expr* Parser::parseAssignmentExpression(bool noIn) {
    Expr* LHS = parseConditionalExpression(noIn);
    while (isAssignmentOperator()) {
        Token t = getCurrentToken();
        next();
        Expr* RHS = parseAssignmentExpression(noIn);
        LHS = new AssignmentExpr(LHS, t, RHS);
    } 
    return LHS;
}

Expr* Parser::parseAssignmentExpressionNoIn() {}

Expr* Parser::parseVariableDeclarationList(bool noIn) {
    ASTVariableDeclaration* VD = new ASTVariableDeclaration(std::move(getCurrentToken()));
    next();
    
    Expr* e = parseVariableDeclaration(noIn);
    VD->push_back(e);
    while (match(TokenType::TOKEN_COMMA)) {
        VD->push_back(parseVariableDeclaration(noIn));
    }
    return VD;
}
Expr* Parser::parseVariableDeclarationListNoIn() {}
Expr* Parser::parseVariableDeclaration(bool noIn) {
    ASTVariableDeclarator* VD = new ASTVariableDeclarator;
    if (match(TokenType::TOKEN_IDENTIFIER, false)) {
        VD->setId(parseIdentifier());
        if (match(TokenType::TOKEN_EQUAL)) {
            VD->setInit(parseInitialiser(noIn));
        } else {
            VD->setInit(new Literal(std::monostate()));
        }
    }
    return VD;
}
Expr* Parser::parseVariableDeclarationNoIn() {}
Expr* Parser::parseInitialiser(bool noIn) {
    return parseAssignmentExpression(noIn);
}

Expr* Parser::parseInitialiserNoIn() {}

Expr* Parser::parsePropertyNameAndValueList() {
    parsePropertyAssignment();
    match(TokenType::TOKEN_RIGHT_BRACE);
}

Expr* Parser::parsePropertyAssignment() {
    Property* p = new Property();
    auto isGetOrSet = [](const Token& t) {
        if (std::memcmp(t.getStart(), "get", 3) == 0)
            return 1;
        if (std::memcmp(t.getStart(), "set", 3) == 0)
            return 2;
        return 0;
    };
    if (match(TokenType::TOKEN_IDENTIFIER, false)) {
        switch(isGetOrSet(getCurrentToken())) {
            case 0: {
                p->setKey(parsePropertyName());
                if (match(TokenType::TOKEN_COLON)) {
                    p->setValue(parseAssignmentExpression());
                }
                break;
            }
            case 1: {
                p->setKind(1);
                p->setKey(parsePropertyName());
                if (match(TokenType::TOKEN_LEFT_PAREN)) {
                    if (match(TokenType::TOKEN_RIGHT_PAREN))
                        if (match(TokenType::TOKEN_LEFT_BRACE)) {
                            // functionbody
                            match(TokenType::TOKEN_RIGHT_BRACE);
                        }
                }
                break;
            }
            case 2: {
                p->setKind(2);
                p->setKey(parsePropertyName());
                if (match(TokenType::TOKEN_LEFT_PAREN)) {
                    if (match(TokenType::TOKEN_RIGHT_PAREN))
                        if (match(TokenType::TOKEN_LEFT_BRACE)) {
                            // functionbody
                            match(TokenType::TOKEN_RIGHT_BRACE);
                        }
                }
                break;
            }
            default: break;
        }
    }
    return p;
}

Expr* Parser::parsePropertyName() {
    if (match(TokenType::TOKEN_NUMBER, false) ||
        match(TokenType::TOKEN_STRING, false)) {
        return  parseLiteral();
    } else if (match(TokenType::TOKEN_IDENTIFIER, false)) {
        return parseIdentifier();
    }
}

Expr* Parser::parseFunctionDeclaration() {
    ASTFunctionDeclaration* fun = new ASTFunctionDeclaration;
    match(TokenType::TOKEN_FUNCTION); // opt
    fun->setId(parseIdentifier());
    match(TokenType::TOKEN_LEFT_PAREN);
    if (!match(TokenType::TOKEN_RIGHT_PAREN)) {
        fun->setParams(parseFormalParameterList());
        match(TokenType::TOKEN_RIGHT_PAREN);
    }
    match(TokenType::TOKEN_LEFT_BRACE);
    fun->setBlock(parseFunctionBody());
    match(TokenType::TOKEN_RIGHT_BRACE);
    return fun;
}

Expr* Parser::parseFunctionExpression() {
    match(TokenType::TOKEN_FUNCTION); // opt
    if (match(TokenType::TOKEN_LEFT_PAREN)) {

    }
}

std::vector<Identifier*> Parser::parseFormalParameterList() {
    std::vector<Identifier*> res;
    res.emplace_back(parseIdentifier());
    while (match(TokenType::TOKEN_COMMA)) {
        res.emplace_back(parseIdentifier());
    }
    return res;;
}

Expr* Parser::parseFunctionBody() {
    BlockStmt* stmt = new BlockStmt;
    while (!match(TokenType::TOKEN_RIGHT_BRACE)) 
        stmt->push_back(parseSourceElement());
    return stmt;
}

void Parser::next() {
    if (record) tokenBuffer.emplace(token);
    if (rollback && tokenBuffer.size()) {
        token = *tokenBuffer.begin();
        tokenBuffer.erase(tokenBuffer.begin());
        return;
    }
    token = scanner->scanToken();
}

Token Parser::getCurrentToken() {
    if (rollback && tokenBuffer.size()) token = *tokenBuffer.begin();
    return token;
}

bool Parser::match(const TokenType& t, bool next) {
    if (getCurrentToken().getTokenType() != t) return false;
    if (next) {
        this->next();
    } else if (record) {
        tokenBuffer.emplace(token);
    }
    return true; 
}

bool Parser::isAssignmentOperator() {
    // 77 ~ 88
    switch (getCurrentToken().getTokenType()) {
        case TokenType::TOKEN_EQUAL:
        case TokenType::TOKEN_MUL_EQUAL:
        case TokenType::TOKEN_DIV_EQUAL:
        case TokenType::TOKEN_MOD_EQUAL:
        case TokenType::TOKEN_PLUS_EQUAL:
        case TokenType::TOKEN_MINUS_EQUAL:
        case TokenType::TOKEN_DLESS_EQUAL:
        case TokenType::TOKEN_DGREATER_EQUAL:
        case TokenType::TOKEN_TGREATER_EQUAL:
        case TokenType::TOKEN_BITWISE_AND_EQUAL:
        case TokenType::TOKEN_BITWISE_OR_EQUAL:
        case TokenType::TOKEN_BITWISE_XOR_EQUAL:
            return true;
        default: return false;
    }
}

bool Parser::isLiteral() {
    // regularExpression
    switch (getCurrentToken().getTokenType()) {
        case TokenType::TOKEN_NULL:
        case TokenType::TOKEN_NUMBER:
        case TokenType::TOKEN_TRUE:
        case TokenType::TOKEN_FALSE:
        case TokenType::TOKEN_STRING: 
            return true;
        default: return false;
    }
}

bool Parser::isPropertyName() {
    TokenType tt = getCurrentToken().getTokenType();
    return  tt == TokenType::TOKEN_IDENTIFIER || 
            tt == TokenType::TOKEN_STRING ||
            tt == TokenType::TOKEN_NUMBER;
}

bool Parser::isUnaryOperator() {
    switch (getCurrentToken().getTokenType()) {
        case TokenType::TOKEN_DELETE:
        case TokenType::TOKEN_VOID:
        case TokenType::TOKEN_TYPEOF:
        case TokenType::TOKEN_DPLUS:
        case TokenType::TOKEN_DMINUS:
        case TokenType::TOKEN_PLUS:
        case TokenType::TOKEN_MINUS:
        case TokenType::TOKEN_BITWISE_NOT:
        case TokenType::TOKEN_NOT:
            return true;
        default: return false;
    }
}