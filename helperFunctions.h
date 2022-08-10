#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include "globals.h"

class parameterInfo;

void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType,char* directoryPath,char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],char (*stringLabels)[MAX_STRING_SIZE]);

void parseLabelsConfigFile(char (*enumeratedLabels)[MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE],int logType,char* directoryPath);

int fast_atoi(const char* str);

char* fast_strcat(char* dest, const char* src);

char* epochTimeToDate(long long value, char* str,const char* fmt);

#endif
