#include "parse.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LENGTH 100001

//COMMANDS TEMPLATES
//'$' - description, '%' - name1, '^' - name2, '&' - number
#define ENTER_TEMPLATE "NEW_DISEASE_ENTER_DESCRIPTION % $"
#define COPY_TEMPLATE "NEW_DISEASE_COPY_DESCRIPTION % ^"
#define CHANGE_TEMPLATE "CHANGE_DESCRIPTION % & $"
#define PRINT_TEMPLATE "PRINT_DESCRIPTION % &"
#define DELETE_TEMPLATE "DELETE_PATIENT_DATA %"

#define ENTER_COMMAND "NEW_DISEASE_ENTER_DESCRIPTION"
#define COPY_COMMAND "NEW_DISEASE_COPY_DESCRIPTION"
#define CHANGE_COMMAND "CHANGE_DESCRIPTION"
#define PRINT_COMMAND "PRINT_DESCRIPTION"
#define DELETE_COMMAND "DELETE_PATIENT_DATA"

#define DESCRIPTION '$'
#define NAME1 '%'
#define NAME2 '^'
#define NUMBER '&'
#define SPACE ' '
#define END_OF_LINE '\n'
#define END_OF_STRING '\0'

char buffer[MAX_LENGTH];

//expression is my current commandTemplate, nameCommand is my current command
char* expression;
char* nameCommand; 

bool isSpace(char a){
	return (a == SPACE);
}

bool isDigit(char a){
	return (a >= '0' && a <= '9');
}
	
void findCommand(){
	//function take first word in the line and asign it to nameCommand
	int index = 0, starterIndex, lengthInscription;
	while(isSpace(buffer[index])){
		if(buffer[index]==END_OF_STRING || buffer[index] == END_OF_LINE) break;
		index++;
    }
    starterIndex = index;
    while(!isSpace(buffer[index])){
		if(buffer[index]==END_OF_STRING || buffer[index] == END_OF_LINE) break;
		index++;
	}
    lengthInscription = index - starterIndex;
    nameCommand = malloc(lengthInscription+1);
    nameCommand = strncpy(nameCommand, buffer+starterIndex, lengthInscription);
    nameCommand[lengthInscription] = END_OF_STRING;
}

void chooseTemplate(Command* newCommand, char* nameCommand){
	//function compare nameCommand with command in task and asign suitable template to expression 
	expression = malloc(40);
	if(!strcmp(nameCommand, COPY_COMMAND)){	
		strcpy(expression, COPY_TEMPLATE);
		newCommand->query = NEW_DISEASE_COPY_DESCRIPTION;
	}
	else if(!strcmp(nameCommand, PRINT_COMMAND)){
		strcpy(expression, PRINT_TEMPLATE);
		newCommand->query = PRINT_DESCRIPTION;
	}
	else if(!strcmp(nameCommand, DELETE_COMMAND)){
		strcpy(expression, DELETE_TEMPLATE);
		newCommand->query = DELETE_PATIENT_DATA;
	}
	else if(!strcmp(nameCommand, ENTER_COMMAND)){
		strcpy(expression, ENTER_TEMPLATE);
		newCommand->query = NEW_DISEASE_ENTER_DESCRIPTION;
	}
	else if(!strcmp(nameCommand, CHANGE_COMMAND)){ 
		strcpy(expression, CHANGE_TEMPLATE);
		newCommand->query = CHANGE_DESCRIPTION;
	}
	else{
		newCommand->query = IGNORE;
	}	
}

Command* getCommand(){

	Command* newCommand = malloc(sizeof(Command));

	void* status = fgets(buffer, MAX_LENGTH, stdin);
	
    int index = 0;
    
    //variables use in conditionals
    int starterIndex, lengthInscription;
    char* currentNumber = NULL;
     
    if(status == NULL){
		newCommand->query = END;
		return newCommand;
	}
	
    findCommand(); 
   
    chooseTemplate(newCommand, nameCommand);
    
    free(nameCommand);
    
	if(newCommand->query == IGNORE){
		free(expression);
		return newCommand;
	}
	
	for(int i = 0;i < strlen(expression);i++){
		switch (expression[i]){
			case SPACE:
				if(!isSpace(buffer[index])){
					newCommand->query = IGNORE;
					return newCommand;
				}
				index++;
				break;
			case NAME1:
				starterIndex = index;
				while(buffer[index] != END_OF_STRING && !isSpace(buffer[index]) && buffer[index] != END_OF_LINE){
					index++;
				}
				lengthInscription = index - starterIndex;
				newCommand->name1 = malloc(sizeof(char)*(lengthInscription+1));
				for(int j = 0;j < lengthInscription;j++){
					newCommand->name1[j] = buffer[starterIndex + j];
				}
				newCommand->name1[lengthInscription] = END_OF_STRING;
				break;
			case NAME2:
				starterIndex = index;
				while(buffer[index] != END_OF_STRING && !isSpace(buffer[index]) && buffer[index] != END_OF_LINE){
					index++;
				}
				lengthInscription = index - starterIndex;
				newCommand->name2 = malloc(sizeof(char)*(lengthInscription+1));
				for(int j = 0;j < lengthInscription; j++){
					newCommand->name2[j] = buffer[starterIndex+j];
				}
				newCommand->name2[lengthInscription] = END_OF_STRING;
				break;
			case NUMBER:
				starterIndex = index;
				while(isDigit(buffer[index])){
					index++;			
				}
				lengthInscription = index - starterIndex;
				if(lengthInscription == 0) break;
				currentNumber = malloc(sizeof(char)*(lengthInscription+1));
				for(int j = 0;j < lengthInscription;j++){
					currentNumber[j] = buffer[index-lengthInscription+j];
				}
				currentNumber[lengthInscription] = END_OF_STRING;
				newCommand->number = atoi(currentNumber);
				free(currentNumber);
				break;
			case DESCRIPTION:
				starterIndex = index;
				while(buffer[index] != END_OF_LINE){
					index++;
				}
				if(buffer[index-1] == SPACE){
					lengthInscription = index - starterIndex - 1;
				}
				else{
					lengthInscription = index - starterIndex;
				}
				newCommand->description = malloc(sizeof(char)*(lengthInscription+1));
				for(int j = 0;j < lengthInscription;j++){
					newCommand->description[j] = buffer[j + starterIndex];
				}
				newCommand->description[lengthInscription] = END_OF_STRING;
				break;
			default:
				if(buffer[index] != expression[i]){
					newCommand->query = IGNORE;
					return newCommand;
				}
				else{
					index++;
					continue;
				}
		}	
	}
	free(expression);
	return newCommand;
}
