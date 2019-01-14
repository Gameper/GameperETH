// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GameperETHBPLibrary.h"
#include "GameperETH.h"

UGameperETHBPLibrary::UGameperETHBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UGameperETHBPLibrary::GameperETHSampleFunction(float Param)
{
	return -1;
}


FString UGameperETHBPLibrary::GameperETH_MakeSignedTransaction(
	const FString &nonce,
	const FString &gasPrice,
	const FString &gasLimit,
	const FString &value,
	const FString &data,
	const FString &to,
	const FString &chainID,
	const FString &privatekey)
{
	string _nonce(TCHAR_TO_UTF8(*nonce));
	string _gasPrice(TCHAR_TO_UTF8(*gasPrice));
	string _gasLimit(TCHAR_TO_UTF8(*gasLimit));
	string _value(TCHAR_TO_UTF8(*value));
	string _data(TCHAR_TO_UTF8(*data));
	string _to(TCHAR_TO_UTF8(*to));
	string _chainID(TCHAR_TO_UTF8(*chainID));
	string _privatekey(TCHAR_TO_UTF8(*privatekey));
	//string makeSignedTransaction(string chainID, string nonce, string gas, string gasPrice, string value, string data, string to, string privateKey)
	string return_tx_obj = makeSignedTransaction(_chainID, _nonce, _gasLimit, _gasPrice, _value, _data, _to, _privatekey); 

	FString res(return_tx_obj.c_str());
	// FString res("hello");
	return res;
}

FString UGameperETHBPLibrary::GameperETH_makeRandomPrivateKey()
{
	string return_privatekey = makeRandomPrivateKey(100);
	FString res(return_privatekey.c_str());
	return res;
}

FString UGameperETHBPLibrary::GameperETH_getAddress(const FString &privatekey)
{
	string _privatekey(TCHAR_TO_UTF8(*privatekey));
    string return_address =  privateKeyToAddress(_privatekey);
	FString res(return_address.c_str());
	return res;
}