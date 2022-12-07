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
static int yyerror(char * message);
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

program               : declaration_list
                        { savedTree = $1;} 
                      ;
declaration_list :    declaration-list declaration 
                      { $$ = add_sibling($1, $2);}
                      | declaration 
                      { $$ = $1; }
                      ;
declaration : var_declaration 
              { $$ = $1; }
              | fun_declaration 
              { $$ = $1; }
              ;
var_declaration : type_specifier ID SEMI
                  {
                    $$ = node_initialize();
                    declare_var($$, $1, $2);
                  }
                 | type_specifier ID LBRACKET NUM RBRACKET SEMI 
                 {
                    $$ = node_initialize();
                    declare_array($$, $1, $2, $4);
                 }
                 ;
type_specifier :  INT 
                  { 
                    $$ = node_initialize();
                    set_node_type($$, $1);
                  } 
                  | VOID 
                  {
                    $$ = node_initialize();
                    set_node_type($$, $1);
                  }
                  ;
fun_declaration :  type_specifier ID LPAREN params RPAREN compound_stmt
                  {
                    $$ = node_initialize();
                    declare_func($$,$1,$2,$4,$6);
                  }
                  ;
params : param_list 
        { $$ = $1; }
       | VOID 
       { $$ = NULL;}
       ;
param_list : param_list COMMA param 
             {
              add_sibling($1, $3);
             }
            | param 
              { $$ = $1; }
            ;
param : type_specifier ID
        {
          $$ = node_initialize();
          set_node_var_param($$, $1, $2);
        }
       | type_specifier ID LBRACKET RBRACKET
        {
          $$ = node_initialize();
          set_node_array_param($$, $1, $2);
        }
        ;
compound_stmt : LBRACE local_declarations stmt_list RBRACE
                {
                  $$ = node_initialize();
                  set_node_compound_stmt($$, $2, $3);
                }
                ;
    
local_declarations : local_declarations var_declaration 
                    | empty;
stmt_list : stmt_list stmt 
          | empty
          ;
stmt : expressions_stmt { $$ = $1; } 
     | compound_stmt { $$ = $1; } 
     | selection_stmt { $$ = $1; }
     | iteration_stmt { $$ = $1; } 
     | return_stmt { $$ = $1; }
     ;
expressions_stmt : expression SEMI
                 | SEMI ; 
selection_stmt : IF LPAREN expression RPAREN stmt 
              |  IF LPAREN expression RPAREN stmt else stmt;
iteration_stmt : WHILE LPAREN expression RPAREN stmt ;
return_stmt : return SEMI 
            | return expression ;
expression : var ASSIGN expression 
          | simple_expression ;
var : ID 
    | ID LPAREN expression RPAREN;
simple_expression : additive_expression relop additive_expression 
                  | additive_expression;
relop :  EQ 
      | LT 
      | LE 
      | GT 
      | GE 
      | NOTEQ;
additive_expression : additive_expression addop term 
                      | term { $$ = $1; }
addop : PLUS 
      | MINUS ;
term : term mulop factor 
    | factor 
    ; 
mulop : TIMES
     |  OVER 
     ; 
factor : LPAREN expression RPAREN
       | var { $$ = $1; } 
       | call { $$ = $1; }
       | NUM 
       ; 
call : ID LPAREN args RPAREN ; 
args : arg_list { $$ = $1; }
      | empty 
      ; 
arg_list : arg_list COMMA expression 
        | expression ;
%%

int yyerror(char * message)
{ 
  fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
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
  printf("%d\n", token);
  //fprintf(listing,"Debug: %d\n", token);

  return token; }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

