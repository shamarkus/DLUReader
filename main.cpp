#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <wchar.h>

#define ATO_NUM 0 
#define ATP_NUM 1 
#define AT_UNDEFINED_NUM 2
#define ATP_STR "80_"
#define ATO_STR "c0_"

#define MAX_STRING_SIZE 512
#define TXT_SUFFIX ".txt" 

struct fileInfo {
	char fileName[MAX_STRING_SIZE];
	FILE* inputFile;
	FILE* outputFile;

	int logType;
};

int determineATPorATO(char* fileName){
      if(strstr(fileName,ATP_STR) != NULL){
	      return ATP_NUM;
      }
      else if(strstr(fileName,ATO_STR) != NULL){
	      return ATO_NUM;
      }
      else{
	      printf("Error: Passed filename was named incorrectly, and 8001c0 or 800180 OR 8002c0 or 800280  was unfound\n");
	      return AT_UNDEFINED_NUM;
      }
}

void initFileVec(std::vector<fileInfo*> &fileVec,int argc,char** argv){
	fileVec.reserve(argc-1);
	for(int i = 1;i < argc;i++){
		struct fileInfo* tempFileInfo = new fileInfo();

		strcpy(tempFileInfo->fileName,argv[i]);
		tempFileInfo->inputFile = fopen(argv[1],"r");
		tempFileInfo->logType = determineATPorATO(argv[1]);

		tempFileInfo->outputFile = fopen(strcat(argv[1],TXT_SUFFIX),"w");

		fileVec.push_back(tempFileInfo);
	}
}

int main(int argc,char** argv){
	
	std::setlocale(LC_ALL,"en_US.UTF8");
	std::cout << setlocale(LC_ALL, NULL) << std::endl;
	//Read arguments for filenames
	std::vector<fileInfo*> fileVec; 
	initFileVec(fileVec,argc,argv);

	//Initialize all helper files
	int c;
	while((c = fgetwc(fileVec[0]->inputFile)) != WEOF){
		printf("%d\n",c);
	}
	return 0;
}


