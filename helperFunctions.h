#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include "globals.h"

class parameterInfo;

void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType,char* directoryPath,char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],char (*stringLabels)[MAX_STRING_SIZE]);

void parseLabelsConfigFile(char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],int logType,char* directoryPath);

int fast_atoi(const char* str);

char* fast_strcat(char* dest, const char* src);


char* epochTimeToDate(long long value, char* str,const char* fmt);

struct tm* negativeEpochTimeToDate(long long time);

int isLeapYear(long long y);

const unsigned short int __mon_yday[2][13] =
{
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};
#endif
