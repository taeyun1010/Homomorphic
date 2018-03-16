#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

FHEW::EvalKey* EK;

void XorGate(LWE::CipherText* res, const FHEW::EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2) {
    LWE::CipherText *orResult, *nandResult;
    
    orResult = new LWE::CipherText;
    nandResult = new LWE::CipherText;
    
    
    BinGate gate;
    gate = OR;
    
    FHEW::HomGate(orResult, gate, EK,ct1,ct2);
    
    gate = NAND;
    
    FHEW::HomGate(nandResult, gate, EK,ct1,ct2);
    
    gate = AND;
    
    FHEW::HomGate(res, gate, EK,orResult,nandResult);
    
}


int main(int argc, char *argv[]) {

  char *EKfilename = "ev.key";
  char *SKfilename = "sec.key";
  


  FHEW::Setup();

  LWE::SecretKey* SK = LoadSecretKey(SKfilename);
  EK = LoadEvalKey(EKfilename);

  LWE::CipherText *ct1,*ct2, *finalResult;//,*orResult, *nandResult, *finalResult;

//   BinGate gate;
//   gate = OR;
//   
  ct1 = new LWE::CipherText;
  ct2 = new LWE::CipherText;
//   orResult = new LWE::CipherText;
//   nandResult = new LWE::CipherText;
   finalResult = new LWE::CipherText;

  LWE::Encrypt(ct1, *SK, 0);
  LWE::Encrypt(ct2, *SK, 0);
  
//   FHEW::HomGate(orResult, gate, *EK,*ct1,*ct2);
//   
//   gate = NAND;
//   
//   FHEW::HomGate(nandResult, gate, *EK,*ct1,*ct2);
//   
//   gate = AND;
//   
//   FHEW::HomGate(finalResult, gate, *EK,*orResult,*nandResult);

  XorGate(finalResult, *EK, *ct1, *ct2);
  
  int result = LWE::Decrypt(*SK, *finalResult);
  cout << "result = " << result << "\n";
}

