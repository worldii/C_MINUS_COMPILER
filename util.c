/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
static char* token_name[100] = {
  [IF] = "IF", [ELSE] = "ELSE",  [INT] = "INT", [VOID] = "VOID", [RETURN] = "RETURN", [WHILE] = "WHILE",
  [ID] ="ID",  [NUM] = "NUM",  [PLUS] = "+", [MINUS] = "-", [TIMES] = "*", [OVER] = "/", 
  [LT] = "<", [LE] = "<=",  [GT] = ">",  [GE]= ">=", [EQ] ="==" , [NOTEQ] = "!=", [ASSIGN] = "=", 
  [SEMI] = ";", [COMMA] = ",", [LPAREN] = "(", [RPAREN] = ")", [LBRACKET] = "[", [RBRACKET] = "]", 
  [LBRACE]= "{", [RBRACE]= "}" , [ENDFILE] = "EOF"
};

void printToken( TokenType token, const char* tokenString )
{ 

  switch (token)
  { case IF:case ELSE:case INT:
    case RETURN:case WHILE:
    case VOID:    case ASSIGN: 
    case LT:     case LE: 
    case GT:     case GE: 
    case EQ:     case NOTEQ: 
    case LPAREN:    case RPAREN: 
    case LBRACKET:    case RBRACKET: 
    case LBRACE:    case RBRACE: 
    case SEMI: ;    case COMMA: 
    case PLUS:     case MINUS: 
    case TIMES:    case OVER: 
    case ENDFILE:    case NUM:
    case ID:
      fprintf(listing, "\t\t\t%s\t\t\t%s\n", token_name[token], tokenString);
      break;
    case COMMENT : break;
    case COMMENTERR :
     fprintf(listing,
          "\t\t\tERROR\t\t\t%s\n","Comment Error");
          break;
    case ERROR:
      fprintf(listing,
          "\t\t\tERROR\t\t\t%s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case RepeatK:
          fprintf(listing,"Repeat\n");
          break;
        case AssignK:
          fprintf(listing,"Assign to: %s\n",tree->attr.name);
          break;
        case ReadK:
          fprintf(listing,"Read: %s\n",tree->attr.name);
          break;
        case WriteK:
          fprintf(listing,"Write\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
