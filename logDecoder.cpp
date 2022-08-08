#include <logDecoder.h>
fileParsingInfo::fileParsingInfo(struct fileInfo* fileInfoStruct, int logType) : fileInfoStruct(fileInfoStruct) {
    //If log is of ATP type
    if(logType == ATP_NUM){
        this->bitNumToSkip = ATP_BIT_NUM_TO_SKIP; 

        if(ATP_parameterInfo != NULL){
            this->parameterInfoVec = *ATP_parameterInfo;
        }
        else{
            parseLabelsConfigFile(ATP_EnumeratedLabels,logType,fileInfoStruct->directoryPath);

            parseParameterConfigFile(this->parameterInfoVec,logType,fileInfoStruct->directoryPath,ATP_EnumeratedLabels);
            ATP_parameterInfo = &(this->parameterInfoVec); 
        }
        this->enumeratedLabels = ATP_EnumeratedLabels;
    }
    //if log is of ATO type
    else{
        this->bitNumToSkip = ATO_BIT_NUM_TO_SKIP; 

        if(ATO_parameterInfo != NULL){
            this->parameterInfoVec = *ATO_parameterInfo;
        }
        else{
            parseLabelsConfigFile(ATO_EnumeratedLabels,logType,fileInfoStruct->directoryPath);

            parseParameterConfigFile(this->parameterInfoVec,logType,fileInfoStruct->directoryPath,ATO_EnumeratedLabels);
            ATO_parameterInfo = &(this->parameterInfoVec); 
        }
        this->enumeratedLabels = ATO_EnumeratedLabels;
    }
}

//Destructor
fileParsingInfo::~fileParsingInfo(){
    free(this->fileInfoStruct);
}

