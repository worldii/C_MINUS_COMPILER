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
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case ELSE:  
    case WHILE: 
    case RETURN: 
    case INT:
    case VOID:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
    case ASSIGN: fprintf(listing, "=\n"); break;
    case EQ: fprintf(listing, "==\n"); break;
    case NE: fprintf(listing, "!=\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case LE: fprintf(listing, "<=\n"); break;
    case GT: fprintf(listing, ">\n"); break;
    case GE: fprintf(listing, ">=\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LBRACE: fprintf(listing, "[\n"); break;
    case RBRACE: fprintf(listing, "]\n"); break;
    case LCURLY: fprintf(listing, "{\n"); break;
    case RCURLY: fprintf(listing, "}\n"); break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case SEMI: fprintf(listing, ";\n"); break;
    case COMMA: fprintf(listing, ",\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
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

/* Function newDecNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode * newDecNode(DecKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DecK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newTypeNode creates a new type
 * node for syntax tree construction
 */
TreeNode * newTypeNode(TypeKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = TypeK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newPrmNode creates a new parameter
 * node for syntax tree construction
 */
TreeNode * newPrmNode(ParamKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if(t==NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ParamK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

TreeNode * newOpNode(OptKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if(t==NULL)
    fprintf(listing, "Out of memory error at line %d\n", lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = OptK;
    t->kind.exp = kind;
    t->lineno = lineno;
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
	case CompoundK:
	  fprintf(listing,"Compound Statement:\n");
	  break;
	case IfK:
          fprintf(listing,"If Statement:\n");
          break;
        case IfelseK:
          fprintf(listing,"If-Else Statement:\n");
          break;
        case WhileK:
          fprintf(listing,"While Statement:\n");
          break;
        case ReturnK:
          fprintf(listing,"Return Statement:\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp){
	case AssignK:
	  fprintf(listing,"Assign:\n");
	  break;
	case VariableK:
	  fprintf(listing,"Variable: name = %s\n", tree->attr.name);
	  break;
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
	case CallK:
	  fprintf(listing,"Call: function name = %s\n", tree->attr.name);
	  break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==DecK)
    { switch (tree->kind.exp){ 
        case VarK:
	  if(tree->child[0] == NULL)
	    fprintf(listing,"Variable Declaration: name = %s, type = int\n", tree->attr.name);
	  else
	    fprintf(listing,"Variable Declaration: name = %s, type = int[]\n", tree->attr.name);
  	  break;
        case FunK:
	  if(tree->type == Integer)
	    fprintf(listing,"Function Declaration: name = %s, return type = %s\n", tree->attr.name, "int");
	  else if(tree->type == Void)
	    fprintf(listing,"Function Declaration: name = %s, return type = %s\n", tree->attr.name, "void");
	  break;
	default:
	  fprintf(listing,"Unknown DecNode kind\n");
	  break;	  
      }
    }
    else if (tree->nodekind==ParamK)
    { switch (tree->kind.exp){
	case SingleK:
	  if(tree->type == Integer)
	    fprintf(listing,"Parameter: name = %s, type = int\n", tree->attr.name);
	  else if(tree->type == Void)
	    fprintf(listing,"Void Parameter\n");
	  break;
	case ArrayK:
	  if(tree->type == Integer)
	    fprintf(listing,"Parameter: name = %s, type = int[]\n", tree->attr.name);
	  else if(tree->type == Void)
	    fprintf(listing,"Parameter: name = %s, type = void[]\n", tree->attr.name);
	  break;
	default:
	  fprintf(listing,"Unknown PrmNode kind\n");
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
