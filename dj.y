%code provides {
  #include <stdio.h>
  #include "ast.h"
  #include "lex.yy.c"
  #define YYSTYPE ASTree *
  ASTree *root;

  /* Function for printing generic syntax-error messages */
  void yyerror(const char *str) {
    printf("Syntax error on line %d at token %s\n",yylineno,yytext);
    printf("(This version of the compiler exits after finding the first ");
    printf("syntax error.)\n");
    exit(-1);
  }
}

%token CLASS ID EXTENDS MAIN NATTYPE BOOLTYPE
%token TRUELITERAL FALSELITERAL AND NOT IF ELSE FOR
%token NATLITERAL PRINTNAT READNAT PLUS MINUS TIMES EQUALITY LESS
%token STATIC ASSIGN NUL NEW THIS DOT INSTANCEOF
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN
%token ENDOFFILE

%start pgm

%right ASSIGN
%left AND
%nonassoc EQUALITY
%nonassoc LESS
%left PLUS MINUS
%left TIMES
%right NOT
%nonassoc INSTANCEOF
%left DOT

%%

pgm : instlist ENDOFFILE { root = $1; return 0; }
    ;

instlist: classlist MAIN LBRACE vardecllist exp_list RBRACE {$$ = newAST(PROGRAM, $1, 0, NULL, yylineno);
				 	      $$ = appendToChildrenList($$, $4);
					      $$ = appendToChildrenList($$, $5); }
   	;

classlist: classlist classdecl {$$ = newAST(CLASS_DECL_LIST, $2, 0, NULL, yylineno); }
	 | %empty {$$ = newAST(CLASS_DECL_LIST, NULL, 0, NULL, yylineno); }
	 ;				

classdecl: CLASS id_nt EXTENDS id_nt LBRACE staticvarlist classvarorclassmethod RBRACE {$$ = newAST(CLASS_DECL, $2, 0, NULL, yylineno);
											$$ = appendToChildrenList($$, $4);
											$$ = appendToChildrenList($$, $6);
											$$ = appendToChildrenList($$, $7); }
	 ;

id_nt: ID {$$ = newAST(AST_ID, NULL, 0, yytext, yylineno);}
     ;

staticvarlist: staticvarlist staticvar {$$ = newAST(STATIC_VAR_DECL_LIST, $2, 0, NULL, yylineno); }
	     | %empty {$$ = newAST(STATIC_VAR_DECL_LIST, NULL, 0, NULL, yylineno); }
	     ;

staticvar: STATIC nat_bool_id id_nt SEMICOLON {$$ = newAST(STATIC_VAR_DECL, $2, 0, NULL, yylineno);
					       $$ = appendToChildrenList($$, $3); }
	 ;

nat_bool_id: NATTYPE {$$ = newAST(NAT_TYPE, NULL, 0, NULL, yylineno); }
	   | BOOLTYPE {$$ = newAST(BOOL_TYPE, NULL, 0, NULL, yylineno); }
	   | id_nt {$$ = $1;}
	   ;

classvarorclassmethod: vardecl varrepeat {$$ = newAST(VAR_DECL_LIST, $1, 0, NULL, yylineno);
						   $$ = appendToChildrenList($$, $2); }
		     | classmethod methodrepeat {$$ = newAST(METHOD_DECL_LIST, $1, 0, NULL, yylineno);
						      $$ = appendToChildrenList($$, $2); }
		     | %empty {$$ = newAST(VAR_DECL_LIST, NULL, 0, NULL, yylineno); }
		     ;

classmethod: nat_bool_id id_nt LPAREN nat_bool_id id_nt RPAREN LBRACE vardecllist exp_list RBRACE {$$ = newAST(METHOD_DECL, $1, 0, NULL, yylineno);
									       $$ = appendToChildrenList($$, $2);
									       $$ = appendToChildrenList($$, $4);
									       $$ = appendToChildrenList($$, $5);
									       $$ = appendToChildrenList($$, $8);
									       $$ = appendToChildrenList($$, $9); }
	   ; 

varrepeat: nat_bool_id id_nt SEMICOLON varrepeat {$$ = newAST(VAR_DECL, $1, 0, NULL, yylineno);
						  $$ = appendToChildrenList($$, $2);
						  $$ = appendToChildrenList($$, $4); }
	 | nat_bool_id id_nt LPAREN nat_bool_id id_nt RPAREN LBRACE vardecllist exp_list RBRACE varrepeat {$$ = newAST(METHOD_DECL, $1, 0, NULL, yylineno);
									    		    $$ = appendToChildrenList($$, $2); 
									     		    $$ = appendToChildrenList($$, $4);
											    $$ = appendToChildrenList($$, $5);
								     			    $$ = appendToChildrenList($$, $8);
											    $$ = appendToChildrenList($$, $9);
											    $$ = appendToChildrenList($$, $11); }
	 | %empty 
	 ;

methodrepeat: nat_bool_id id_nt LPAREN nat_bool_id id_nt RPAREN LBRACE vardecllist exp_list RBRACE methodrepeat {
				$$ = newAST(METHOD_DECL, $1, 0, NULL, yylineno);
				$$ = appendToChildrenList($$, $2);
				$$ = appendToChildrenList($$, $4);
				$$ = appendToChildrenList($$, $5);
				$$ = appendToChildrenList($$, $8);
				$$ = appendToChildrenList($$, $9);
				$$ = appendToChildrenList($$, $11); }
	    | %empty
	    ;

