#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

ASTree *newAST(ASTNodeType t, ASTree *child, unsigned int natAttribute, char *idAttribute, unsigned int lineNum){
	ASTree *node = (ASTree *)malloc(sizeof(ASTree));
	if(node == NULL){
		printf("malloc in newAST");
	}
	
	node->typ = t;

	node->natVal = natAttribute;

	if(idAttribute==NULL) node->idVal = NULL;
	else{
		char *copyStr1 = malloc(strlen(idAttribute)+1);
		if(copyStr1==NULL) printf("malloc in newAST");
		strcpy(copyStr1, idAttribute);
		node->idVal = copyStr1;
	}

	node->lineNumber = lineNum;

/*	node->staticClassNum = 0;
	node->isMemberStaticVar = 0;
	node->staticMemberNum = 0;	 */

	ASTList *childList = malloc(sizeof(ASTList));
	if(childList==NULL){
		printf("malloc in newAST");
	}
	childList->data = child;
	childList->next = NULL;
	node->children = childList;
	node->childrenTail = childList;

	return node;
}

ASTree *appendToChildrenList(ASTree *parent, ASTree *newChild){
	if(parent == NULL){
		printf("append called with null parent");
	}
	if(parent->childrenTail->data == NULL){
		parent->childrenTail->data = newChild;
	}
	else{
		ASTList *newList = malloc(sizeof(ASTList));
		if(newList==NULL) printf("malloc in appendToChildrenList");
		newList->data = newChild;
		newList->next = NULL;
		parent->childrenTail->next = newList;
		parent->childrenTail = newList;
	}
	return parent;
}


void printNodeTypeAndAttribute(ASTree *t){
	if(t==NULL) return;
	switch(t->typ){
		case PROGRAM: printf("PROGRAM"); break;
  		case CLASS_DECL_LIST: printf("CLASS_DECL_LIST"); break;
  		case CLASS_DECL: printf("CLASS_DECL"); break;
  		case STATIC_VAR_DECL_LIST: printf("STATIC_VAR_DECL_LIST"); break;
  		case STATIC_VAR_DECL: printf("STATIC_VAR_DECL"); break;
  		case VAR_DECL_LIST: printf("VAR_DECL_LIST"); break;
  		case VAR_DECL: printf("VAR_DECL"); break;
  		case METHOD_DECL_LIST: printf("METHOD_DECL_LIST"); break;
  		case METHOD_DECL: printf("METHOD_DECL"); break;
  		case NAT_TYPE: printf("NAT_TYPE"); break;
  		case BOOL_TYPE: printf("BOOL_TYPE"); break;
  		case AST_ID: printf("AST_ID(%s)", t->idVal); break;
  		case EXPR_LIST: printf("EXPR_LIST"); break;
  		case DOT_METHOD_CALL_EXPR: printf("DOT_METHOD_CALL_EXPR"); break;
  		case METHOD_CALL_EXPR: printf("METHOD_CALL_EXPR"); break; 
  		case DOT_ID_EXPR: printf("DOT_ID_EXPR"); break;
  		case ID_EXPR: printf("ID_EXPR"); break; 
  		case DOT_ASSIGN_EXPR: printf("DOT_ASSIGN_EXPR"); break;
  		case ASSIGN_EXPR: printf("ASSIGN_EXPR"); break;
  		case PLUS_EXPR: printf("PLUS_EXPR"); break;
  		case MINUS_EXPR: printf("MINUS_EXPR"); break;
  		case TIMES_EXPR: printf("TIMES_EXPR"); break;
  		case EQUALITY_EXPR: printf("EQUALITY_EXPR"); break;
  		case LESS_THAN_EXPR: printf("LESS_THAN_EXPR"); break;
  		case NOT_EXPR: printf("NOT_EXPR"); break;
  		case AND_EXPR: printf("AND_EXPR"); break;
  		case INSTANCEOF_EXPR: printf("INSTANCEOF_EXPR"); break;
  		case IF_THEN_ELSE_EXPR: printf("IF_THEN_ELSE_EXPR"); break;
  		case FOR_EXPR: printf("FOR_EXPR"); break;
  		case PRINT_EXPR: printf("PRINT_EXPR"); break;
  		case READ_EXPR: printf("READ_EXPR"); break;
  		case THIS_EXPR: printf("THIS_EXPR"); break;
  		case NEW_EXPR: printf("NEW_EXPR"); break;
  		case NULL_EXPR: printf("NULL_EXPR"); break;
  		case NAT_LITERAL_EXPR: printf("NAT_LITERAL_EXPR(%d)", t->natVal); break;
  		case TRUE_LITERAL_EXPR: printf("TRUE_LITERAL_EXPR"); break;
  		case FALSE_LITERAL_EXPR: printf("FALSE_LITERAL_EXPR"); break;
	}
}

void printASTHelper(ASTree *node, int depth) {
	if(node==NULL) return;
	printf("%d:",depth);
	int i = 0;
	for(; i<depth; i++) printf("   ");
	printNodeTypeAndAttribute(node);
	printf("   (ends on line %d)", node->lineNumber);
	printf("\n");
	
	ASTList *childListIterator = node->children;
	while(childListIterator!=NULL){
		printASTHelper(childListIterator->data, depth+1);
		childListIterator = childListIterator->next;
	}
}

void printAST(ASTree *t) { printASTHelper(t, 0); }




