/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include <string.h>

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */

 static char* token_name[] = {
   [IF] = "IF", [ELSE] = "ELSE",  [INT] = "INT", [VOID] = "VOID", [RETURN] = "RETURN", [WHILE] = "WHILE",
   [ID] ="ID",  [NUM] = "NUM",  [PLUS] = "+", [MINUS] = "-", [TIMES] = "*", [OVER] = "/", 
   [LT] = "<", [LE] = "<=",  [GT] = ">",  [GE]= ">=", [EQ] ="==" , [NOTEQ] = "!=", [ASSIGN] = "=", 
   [SEMI] = ";", [COMMA] = ",", [LPAREN] = "(", [RPAREN] = ")", [LBRACKET] = "[", [RBRACKET] = "]", 
  [LBRACE]= "{", [RBRACE]= "}" , [ENDFILE] = "ENDFILE", [ERROR] = "ERROR"
};

void printToken( TokenType token, const char* tokenString )
{ 
   if (token == COMMENT)
    return;
 // fprintf(listing,"hehe %d %s", token, tokenString);
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
          "\t\t\tERRORsss\t\t\t%s\n","Comment Error");
      break;
    case ERROR:
      fprintf(listing,
          "\t\t\tERRORccc\t\t\t%s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}


// My Implementation Start
TreeNode * node_initialize(){
  TreeNode * temp_node = (TreeNode *) malloc(sizeof(TreeNode));
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
  if (temp_node  == NULL) 
    node = new_node;
  else 
  {
    
    while (temp_node->sibling){
      temp_node = temp_node->sibling;
    }
    temp_node-> sibling  = new_node;
  }
  return node;
}

void set_node_id(TreeNode *node,  char * id){
  //printf("make node id\n");
  node->nodekind = IdK;
  int len = strlen(id);
  node->specific_kind.ID.id = malloc(sizeof(char)*(len+1));
  strcpy(node->specific_kind.ID.id , id);
}

void set_node_type(TreeNode *node, TypeKind type){
 // printf("make node type\n");
  node->nodekind = TypeK;
  node->specific_kind.Type.kind = type;
}

void declare_var(TreeNode * node, TreeNode * type, TreeNode *id)
{
 // printf("make declare var\n");
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = VarK;
  node->specific_kind.Dec.type_specifier = type;
  node->specific_kind.Dec.id = id;
}

void declare_array(TreeNode * node,TreeNode * type, TreeNode *id, TreeNode * num )
{
 // printf("make declar array\n");
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = ArrayK;
  node->specific_kind.Dec.id = id;
  node->specific_kind.Dec.type_specifier = type;
  node->specific_kind.Dec.num = num;
}

void set_node_array(TreeNode * node,TreeNode * type, TreeNode *id )
{
  //printf("make node array\n");
  node->nodekind = DecK;
  node->specific_kind.Dec.kind = ArrayK2;
  node->specific_kind.Dec.id = id;
  node->specific_kind.Dec.type_specifier = type;
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

void set_node_exp(TreeNode * node,  TreeNode * exp){
      // printf("make node exp\n");
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = expK;
  node->specific_kind.Stmt.exp = exp;
}
void set_node_exp_assign(TreeNode* node, TreeNode *var, TreeNode * exp){
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = AssignK;
  node->specific_kind.Stmt.var = var;
  node->specific_kind.Stmt.exp = exp;
}

void set_node_exp_simple(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp)
{
      // printf("make node simples\n");

  node->nodekind = ExpK;
  node->specific_kind.Exp.kind = SimpleK;
  node->specific_kind.Exp.left_exp =  var;
  node->specific_kind.Exp.op = op;
  node->specific_kind.Exp.right_exp = exp;
}
void set_node_exp_addictive(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp)
{
  node->nodekind = ExpK;
  node->specific_kind.Exp.kind = AddictiveK;
  node->specific_kind.Exp.left_exp =  var;
  node->specific_kind.Exp.op = op;
  node->specific_kind.Exp.right_exp = exp;
}

void set_node_exp_mulop(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp)
{
  node->nodekind = ExpK;
  node->specific_kind.Exp.kind = MulopK;
  node->specific_kind.Exp.left_exp =  var;
  node->specific_kind.Exp.op = op;
  node->specific_kind.Exp.right_exp = exp;
}

void set_node_array_param(TreeNode * node, TreeNode * type,  TreeNode * id)
{
      // printf("make node array param\n");

  node->nodekind = ParamK;
  node->specific_kind.Param.kind = ArrayK;
  node->specific_kind.Param.id = id;
  node->specific_kind.Param.type_specifier = type;
}

void set_node_compound_stmt(TreeNode * node ,TreeNode * local_declarations,  TreeNode * stmt_list)
{
  //printf("make node compound stmt\n");
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = CompoundK;
  node->specific_kind.Stmt.local_declarations = local_declarations;
  node->specific_kind.Stmt.stmt_list = stmt_list;
  node->specific_kind.Stmt.exp =NULL;
}

void set_node_selection(TreeNode* node, TreeNode* exp,  TreeNode* if_stmt , TreeNode* else_stmt)
{
  // printf("make node select stmt\n");

  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = SelectK;
  node->specific_kind.Stmt.exp = exp;
  node->specific_kind.Stmt.if_stmt= if_stmt;
  node->specific_kind.Stmt.else_stmt = else_stmt;
}
void set_node_iteration(TreeNode *node, TreeNode * exp , TreeNode *loop_stmt ){
      // printf("make node iter stmt\n");
 
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = IterK;
  node->specific_kind.Stmt.exp = exp;
  node->specific_kind.Stmt.loop_stmt= loop_stmt;
}


void set_node_return(TreeNode *node, TreeNode * exp){
  
      // printf("make node rets stmt\n");
  node->nodekind = StmtK;
  node->specific_kind.Stmt.kind = RetK;
  node->specific_kind.Stmt.exp = exp;
}
 char * token_to_char( TokenType op) {

  switch (op) {
    case EQ : return "==";
    case LT : return "<";
    case LE : return "<=";
    case GT : return ">";
    case NOTEQ : return "!=";
    case ASSIGN : return "=";

    case PLUS : return "+";
    case MINUS : return "-";
    case TIMES : return "*";
    case OVER : return "/";

    default : 
      printf("TOKEN ERROR \n");
      exit(-1);
  }
}

void set_node_op(TreeNode * node , TokenType op )
{
      // printf("make node op \n");

  node->nodekind = OpK;
  strcpy(node->specific_kind.Op.op, token_to_char(op));
}

void set_node_num (TreeNode* node , char * string) {
      // printf("make node num \n");

  node->nodekind = NumK;
  node->specific_kind.Num.num = atoi (string);
}
void set_id_type(TreeNode * node, Idkind isNewline) {
  if (isNewline){
    node->specific_kind.ID.kind = IsNewline;
  }
  else {
    node->specific_kind.ID.kind = NoNewline;
  }
}
void set_node_call_func(TreeNode *node , TreeNode *id , TreeNode * args)
{
      // printf("make node call \n");

  node->nodekind = DecK;
  node->specific_kind.Dec.kind = CallK;
  node->specific_kind.Dec.args_list = args;
  node->specific_kind.Dec.id = id;
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

void printTree( TreeNode * tree )
{ 

  INDENT;
  if (tree == NULL) { UNINDENT;return; }
  for (; tree != NULL; tree = tree->sibling){
   if (!(tree-> nodekind == StmtK && tree->specific_kind.Stmt.kind ==expK)) {
    if (tree->nodekind == IdK ){
        if( tree->specific_kind.ID.kind == IsNewline)
          printSpaces();
    }
    else printSpaces();
   }
   // printf("PRINT %d\n",  indentno);
    if (tree->nodekind == DecK)
    {
      switch (tree->specific_kind.Dec.kind) {
        case VarK:
          fprintf(listing,"Variable Declare : ");
          printTree(tree->specific_kind.Dec.id);  
          printTree(tree->specific_kind.Dec.type_specifier);
          break;
        case ArrayK:
          fprintf(listing,"Variable Array Declare : ");
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.num);
          break;
        case ArrayK2:
          fprintf(listing,"make Array\n");
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.id);
          break;
        case FunK:
          fprintf(listing,"Function Declare : ");
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.type_specifier);
          printTree(tree->specific_kind.Dec.params); 
          printTree(tree->specific_kind.Dec.compound_stmt);  
          break;
        case CallK:
          fprintf(listing,"Function Call \n");
          printTree(tree->specific_kind.Dec.id);
          printTree(tree->specific_kind.Dec.args_list); 
          break;
        default:
          fprintf(listing,"Unknown DecNode kind\n");
          break;
      }
    }
    else if (tree->nodekind == ParamK)
    { 
      switch (tree->specific_kind.Param.kind)
      {
        case VarParamK:
          fprintf(listing,"Parameter Variable : ");
          printTree(tree->specific_kind.Param.id);
          printTree(tree->specific_kind.Param.type_specifier);
          break;
        case ArrayParamK:
          fprintf(listing,"Parameter Array : ");
          printTree(tree->specific_kind.Param.id);
          printTree(tree->specific_kind.Param.type_specifier);
          break;
        default:
          fprintf(listing,"Unknown ParamK kind\n");
          break;
      }
    }
    else if (tree->nodekind == StmtK)
    { 
        switch (tree->specific_kind.Stmt.kind)
        {
          case expK : 
            printTree(tree->specific_kind.Stmt.exp);
            break;
          case CompoundK:
            fprintf(listing,"Compound Statement \n");
            printTree(tree->specific_kind.Stmt.local_declarations);  
            printTree(tree->specific_kind.Stmt.stmt_list);
            break;
          case SelectK:
            fprintf(listing,"If\n");
            printTree(tree->specific_kind.Stmt.exp);
            printTree(tree->specific_kind.Stmt.if_stmt);
            printTree(tree->specific_kind.Stmt.else_stmt);
            break;
          case IterK:
            fprintf(listing,"Iteration Statement \n");
            printTree(tree->specific_kind.Stmt.exp);
            printTree(tree->specific_kind.Stmt.loop_stmt);
            break;
          case RetK:
            fprintf(listing,"Return Statement \n");
            printTree(tree->specific_kind.Stmt.exp);
            break;
          case AssignK:
            fprintf(listing,"Assign : = \n");
            printTree(tree->specific_kind.Stmt.var);
            printTree(tree->specific_kind.Stmt.exp);
            break;
          default:
            fprintf(listing,"Unknown Stmt kind\n");
            break;
      }
    }
    else if (tree->nodekind == ExpK)
    {    
      switch (tree->specific_kind.Exp.kind) {
        case SimpleK:
            fprintf(listing,"Simple Expression \n");
            printTree(tree->specific_kind.Exp.left_exp);
            printTree(tree->specific_kind.Exp.op);   
            printTree(tree->specific_kind.Exp.right_exp);         
          break;
        case AddictiveK:
          fprintf(listing,"Additive Expression \n");
          printTree(tree->specific_kind.Exp.left_exp);
          printTree(tree->specific_kind.Exp.op);   
          printTree(tree->specific_kind.Exp.right_exp);         
          break;
        case MulopK:
          fprintf(listing,"Multiplcative/Over Expression \n");
          printTree(tree->specific_kind.Exp.left_exp);
          printTree(tree->specific_kind.Exp.op);   
          printTree(tree->specific_kind.Exp.right_exp);         
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind == TypeK)
    {  
      printf("Type : ");
      switch (tree->specific_kind.Type.kind) {
        case IntK:
          fprintf(listing,"int\n");         
          break;
        case VoidK:
         fprintf(listing,"void \n");         
          break;
        default:
          fprintf(listing,"Unknown Type kind\n");
          break;
      }
    }
    else if (tree->nodekind == OpK)
    {   
      fprintf(listing,"Operator : ");
      fprintf(listing,"%s\n", tree->specific_kind.Op.op);
    }
    else if (tree->nodekind == IdK)
    { 
      if (tree->specific_kind.ID.kind == IsNewline) fprintf(listing,"Variable : ");
      fprintf(listing,"%s\n", tree->specific_kind.ID.id);
    }
    else if (tree->nodekind == NumK)
    { 
      fprintf(listing,"Constant : ");
      fprintf(listing,"%d\n", tree->specific_kind.Num.num);
    }
    else fprintf(listing,"Unknown Extra node kind\n");
 
  }
  UNINDENT;

}