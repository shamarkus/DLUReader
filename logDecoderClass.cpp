#include "globals.h"
#include "logDecoderClass.h"
#include "helperFunctions.h"

fileParsingInfo::fileParsingInfo(struct fileInfo* fileInfoStruct, int logType) : fileInfoStruct(fileInfoStruct) {
    //If log is of ATP type
    if(logType == ATP_NUM){
        this->byteNumToSkip = ATP_BYTE_NUM_TO_SKIP - 1; 
	this->byteNumForLine = MAX_ATP_PARAMS_BIT_SIZE/8;

        if(ATP_parameterInfo != NULL){
            this->parameterInfoVec = *ATP_parameterInfo;
        }
        else{
            parseLabelsConfigFile(ATP_EnumeratedLabels,logType,fileInfoStruct->directoryPath);

           parseParameterConfigFile(this->parameterInfoVec,logType,fileInfoStruct->directoryPath,ATP_EnumeratedLabels,ATP_StringLabels);
            ATP_parameterInfo = &(this->parameterInfoVec); 
        }
	this->stringLabels = ATP_StringLabels;
        this->enumeratedLabels = ATP_EnumeratedLabels;
    }
    //if log is of ATO type
    else{
        this->byteNumToSkip = ATO_BYTE_NUM_TO_SKIP - 1; 
	this->byteNumForLine = MAX_ATO_PARAMS_BIT_SIZE/8;

        if(ATO_parameterInfo != NULL){
            this->parameterInfoVec = *ATO_parameterInfo;
        }
        else{
            parseLabelsConfigFile(ATO_EnumeratedLabels,logType,fileInfoStruct->directoryPath);

            parseParameterConfigFile(this->parameterInfoVec,logType,fileInfoStruct->directoryPath,ATO_EnumeratedLabels,ATO_StringLabels);
            ATO_parameterInfo = &(this->parameterInfoVec); 
        }
	this->stringLabels = ATO_StringLabels;
        this->enumeratedLabels = ATO_EnumeratedLabels;
    }
}

//Destructor
fileParsingInfo::~fileParsingInfo(){
    fclose(this->fileInfoStruct->outputFile);
    free(this->fileInfoStruct);
}

