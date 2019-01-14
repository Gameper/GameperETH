/*
  RLP.h - RLP library for RLP functions
*/
#ifndef RLP_h
#define RLP_h
#include <stdio.h>
#include<string>
#include <sstream>
#include "TX.h"

class RLP
{
  public:
    std::string encode(std::string);
    std::string encode(TX, bool);
  	std::string encodeLength(int, int);
  	std::string intToHex(int);
  	std::string bytesToHex(std::string);
  	std::string removeHexFormatting(std::string);
  	std::string hexToRlpEncode(std::string);
  	std::string hexToBytes(std::string);
  	int char2int(char);
  	void hex2bin(char*, char*, uint16_t);

};

#endif


