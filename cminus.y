/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
%}

%token IF ELSE INT VOID RETURN WHILE  
%token ID NUM 
%token  EQ LT LE GT GE NOTEQ ASSIGN
%token PLUS MINUS TIMES OVER
%token  SEMI COMMA
%token LPAREN RPAREN 
%token LBRACKET RBRACKET LBRACE RBRACE
%token ERROR COMMENT COMMENTERR

%% /* Grammar for TINY */

program     : declaration_list
                 { savedTree = $1;} 
            ;
declaration_list : 
            ;
declaration : declaration-list declaration | declaration;
declaration : var_declaration | fun_declaration
var_declaration : type_specifier ID SEMI | type_specifier ID LBRACKET NUM RBRACKET SEMI ;
type_specifier : INT | VOID 
fun_declaration :  type_specifier ID LPAREN params RPAREN compond_stmt
params : param_list | VOID 
param_list : param_list COMMA param | param ;
param : type_specifier ID | type_specifier ID LBRACKET RBRACKET
compound_stmt : LBRACE local_declarations stmt_list RBRACE;
local_declarations : local_declarations var_declaration | empty;
stmt_list : stmt_list stmt | empty;
stmt : expressions_stmt | compound_stmt | selection_stmt | iteration_stmt |return_stmt;
expressions_stmt : expression SEMI | SEMI ; 
selection_stmt : IF LPAREN expression RPAREN stmt |  IF LPAREN expression RPAREN stmt else stmt;
iteration_stmt : WHILE  LPAREN expression RPAREN stmt ;
return_stmt : return SEMI | return expression ;
expression : var ASSIGN expression | simple_expression ;
var : ID | ID LPAREN expression RPAREN;
simple_expression : additive_expression relop additive_expression | additive_expression;
relop :  EQ | LT | LE | GT | GE | NOTEQ;
additive_expression : additive_expression addop term | term 
addop : PLUS | MINUS ;
term : term mulop factor | factor ; 
mulop : TIMES |  OVER ; 
factor : LPAREN expression RPAREN | var | call | NUM ; 
call : ID LPAREN args RPAREN ; 
args : arg_list | empty ; 
arg_list : arg_list COMMA expression | expression ;
%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ 
  int token = getToken();
  
  //fprintf(listing,"Debug: %d\n", token);

  return token; }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

