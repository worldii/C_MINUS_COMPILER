/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* );

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString( char * );

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */


// MY IMPLEMENTATION 
void printTree( TreeNode * );

// INITIALIZE 
TreeNode * node_initialize();
TreeNode* add_sibling(TreeNode *node, TreeNode* new_node);

// DECLARE 
void declare_var(TreeNode * node, TreeNode * type, TreeNode *id);
void declare_array(TreeNode * node,TreeNode * type, TreeNode *id, TreeNode * num );
void declare_func(TreeNode *node, TreeNode* type, TreeNode * id, TreeNode * params , TreeNode* compound_stmt);
void set_node_call_func(TreeNode *node , TreeNode *id , TreeNode * args);

// STATEMENT 
void set_node_compound_stmt(TreeNode * node ,TreeNode * local_declarations,  TreeNode * stmt_list);
void set_node_selection(TreeNode* node, TreeNode* exp,  TreeNode* if_stmt , TreeNode* else_stmt);
void set_node_iteration(TreeNode *node, TreeNode * exp , TreeNode *loop_stmt );
void set_node_return(TreeNode *node, TreeNode * exp);

// EXPRESSION
void set_node_exp(TreeNode * node,  TreeNode * exp);
void set_node_exp_assign(TreeNode* node, TreeNode *var, TreeNode * exp);
void set_node_exp_simple(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp);
void set_node_exp_mulop(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp);
void set_node_exp_addictive(TreeNode* node, TreeNode *var,TreeNode* op,  TreeNode * exp);

// TYPE , NUM , OP , TYPE , ARRAY
void set_node_op(TreeNode * node , TokenType op );
void set_node_num (TreeNode* node , char * string) ;
void set_node_id(TreeNode *node,  char * id);
void set_node_type(TreeNode *node, TypeKind type);
void set_node_array(TreeNode * node,TreeNode * type, TreeNode *id );

// PARAMETER 
void set_node_var_param(TreeNode * node,  TreeNode * type,  TreeNode * id);
void set_node_array_param(TreeNode * node, TreeNode * type,  TreeNode * id);

// UTILS
char * token_to_char( TokenType op);
void set_id_type(TreeNode * node, Idkind isNewline);

#endif
