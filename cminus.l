/****************************************************/
/* File: cminus.l                                   */
/* Lex specification for C-                         */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%option noyywrap

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

%x COMMENTS

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%


"/*"                { BEGIN(COMMENTS); }
<COMMENTS>"*/"       { BEGIN(0); }
<COMMENTS>{newline}  { lineno++; }
<COMMENTS><<EOF>>    {  return COMMENTERR; }
<COMMENTS>.          { }
"*/"                { return COMMENTERR; }
<<EOF>>             { return ENDFILE; }

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"void"          {return VOID;}
"return"        {return RETURN;}
"while"         {return WHILE;}
"="             {return ASSIGN;}
"=="             {return EQ;}
"!="            {return NOTEQ;}
"<"             {return LT;}
"<="            {return LE;}
">"             {return GT;}
">="            {return GE;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
";"             {return SEMI;}
","             {return COMMA;}
{number}        {return NUM;}
{identifier}    {return ID;}
{whitespace}    {/* skip whitespace */}
{newline}           { lineno++; }

%%

TokenType getToken(void)
{ 
  static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { 
    firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = stdout;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"%d ",lineno);
    printToken(currentToken,tokenString);
  }
  
  return currentToken;
}
