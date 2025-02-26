#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "typecheck.h"

void typecheckProgram(){
	//typecheck main block
	int i, j;
	//check for duplicate names
	for(i=0; i < numMainBlockLocals; i++){
		for(j=i+1; j < numMainBlockLocals; j++){
			if(strcmp(mainBlockST[i].varName, mainBlockST[j].varName)==0){
				printf("error on line %d: main block local variable duplicate name\n", mainBlockST[i].varNameLineNumber);
				return;
			}
		}
	}
	
	//check for valid types	
	for(i=0; i < numMainBlockLocals; i++){
		if(mainBlockST[i].type < -2){
			printf("error on line %d: invalid main block local variable type\n", mainBlockST[i].typeLineNumber);
			return;
		}
	}

	//check main expressions
	typeExprs(mainExprs, 0, 0);		
}

int isSubtype(int sub, int super){
	if(sub == -3 || sub == super){
		return 1;
	} else{
		return 0;
	}
}

int typeExpr(ASTree *t, int classContainingExpr, int methodContainingExpr){
	if(t==NULL){
		printf("tree is empty\n");
		exit(EXIT_FAILURE);
	}

	//non id expressions
	if(t->typ == NAT_LITERAL_EXPR) return -1;
	if(t->typ == TRUE_LITERAL_EXPR) return -2;
	if(t->typ == FALSE_LITERAL_EXPR) return -2;
	if(t->typ == NULL_EXPR) return -3;
	if(t->typ == NOT_EXPR) {
		int T1 = typeExpr(t->children->data, 0, 0);
		if(T1 != -2){
			printf("NOT_EXPR type error (not bool) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -2;
	}
	if(t->typ == PLUS_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		int T2 = typeExpr(t->children->next->data, 0, 0);
		if(T1 != -1 || T2 != -1){
			printf("PLUS_EXPR type error (not nat) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	if(t->typ == MINUS_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		int T2 = typeExpr(t->children->next->data, 0, 0);
		if(T1 != -1 || T2 != -1){
			printf("MINUS_EXPR type error (not nat) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	if(t->typ == TIMES_EXPR){
                int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
                if(T1 != -1 || T2 != -1){
                        printf("TIMES_EXPR type error (not nat) on line %d\n", t->lineNumber);
                        exit(EXIT_FAILURE);
                }
                return -1;
	}
	if(t->typ == EQUALITY_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
		if(!isSubtype(T1,T2) && !isSubtype(T2,T1)){
			printf("EQUALITY_EXPR error (comparison is not a subtype) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -2;
	}
	if(t->typ == LESS_THAN_EXPR){
                int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
                if(T1 != -1 || T2 != -1){
                        printf("LESS_THAN_EXPR type error (not nat) on line %d\n", t->lineNumber);
                	exit(EXIT_FAILURE);
                }
                return -2;
	}
	if(t->typ == AND_EXPR){
                int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
		if(T1 != -2 || T2 != -2){
			printf("AND_EXPR type error (not bool) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -2;
	}
	if(t->typ == PRINT_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		if(T1 != -1){
			printf("PRINT_EXPR type error (not nat) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	if(t->typ == READ_EXPR) return -1;
	int i;
	if(t->typ == NEW_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		//only used with object keyword
		if(t->children->data->idVal != NULL){
			if(strcmp(t->children->data->idVal, "Object")==0){
				return 0;
			}
			else{
				printf("NEW_EXPR error (new can only be used with the Object keyword since no classes declared) on line %d\n", t->lineNumber);
				exit(EXIT_FAILURE);
			}
		}
		else{
			printf("NEW_EXPR error (new must have id) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
	}
	if(t->typ == IF_THEN_ELSE_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		if(T1!=-2){
			printf("IF_EXPR type error (not bool) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		int T2 = typeExprs(t->children->next->data, 0, 0);
		int T3 = typeExprs(t->children->next->next->data, 0, 0);
		if((T2==-1 && T3==-1) || (T2==-2 && T3==-2)){
			return T2;
		}
		if((T2>=0 || T2==-3) && (T3>=0 || T3==-3)){
			int t4 = join(T2, T3);
			if(t4==-5){
				printf("IF_EXPR type error (not joinable) on line %d\n", t->lineNumber);
				exit(EXIT_FAILURE);
			}
			else{
				return t4;
			}
		}
		printf("IF_THEN_ELSE type error (then and else expression types are wrong) on line %d\n", t->lineNumber);
		exit(EXIT_FAILURE);
	}	
	if(t->typ == FOR_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		int T2 = typeExpr(t->children->next->data, 0, 0);
		int T3 = typeExpr(t->children->next->next->data, 0, 0);
		int T4 = typeExprs(t->children->next->next->next->data, 0, 0);
		if(T2 != -2){
			printf("FOR_EXPR type error (test is not bool) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		if(T1 == -5 || T3 == -5 || T4 == 5){
			printf("FOR_EXPR type error on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return -1;
	}
	if(t->typ == THIS_EXPR){
		//no classes in prgm
		printf("THIS_EXPR error (use outside of a class) on line %d\n", t->lineNumber);
		exit(EXIT_FAILURE);
	}
	//id expressions
	int j;
	if(t->typ == ID_EXPR){
		for(i = 0; i < numMainBlockLocals; i++){
			if(strcmp(mainBlockST[i].varName, t->children->data->idVal)==0){
				return mainBlockST[i].type;
			}
		}
		printf("ID_EXPR error (var not found) on line %d\n", t->lineNumber);
		exit(EXIT_FAILURE);
	}
	if(t->typ == METHOD_CALL_EXPR){
		int T1=-6;
		for(i = 0; i < numMainBlockLocals; i++){
			if(strcmp(mainBlockST[i].varName, t->children->data->idVal)==0){
				T1 = mainBlockST[i].type;
			}
		}
		if(T1 == -6){
			printf("METHOD_CALL_EXPR error (id not found) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		if(T1 == -5){
			printf("METHOD_CALL_EXPR error (id not well typed) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		int T2 = typeExpr(t->children->next->data, 0, 0);
		if(T2 == -5){
			printf("METHOD_CALL_EXPR error (expr not well typed) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		return T1;		
	}
	if(t->typ == ASSIGN_EXPR){
		int T1=-6;
                for(i = 0; i < numMainBlockLocals; i++){
                        if(strcmp(mainBlockST[i].varName, t->children->data->idVal)==0){
                                T1 = mainBlockST[i].type;
                        }
                }
                if(T1 == -6){
                        printf("ASSIGN_EXPR error (id not found) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
                }
		if(T1 == -5){
			printf("ASSIGN_EXPR error (id not well typed) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		int T2 = typeExpr(t->children->next->data, 0, 0);
		if(T2 == -5){
			printf("ASSIGN_EXPR error (E not well typed) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		else if(!isSubtype(T2, T1)){
			printf("ASSIGN_EXPR error (E is not a subtype of ID) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
		else if(isSubtype(T2, T1)){
			return T1;
		}
		else{
			return -5;
		}
	}
	if(t->typ == INSTANCEOF_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		int T2 = typeExpr(t->children->next->data, 0, 0);
		//no classes in prgm
		printf("INSTANCEOF_EXPR error (there are no classes declared, cannot use instance of without a class) on line %d\n", t->lineNumber);
		exit(EXIT_FAILURE);
	}
	if(t->typ == DOT_ID_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
		if(T1==0){
			printf("DOT_ID_EXPR error (object cannot be used) on line %d\n", t->lineNumber);
			exit(EXIT_FAILURE);
		}
                int T2 = typeExpr(t->children->next->data, 0, 0);
		printf("DOT_ID_EXPR error (there are no classes declared, so cannot use dot expressions) on line %d\n", t->lineNumber);
		exit(EXIT_FAILURE);
	}
	if(t->typ == DOT_ASSIGN_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
		int T3 = typeExpr(t->children->next->next->data, 0, 0);
		if(T1==0){
                        printf("DOT_ASSIGN_EXPR error (object cannot be used) on line %d\n", t->lineNumber);
                        exit(EXIT_FAILURE);
                }
		printf("DOT_ASSIGN_EXPR error (there are no classes declared, so cannot use dot expressions) on line %d\n", t->lineNumber);
                exit(EXIT_FAILURE);
	}
	if(t->typ == DOT_METHOD_CALL_EXPR){
		int T1 = typeExpr(t->children->data, 0, 0);
                int T2 = typeExpr(t->children->next->data, 0, 0);
		int T3 = typeExpr(t->children->next->next->data, 0, 0);
		if(T1==0){
                        printf("DOT_METHOD_CALL_EXPR error (object cannot be used) on line %d\n", t->lineNumber);
                        exit(EXIT_FAILURE);
                }
		printf("DOT_METHOD_CALL_EXPR error (there are no classes declared, so cannot use dot expressions) on line %d\n", t->lineNumber);
                exit(EXIT_FAILURE);
	}

	return -5;	
}

int join(int t1, int t2){
	if(isSubtype(t1, t2)){
		return t2;
	}
	else if(isSubtype(t2, t1)){
		return t1;
	}
	else{
		return join(classesST[t1].superclass, t2);
	}
	return -5;
}

int typeExprs(ASTree *t, int classContainingExpr, int methodContainingExpr){
	if(t==NULL){
		printf("tree is empty\n");
		return -5;
	}

	int overallType = -5;

	ASTList *child;
	for(child = t->children; child!=NULL; child = child->next){
		if(child->data != NULL){
			int exprType = typeExpr(child->data, 0, 0);
			overallType = exprType; //expr list type = last expr type
		}
	}
	
	return overallType;

}






