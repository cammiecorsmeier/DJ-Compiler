#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "ast.h"
#include "typecheck.h"

#define MAX_DISM_ADDR 65535

FILE *fout;
unsigned int labelNum = 0;

void codeGenExpr(ASTree *t, int classNumber, int methodNumber);

void incSP(){
	fprintf(fout, "mov 1 1\n");
        fprintf(fout, "add 6 6 1 ;increase SP\n");  	//incrememnt the SP stored in register 6
        fprintf(fout, "blt 5 6 #%d\n", labelNum);   	//if heap ptr < stack ptr go to labelNum
        fprintf(fout, "mov 1 77\n");			//else error 77: out of stack memory
        fprintf(fout, "hlt 1\n");               	//halt after error
        fprintf(fout, "#%d: mov 0 0\n", labelNum);
        fprintf(fout, "mov 0 0\n");
        labelNum++;
}

void decSP(){
	fprintf(fout, "mov 1 1\n");
	fprintf(fout, "sub 6 6 1 ;decrease SP\n"); 	//decrememnt the SP stored in register 6
	fprintf(fout, "blt 5 6 #%d\n", labelNum); 	//if heap ptr < stack ptr go to labelNum
	fprintf(fout, "mov 1 77\n"); 			//else error 77: out of stack memory
	fprintf(fout, "hlt 1\n"); 			//halt after error
	fprintf(fout, "#%d: mov 0 0\n", labelNum);
	fprintf(fout, "mov 0 0\n");
	labelNum++;
}

void codeGenExprs(ASTree *expList, int classNumber, int methodNumber){
        if(expList==NULL){
                printf("tree is empty\n");	//make sure the tree exists
                fprintf(fout, "hlt 1\n");	//else halt and exit
                exit(EXIT_FAILURE);
        }
        ASTList *child;
        for(child = expList->children; child!=NULL; child = child->next){
                if(child->data != NULL){		//make sure the tree has children (aka expressions to generate code for)
                        codeGenExpr(child->data, 0, 0);	//generate code for each expression
                }
                else{
                        printf("child empty\n");	//else there are no expressions to generate code for
                        fprintf(fout, "hlt 1\n");	//halt and exit
                        exit(EXIT_FAILURE);
                }
        }
}

