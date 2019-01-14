//
//  eth.cpp
//  Ethereum-Arduino
//
//  Created by gijinBang on 2018. 8. 19..
//  Copyright © 2018년 etherland. All rights reserved.
//

#include "eth.h"
#include "TX.h"
#include "RLP.h"
//#include "keccak.h"
#include "keccak256.h"
#include <cstdlib>
#include <ctime>

extern "C" {
#include "libs/ecdsa.h"
#include "libs/bignum256.h"
}
int hexstring2ints(char ch){
    int chi=0;
    if(ch>='a' && ch<='z'){
        chi = ch - 87;
    }else if(ch>='0' && ch <= '9'){
        chi = ch - '0';
    }
    return chi;
}


string uint8_t2string(uint8_t r[], int length){
    string s="0x";
    char hexarray[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(int i=0;i<length;i++){
        s = s + hexarray[r[i]/16];
        s = s + hexarray[r[i]%16];
    }
    return s;
}
// string like [0x71,0xab] -> "0x71ab"
string bytestring2string(string h){
    string s="0x";
    char hexarray[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(int i=0;i<h.length();i++){
        unsigned char c = ((unsigned char)(unsigned char)h[i]);
        //printf("%c,%c ", hexarray[c/16], hexarray[c%16]);
        s = s + hexarray[c/16];
        s = s + hexarray[c%16];
    }
    return s;
}

string makeZero2Space(string s){
    if(s[0]=='0'&&s[1]=='x')
        s = s.substr(2,s.length()-2);
    for(int i=0;i<s.length();i++){
        if(s[i] != '0') return s;
    }
    return "0x";
}
//chainID should be 1 byte
string makeSignedTransaction(string chainID, string nonce, string gas, string gasPrice, string value, string data, string to, string privateKey){
    int i;
    /*
    from eth-lib, account.js transaction.js
    data = signingData(tx) = return RLP.encode([Bytes.fromNat(tx.nonce), Bytes.fromNat(tx.gasPrice), Bytes.fromNat(tx.gas), tx.to ? tx.to.toLowerCase() : "0x", Bytes.fromNat(tx.value), tx.data, Bytes.fromNat(tx.chainId || "0x1"), "0x", "0x"]);
    
    const signature = Account.makeSigner(Nat.toNumber(tx.chainId || "0x1") * 2 + 35)(keccak256(data), account.privateKey);

    const rawTransaction = RLP.decode(data).slice(0, 6).concat(Account.decodeSignature(signature));
     Account.decodeSignature(signature) = [v, r, s]
    return RLP.encode(rawTransaction);
    
    */
    
    if(chainID.length()%2==1) chainID.insert(2, "0");
    if(nonce.length()%2==1) nonce.insert(2, "0");
    if(gas.length()%2==1) gas.insert(2, "0");
    if(gasPrice.length()%2==1) gasPrice.insert(2, "0");
    if(value.length()%2==1) value.insert(2, "0");
    if(data.length()%2==1) data.insert(2, "0");
    if(to.length()%2==1) to.insert(2, "0");
    if(privateKey.length()%2==1) privateKey.insert(2, "0");
    
    nonce = makeZero2Space(nonce);
    to = makeZero2Space(to);
    value = makeZero2Space(value);
    data = makeZero2Space(data);
    chainID = makeZero2Space(chainID);
    gasPrice = makeZero2Space(gasPrice);
    gas = makeZero2Space(gas);
    privateKey = makeZero2Space(privateKey);
    
    TX tx;
    tx.chainID = chainID;
    tx.value = value;
    tx.nonce = nonce;
    tx.gasLimit = gas;
    tx.gasPrice = gasPrice;
    tx.data = data;
    tx.to = to;
    tx.r = "0x";
    tx.s = "0x";
    tx.v = chainID;

    RLP rlp;
    std::string signingData = rlp.encode(tx, false);
    // return "0x8bcdef";
    uint8_t *_data = new uint8_t[signingData.length()];
    for(i=0;i<signingData.length();i++){
        _data[i] = (unsigned char)signingData[i];
//        printf("%x ", _data[i]);
    }
//    printf("\n");
    uint8_t hashval[32];
    
    keccak256(_data, signingData.length(), hashval);
    
//    for(i=0;i<32;i++){
//        printf("%x ", hashval[i]);
//    }
//    void keccak256(const uint8_t *data, uint16_t length, uint8_t *result)
    
    delete [] _data;
//    cout << "------------- digest test ---------" << endl;
//    uint8_t hashval[32];
    uint8_t privkey[32];
    privateKey = rlp.removeHexFormatting(privateKey);
//
//    for(i=0;i<64;i+=2){
//        uint8_t con = 0;
//        con += (hexstring2ints(digest[i]) * 16);
//        con += (hexstring2ints(digest[i+1]) * 1);
//        hashval[i/2] = con;
//    }
    
    for(i=0;i<64;i+=2){
        uint8_t con = 0;
        con += (hexstring2ints(privateKey[i]) * 16);
        con += (hexstring2ints(privateKey[i+1]) * 1);
        privkey[i/2] = con;
    }
    uint8_t r[32], s[32];
    uint8_t v = ecdsaSign((BigNum256)r, (BigNum256)s, (BigNum256)hashval, (BigNum256)privkey); // r, s return reversed endian
    uint8_t r_modified[32], s_modified[32];
    for(i=0;i<32;i++){
        r_modified[i] = r[31-i];
        s_modified[i] = s[31-i];
    }
    
    chainID = rlp.removeHexFormatting(chainID);
    uint8_t _chainID = 0;
    _chainID += (hexstring2ints(chainID[0]) * 16);
    _chainID += (hexstring2ints(chainID[1]) * 1);
    
    uint8_t eip155v[1];
    eip155v[0] = v + 35 + _chainID * 2;
    
    tx.r = uint8_t2string(r_modified, 32);
    tx.s = uint8_t2string(s_modified, 32);
    tx.v = uint8_t2string(eip155v, 1);
    std::string fullTx = rlp.encode(tx, false);
    string raw_tx = bytestring2string(fullTx);
    return raw_tx;
}
string makeRandomPrivateKey(int seed){
    string defaultKey = "59bb6d2b798c4b26d4c9db6aeff0d79794f2ffa1972b280e7ad26eae9d65afee";
    string key = "0x";
    srand((unsigned int)time(NULL) + seed);
    
    char hexarray[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(int i=0;i<64;i++){
        key += hexarray[rand() % 16];
    }
    return key;
}

string privateKeyToAddress(string privateKey){
    RLP rlp;
    
    int i;
    privateKey = rlp.removeHexFormatting(privateKey);
    //0x6f0f5673d69b4608ac9be5887b2f71f20d0c3587
    uint8_t address[20];
    //0x69,0x70,0xee,0x39,0x33,0xec,0x4e,0x62,0xbf,0x42,0x65,0x08,0x27,0x51,0x0c,0xfa,0xb5,0x35,0x7a,0x63,0x83,0x37,0x9c,0x5a,0x58,0x66,0xa2,0x50,0x41,0x9a,0xf7,0x05
    uint8_t priv[32];
    
    for(i=0;i<64;i+=2){
        uint8_t con = 0;
        con += (hexstring2ints(privateKey[i]) * 16);
        con += (hexstring2ints(privateKey[i+1]) * 1);
        priv[i/2] = con;
    }
    // public key = privatekey * G
    PointAffine publicKey;
    publicKey = privateKeyToPublicKey(priv);
    
    uint8_t pubs[64];
    for(i=0;i<32;i++){
        pubs[i] = publicKey.x[i];
        pubs[i+32] = publicKey.y[i];
    }
    uint8_t pubhashs[32];
    keccak256(pubs, 64, pubhashs);
    string testPubkeyHash = uint8_t2string(pubhashs, 32);
    testPubkeyHash.erase(0,26);
    return "0x"+testPubkeyHash;
}

void keccak256(const uint8_t *data, uint16_t length, uint8_t *result) {
    
    SHA3_CTX context;
    keccak_init(&context);
    keccak_update(&context, (const unsigned char*)data, (size_t)length);
    keccak_final(&context, (unsigned char*)result);
    
    // Clear out the contents of what we hashed (in case it was secret)
    memset((char*)&context, 0, sizeof(SHA3_CTX));
}
