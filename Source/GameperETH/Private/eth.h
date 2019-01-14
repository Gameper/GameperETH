//
//  eth.h
//
//  Created by gijinBang on 2018. 8. 18..
//  Copyright © 2018년 etherland. All rights reserved.
//

#ifndef eth_h
#define eth_h

#include <iostream>
//privatekey to address(privatekey) : using uECC
//privatekey -> public key -> keccak -> address : using uECC
//privatekey to checksum address(privatekey) : using uECC
//make signed Transaction : using ethereum arduino
//make random privatekey(seed)

using namespace std;

string makeSignedTransaction(string chainID, string nonce, string gas, string gasPrice, string value, string data, string to, string privateKey);
string makeRandomPrivateKey(int seed);
string privateKeyToAddress(string privateKey);
void keccak256(const uint8_t *data, uint16_t length, uint8_t *result);
#endif /* eth_h */

