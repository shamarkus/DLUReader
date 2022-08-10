CC = gcc
DEBUG = -g

all :
	g++ -g main.cpp logDecoderClass.cpp helperFunctions.cpp -o DLUReader
