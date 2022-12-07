/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE TRUE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE


/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8
typedef enum 
    /* book-keeping tokens */
   {ENDFILE,ERROR,
    /* reserved words */
    IF,ELSE,INT, VOID, RETURN, WHILE,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    PLUS,MINUS,TIMES,OVER,
    LT, LE, GT, GE, EQ, NOTEQ,ASSIGN,
    SEMI,COMMA, 
    LPAREN,RPAREN, // ()
    LBRACKET, RBRACKET, // []
    LBRACE,RBRACE, // {}, 
    COMMENT, COMMENTERR
   } TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/
// selectK : if else 합쳐져 있음
// Iter : while 문임 

typedef enum {StmtK,ExpK, DecK, ParamK, TypeK} NodeKind;
typedef enum {ExpK, CompoundK, IfK, IfelseK, IterK, RetK} StmtKind;
typedef enum {AssignK, CompareK, OpK} ExpKind;
typedef enum {VarK, ArrayK, FunK}DecKind;
typedef enum {VarK, ArrayK} Paramkind;
typedef enum {IntK ,VoidK}TypeKind;
/* ExpType is used for type checking */
typedef enum {Void,Integer} ExpType;

#define MAXCHILDREN 3

// typedef struct treeNode
//    { struct treeNode * child[MAXCHILDREN];
//      struct treeNode * sibling;
//      int lineno;
//      NodeKind nodekind;
//      union { StmtKind stmt; ExpKind exp;} kind;
//      union { TokenType op;
//              int val;
//              char * name; } attr;
//      ExpType type; /* for type checking of exps */
//    } TreeNode;

typedef struct treeNode
{
   struct treeNode * sibling;
   int lineno;
   NodeKind nodekind;
   
   union {
      // DecK 
      // DECLARE -> function, array ,var 
      struct {
         // var 
         struct treeNode * type_specifier;
         struct treeNode * id;
         DecKind deckind;
         union {
            struct treeNode * num; // ARRAY ; 
            struct { // FUNCTIon 
               struct treeNode * params;
               struct treeNode *compound_stmt;
            }            
         }
      };

      // ParamK 
      // {VarK, ArrayK} Paramkind;
      struct {
         // var 
         struct treeNode * type_specifier;
         struct treeNode * id;
         Paramkind paramkind;
         union {
            struct treeNode * num; // ARRAY ; 
            struct { // FUNCTIon 
               struct treeNode * params;
               struct treeNode *compound_stmt;
            }            
         }
      };

      // 


      //StmtK,ExpK, DecK, ParamK, TypeK

      // STMTK
      struct {
         union {
            // exp
            struct treeNode * exp;
            // Compound
            struct {
               struct treeNode * local_declarations;
               struct treeNode * stmt_list;
            };
            // if_else stmt 
            struct {
               struct treeNode * if_stmt;
               struct treeNode * else_stmt;
            };
            struct treeNode * if_stmt; // if_stmt 
            struct treeNode * loop_stmt; // loop;
            struct treeNode * ret_stmt ;// ret 
         } 
      };
      struct {
         struct treeNode * left_exp;
         struct treeNode * right_exp;
         union {
            // OpK
            struct treeNode * op;
            // AssignK
            struct treeNode * assign;
            // CompareK;
            struct treeNode * compare;
         }
      };
      struct {
         // var 
         struct treeNode * type_specifier;
         struct treeNode * id;
         union {
            struct treeNode * NUM; // ARRAY ; 
         }
      };
      struct 
      {
         TokenType type;
      } 
      struct {
         char * id;
      }
   }specific_kind;

}TreeNode;
/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
