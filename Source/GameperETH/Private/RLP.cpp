/*
  RLP.cpp - RLP library for RLP functions
*/
#include "RLP.h"
using namespace std;

string RLP::encode(string s)
{
    // return "0x12341234";
  	if(s.size()==1 && (unsigned char)(s[0])<128)
  		return s;
	else{
        //cout << s << " : " << encodeLength(s.size(), 128)+s << endl;
        
        return encodeLength(s.size(), 128) + s;
        //return hexToBytes(encodeLength(s.size(), 128))+s;
	}
}
//30783536373835363738
string RLP::encode(TX tx, bool toSign)
{
    
    string serialized = hexToRlpEncode(tx.nonce)+
                        hexToRlpEncode(tx.gasPrice)+
                        hexToRlpEncode(tx.gasLimit)+
                        hexToRlpEncode(tx.to)+
                        hexToRlpEncode(tx.value)+
                        hexToRlpEncode(tx.data) + (hexToRlpEncode(tx.v)+
                        hexToRlpEncode(tx.r)+
                        hexToRlpEncode(tx.s));
    // return "0x56785678";
    //printf("\nin encode : %x\n", hexToBytes(encodeLength(serialized.length(),192)).length());
    
    string encodedLength = encodeLength(serialized.length(),192);
    
    if(encodedLength.length() > 1){
     //   return hexToBytes(encodedLength) + serialized;
    }
    return encodedLength + serialized;
    /*
    if(toSign==true){
        return encodeLength(serialized.length(),192)+serialized;
    }
    return hexToBytes(encodeLength(serialized.length(),192))+serialized;
    */
    
}
string RLP::hexToBytes(string s){
    
    char inp [s.length()];
	memcpy(inp,s.c_str(),s.length());
    char dest [sizeof(inp)/2];
	hex2bin(inp, dest, s.length()); // crimial!!!!!!@!!!!!!!!!
    
	return string(dest,sizeof(dest));
}
string RLP::hexToRlpEncode(string s){
    s = removeHexFormatting(s);
	return encode(hexToBytes(s));
    //return s;
}
string RLP::removeHexFormatting(string s){
    if(s[0]=='0'&&s[1]=='x')
        return s.substr(2,s.length()-2);
    return s;
}
string RLP::encodeLength(int len, int offset)
{
	string temp;
  	if(len<56){
  		temp=(char)(len+offset);
  		return temp;
  	}else{
          // criminal !!!!!!!!!!!!
  		string hexLength = intToHex(len);
		int	lLength =   hexLength.length()/2;
		string fByte = intToHex(offset+55+lLength);
        return hexToBytes(fByte+hexLength);
	}
}
string RLP::intToHex(int n){
	
    stringstream stream;
	stream << std::hex << n;
	string result( stream.str() );
	if(result.size() % 2)
		result = "0"+result;
        
	return result;
}
string RLP::bytesToHex(string input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
int RLP::char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
    
    
  return 1;
}
void RLP::hex2bin(char* src, char* target, uint16_t length)
{
    for(int i=0;i<length;i+=2){
        target[i/2] = char2int(src[i]) * 16 + char2int(src[i+1]);
    }
}
