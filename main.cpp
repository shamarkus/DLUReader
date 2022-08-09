#include <logDecoder.h>

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
		tempFileInfo->inputFile = fopen(argv[i],"r");
		tempFileInfo->logType = determineATPorATO(strlen(argv[1]),argv[i]);

		tempFileInfo->outputFile = fopen(strcat(argv[i],TXT_SUFFIX),"w");

		fileVec.push_back(tempFileInfo);
	}
}

int main(int argc,char** argv){
	
	std::setlocale(LC_ALL,"en_US.UTF8");

	std::vector<fileInfo*> fileVec; 
	initFileVec(fileVec,argc,argv);

	//Create an object for every file to parse
	std::vector<class fileParsingInfo*> fileObjVec;
	for(auto fileInfo : fileVec){
		class fileParsingInfo* fileObj = new fileParsingInfo(fileInfo,fileInfo->logType);
		fileObjVec.push_back(fileObj);
	}

	//Potentially multi-thread this
	for(auto fileObj : fileObjVec){
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