vardecllist: %empty {$$ = newAST(VAR_DECL_LIST, NULL, 0, NULL, yylineno);}
	   | vardecllist vardecl {$$ = appendToChildrenList($1, $2); }

	   ;

vardecl: nat_bool_id id_nt SEMICOLON {$$ = newAST(VAR_DECL, $1, 0, NULL, yylineno);
				      $$ = appendToChildrenList($$, $2);}
       ;

exp_list: exp SEMICOLON {$$ = newAST(EXPR_LIST, $1, 0, NULL, yylineno); } 
	| exp_list exp SEMICOLON {$$ = appendToChildrenList($1, $2); }
	;

exp: exp PLUS exp {$$ = newAST(PLUS_EXPR, $1, 0, NULL, yylineno);
		   $$ = appendToChildrenList($$, $3); }
   | exp MINUS exp {$$ = newAST(MINUS_EXPR, $1, 0, NULL, yylineno);
                    $$ = appendToChildrenList($$, $3); }
   | exp TIMES exp {$$ = newAST(TIMES_EXPR, $1, 0, NULL, yylineno);
                    $$ = appendToChildrenList($$, $3); }
   | exp EQUALITY exp {$$ = newAST(EQUALITY_EXPR, $1, 0, NULL, yylineno);
                       $$ = appendToChildrenList($$, $3); }
   | exp LESS exp {$$ = newAST(LESS_THAN_EXPR, $1, 0, NULL, yylineno);
                   $$ = appendToChildrenList($$, $3); }
   | NOT exp {$$ = newAST(NOT_EXPR, $2, 0, NULL, yylineno); }
   | exp AND exp {$$ = newAST(AND_EXPR, $1, 0, NULL, yylineno);
                  $$ = appendToChildrenList($$, $3); }
   | NATLITERAL {$$ = newAST(NAT_LITERAL_EXPR, NULL, atoi(yytext), NULL, yylineno); }
   | TRUELITERAL {$$ = newAST(TRUE_LITERAL_EXPR, NULL, 0, NULL, yylineno); }
   | FALSELITERAL {$$ = newAST(FALSE_LITERAL_EXPR, NULL, 0, NULL, yylineno); }
   | NUL {$$ = newAST(NULL_EXPR, NULL, 0, NULL, yylineno); }
   | IF LPAREN exp RPAREN LBRACE exp_list RBRACE ELSE LBRACE exp_list RBRACE {$$ = newAST(IF_THEN_ELSE_EXPR, $3, 0, NULL, yylineno);
									      $$ = appendToChildrenList($$, $6);
									      $$ = appendToChildrenList($$, $10); } 
   | FOR LPAREN exp SEMICOLON exp SEMICOLON exp RPAREN LBRACE exp_list RBRACE {$$ = newAST(FOR_EXPR, $3, 0, NULL, yylineno);
									       $$ = appendToChildrenList($$, $5);
									       $$ = appendToChildrenList($$, $7);
									       $$ = appendToChildrenList($$, $10); } 
   | NEW id_nt LPAREN RPAREN { $$ = newAST(NEW_EXPR, $2, 0, NULL, yylineno); }
   | THIS {$$ = newAST(THIS_EXPR, NULL, 0, NULL, yylineno); }
   | exp INSTANCEOF id_nt {$$ = newAST(INSTANCEOF_EXPR, $1, 0, NULL, yylineno);
			   $$ = appendToChildrenList($$, $3); }
   | PRINTNAT LPAREN exp RPAREN {$$ = newAST(PRINT_EXPR, $3, 0, NULL, yylineno); }
   | READNAT LPAREN RPAREN {$$ = newAST(READ_EXPR, NULL, 0, NULL, yylineno); }
   | exp DOT id_nt {$$ = newAST(DOT_ID_EXPR, $1, 0, NULL, yylineno);
		    $$ = appendToChildrenList($$, $3); }
   | id_nt ASSIGN exp {$$ = newAST(ASSIGN_EXPR, $1, 0, NULL, yylineno);
		       $$ = appendToChildrenList($$, $3); }
   | exp DOT id_nt ASSIGN exp {$$ = newAST(DOT_ASSIGN_EXPR, $1, 0, NULL, yylineno);

			       $$ = appendToChildrenList($$, $3);
			       $$ = appendToChildrenList($$, $5); }
   | id_nt LPAREN exp RPAREN {$$ = newAST(METHOD_CALL_EXPR, $1, 0, NULL, yylineno);
			      $$ = appendToChildrenList($$, $3); }
   | exp DOT id_nt LPAREN exp RPAREN {$$ = newAST(DOT_METHOD_CALL_EXPR, $1, 0, NULL, yylineno);
				      $$ = appendToChildrenList($$, $3);
				      $$ = appendToChildrenList($$, $5); }
   | LPAREN exp RPAREN {$$ = $2;}
   | id_nt {$$ = newAST(ID_EXPR, $1, 0, NULL, yylineno);}
   ;

%%

int main(int argc, char **argv) {
  if(argc!=2) {
    printf("Usage: dj-parse filename\n");
    exit(-1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin==NULL) {
    printf("ERROR: could not open file %s\n",argv[1]);
    exit(-1);
  }
  /* parse the input program */
  if(yyparse() == 0){
    printAST(root);
  }else{
    printf("parsing failed");
    return -1;
  }

  return 0; 
}
