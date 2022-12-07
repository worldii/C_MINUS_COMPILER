/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"



#include "util.h"
#if NO_PARSE
// Flex 이용할 것이므로 scan 은 필요가 없음.
//#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  char pgm[120]; /* source code file name */

  if (argc != 2)
  { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
    exit(1);
  }
  strcpy(pgm,argv[1]) ;

  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".tny");
   
  source = fopen("test1.c","r");
  if (source == NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }

  listing = stdout; /* send listing to screen */
  //  fprintf(listing,"TINY COMPILATION: %s\n",pgm);

  /*for (int i = 0 ;i < strlen(pgm2) ; i++) 
  {
    if (pgm2[i] == '.')
    {
      pgm2[i] = 0;
      strcat(pgm2, "_20182202.txt");
      break;
    }
  }*/

  //listing = fopen("test1_20182202.txt", "wt");

#if NO_PARSE
  fprintf(listing,"TINY COMPILATION: hw1_20182202.txt\n");
  fprintf(listing,"    line number		token		lexeme\n");
  fprintf(listing,"================================================================================\n");
  while (getToken()!=ENDFILE);
#else
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);
  }
#if !NO_ANALYZE
  if (! Error)
  { if (TraceAnalyze) fprintf(listing,"\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nChecking Types...\n");
    typeCheck(syntaxTree);
    if (TraceAnalyze) fprintf(listing,"\nType Checking Finished\n");
  }
#if !NO_CODE
  if (! Error)
  { char * codefile;
    int fnlen = strcspn(pgm,".");
    codefile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".tm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("Unable to open %s\n",codefile);
      exit(1);
    }
    codeGen(syntaxTree,codefile);
    fclose(code);
  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}

