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
<<EOF>>             { return ENDFILE; }
"/*"                { BEGIN(COMMENTS); }
<COMMENTS>"*/"      { BEGIN(INITIAL); }
"*/"                { return ERROR; }
<COMMENTS>.         { }
<COMMENTS><<EOF>>   { 
                      BEGIN(INITIAL); 
                      return ERROR; 
                    }
<COMMENTS>{newline} { lineno++; }


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
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t\t%d ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}
