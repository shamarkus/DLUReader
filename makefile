CC = gcc
DEBUG = -g

all :
	g++ -o3 -Wall main.cpp logDecoderClass.cpp helperFunctions.cpp -o DLUReader
