%option yylineno
%option noyywrap

%{
  #define DEBUG 0
  typedef YYTOKENTYPE Token;
  Token scanned(Token t);
%}

id	[a-zA-Z]+[a-zA-Z0-9]*
natliteral	[0-9]+
wspace	[ \t\r\n ]+

%%

"//".*$    		{/* Ignore comments starting with // */}
"class"                 { return scanned(CLASS); }
"extends"               { return scanned(EXTENDS); }
"main"                  { return scanned(MAIN); }
"nat"                   { return scanned(NATTYPE); }
"bool"                  { return scanned(BOOLTYPE); }
"true"                  { return scanned(TRUELITERAL); }
"false"                 { return scanned(FALSELITERAL); }
"&&"                    { return scanned(AND); }
"!"                     { return scanned(NOT); }
"if"                    { return scanned(IF); }
"else"                  { return scanned(ELSE); }
"for"                   { return scanned(FOR); }
"printNat"              { return scanned(PRINTNAT); }
"readNat"               { return scanned(READNAT); }
"+"                     { return scanned(PLUS); }
"-"                     { return scanned(MINUS); }
"*"                     { return scanned(TIMES); }
"="                     { return scanned(ASSIGN); }
"<"                     { return scanned(LESS); }
"static"                { return scanned(STATIC); }
"=="                    { return scanned(EQUALITY); }
"null"                  { return scanned(NUL); }
"new"                   { return scanned(NEW); }
"this"                  { return scanned(THIS); }
"."                     { return scanned(DOT); }
"instanceof"            { return scanned(INSTANCEOF); }
{id}			{ return scanned(ID); }
{natliteral}+       { return scanned(NATLITERAL); }
";"                     { return scanned(SEMICOLON); }
"{"                     { return scanned(LBRACE); }
"}"                     { return scanned(RBRACE); }
"("                     { return scanned(LPAREN); }
")"                     { return scanned(RPAREN); }
<<EOF>>                 { return scanned(ENDOFFILE); }
{wspace}		{ /* skip whitespace*/}
.                       { /* Ignore any other characters */ 
			 if(DEBUG==1){ printf("Lex error on line %d: Illegal character %s\n",
				  yylineno, yytext);
			  exit(-1);
				}
			}

%%

/*process and return scanned token t */
Token scanned(Token t){
	if(DEBUG==0) return t;
	 switch (t) {
    		case CLASS: printf("CLASS "); return t;
		case EXTENDS: printf("EXTENDS "); return t;
    		case MAIN: printf("MAIN "); return t;
    		case NATTYPE: printf("NATTYPE "); return t;
    		case BOOLTYPE: printf("BOOLTYPE "); return t;
   		case TRUELITERAL: printf("TRUELITERAL "); return t;
    		case FALSELITERAL: printf("FALSELITERAL "); return t;
    		case AND: printf("AND "); return t;
    		case NOT: printf("NOT "); return t;
    		case IF: printf("IF "); return t;
    		case ELSE: printf("ELSE "); return t;
    		case FOR: printf("FOR "); return t;
    		case PRINTNAT: printf("PRINTNAT "); return t;
    		case READNAT: printf("READNAT "); return t;
    		case PLUS: printf("PLUS "); return t;
    		case MINUS: printf("MINUS "); return t;
    		case TIMES: printf("TIMES "); return t;
    		case EQUALITY: printf("EQUALITY "); return t;
    		case LESS: printf("LESS "); return t;
    		case STATIC: printf("STATIC "); return t;
    		case ASSIGN: printf("ASSIGN "); return t;
    		case NUL: printf("NUL "); return t;
    		case NEW: printf("NEW "); return t;
    		case THIS: printf("THIS "); return t;
    		case DOT: printf("DOT "); return t;
    		case INSTANCEOF: printf("INSTANCEOF "); return t;
    		case LBRACE: printf("LBRACE "); return t;
    		case RBRACE: printf("RBRACE "); return t;
    		case LPAREN: printf("LPAREN "); return t;
    		case RPAREN: printf("RPAREN "); return t;
                case ID: printf("ID(%s) ", yytext); return t;
		case NATLITERAL: printf("NATLITERAL(%s) ", yytext); return t;
    		case SEMICOLON: printf("SEMICOLON "); return t;
    		case ENDOFFILE: printf("ENDOFFILE\n"); return t;
    		default: printf("ERROR: invalid token in scanned().\n"); exit(-1);
  }
}
		

