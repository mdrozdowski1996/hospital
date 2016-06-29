#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "structure.h"

int main(){
	Command* command;
	int breakTmp = 0;
	while(1){
		int kindOfOutput = DES;
		//DEScription is neutral for output, because then program printf no more
		command = getCommand();
		switch(command->query){
			case IGNORE:
				printf("IGNORED\n");
				break;
			case END:
				breakTmp = 1;
				break;
			case NEW_DISEASE_ENTER_DESCRIPTION:
				kindOfOutput = newDiseaseEnterDescription(command->name1, command->description);
				free(command->description);
				free(command->name1);	
				break;
			case NEW_DISEASE_COPY_DESCRIPTION:
				kindOfOutput = newDiseaseCopyDescription(command->name1, command->name2);
				free(command->name2);
				free(command->name1);
				break;
			case PRINT_DESCRIPTION:
				kindOfOutput = printDescription(command->name1, command->number);
				free(command->name1);
				break;
			case CHANGE_DESCRIPTION:
				kindOfOutput = changeDescription(command->name1, command->number, command->description);
				free(command->name1);
				free(command->description);
				break;
			case DELETE_PATIENT_DATA:
				kindOfOutput = deletePatientData(command->name1);
				free(command->name1);
				break;
		}
		switch(kindOfOutput){
			case IGN:
				printf("IGNORED\n");
				break;
			case OK:
				printf("OK\n");
				break;
		}
		free(command);
		if(breakTmp == 1){
			break;
		}
		fprintf(stderr, "DESCRIPTIONS:");
		fprintf(stderr, " %d\n", returnNumberOfDisease());
	
	}
	deleteEverything();
	
	return 0;
}