void codeGenExpr(ASTree *t, int classNumber, int methodNumber){
	//generate code for main block
	if(t->typ == PLUS_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL){
			printf("Error: PLUS_EXPR child is null\n");
			fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
		}
		codeGenExpr(t->children->data, -1, -1);
		codeGenExpr(t->children->next->data, -1, -1);
		fprintf(fout, "lod 1 6 2\n"); 		 //loads e1 from top of stack
		fprintf(fout, "lod 2 6 1\n");		 //grabs e2 from the stack
		fprintf(fout, "add 1 1 2 ;plus expr\n"); //adds e1 and e2
		fprintf(fout, "str 6 2 1\n");		 //stores the result in stack
		incSP(); //increase because result is being stored now instead of both e1 and e2
						
	}
	if(t->typ == MINUS_EXPR){
		 if(t->children->data == NULL || t->children->next->data == NULL){
                        printf("Error: MINUS_EXPR child is null\n");
                        fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
                }
                codeGenExpr(t->children->data, -1, -1);
                codeGenExpr(t->children->next->data, -1, -1);
                fprintf(fout, "lod 1 6 2\n");             //loads e1 from top of stack
                fprintf(fout, "lod 2 6 1\n");             //grabs e2 from the stack
                fprintf(fout, "sub 1 1 2 ;minus expr\n"); //subtracts e1 and e2
                fprintf(fout, "str 6 2 1\n");             //stores the result in stack
                incSP(); //increase because result is being stored now instead of both e1 and e2
	}
	if(t->typ == TIMES_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL){
                        printf("Error: TIMES_EXPR child is null\n");
			fprintf(fout, "hlt 1\n");
                        exit(EXIT_FAILURE);
                }
                codeGenExpr(t->children->data, -1, -1);
                codeGenExpr(t->children->next->data, -1, -1);
                fprintf(fout, "lod 1 6 2\n");             //loads e1 from top of stack
                fprintf(fout, "lod 2 6 1\n");             //grabs e2 from the stack
		fprintf(fout, "mul 1 1 2 ;times expr\n"); //multiply them together
                fprintf(fout, "str 6 2 1\n");		  //stores the result in stack
                incSP(); //increase because result is being stored now instead of both e1 and e2
	}
	if(t->typ == EQUALITY_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL){
                        printf("Error: EQUALITY_EXPR child is null\n");
                        fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
                }
		int trueLabel = labelNum++;
		int endLabel = labelNum++;
		labelNum+=2;
                codeGenExpr(t->children->data, -1, -1);
                codeGenExpr(t->children->next->data, -1, -1);
                fprintf(fout, "lod 1 6 2\n");           //loads e1 from top of stack
                fprintf(fout, "lod 2 6 1\n");           //grabs e2 from the stack
		fprintf(fout, "beq 1 2 #%d ;equality expr\n", trueLabel);	//if they are equal, go to true
		fprintf(fout, "mov 1 0\n");		//else, set to false
		fprintf(fout, "jmp 0 #%d\n", endLabel);	//and jump to the end
		fprintf(fout, "#%d: mov 0 0\n", trueLabel);		
		fprintf(fout, "mov 1 1\n");		//set to true
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
		fprintf(fout, "str 6 2 1\n");           //stores the result in stack
                incSP(); //increase because result (0 or 1) is being stored now instead of both e1 and e2
	}
	if(t->typ == LESS_THAN_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL){
                        printf("Error: LESS_THAN child is null\n");
                        fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
                }
		int trueLabel = labelNum++;
		int endLabel = labelNum++;
		labelNum+=2;
                codeGenExpr(t->children->data, -1, -1);
                codeGenExpr(t->children->next->data, -1, -1);
                fprintf(fout, "lod 1 6 2\n");           //loads e1 from top of stack
                fprintf(fout, "lod 2 6 1\n");           //grabs e2 from the stack
                fprintf(fout, "blt 1 2 #%d ;less than expr\n", trueLabel);       //if 1 is less than 2, go to true
                fprintf(fout, "mov 1 0\n");             //else, set to false
		fprintf(fout, "jmp 0 #%d\n", endLabel);	//and jump to end
                fprintf(fout, "#%d: mov 0 0\n", trueLabel);
                fprintf(fout, "mov 1 1\n");             //set to true
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
                fprintf(fout, "str 6 2 1\n");           //stores the result in stack
                incSP(); //increase because result (0 or 1) is being stored now instead of both e1 and e2
	}
	if(t->typ == NOT_EXPR){
		if(t->children->data == NULL){
                        printf("Error: NOT_EXPR child is null\n");
                        fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
                }
		int trueLabel = labelNum++;
		int endLabel = labelNum++;
		labelNum+=2;
                codeGenExpr(t->children->data, -1, -1);
                fprintf(fout, "lod 1 6 1\n");           //loads e1 from top of stack
		fprintf(fout, "mov 2 0\n");		//set register 2 = 0 to use for comparison
                fprintf(fout, "beq 1 2 #%d ;not expr\n", trueLabel);       //if e1 = 0, return true (bc !false = true)
                fprintf(fout, "mov 1 0\n");             //else, set to false
		fprintf(fout, "jmp 0 #%d\n", endLabel);	//and jump to end
                fprintf(fout, "#%d: mov 0 0\n", trueLabel);
                fprintf(fout, "mov 1 1\n");             //set to true
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
                fprintf(fout, "str 6 1 1\n");           //stores the result in stack
		//no incSP() because the result (0 or 1) just replaces where e1 was on the stack
	}
	if(t->typ == AND_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL){
                        printf("Error: AND_EXPR child is null\n");
			fprintf(fout, "hlt 1\n");
                        exit(EXIT_FAILURE);
                }
		int endLabel = labelNum++;
		int check = labelNum++;
		int trueLabel = labelNum++;
		labelNum+=3;
                codeGenExpr(t->children->data, -1, -1);
                codeGenExpr(t->children->next->data, -1, -1);
                fprintf(fout, "lod 1 6 2\n");           //loads e1 from top of stack
                fprintf(fout, "lod 2 6 1\n");           //grabs e2 from the stack
		fprintf(fout, "mov 3 1\n");		//register 3 = 1 for comparison
                fprintf(fout, "beq 1 3 #%d ;and expr\n", check);	//if 1 is = 1, go to check
                fprintf(fout, "mov 1 0\n");             //else, set to false
		fprintf(fout, "jmp 0 #%d\n", endLabel);	//and jump to end
                fprintf(fout, "#%d: mov 0 0\n", check);
		fprintf(fout, "beq 2 3 #%d\n", trueLabel); //if 2 is also = 1, go to true		
                fprintf(fout, "mov 1 0\n");             //else set to false
		fprintf(fout, "jmp 0 #%d\n", endLabel);	//and jump to end
		fprintf(fout, "#%d: mov 0 0\n", trueLabel);
		fprintf(fout, "mov 1 1\n");		//if both 1, set result to 1
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
                fprintf(fout, "str 6 2 1\n");           //stores the result in stack
                incSP(); //increase because result (0 or 1) is being stored now instead of both e1 and e2
	}
	if(t->typ == IF_THEN_ELSE_EXPR){
		if(t->children->data == NULL || t->children->next->data == NULL || t->children->next->next->data == NULL){
                        printf("Error: IF_EXPR child is null\n");
                        fprintf(fout, "hlt 1\n");
                        exit(EXIT_FAILURE);
                }
		int elseLabel = labelNum++;
		int endLabel = labelNum++;
		labelNum+=2;
		codeGenExpr(t->children->data, -1, -1);
		fprintf(fout, "lod 1 6 1\n");		//load e0 from stack
		fprintf(fout, "mov 2 0\n");		//reg 2 = 0 for comparison
		fprintf(fout, "beq 1 2 #%d ;if expr\n", elseLabel);	//check if condition false or true
		incSP();	//true? increase SP for the "then" expression
		codeGenExprs(t->children->next->data, -1, -1);		//execute the then-statements
		fprintf(fout, "jmp 0 #%d ;then expr\n", endLabel);
		fprintf(fout, "#%d: mov 0 0 ;else expr\n", elseLabel);
		incSP();	//false? increase SP for the "else" expression
		codeGenExprs(t->children->next->next->data, -1, -1); 	//execute the else statements
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
		//dont need to store because the result is already stored with codeGen of then/else statements
	}
	if(t->typ == FOR_EXPR){
		int endLabel = labelNum++;
		int trueLabel = labelNum++;
		int loopStart = labelNum++;
		labelNum+=3;
		if(t->children->data == NULL || t->children->next->data == NULL || t->children->next->next->data == NULL || t->children->next->next->next->data == NULL){
                        printf("Error: FOR_EXPR child is null\n");
                        fprintf(fout, "hlt 1\n");
                        exit(EXIT_FAILURE);
                }
		codeGenExpr(t->children->data, -1, -1);
		fprintf(fout, "#%d: mov 0 0 ;for loop\n", loopStart);	//beginning of loop, check bool expression
		codeGenExpr(t->children->next->data, -1, -1);
		fprintf(fout, "lod 1 6 1\n");			//loads the bool expression from stack
		fprintf(fout, "mov 2 1\n"); 			//reg 2 = 1 for comparison
		fprintf(fout, "beq 1 2 #%d\n", trueLabel); 	//checks if true to go to loop expressions
		fprintf(fout, "jmp 0 #%d\n", endLabel); 	//if not, end loop
		fprintf(fout, "#%d: mov 0 0\n", trueLabel);
		incSP(); //increase SP for the expressions to override the true that was stored there
		codeGenExprs(t->children->next->next->next->data, -1, -1); //code gen expressions
		codeGenExpr(t->children->next->next->data, -1, -1);	   //increase i or whatever 
		fprintf(fout, "jmp 0 #%d\n", loopStart);		   //restart to check the bool expression again
		fprintf(fout, "#%d: mov 0 0\n", endLabel);
		//doesnt store anything just exits the loop
	}
	if(t->typ == PRINT_EXPR){
		if(t->children->data == NULL){
			printf("Error: PRINT_EXPR child is null\n");
			fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
		}
		codeGenExpr(t->children->data, -1, -1);
		fprintf(fout, "lod 1 6 1\n");		//load the expression to be printed
		fprintf(fout, "ptn 1 ;print nat\n");	//print it to the screen
		//save print value on stack in case it is needed
	}
	if(t->typ == READ_EXPR){
                fprintf(fout, "rdn 1 ;read nat\n");	//read nat to reg 1
		fprintf(fout, "str 6 0 1\n");		//store to stack
		decSP();	//since this takes a spot in the stack
	}
	if(t->typ == THIS_EXPR){
		fprintf(fout, "hlt 1\n");
		printf("THIS_EXPR cannot be used without classes\n");
		exit(EXIT_FAILURE);
		//can only be used in classes and my dj files will not have any classes
	}
	if(t->typ == NEW_EXPR){
		if(t->children->data == NULL){
			printf("Error: NEW_EXPR child is null\n");
			fprintf(fout, "hlt 1\n");
			exit(EXIT_FAILURE);
		}
		if(strcmp(t->children->data->idVal, "Object")==0){
			fprintf(fout, "mov 1 0\n");	//allocate 0 for object
			fprintf(fout, "str 6 0 1\n"); 	//push object to the stack 
			decSP();
		}else{
			fprintf(fout, "hlt 1\n");	//else halt because object is the only option for level 2
                	printf("NEW_EXPR cannot be used without classes (unless used with object()\n");
                	exit(EXIT_FAILURE);
		}
	}
	if(t->typ == NULL_EXPR){
		fprintf(fout, "mov 1 0 ;null expr\n"); 	//use zero for null
		fprintf(fout, "str 6 0 1\n");		//store on stack
		decSP();
	}
	if(t->typ == NAT_LITERAL_EXPR){
		fprintf(fout, "mov 1 %d ;nat literal expr\n", t->natVal); //store nat on reg 1
		fprintf(fout, "str 6 0 1\n");	//place on stack
		decSP();
	}
	if(t->typ == TRUE_LITERAL_EXPR){
		fprintf(fout, "mov 1 1 ;true literal expr\n");	//store true as 1
		fprintf(fout, "str 6 0 1\n");			//on stack
		decSP();
	}
	if(t->typ == FALSE_LITERAL_EXPR){
		fprintf(fout, "mov 1 0 ;false literal expr\n");	//store false as 0
		fprintf(fout, "str 6 0 1\n");			//on stack
		decSP();
	}
	if(t->typ == ID_EXPR){
		int i, lval;
		for(i = 0; i < numMainBlockLocals; i++){
			if(strcmp(mainBlockST[i].varName, t->children->data->idVal)==0){
				lval = 65535 - i;	//find the address of the ID where it was declared
				fprintf(fout, "mov 1 %d\n", lval); 	//store the address on register 1
				fprintf(fout, "lod 2 1 0 ;ID expr\n");  //find the rval at that address and put it into register 2
				fprintf(fout, "str 6 0 2\n");		//and store it on the top of stack
				decSP();	//decrease SP after storing the ID on the stack
				return;
			}
		}
		//if ID is not found there is an error
		printf("ID_EXPR error (var not found) on line %d\n", t->lineNumber);
		fprintf(fout, "hlt 1\n");
		exit(EXIT_FAILURE);
	}
	if(t->typ == ASSIGN_EXPR){
		int i, lval;
                for(i = 0; i < numMainBlockLocals; i++){
                        if(strcmp(mainBlockST[i].varName, t->children->data->idVal)==0){
				codeGenExpr(t->children->next->data, -1, -1);
				lval = 65535 - i;       //find the address of the ID where it was declared
                                fprintf(fout, "lod 2 6 1 ;assign expr\n"); //grab the rval to be stored from top of stack
				fprintf(fout, "mov 3 %d\n", lval);         //store the address on register 3
				fprintf(fout, "str 3 0 2\n");		   //assign the rval at the memory address of the ID
				//the assign value still on the stack
				return;
                        }
                }
		//if ID not found, error
		printf("ASSIGN_EXPR error (var not found) on line %d\n", t->lineNumber);
                fprintf(fout, "hlt 1\n");
                exit(EXIT_FAILURE);	
        }
        if(t->typ == METHOD_CALL_EXPR){
		fprintf(fout, "hlt 1\n");
                printf("METHOD_CALL_EXPR cannot be used without classes\n");
                exit(EXIT_FAILURE);
		//u cant call methods without classes
	}
	if(t->typ == DOT_METHOD_CALL_EXPR){
		fprintf(fout, "hlt 1\n");
                printf("DOT_METHOD_CALL_EXPR cannot be used without classes\n");
                exit(EXIT_FAILURE);
		//cannot use DOT_METHOD_CALL_EXPR without classes	
	}
	if(t->typ == DOT_ID_EXPR){
		fprintf(fout, "hlt 1\n");
                printf("DOT_ID_EXPR cannot be used without classes\n");
                exit(EXIT_FAILURE);
		//cannot use DOT_ID_EXPR without classes	
	}
	if(t->typ == DOT_ASSIGN_EXPR){
		fprintf(fout, "hlt 1\n");
                printf("DOT_ASSIGN_EXPR cannot be used without classes\n");
                exit(EXIT_FAILURE);
		//cannot use DOT_ASSIGN_EXPR without classes
	}
	if(t->typ == INSTANCEOF_EXPR){
		fprintf(fout, "hlt 1\n");
                printf("INSTANCEOF_EXPR cannot be used without classes\n");
                exit(EXIT_FAILURE);
		//cannot use INSTANCEOF_EXPR without classes
	}
}

void generateDISM(FILE *outputFile){
	fout = outputFile; 
	
	int i;
	//prologue	
	fprintf(fout, "mov 7 65535\n"); //initialize FP
	fprintf(fout, "mov 6 65535\n"); //initialize SP
	fprintf(fout, "mov 5 1\n"); 	//initialize HP
	for(i = 0; i < numMainBlockLocals; i++){
		fprintf(fout, "mov %d 0\n", i); //allocate stack space for main locals
		decSP();
	}
	fprintf(fout, "mov 0 0 \n"); //begin main block body	

	if(mainExprs == NULL){
		printf("mainExprs empty\n");
		exit(EXIT_FAILURE);
	}
	codeGenExprs(mainExprs, -1, -1);	
	
	//epilogue
	fprintf(fout, "hlt 0\n"); //terminate main block
}


