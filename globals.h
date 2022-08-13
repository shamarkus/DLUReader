#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <wchar.h>
#include <stdint.h>
#include <time.h>
#include <chrono>

class parameterInfo;

#define ATO_NUM 0 
#define ATP_NUM 1 
#define AT_UNDEFINED_NUM 2
#define ATP_STR "80_"
#define ATO_STR "c0_"

#define ATP_PARAMETERS_FILENAME "configFiles\\ATP_PARAMS.txt"
#define ATO_PARAMETERS_FILENAME "configFiles\\ATO_PARAMS.txt"

#define ATP_ENUMERATED_LABELS_FILENAME "configFiles\\ATP_LABELS.txt"
#define ATO_ENUMERATED_LABELS_FILENAME "configFiles\\ATO_LABELS.txt"

#define HEADER_TIME_SUFFIX ".000"

#define YEARS_101_TIME 3187296000
#define MAX_ATP_LABELS 26
#define MAX_ATO_LABELS 149
#define MAX_ATP_VALUES 99
#define MAX_ATO_VALUES 256
#define MAX_ATP_PARAMS 1765
#define MAX_ATO_PARAMS 3095
#define MAX_ATP_PARAMS_BIT_SIZE 12496
#define MAX_ATO_PARAMS_BIT_SIZE 20176

#define MAX_LINE_SIZE 2700
#define MAX_STRING_SIZE 512
#define MAX_SHORT_STRING_SIZE 64
#define MAX_ENUMERATED_SIZE 50
#define MAX_CHARS_SIZE 256
#define MAX_HEADER_BIT_SIZE 248
#define MAX_HEADER_BYTE_SIZE 31
#define HEADER_TIME_BIT_POS 136
#define INNER_HEADER_BIT_POS 11144
#define HEADER_TIME_BIT_SIZE 32
#define MAX_BYTE_SIZE 8
#define TXT_SUFFIX ".txt" 

#define UNSIGNED_INTEGER 0
#define SIGNED_INTEGER 1
#define NOT_APPLICABLE -1

#define ATP_BYTE_NUM_TO_SKIP 1261
#define ATO_BYTE_NUM_TO_SKIP 4

#define DISPLAY_TYPE_ENUMERATED 0
#define DISPLAY_TYPE_HEXADECIMAL 1
#define DISPLAY_TYPE_DECIMAL 2
#define DISPLAY_TYPE_BINARY 3
#define DISPLAY_TYPE_DATE 4
#define DISPLAY_TYPE_TIME 5

#define DECIMAL_PRECISION 3
#define DATE_TIME_BIT_COUNT 64
#define DATE_TIME_SHORT_BIT_COUNT 32
#define TIME_SINCE_1900_01_01 2208988800

extern std::vector<class parameterInfo*> *ATP_parameterInfo;
extern std::vector<class parameterInfo*> *ATO_parameterInfo;

extern char ATP_EnumeratedLabels[MAX_ATP_LABELS][MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE];
extern char ATO_EnumeratedLabels[MAX_ATO_LABELS][MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE];
extern char ATP_StringLabels[MAX_ATP_PARAMS][MAX_STRING_SIZE];
extern char ATO_StringLabels[MAX_ATO_PARAMS][MAX_STRING_SIZE];

#endif
