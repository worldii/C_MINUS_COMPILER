/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "string.h"
#define ENDFILE 44
/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
 static char* token_name[] = {
   [IF] = "IF", [ELSE] = "ELSE",  [INT] = "INT", [VOID] = "VOID", [RETURN] = "RETURN", [WHILE] = "WHILE",
   [ID] ="ID",  [NUM] = "NUM",  [PLUS] = "+", [MINUS] = "-", [TIMES] = "*", [OVER] = "/", 
   [LT] = "<", [LE] = "<=",  [GT] = ">",  [GE]= ">=", [EQ] ="==" , [NOTEQ] = "!=", [ASSIGN] = "=", 
   [SEMI] = ";", [COMMA] = ",", [LPAREN] = "(", [RPAREN] = ")", [LBRACKET] = "[", [RBRACKET] = "]", 
  [LBRACE]= "{", [RBRACE]= "}" , [ENDFILE] = "ENDFILE"
};

void printToken( TokenType token, const char* tokenString )
{ 
printf("%d", token);
  switch (token)
  { 
    case IF:      case ELSE:
    case INT:     case RETURN:
    case WHILE:   case VOID:    
    case ASSIGN:  case LT:     
    case LE:      case GT:     
    case GE:      case EQ:     
    case NOTEQ: 
    case LPAREN:  case RPAREN: 
    case LBRACKET:case RBRACKET: 
    case LBRACE:  case RBRACE: 
    case SEMI: ;  case COMMA: 
    case PLUS:    case MINUS: 
    case TIMES:   case OVER: 
    case NUM:     case ID:
      fprintf(listing, "\t\t\t%s\t\t\t%s\n", token_name[token], tokenString);
      break;
    case ENDFILE: 
      fprintf(listing, "EOF\n"); 
      break;
    case COMMENT: 
      break;
    case COMMENTERR:
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


// My Implementation Start

TreeNode * node_initialize(){
  TreeNode * temp_node = malloc(sizeof(TreeNode));
  if (temp_node == NULL) {
     // ERROR 
     printf("Allocation Error\n");
     return NULL;
  }

  temp_node->sibling = NULL;
  temp_node->lineno = lineno;

  return temp_node;
}

TreeNode* add_sibling(TreeNode *node, TreeNode* new_node){
  TreeNode * temp_node = node;
  if (!temp_node) 
    temp_node = new_node;
  else 
  {
    while (!temp_node->sibling){
      temp_node = temp_node->sibling;
    }
      temp_node = new_node;
  }
  return node;
}

void set_node_id(TreeNode *node,  char * id){
  node->nodekind = IdK;
  int len = strlen(id);
  node->specific_kind.ID.id = malloc(sizeof(char)*(len+1));
  strcpy(node->specific_kind.ID.id , id);
}

void set_node_type(TreeNode *node, TypeKind type){
  node->nodekind = TypeK;
  node->specific_kind.Type.kind = type;
}

void declare_var(TreeNode * node, TreeNode * type, TreeNode *id)
{
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = VarK;
  node->specific_kind.Dec.type_specifier = type;
  node->specific_kind.Dec.id = id;
}

void declare_array(TreeNode * node,TreeNode * type, TreeNode *id, TreeNode * num )
{
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = ArrayK;
  node->specific_kind.Dec.id = id;
  node->specific_kind.Dec.type_specifier = type;
  node->specific_kind.Dec.num = num;
}

void declare_func(TreeNode *node, TreeNode* type, TreeNode * id, TreeNode * params , TreeNode* compound_stmt)
{
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = FunK;
  node->specific_kind.Dec.id = id;
  node->specific_kind.Dec.type_specifier = type;
  node->specific_kind.Dec.params = params;
  node->specific_kind.Dec.compound_stmt = compound_stmt;
}

void set_node_var_param(TreeNode * node,  TreeNode * type,  TreeNode * id)
{
  node->nodekind = ParamK;
  node->specific_kind.Param.kind = VarK;
  node->specific_kind.Param.id = id;
  node->specific_kind.Param.type_specifier = type;
}
void set_node_var(TreeNode * node,  TreeNode * id)
{
  node->nodekind = VariK;
  node->specific_kind.Vari.kind = VVarK;
  node->specific_kind.Vari.id = id;
  node->specific_kind.Vari.exp= NULL;
}

void set_node_array(TreeNode * node,  TreeNode * id, TreeNode * exp)
{
  node->nodekind = VariK;
  node->specific_kind.Vari.kind = VArrayK;
  node->specific_kind.Vari.id = id;
  node->specific_kind.Vari.exp= NULL;
}


void set_node_exp_assign(TreeNode* node, TreeNode *var, TreeNode * exp){
  node->nodekind = ExpK;
  node->specific_kind.Exp.kind = AssignK;
  node->specific_kind.Exp.left_exp =  var;
  node->specific_kind.Exp.right_exp = exp;
}
void set_node_exp_simple(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp)
{
  node->nodekind = ExpK;
  node->specific_kind.Exp.kind = SimpleK;
  node->specific_kind.Exp.left_exp =  var;
  node->specific_kind.Exp.op = op;
  node->specific_kind.Exp.right_exp = exp;
}

void set_node_array_param(TreeNode * node, TreeNode * type,  TreeNode * id)
{
  node->nodekind = ParamK;
  node->specific_kind.Param.kind = ArrayK;
  node->specific_kind.Param.id = id;
  node->specific_kind.Param.type_specifier = type;
}

void set_node_compound_stmt(TreeNode * node ,TreeNode * local_declarations,  TreeNode * stmt_list)
{
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = CompoundK;
  node->specific_kind.Stmt.local_declarations = local_declarations;
  node->specific_kind.Stmt.stmt_list = stmt_list;
  node->specific_kind.Stmt.exp =NULL;
}

void set_node_selection(TreeNode* node, TreeNode* exp,  TreeNode* if_stmt , TreeNode* else_stmt)
{
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = SelectK;
  node->specific_kind.Stmt.exp = exp;
  node->specific_kind.Stmt.if_stmt= if_stmt;
  node->specific_kind.Stmt.else_stmt = else_stmt;
}
void set_node_iteration(TreeNode *node, TreeNode * exp , TreeNode *loop_stmt ){
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = IterK;
  node->specific_kind.Stmt.exp = exp;
  node->specific_kind.Stmt.loop_stmt= loop_stmt;
}


void set_node_return(TreeNode *node, TreeNode * exp){
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = RetK;
  node->specific_kind.Stmt.exp = exp;
}
static char * token_to_char( TokenType op) {
  switch (op) {
    case EQ : return "==";
    case LT : return "<";
    case LE : return "<=";
    case GT : return ">";
    case NOTEQ : return "!=";
    case ASSIGN : return "=";
    default : 
      printf("ERROR \n");
      exit(-1);
  }
}

void set_node_op(TreeNode * node , TokenType op )
{
  node->nodekind = OpK;
  strcpy(node->specific_kind.Op.op, token_to_char(op));
}


void set_node_num (TreeNode* node , char * string) {
  node->nodekind = NumK;
  node->specific_kind.Num.num = atoi (string);
}
void set_node_call_func(TreeNode *node , TreeNode *id , TreeNode * args)
{
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = CallK;
  node->specific_kind.Dec.args_list = args;
  node->specific_kind.Dec.id = id;
}


// /* Function newStmtNode creates a new statement
//  * node for syntax tree construction
//  */
// TreeNode * newStmtNode(StmtKind kind)
// { TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
//   int i;
//   if (t==NULL)
//     fprintf(listing,"Out of memory error at line %d\n",lineno);
//   else {
//     for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
//     t->sibling = NULL;
//     t->nodekind = StmtK;
//     t->kind.stmt = kind;
//     t->lineno = lineno;
//   }
//   return t;
// }

// /* Function newExpNode creates a new expression 
//  * node for syntax tree construction
//  */
// TreeNode * newExpNode(ExpKind kind)
// { TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
//   int i;
//   if (t==NULL)
//     fprintf(listing,"Out of memory error at line %d\n",lineno);
//   else {
//     for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
//     t->sibling = NULL;
//     t->nodekind = ExpK;
//     t->kind.exp = kind;
//     t->lineno = lineno;
//     t->type = Void;
//   }
//   return t;
// }

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
/*void printTree( TreeNode * tree )
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
    else if (tree->nodekind==DecK){}
    else if (tree->nodekind==ParamK){}
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
*/

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==DecK)
    { switch (tree->specific_kind.Dec.kind) {
        case VarK:
          printf("Declare Variable\n");
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.id);
          break;
        case ArrayK:
          printf("Declare Array\n");
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.num);
          break;
        case FunK:
          printf("Declare Func\n");
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.params); 
          printTree(tree->specific_kind.Dec.compound_stmt);  
          break;
        case CallK:
           printf("Call\n");
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.args_list); 
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind== ParamK)
    { switch (tree->specific_kind.Param.kind) {
        case VarParamK:
          printf("Parameter Variable\n");
          printTree(tree->specific_kind.Param.type_specifier);
          printTree(tree->specific_kind.Param.id);
          break;
        case ArrayParamK:
          printf("Parameter Array\n");
          printTree(tree->specific_kind.Param.type_specifier);
          printTree(tree->specific_kind.Param.id);
          printTree(tree->specific_kind.Param.num);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==StmtK)
    { switch (tree->specific_kind.Stmt.kind) {
        case CompoundK:
          printf("Compound Statement \n");
          printTree(tree->specific_kind.Stmt.local_declarations);
          printTree(tree->specific_kind.Stmt.stmt_list);
          break;
        case SelectK:
          printf("Select Statement \n");
          printTree(tree->specific_kind.Stmt.exp);
          printTree(tree->specific_kind.Stmt.if_stmt);
          printTree(tree->specific_kind.Stmt.else_stmt);
          break;
        case IterK:
          printf("Iteration Statement \n");
          printTree(tree->specific_kind.Stmt.exp);
          printTree(tree->specific_kind.Stmt.loop_stmt);
          break;
        case RetK:
          printf("Return Statement \n");
          printTree(tree->specific_kind.Stmt.exp);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->specific_kind.Exp.kind) {
        case AssignK:
          printf("Assignment Statement \n");
          printTree(tree->specific_kind.Exp.left_exp);
          printTree(tree->specific_kind.Exp.op);   
          printTree(tree->specific_kind.Exp.right_exp);
          break;
        case SimpleK:
          printf("Simple Statement \n");
          printTree(tree->specific_kind.Exp.left_exp);
          printTree(tree->specific_kind.Exp.op);   
          printTree(tree->specific_kind.Exp.right_exp);         
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==VariK)
    { switch (tree->specific_kind.Vari.kind) {
        case VVarK:
          printf("Variable \n");
          printTree(tree->specific_kind.Vari.id);
          printTree(tree->specific_kind.Vari.exp);            
          break;
        case VArrayK:
          printf("Variable Array\n");
          printTree(tree->specific_kind.Vari.id);
          printTree(tree->specific_kind.Vari.exp);   
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==TypeK)
    { switch (tree->specific_kind.Type.kind) {
        case IntK:
        printf("INT type\n");         
          break;
        case VoidK:
        printf("Void type\n");         
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==OpK)
    { 
        printf("%s\n", tree->specific_kind.Op.op);
    }
    else if (tree->nodekind==IdK)
    { 
        printf("%s\n", tree->specific_kind.ID.id);
    }
    else if (tree->nodekind==NumK)
    { 
        printf("%s\n", tree->specific_kind.Num.num);
    }
    else fprintf(listing,"Unknown node kind\n");
    tree = tree->sibling;
  }
  UNINDENT;
}