#ifndef LOGDECODER_H_
#define LOGDECODER_H_

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <wchar.h>
#include <stdint.h>
#include <time.h>

#define ATO_NUM 0 
#define ATP_NUM 1 
#define AT_UNDEFINED_NUM 2
#define ATP_STR "80_"
#define ATO_STR "c0_"

#define ATP_PARAMETERS_FILENAME "configFiles/ATP_PARAMS.txt"
#define ATO_PARAMETERS_FILENAME "configFiles/ATO_PARAMS.txt"

#define ATP_ENUMERATED_LABELS_FILENAME "configFiles/ATP_LABELS.txt"
#define ATO_ENUMERATED_LABELS_FILENAME "configFiles/ATO_LABELS.txt"

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

#define ATP_BYTE_NUM_TO_SKIP 1262
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

static const char byteArray[MAX_CHARS_SIZE][MAX_BYTE_SIZE] = ["00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111", "00001000", "00001001", 
"00001010", "00001011", "00001100", "00001101", "00001110", "00001111", "00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111", "00011000", "00011001", 
"00011010", "00011011", "00011100", "00011101", "00011110", "00011111", "00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111", "00101000", "00101001", 
"00101010", "00101011", "00101100", "00101101", "00101110", "00101111", "00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111", "00111000", "00111001", 
"00111010", "00111011", "00111100", "00111101", "00111110", "00111111", "01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111", "01001000", "01001001", 
"01001010", "01001011", "01001100", "01001101", "01001110", "01001111", "01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111", "01011000", "01011001", 
"01011010", "01011011", "01011100", "01011101", "01011110", "01011111", "01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111", "01101000", "01101001", 
"01101010", "01101011", "01101100", "01101101", "01101110", "01101111", "01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111", "01111000", "01111001", 
"01111010", "01111011", "01111100", "01111101", "01111110", "01111111", "10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111", "10001000", "10001001", 
"10001010", "10001011", "10001100", "10001101", "10001110", "10001111", "10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111", "10011000", "10011001", 
"10011010", "10011011", "10011100", "10011101", "10011110", "10011111", "10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111", "10101000", "10101001", 
"10101010", "10101011", "10101100", "10101101", "10101110", "10101111", "10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111", "10111000", "10111001", 
"10111010", "10111011", "10111100", "10111101", "10111110", "10111111", "11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111", "11001000", "11001001", 
"11001010", "11001011", "11001100", "11001101", "11001110", "11001111", "11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111", "11011000", "11011001", 
"11011010", "11011011", "11011100", "11011101", "11011110", "11011111", "11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111", "11101000", "11101001",
"11101010", "11101011", "11101100", "11101101", "11101110", "11101111", "11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111", "11111000", "11111001", 
"11111010", "11111011", "11111100", "11111101", "11111110", "11111111"];

struct headerInfo {
    size_t headerBitSize;
    size_t headerByteSize;
    int headerBitPos;
    int timeBitPos;
    int timeBitSize;
};
struct fileInfo {
    char directoryPath[MAX_STRING_SIZE];
	char fileName[MAX_STRING_SIZE];
	FILE* inputFile;
	FILE* outputFile;

	int logType;
};

class parameterInfo {
    private:
        int parameterID;
        int unsignedInt;
        int firstBitPosition;
        int bitCount;
        int firstByte;
        int lastByte;
        double quantum;
        int offset;
        int displayType;
        int enumeratedLabel;
        int decimalCount;

        char* unit = NULL;
        char*** enumeratedLabels = NULL;
    public:
        char* (parameterInfo::*intToString)(long long, char*) = NULL;

        parameterInfo();
        ~parameterInfo();
        //ALL METHODS
};

class fileParsingInfo {
    private:
        struct fileInfo* fileInfoStruct;
        struct headerInfo headerInfoStruct = (struct headerInfo) {MAX_HEADER_BIT_SIZE,
								  MAX_HEADER_BIT_SIZE/8,
                                                                  INNER_HEADER_BIT_POS,
                                                                  HEADER_TIME_BIT_POS,
								  HEADER_TIME_BIT_SIZE};
        std::vector<class parameterInfo*> parameterInfoVec;
	char** stringLabels;
        char*** enumeratedLabels;
        size_t byteNumToSkip;
	size_t byteNumForLine;
    public:
        fileParsingInfo(struct fileInfo* fileInfoStruct, int logType);
        ~fileParsingInfo();
};

std::vector<class parameterInfo*> *ATP_parameterInfo = NULL;
std::vector<class parameterInfo*> *ATO_parameterInfo = NULL;
char ATP_EnumeratedLabels[MAX_ATP_LABELS][MAX_ATP_VALUES][MAX_SHORT_STRING_SIZE];
char ATO_EnumeratedLabels[MAX_ATO_LABELS][MAX_ATO_VALUES][MAX_SHORT_STRING_SIZE];
char ATP_StringLabels[MAX_ATP_PARAMS][MAX_STRING_SIZE];
char ATO_StringLabels[MAX_ATO_PARAMS][MAX_STRING_SIZE];

void parseFile(class fileParsingInfo* fileObj);

void parseLabelsConfigFile(char* enumeratedLabels[MAX_LABELS_SIZE],int logType);

void parseParameterConfigFile(std::vector<class parameterInfo*> &parameterInfoVec,int logType);

int fast_atoi(const char* str);
#endif