void fileParsingInfo::parseFile(){
	struct headerInfo* headerStruct = &(this->headerInfoStruct);
	int logType = this->fileInfoStruct->logType;

	int numParameters = (logType == ATO_NUM) ? MAX_ATO_PARAMS : MAX_ATP_PARAMS;
	int numLineBits = (logType == ATO_NUM) ? MAX_ATO_PARAMS_BIT_SIZE : MAX_ATP_PARAMS_BIT_SIZE;
	int paramsCharSize = this->byteNumForLine;
	int headerCharSize = headerStruct->headerByteSize;
	int skipCharSize = 0;

	char curParams[numParameters][MAX_SHORT_STRING_SIZE];	
	char curLine[numLineBits + 1];
	char curHeader[headerStruct->headerBitSize + 1];
	char curSkipLine[MAX_SHORT_STRING_SIZE];
	curLine[0] = '\0';
	curHeader[0] = '\0';

	char* headerP = curHeader;
	char* lineP = curLine;
	int (*nullishP)[2];
	int (*nullishArrayP)[2] = (logType == ATO_NUM) ? ATONullishArray : ATPNullishArray;
	int curChar;
	
	printHeader(numParameters);
	//Iterate over parameterInfoVec
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while(curChar != EOF){
		curChar = fgetc(this->fileInfoStruct->inputFile);
		//Get Header
		if(skipCharSize){
			if(nullishP[this->byteNumToSkip-skipCharSize][0] == curChar || nullishP[this->byteNumToSkip-skipCharSize][1] == curChar){
				skipCharSize--;
			}
			else{
				nullishP = nullishArrayP;
				skipCharSize = this->byteNumToSkip;
			}
			continue;
		}
		else if(headerCharSize){
			headerP = fast_strcat(headerP,byteArray[curChar]);	
			headerCharSize--;
			continue;
		}
		else if(paramsCharSize){
			lineP = fast_strcat(lineP,byteArray[curChar]);
			paramsCharSize--;
			continue;
		}

		parseLine(curHeader,curLine,curParams);
		printLine(curParams,numParameters);

		//Re-initialize -- Skip over nullish sequence
		memset(curHeader,0,headerStruct->headerBitSize);
		memset(curLine,0,numLineBits);
		headerP = curHeader;
		lineP = curLine;
		nullishP = nullishArrayP;

		//Account for the current character - 1
		skipCharSize = this->byteNumToSkip;
		headerCharSize = headerStruct->headerByteSize;
		paramsCharSize = this->byteNumForLine;
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Time To Parse = " << (double) std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/ 1000000 << "[s]" << std::endl;
}

void fileParsingInfo::parseLine(char* curHeader, char* curLine, char (*curParams)[MAX_SHORT_STRING_SIZE]){
	//Get timestamp for first 2 params

	struct headerInfo* headerStruct = &(this->headerInfoStruct);
	char binaryParam[MAX_SHORT_STRING_SIZE];
	char resultStr[MAX_SHORT_STRING_SIZE];
	class parameterInfo* parameterObj = this->parameterInfoVec[0];

	memcpy(binaryParam,curHeader + headerStruct->timeBitPos, headerStruct->timeBitSize);
	binaryParam[headerStruct->timeBitSize] = '\0';

	long long decimalParam = parameterObj->unsignedBinaryToDecimal(binaryParam);
	(parameterObj->*(parameterObj->intToString))(decimalParam,curParams[0]);

	parameterObj = this->parameterInfoVec[1];
	(parameterObj->*(parameterObj->intToString))(decimalParam,curParams[1]);
	strcat(curParams[1],HEADER_TIME_SUFFIX);

	
	for(int i = 2; i < this->parameterInfoVec.size(); i++){
		parameterObj = this->parameterInfoVec[i];

		if(parameterObj->getDisplayType() == DISPLAY_TYPE_BINARY){
			memcpy(curParams[parameterObj->getParameterID()], curLine + parameterObj->getFirstBitPosition(), parameterObj->getBitCount());
			curParams[parameterObj->getParameterID()][parameterObj->getBitCount()] = '\0';
			continue;
		}
		else{
			memcpy(binaryParam,curLine + parameterObj->getFirstBitPosition(), parameterObj->getBitCount());
		}
		
		binaryParam[parameterObj->getBitCount()] = '\0';
		if(parameterObj->getUnsignedInt() == 0){
			decimalParam = parameterObj->unsignedBinaryToDecimal(binaryParam);
		}
		else{
			decimalParam = parameterObj->signedBinaryToDecimal(binaryParam);
		}

		(parameterObj->*(parameterObj->intToString))(decimalParam,curParams[parameterObj->getParameterID()]);

	}
}

void fileParsingInfo::printHeader(int numParameters){
	for(int i = 0; i < numParameters; i++){
		fprintf(this->fileInfoStruct->outputFile,"%s\t",this->stringLabels[i]);
	}
	fprintf(this->fileInfoStruct->outputFile,"\n");
}

void fileParsingInfo::printLine(char (*curParams)[MAX_SHORT_STRING_SIZE],int numParameters){
	for(int i = 0; i < numParameters; i++){
		fprintf(this->fileInfoStruct->outputFile,"%s\t",curParams[i]);
	}
	fprintf(this->fileInfoStruct->outputFile,"\n");
}

parameterInfo::parameterInfo(char* line,char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],char (*stringLabels)[MAX_STRING_SIZE]){
	this->parameterID = fast_atoi(strtok(line,"\t"));
	strcpy(stringLabels[this->parameterID],strtok(NULL,"\t"));
	this->unsignedInt = fast_atoi(strtok(NULL,"\t"));
	this->firstBitPosition = fast_atoi(strtok(NULL,"\t"));
	this->bitCount = fast_atoi(strtok(NULL,"\t"));
	this->firstByte = fast_atoi(strtok(NULL,"\t"));
	this->lastByte = fast_atoi(strtok(NULL,"\t"));
	this->quantum = atof(strtok(NULL,"\t"));
	this->offset = fast_atoi(strtok(NULL,"\t"));
	this->displayType = fast_atoi(strtok(NULL,"\t"));
	this->enumeratedLabel = fast_atoi(strtok(NULL,"\t"));
	this->decimalCount = fast_atoi(strtok(NULL,"\t"));

	char* unit = strtok(NULL,"\t");
	if(unit[0] != '-' && unit[1] != '1'){
		this->unit = (char*) malloc(MAX_SHORT_STRING_SIZE);
		strcpy(this->unit,unit);
	}
	else{
		//do nothing
	}

	if(this->bitCount == DATE_TIME_BIT_COUNT){
		this->bitCount = DATE_TIME_SHORT_BIT_COUNT;
	}
	else{
		//do nothing
	}

	if(this->firstBitPosition > INNER_HEADER_BIT_POS){
		this->firstBitPosition += MAX_HEADER_BIT_SIZE;
	}
	else{
		//do nothing
	}

	//Function declaration based on the above variables
	if(this->displayType == DISPLAY_TYPE_ENUMERATED){
		this->enumeratedLabels = enumeratedLabels;
		this->intToString = &parameterInfo::IntToEnumeratedLabel;
	}
	else if(this->displayType == DISPLAY_TYPE_HEXADECIMAL){
		this->intToString = &parameterInfo::IntToHexadecimal;
	}
	else if(this->displayType == DISPLAY_TYPE_DATE){
		this->intToString = &parameterInfo::IntToDate;
	}
	else if(this->displayType == DISPLAY_TYPE_TIME){
		this->intToString = &parameterInfo::IntToTime;
	}
	else {
		//this->displayType == DISPLAY_TYPE_DECIMAL
		if(this->decimalCount != -1){
			this->intToString = &parameterInfo::IntToDecimalPrecision;
		}
		else if(this->quantum == 1){
			this->intToString = &parameterInfo::IntToInteger;
		}
		else{
			this->intToString = &parameterInfo::IntToDecimal;
		}
	}
}

