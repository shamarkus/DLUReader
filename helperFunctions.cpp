#include "globals.h"
#include "helperFunctions.h"
#include "logDecoderClass.h" 

struct tm tm_buf;

 //Each ATO & ATP Parameter file is formatted like this:
 //     Parameter ID    Parameter Label    Unsigned/Signed_Integer    FirstBitPosition    BitCount    FirstByte   LastByte    Quantum    Offset    DisplayType    EnumeratedLabel    DecimalCount    Unit  
 //Sorted by FirstBitPosition
void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType,char* directoryPath,char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],char (*stringLabels)[MAX_STRING_SIZE]){
	char filePath[MAX_STRING_SIZE];
	char line[MAX_STRING_SIZE];

	sprintf(filePath,"%s%s",directoryPath, (logType == ATO_NUM) ? ATO_PARAMETERS_FILENAME : ATP_PARAMETERS_FILENAME);
	FILE* file = fopen(filePath,"r");

	while(fgets(line, sizeof(line),file) != NULL){
		//Make object dynamically and push back
		class parameterInfo* parameterInfoObj = new parameterInfo(line,enumeratedLabels,stringLabels);
		parameterInfoVec.push_back(parameterInfoObj);
	}

	fclose(file);
}

//Each ATO & ATP Label file is formatted like this:
 //     Enumerated ID   Enumerated Label    0    Label0    1    Label1     2    Label2 ... N    LabelN
 //Sorted by Enumerated ID
void parseLabelsConfigFile(char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],int logType,char* directoryPath){
    char filePath[MAX_STRING_SIZE];
    char line[MAX_LINE_SIZE];

    sprintf(filePath,"%s%s",directoryPath, (logType == ATO_NUM) ? ATO_ENUMERATED_LABELS_FILENAME : ATP_ENUMERATED_LABELS_FILENAME);
    FILE* file = fopen(filePath,"r");

    while(fgets(line, sizeof(line),file) != NULL){
        //First field is Enumerated ID
        char* field = strtok(line,"\t");
        int enumID = fast_atoi(field);

        //Skip over Label Name
        strtok(NULL,"\t");
        field = strtok(NULL,"\t");

        //First token is Numeric Value
        //Second token is String Value
        while(*field != '\n' && field != NULL){
            int valueID = fast_atoi(field);
            char* valueStr = strtok(NULL,"\t");

            strcpy(enumeratedLabels[enumID][valueID],valueStr);
            field = strtok(NULL,"\t");
        }
    }

    fclose(file);
}

//Outperforms atoi() by 4x
int fast_atoi(const char* str){
	if(!(*str - '-')) return fast_atoi(++str) * (-1);
	int val = 0;
	while(*str){
		val = val*10 + (*str++ - '0');
	}
	return val;
}

//Linear concat algorithm VS O(N^2) strcat()
char* fast_strcat(char* dest, const char* src)
{
	while (*dest) dest++;
	while (*dest++ = *src++);
	return --dest;
}

char* epochTimeToDate(unsigned long long value, char* str,const char* fmt){

	long long diff = (long long)(value - (long long) TIME_SINCE_1900_01_01);
	struct tm* tm;
	if(diff < 0){
		tm = negativeEpochTimeToDate(value);
	}
	else{
		time_t time = diff;
		tm = gmtime((time_t*) &time);
	}
	strftime(str,MAX_SHORT_STRING_SIZE,fmt,tm);
	return str;
}

struct tm* negativeEpochTimeToDate(unsigned long long time){
	struct tm* tm = &tm_buf;
	long long days, rem, y;
	const unsigned short int *ip;

	days = time / SECS_PER_DAY;
	rem = time % SECS_PER_DAY;

	tm->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	tm->tm_min = rem / 60;
	tm->tm_sec = rem % 60;

	y=1900;

	#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
	#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

	while(days >= (isLeapYear(y) ? 366 : 365)){
		long long yg = y + days / 365 - (days % 365 < 0);

		days -= ((yg - y) * 365 + LEAPS_THRU_END_OF(yg - 1) + LEAPS_THRU_END_OF(y - 1));

		y = yg;
	}
	tm->tm_year = y - 1900;

	ip = __mon_yday[isLeapYear(y)];
	for(y = 11; days < (long int) ip[y] && y >= 0; --y){
		continue;
	}
	days -= ip[y];
	tm->tm_mon = y;
	tm->tm_mday = days + 1;
	return tm;
}

int isLeapYear(unsigned long long y){
	return ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0));
}

char* convertToMillisecond(unsigned long long ms, char* str){
	char decimalStr[MAX_SHORT_STRING_SIZE];

	sprintf(decimalStr,".%d", ms / 100);

	return strcat(str,decimalStr);
}


