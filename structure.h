#ifndef STRUCTURE_H
#define STRUCTURE_H

#define IGN 1
#define DES 2
#define OK 3

int newDiseaseEnterDescription(char* name,char* diseaseDescription);

int newDiseaseCopyDescription(char* name1, char* name2);

int changeDescription(char* name, int n, char* diseaseDescription);

int printDescription(char* name, int n);

int deletePatientData(char* name);

void deleteEverything();

int returnNumberOfDisease();

#endif