//Destructor
parameterInfo::~parameterInfo(){
	if(NULL != this->unit){
		free(this->unit);
	}
}

//Accepts integer value, and gets corresponding string label
char* parameterInfo::IntToEnumeratedLabel(long long value,char* str){
	if(value > MAX_ATO_VALUES || *this->enumeratedLabels[this->enumeratedLabel][value] == '\0'){
		sprintf(str,"? key : %llu", value);
	}
	else{
		strcpy(str,this->enumeratedLabels[this->enumeratedLabel][value]);
	}
	return str;
}
char* parameterInfo::IntToHexadecimal(long long value, char* str){
	sprintf(str,"%0*x",this->bitCount >> 2,value);
	return str;
}
char* parameterInfo::IntToInteger(long long value, char* str){
	sprintf(str,"%d",value+this->offset);
	return str;
}
char* parameterInfo::IntToDecimal(long long value, char* str){
	sprintf(str,"%.0f",value*this->quantum+this->offset);
	return str;
}
char* parameterInfo::IntToDecimalPrecision(long long value, char* str){
	sprintf(str,"%0.*f",this->decimalCount,value*this->quantum+this->offset);
	return str;
}
char* parameterInfo::IntToDate(long long value, char* str){
	return epochTimeToDate(value,str,"%Y/%m/%d");
}
char* parameterInfo::IntToTime(long long value, char* str){
	return epochTimeToDate(value,str,"%H:%M:%S");
}

long long parameterInfo::unsignedBinaryToDecimal(const char* binaryStr){
	int len = this->bitCount - 1;
	long long val = 0;
	while(*binaryStr){
		if(*binaryStr++ - '0'){
			val = val | ( (long long) 1 << len);
		}
		len--;
	}
	return val;
}

long long parameterInfo::signedBinaryToDecimal(const char* binaryStr){
	//Check if MSB is 1
	if(*binaryStr - '0'){
		int len = this->bitCount - 1;       
		long long val = 0;
		while(*binaryStr){
			if(*binaryStr++ - '1'){
				val = val | ( (long long) 1 << len);
			}
			len--;
		}
		++val *= -1;
		return val;
	}
	else{
		return unsignedBinaryToDecimal(binaryStr);
	}
}

int parameterInfo::getParameterID(){
	return this->parameterID;
}
int parameterInfo::getUnsignedInt(){
	return this->unsignedInt;
}
int parameterInfo::getFirstBitPosition(){
	return this->firstBitPosition;
}
int parameterInfo::getBitCount(){
	return this->bitCount;
}
int parameterInfo::getDisplayType(){
	return this->displayType;
}
