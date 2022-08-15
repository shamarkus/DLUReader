#include "globals.h"
#include "logDecoderClass.h"

std::vector<class parameterInfo*> *ATP_parameterInfo = NULL;
std::vector<class parameterInfo*> *ATO_parameterInfo = NULL;

char ATP_EnumeratedLabels[MAX_ATP_LABELS][MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE];
char ATO_EnumeratedLabels[MAX_ATO_LABELS][MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE];
char ATP_StringLabels[MAX_ATP_PARAMS][MAX_STRING_SIZE];
char ATO_StringLabels[MAX_ATO_PARAMS][MAX_STRING_SIZE];


int determineATPorATO(int directoryPathLength, char* fileName);
void initFileVec(std::vector<fileInfo*> &fileVec,int argc,char** argv);

int determineATPorATO(int directoryPathLength, char* fileName){
	  //Eliminate directory in ATO/ATP search 
	  char* tempFileName = fileName + directoryPathLength;
      if(strstr(tempFileName,ATP_STR) != NULL){
	      return ATP_NUM;
      }
      else if(strstr(tempFileName,ATO_STR) != NULL){
	      return ATO_NUM;
      }
      else{
	      printf("Error: Passed filename was named incorrectly, and 8001c0 or 800180 OR 8002c0 or 800280 was unfound\n");
	      return AT_UNDEFINED_NUM;
      }
}

//Command formatting should be like this:
// ./DLUReader "DIR" "DIR/FILE1" "DIR/FILE2" ... "DIR/FILEN"
void initFileVec(std::vector<fileInfo*> &fileVec,int argc,char** argv){
	fileVec.reserve(argc-2);
	for(int i = 2;i < argc;i++){
		struct fileInfo* tempFileInfo = new fileInfo();

		strcpy(tempFileInfo->directoryPath,argv[1]);
		strcpy(tempFileInfo->fileName,argv[i]);
		tempFileInfo->inputFile = fopen(argv[i],"rb");
		tempFileInfo->logType = determineATPorATO(strlen(argv[1]),argv[i]);

		char filePath[MAX_STRING_SIZE];
		sprintf(filePath,"%s%s",argv[i],TXT_SUFFIX);
		tempFileInfo->outputFile = fopen(filePath,"w");

		fileVec.push_back(tempFileInfo);
	}
}

int main(int argc,char** argv){
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	std::setlocale(LC_ALL,"en_US.UTF8");

	std::vector<fileInfo*> fileVec; 
	initFileVec(fileVec,argc,argv);
	//Create an object for every file to parse
	std::vector<class fileParsingInfo*> fileObjVec;
	for(auto fileInfo : fileVec){
		class fileParsingInfo* fileObj = new fileParsingInfo(fileInfo,fileInfo->logType);
		fileObjVec.push_back(fileObj);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time To Set Data Structs = " << (double) std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/ 1000000 << "[s]" << std::endl;

	//Potentially multi-thread this
	int i = 1;
	for(auto fileObj : fileObjVec){
		printf("File Number : %d\n",i++);
		fileObj->parseFile();
	}
	
	//Deletion of all dynamically created objects
	if(ATP_parameterInfo != NULL){
		for(auto parameterObj : *ATP_parameterInfo){
			delete parameterObj;
		}
	}
	if(ATO_parameterInfo != NULL){
		for(auto parameterObj : *ATO_parameterInfo){
			delete parameterObj;
		}
	}
	for(auto fileObj : fileObjVec){
		delete fileObj;
	}

	return 0;
}


