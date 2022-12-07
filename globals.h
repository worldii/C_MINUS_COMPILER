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

typedef enum {StmtK,ExpK, DecK, ParamK, TypeK, IdK, VariK, NumK, OpK} NodeKind;
typedef enum {CompoundK, SelectK, IterK, RetK} StmtKind;
typedef enum {AssignK, SimpleK} ExpKind;
typedef enum {VarK, ArrayK, FunK, CallK}DecKind;
typedef enum {VarParamK, ArrayParamK} Paramkind;
typedef enum {VVarK, VArrayK} VariKind;

typedef enum {IntK ,VoidK} TypeKind;
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
         DecKind kind;
         // var 
         struct treeNode * type_specifier;
         struct treeNode * id;
         union {
            struct treeNode * num; // ARRAY ; 
            struct { // FUNCTIon 
               struct treeNode * params;
               struct treeNode *compound_stmt;
            }; 
            struct treeNode * args_list;   // CallK      
         };
      }Dec;

      // ParamK 
      // {VarK, ArrayK} Paramkind;
      struct {
         // var 
         Paramkind kind;
         struct treeNode * type_specifier;
         struct treeNode * id;
         union {
            struct treeNode * num; // ARRAY ; 
            struct { // FUNCTIon 
               struct treeNode * params;
               struct treeNode *compound_stmt;
            };   
         };
      }Param;

      // STMTK
      struct {
         StmtKind kind;
         struct treeNode * exp;
         union {
            // exp
            // Compound
            struct {
               struct treeNode * local_declarations;
               struct treeNode * stmt_list;
            };
            // if_else stmt , if
            struct {
               struct treeNode * if_stmt;
               struct treeNode * else_stmt;
            };
            struct treeNode * loop_stmt; // loop;
         } ;
      }Stmt;

      //StmtK,ExpK, DecK, ParamK, TypeK
      struct {
         ExpKind kind;
         struct treeNode * left_exp;
         struct treeNode * right_exp;
         union {
            // OpK
            struct treeNode * op;
            // AssignK
            //struct treeNode * assign;
            // CompareK;
            //struct treeNode * compare;
         };
      }Exp;

      struct {
         VariKind kind;
         struct TreeNode * id;
         struct TreeNode * exp;
      }Vari;

      struct {
         char * id;
      }ID;
      
      // TypeK
        struct 
      { 
         TypeKind kind;
      }Type;

      struct {
         char op[3];
      } Op;

      struct {
         int num;
      } Num;

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
