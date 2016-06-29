#include "structure.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

int counterDisease = 0;

struct Disease;

typedef struct Disease* MainListDisease;

struct Disease{
	char* description;
	int counterReference;
	MainListDisease nextDisease;
	MainListDisease beforeDisease;
};

MainListDisease mainListDisease = NULL;

struct PatientDiseaseList;

typedef struct PatientDiseaseList* ListOfDisease;

struct PatientDiseaseList{
	MainListDisease currentDisease;
	ListOfDisease nextDisease;
};
/* Disease in PatientDiseaseList i keep in reverse order, because i must have
 * access to last in const time, therefore i keep also in struct Patient numberOfDisease,
 * and if i want to take n-disease i take (numberOfDisease-n)-disease in my list */
struct Patient; 

typedef struct Patient* ListOfPatient;

struct Patient{
	char* patientName;
	int numberOfDisease;
	ListOfDisease disease;
	ListOfPatient nextPatient;
};

ListOfPatient listOfPatient = NULL;

MainListDisease findDiseaseBefore(char* descrpt, MainListDisease list){
	if(!list) return NULL;
	else{
		MainListDisease helpList = list->nextDisease;
		if(!helpList) return NULL;
		else if(!strcmp(helpList->description, descrpt)) return list;
		else return findDiseaseBefore(descrpt, list->nextDisease);
	}
}

void addNewDiseaseToMainList(char* diseaseDescription){
		MainListDisease helpDisease = mainListDisease;
		mainListDisease = malloc(sizeof(struct Disease));
		mainListDisease->nextDisease = helpDisease;
		mainListDisease->description = malloc(sizeof(char)*(strlen(diseaseDescription)+1));
		strcpy(mainListDisease->description, diseaseDescription); 
		mainListDisease->counterReference = 1;
		mainListDisease->beforeDisease = NULL;
		if(helpDisease != NULL) helpDisease->beforeDisease = mainListDisease;
		counterDisease++;
}

int lengthOfDiseaseHistory(ListOfDisease myDisease){
	if(myDisease == NULL) return 0;
	else return lengthOfDiseaseHistory(myDisease->nextDisease)+1;
}

ListOfDisease findNDisease(int n, ListOfDisease myDisease){
	if(myDisease == NULL) return NULL;
	else if(n == 0) return myDisease;
	else return findNDisease(n-1, myDisease->nextDisease);
}

void removeDescriptionInMainList(MainListDisease myDisease){
	myDisease->counterReference--;
	if(myDisease->counterReference == 0){
		MainListDisease beforeDisease = myDisease->beforeDisease;
		MainListDisease nextDisease = myDisease->nextDisease;
		counterDisease--;
		if(beforeDisease != NULL) beforeDisease->nextDisease = nextDisease;
		if(nextDisease != NULL) nextDisease->beforeDisease = beforeDisease;
		free(myDisease->description);
		free(myDisease);
		if(beforeDisease == NULL){
			mainListDisease = nextDisease;
		}
	}
}
		
ListOfPatient findPatient(char* name, ListOfPatient list){
	if(list == NULL)return NULL;
	else if(!strcmp(name, list->patientName)) return list;
	else{
		return findPatient(name, list->nextPatient);
	}
}

void addPatient(char* name){
	ListOfPatient actualPatient = findPatient(name, listOfPatient);
	if(actualPatient == NULL){
		ListOfPatient helpPatient = listOfPatient;
		listOfPatient = malloc(sizeof(struct Patient));
		listOfPatient->nextPatient = helpPatient;
		listOfPatient->patientName = malloc(strlen(name)+1);
		strcpy(listOfPatient->patientName, name);
		listOfPatient->disease = NULL;
		listOfPatient->numberOfDisease = 0;
	}
}
	
void addDiseaseToPatientList(MainListDisease myDisease, ListOfPatient myPatient){
	ListOfDisease helpList = myPatient->disease;
	myPatient->disease = malloc(sizeof(struct PatientDiseaseList));
	myPatient->disease->currentDisease = myDisease;
	myPatient->disease->nextDisease = helpList;
	myPatient->numberOfDisease++;
}
	
int newDiseaseEnterDescription(char* name, char* diseaseDescription){
	addNewDiseaseToMainList(diseaseDescription);
	addPatient(name);
	ListOfPatient myPatient = findPatient(name, listOfPatient);
	addDiseaseToPatientList(mainListDisease, myPatient);
	return OK;
}

int newDiseaseCopyDescription(char* name1, char* name2){
	ListOfPatient patient2 = findPatient(name2, listOfPatient);
	if(patient2 == NULL) return IGN;
	else{ 
		if(patient2->disease == NULL) return IGN;
		else{
			addPatient(name1);
			ListOfPatient patient1 = findPatient(name1, listOfPatient);
			addDiseaseToPatientList(patient2->disease->currentDisease, patient1);
			patient2->disease->currentDisease->counterReference++;
			return OK;
		}	
	}
	return IGN;
}

int changeDescription(char* name, int n, char* diseaseDescription){
	ListOfPatient myPatient = findPatient(name, listOfPatient);
	if(myPatient == NULL)return IGN;
	else{
		int lengthList = myPatient->numberOfDisease;
		ListOfDisease actualDisease = findNDisease(lengthList - n, myPatient->disease);
		if(actualDisease == NULL)return IGN;
		else{
			removeDescriptionInMainList(actualDisease->currentDisease);
			addNewDiseaseToMainList(diseaseDescription);
			actualDisease->currentDisease = mainListDisease;
			return OK;
		}
	}
}

int printDescription(char* name, int n){
	ListOfPatient myPatient = findPatient(name, listOfPatient);
	if(myPatient == NULL) return IGN;
	int lengthList = myPatient->numberOfDisease;
	ListOfDisease myDisease = findNDisease(lengthList - n, myPatient->disease);
	if(myDisease == NULL) return IGN;
	else{
		printf("%s\n",myDisease->currentDisease->description);
		return DES;
	}
}

void removeDiseaseList(ListOfDisease listDisease){
	if(listDisease != NULL){
		removeDescriptionInMainList(listDisease->currentDisease);
		removeDiseaseList(listDisease->nextDisease);
		free(listDisease);
	}
}

int deletePatientData(char* name){
	ListOfPatient myPatient = findPatient(name, listOfPatient);
	if(myPatient == NULL) return IGN;
	else{
		removeDiseaseList(myPatient->disease);
		myPatient->disease = NULL;
		myPatient->numberOfDisease = 0;
		return OK;
	}
}

void removeDiseaseListWithoutChangingMainList(ListOfDisease listDisease){
	if(listDisease != NULL){
		removeDiseaseListWithoutChangingMainList(listDisease->nextDisease);
		free(listDisease);
	}
}

void deleteEveryPatientData(ListOfPatient patients){
	if(patients != NULL){
		removeDiseaseListWithoutChangingMainList(patients->disease);
		deleteEveryPatientData(patients->nextPatient);
		free(patients->patientName);
		free(patients);
	}
}

void deleteEveryDisease(MainListDisease list){
	if(list != NULL){
		deleteEveryDisease(list->nextDisease);
		free(list->description);
		free(list);
	}
}

void deleteEverything(){
	deleteEveryPatientData(listOfPatient);
	deleteEveryDisease(mainListDisease);
}

int returnNumberOfDisease(){
	return counterDisease;
}