parameterInfo::parameterInfo(char* line,char*** enumeratedLabels){
    this->parameterID = fast_atoi(strtok(line,"\t"));
    strcpy(this->parameterID,strtok(NULL,"\t"));
    this->unsignedInt = fast_atoi(strtok(NULL,"\t"));
    this->firstbitPosition = fast_atoi(strtok(NULL,"\t"));
    this->bitCount = fast_atoi(strtok(NULL,"\t"));
    this->firstByte = fast_atoi(strtok(NULL,"\t"));
    this->lastByte = fast_atoi(strtok(NULL,"\t"));
    this->quantum = atof(strtok(NULL,"\t"));
    this->offset = fast_atoi(strtok(NULL,"\t"));
    this->displayType = fast_atoi(strtok(NULL,"\t"));
    this->enumeratedLabel = fast_atoi(strtok(NULL,"\t"));
    this->decimalCount = fast_atoi(strtok(NULL,"\t"));
    
    char* unit = fast_atoi(strtok(NULL,"\t"))
    if(unit != -1){
        this->unit = (char*) malloc(MAX_SHORT_STRING_SIZE);
        strcpy(this->unit,unit);
    }
    else if(this->bitCount == DATE_TIME_BIT_COUNT){
        this->bitCount = DATE_TIME_SHORT_BIT_COUNT;
    }
    else{
        //do nothing
    }

    //Function declaration based on the above variables
    if(this->displayType == DISPLAY_TYPE_ENUMERATED){
        this->enumeratedLabels = enumeratedLabels;
        this->intToString = &parameterinfo::IntToEnumeratedLabel;
    }
    else if(this->displayType == DISPLAY_TYPE_HEXADECIMAL){
        this->intToString = &parameterinfo::IntToHexadecimal;
    }
    else if(this->displayType == DISPLAY_TYPE_BINARY){
        //Keep NULL
    }
    else if(this->displayType == DISPLAY_TYPE_DATE){
        this->intToString = &parameterinfo::IntToDate;
    }
    else if(this->displayType == DISPLAY_TYPE_TIME){
        this->intToString = &parameterinfo::IntToTime;
    }
    else {
        //displayType == DISPLAY_TYPE_DECIMAL
        if(this->decimalCount != -1 && this->unit != NULL){
            this->intToString = &parameterinfo::IntToDecimalPrecisionUnit;
        }
        else if(this->decimalCount != -1){
            this->intToString = &parameterinfo::IntToDecimalPrecision;
        }
        else if(this->unit != NULL){
            this->intToString = &parameterinfo::IntToDecimalUnit;
        }
        else if(this->quantum == 1){
            this->intToString = &parameterinfo::IntToInteger;
        }
        else{
            this->intToString = &parameterinfo::IntToDecimal;
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
    return this->enumeratedLabels[this->enumeratedLabel][value];
}
char* parameterInfo::IntToHexadecimal(long long value, char* str){
    sprintf(str,"%x",value);
    return str;
}
char* parameterInfo::IntToInteger(long long value, char* str){
    sprintf(str,"%d",value+this->offset);
    return str;
}
char* parameterInfo::IntToDecimal(long long value, char* str){
    //Adjust based on predetermined decimal precision
    sprintf(str,"%0.*f",DECIMAL_PRECISION,value*this->quantum+this->offset);
    return str;
}
char* parameterInfo::IntToDecimalPrecision(long long value, char* str){
    sprintf(str,"%0.*f",this->decimalCount,value*this->quantum+this->offset);
    return str;
}
char* parameterInfo::IntToDecimalUnit(long long value, char* str){
    //Adjust based on predetermined decimal precision
    sprintf(str,"%0.*f %s",DECIMAL_PRECISION,value*this->quantum+this->offset,this->unit);
    return str;
}
char* parameterInfo::IntToDecimalPrecisionUnit(long long value, char* str){
    sprintf(str,"%0.*f %s",this->decimalCount,value*this->quantum+this->offset,this->unit);
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
long long parameterInfo::signedbinaryToDecimal(const char* binaryStr){
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
        return unsignedbinaryToDecimal(binaryStr);
    }
}

//Each ATO & ATP Label file is formatted like this:
 //     Enumerated ID   Enumerated Label    0    Label0    1    Label1     2    Label2 ... N    LabelN
 //Sorted by Enumerated ID
void parseLabelsConfigFile(char*** enumeratedLabels,int logType,char* directoryPath){
    char filePath[MAX_STRING_SIZE];
    char line[MAX_LINE_SIZE];

    sprintf(filePath,"%s%s",directoryPath, (logType == ATO_NUM) ? ATO_ENUMERATED_LABELS_FILENAME : ATP_ENUMERATED_LABELS_FILENAME);
    FILE* file = fopen(filePath,"r");

    while(fgets(line, sizeof(line),filePath) != NULL){
        //First field is Enumerated ID
        char* field = strtok(line,"\t");
        int enumID = fast_atoi(field);

        //Skip over Label Name
        strtok(NULL,"\t");
        field = strtok(NULL,"\t");

        //First token is Numeric Value
        //Second token is String Value
        while(field != NULL){
            int valueID = fast_atoi(field);
            char* valueStr = strtok(NULL,"\t");

            strcpy(enumeratedLabels[enumID][valueID],valueStr);
            field = strtok(NULL,"\t");
        }
    }

    fclose(file);
}

 //Each ATO & ATP Parameter file is formatted like this:
 //     Parameter ID    Parameter Label    Unsigned/Signed_Integer    FirstBitPosition    BitCount    FirstByte   LastByte    Quantum    Offset    DisplayType    EnumeratedLabel    DecimalCount    Unit  
 //Sorted by FirstBitPosition
void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType,char* directoryPath,char*** enumeratedLabels){
    char filePath[MAX_STRING_SIZE];
    char line[MAX_STRING_SIZE];

    sprintf(filePath,"%s%s",directoryPath, (logType == ATO_NUM) ? ATO_PARAMETERS_FILENAME : ATP_PARAMETERS_FILENAME);
    FILE* file = fopen(filePath,"r");

    while(fgets(line, sizeof(line),filePath) != NULL){
        //Make object dynamically and push back
        class parameterInfo* parameterInfoObj = new parameterInfo(line,enumeratedLabels);
        parameterInfoVec.push_back(parameterInfoObj);
    }

    fclose(file);
}
//To call function pointer
// (obj->*(obj->func))();
void parseFile(class fileParsingInfo* fileObj){
    //Create current line data structure
    //Get timestamp from Header
    //Skip over Header
    //Get entire line in binary
    //Iterate over parameterInfovec -- Convert to Int (account for offset) & do function
    //Once whole line obtained, print to file
    //REPEAT

	int c;
	while((c = fgetwc(fileVec[0]->inputFile)) != WEOF){
		printf("%d\n",c);
	}
}

//Outperforms atoi() by 4x
int fast_atoi(const char* str){
    int val = 0;
    while(*str){
        val = val*10 + (*str++ - '0');
    }
    return val;
}

//Linear concat algorithm VS O(N^2) strcat()
char* fast_strcat(char* dest, char* src)
{
     while (*dest) dest++;
     while (*dest++ = *src++);
     return --dest;
}

char *strptime_S(const char *buf, const char *fmt, struct tm *tm){
	//Strip the string to get the numeric value
	int S;
	sscanf(buf,fmt,&S);
	
	//Configure based on struct tm format
	tm->tm_sec = S;
	if(mktime(tm) < 1){
		return NULL;
	}
	return buf;
}

char* epochTimeToDate(long long value, char* str,const char* fmt){
    char epochString[MAX_SHORT_STRING_SIZE];
    sprintf(epochString,"%llu",value - TIME_SINCE_1900_01_01);
	struct tm* tm = (struct tm*)malloc(sizeof(struct tm));
    strptime_S(epochString,"%s",tm);
    strftime(str,MAX_SHORT_STRING_SIZE,fmt,tm);
    free(tm);
    return str;
}