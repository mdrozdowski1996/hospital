#ifndef PARSE_H
#define PARSE_H

typedef enum{
	NEW_DISEASE_ENTER_DESCRIPTION,
	NEW_DISEASE_COPY_DESCRIPTION,
	CHANGE_DESCRIPTION,
	PRINT_DESCRIPTION,
	DELETE_PATIENT_DATA,
	END,
	IGNORE
} QueryType;
	
typedef struct{
	QueryType query;
	char* name1;
	char* name2;
	char* description;
	int number;
} Command;		

Command* getCommand();

#endif
