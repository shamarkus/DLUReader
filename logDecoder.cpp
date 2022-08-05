#include <logDecoder.h>
fileParsingInfo::fileParsingInfo(struct fileInfo* fileInfoStruct, int logType) : fileInfoStruct(fileInfoStruct) {
    //If log is of ATP type
    if(logType == ATP_NUM){
        this->bitNumToSkip = ATP_BIT_NUM_TO_SKIP; 

        if(ATP_parameterInfo != NULL){
            this->parameterInfoVec = *ATP_parameterInfo;
            this->enumeratedLabels = ATP_EnumeratedLabels;
        }
        else{
            parseParameterConfigFile(this->parameterInfoVec,logType);
            ATP_parameterInfo = &(this->parameterInfoVec); 

            parseLabelsConfigFile(ATP_EnumeratedLabels,logType);
            this->enumeratedLabels = ATP_EnumeratedLabels;
        }
    }
    //if log is of ATO type
    else{
        this->bitNumToSkip = ATO_BIT_NUM_TO_SKIP; 

        if(ATO_parameterInfo != NULL){
            this->parameterInfoVec = *ATO_parameterInfo;
            this->enumeratedLabels = ATO_EnumeratedLabels;
        }
        else{
            parseParameterConfigFile(this->parameterInfoVec,logType);
            ATO_parameterInfo = &(this->parameterInfoVec); 

            parseLabelsConfigFile(ATO_EnumeratedLabels,logType);
            this->enumeratedLabels = ATO_EnumeratedLabels;
        }
    }
}

//Destructor
fileParsingInfo::~fileParsingInfo(){

}
//Each ATO & ATP Label file is formatted like this:
 //     Enumerated ID   Enumerated Label    0    Label0    1    Label1     2    Label2 ... N    LabelN
 //Sorted by Enumerated ID
void parseLabelsConfigFile(char* enumeratedLabels[MAX_LABELS_SIZE],int logType){
    char fileName[MAX_STRING_SIZE];
    sprintf(fileName,"%s%s")
}

 //Each ATO & ATP Parameter file is formatted like this:
 //     Parameter ID    Parameter Label    Unsigned/Signed_Integer    FirstBitPosition    BitCount    FirstByte   LastByte    Quantum    Offset    DisplayType    EnumeratedLabel    DecimalCount    Unit  
 //Sorted by FirstBitPosition
void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType){

}







void parseFile(class fileParsingInfo* fileObj){
	//int c;
	//while((c = fgetwc(fileVec[0]->inputFile)) != WEOF){
	//	printf("%d\n",c);
	//}
}


int fast_atoi(const char* str){
    int val = 0;
    while(*str){
        val = val*10 + (*str++ - '0');
    }
    return val;
}

char* fast_strcat(char* dest, char* src)
{
     while (*dest) dest++;
     while (*dest++ = *src++);
     return --dest;
}