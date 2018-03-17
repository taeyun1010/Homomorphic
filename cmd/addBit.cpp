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
    
    FHEW::HomGate(res, gate, EK,*orResult,*nandResult);
    
}
void addBit(LWE::CipherText* sum, LWE::CipherText* carry, const FHEW::EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2, const LWE::CipherText& inputcarry){
        LWE::CipherText *andResult,  
          *ct1xorct2Result, *ct1xorct2orcarryResult, 
          *ct1xorct2nandcarryResult, *ct1xorct2andcarryResult;
          
        BinGate gate;
  
  
  //orResult = new LWE::CipherText;
  //nandResult = new LWE::CipherText;
  andResult = new LWE::CipherText;
  ct1xorct2Result = new LWE::CipherText;
  ct1xorct2andcarryResult = new LWE::CipherText;
  
  
  ct1xorct2orcarryResult = new LWE::CipherText;
  ct1xorct2nandcarryResult = new LWE::CipherText;
  
//   // ct1 XOR ct2
//   gate = OR;
//   
//   FHEW::HomGate(orResult, gate, *EK,*ct1,*ct2);
//   
//   gate = NAND;
//   
//   FHEW::HomGate(nandResult, gate, *EK,*ct1,*ct2);
//   
//   gate = AND;
//   
//   FHEW::HomGate(ct1xorct2Result, gate, *EK,*orResult,*nandResult);
//   //
  
  XorGate(ct1xorct2Result, EK, ct1, ct2);
  
//   int xorResult = LWE::Decrypt(*SK, *ct1xorct2Result);
//   cout << "result = " << xorResult << "\n";
  
  
  
//   XorGate(ct1xorct2Result, *EK, *orResult, *nandResult);

  
  // ct1 AND ct2
  gate = AND;
  FHEW::HomGate(andResult, gate, EK,ct1,ct2);
  //
  
  // (ct1 XOR ct2) XOR carry
  gate = OR;
  
  FHEW::HomGate(ct1xorct2orcarryResult, gate, EK,*ct1xorct2Result,inputcarry);
  
  gate = NAND;
  
  FHEW::HomGate(ct1xorct2nandcarryResult, gate, EK,*ct1xorct2Result,inputcarry);
  
  gate = AND;
  
  FHEW::HomGate(sum, gate, EK,*ct1xorct2orcarryResult,*ct1xorct2nandcarryResult);
  //
  
  // (ct1 XOR ct2) AND carry
  gate = AND;
  
  FHEW::HomGate(ct1xorct2andcarryResult, gate, EK, *ct1xorct2Result, inputcarry);
  //
  
  //carryResult = ..;
  gate = OR;
  FHEW::HomGate(carry, gate, EK, *ct1xorct2andcarryResult, *andResult);
  //
    
}

int main(int argc, char *argv[]) {

  char *EKfilename = "ev.key";
  char *SKfilename = "sec.key";
  


  FHEW::Setup();

  LWE::SecretKey* SK = LoadSecretKey(SKfilename);
  EK = LoadEvalKey(EKfilename);

  LWE::CipherText *ct1,*ct2,*carry, 
         *andResult,  
          *ct1xorct2Result, *ct1xorct2orcarryResult, 
          *ct1xorct2nandcarryResult, *sumResult, *ct1xorct2andcarryResult,
          *carryResult;

  BinGate gate;
  
  
  ct1 = new LWE::CipherText;
  ct2 = new LWE::CipherText;
  carry = new LWE::CipherText;
  //orResult = new LWE::CipherText;
  //nandResult = new LWE::CipherText;
  andResult = new LWE::CipherText;
  ct1xorct2Result = new LWE::CipherText;
  ct1xorct2andcarryResult = new LWE::CipherText;
  carryResult = new LWE::CipherText;
  
  ct1xorct2orcarryResult = new LWE::CipherText;
  ct1xorct2nandcarryResult = new LWE::CipherText;
  sumResult = new LWE::CipherText;
  
  //initialize input
  LWE::Encrypt(ct1, *SK, 1);
  LWE::Encrypt(ct2, *SK, 0);
  LWE::Encrypt(carry, *SK, 1);
  
// //   // ct1 XOR ct2
// //   gate = OR;
// //   
// //   FHEW::HomGate(orResult, gate, *EK,*ct1,*ct2);
// //   
// //   gate = NAND;
// //   
// //   FHEW::HomGate(nandResult, gate, *EK,*ct1,*ct2);
// //   
// //   gate = AND;
// //   
// //   FHEW::HomGate(ct1xorct2Result, gate, *EK,*orResult,*nandResult);
// //   //
//   
//   XorGate(ct1xorct2Result, *EK, *ct1, *ct2);
//   
// //   int xorResult = LWE::Decrypt(*SK, *ct1xorct2Result);
// //   cout << "result = " << xorResult << "\n";
//   
//   
//   
// //   XorGate(ct1xorct2Result, *EK, *orResult, *nandResult);
// 
//   
//   // ct1 AND ct2
//   gate = AND;
//   FHEW::HomGate(andResult, gate, *EK,*ct1,*ct2);
//   //
//   
//   // (ct1 XOR ct2) XOR carry
//   gate = OR;
//   
//   FHEW::HomGate(ct1xorct2orcarryResult, gate, *EK,*ct1xorct2Result,*carry);
//   
//   gate = NAND;
//   
//   FHEW::HomGate(ct1xorct2nandcarryResult, gate, *EK,*ct1xorct2Result,*carry);
//   
//   gate = AND;
//   
//   FHEW::HomGate(sumResult, gate, *EK,*ct1xorct2orcarryResult,*ct1xorct2nandcarryResult);
//   //
//   
//   // (ct1 XOR ct2) AND carry
//   gate = AND;
//   
//   FHEW::HomGate(ct1xorct2andcarryResult, gate, *EK, *ct1xorct2Result, *carry);
//   //
//   
//   //carryResult = ..;
//   gate = OR;
//   FHEW::HomGate(carryResult, gate, *EK, *ct1xorct2andcarryResult, *andResult);
//   //
  
  //
  addBit(sumResult, carryResult, *EK, *ct1, *ct2, *carry);
  //
  
  int outputsum = LWE::Decrypt(*SK, *sumResult);
  int outputcarry = LWE::Decrypt(*SK, *carryResult);
  cout << "outputsum = " << outputsum << "\n";
  cout << "outputcarry = " << outputcarry << "\n";
  
  
  
  
}

