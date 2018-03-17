#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

FHEW::EvalKey* EK;

LWE::SecretKey* SK;

int numBits =32;

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

void addition(LWE::CipherText* sum[], int* carryOutput, const FHEW::EvalKey& EK, LWE::CipherText* ct1[], LWE::CipherText* ct2[], LWE::CipherText& initialcarry){

    LWE::CipherText *carryResult, *carry;
    carryResult = new LWE::CipherText;

    carry = new LWE::CipherText;
    
  
    
    //sumResult = new LWE::CipherText;
    for(int i=0; i<numBits; i++){
        LWE::CipherText* inputbit1 = ct1[i];
        LWE::CipherText* inputbit2 = ct2[i];
             int inputonebit = LWE::Decrypt(*SK, *inputbit1);
      
      int inputonebit2 = LWE::Decrypt(*SK, *inputbit2);
      //cout << i <<"th inputbit for array1 " << inputonebit << "\n";
      
      //cout << i <<"th inputbit for array2 " << inputonebit2 << "\n";
        LWE::CipherText *thissumResult;
        thissumResult = new LWE::CipherText;
        if (i==0){
            
            addBit(thissumResult, carryResult, EK, *inputbit1, *inputbit2, initialcarry);
             //int sumresultbit = LWE::Decrypt(*SK, *sumResult);
             
             //cout << "0th sum bit = " << sumresultbit << "\n";
        }
        else{
            addBit(thissumResult, carryResult, EK, *inputbit1, *inputbit2, *carry);
        }
        sum[i] = thissumResult;
        carry = carryResult;
    }
}

int main(int argc, char *argv[]) {

  char *EKfilename = "ev.key";
  char *SKfilename = "sec.key";
  


  FHEW::Setup();

  SK = LoadSecretKey(SKfilename);
  EK = LoadEvalKey(EKfilename);

  LWE::CipherText *ct1,*ct2,*ct3, *ct4, *carry, *initialcarry, 
           *sumResult, 
          *carryResult;

  
  
  ct1 = new LWE::CipherText;
  ct2 = new LWE::CipherText;
  ct3 = new LWE::CipherText;
  ct4 = new LWE::CipherText;
  carry = new LWE::CipherText;
  initialcarry = new LWE::CipherText;


  carryResult = new LWE::CipherText;

  sumResult = new LWE::CipherText;
  
  //initialize input
  LWE::Encrypt(ct1, *SK, 1);
  LWE::Encrypt(ct2, *SK, 1);
   LWE::Encrypt(ct3, *SK, 0);
  LWE::Encrypt(ct4, *SK, 1);
  //LWE::Encrypt(carry, *SK, 1);
  
  //initialize carry to 0 
  LWE::Encrypt(initialcarry, *SK, 0);
  
  
  // test addBit
  //addBit(sumResult, carryResult, *EK, *ct1, *ct2, *carry);
  //
  
  //
  //test addition
  int carryOutput = 0;
  int* carryPointer = &carryOutput;
  
  //4 byte integer
  LWE::CipherText* sum[numBits];
  LWE::CipherText* ctarray1[numBits];
  LWE::CipherText* ctarray2[numBits];
  
  ctarray1[0] = ct1;
  ctarray2[0] = ct2;
  ctarray1[1] = ct3;
  ctarray2[1] = ct4;
  for (int i=2; i<numBits; i++){
      LWE::CipherText *newct1,*newct2;
  
      newct1 = new LWE::CipherText;
      newct2 = new LWE::CipherText;
      LWE::Encrypt(newct1, *SK, 0);
      LWE::Encrypt(newct2, *SK, 0);
      ctarray1[i] = newct1;
      ctarray2[i] = newct2;
  }
  addition(sum, carryPointer, *EK, ctarray1, ctarray2, *initialcarry);
  
  for (int i=0; i<numBits; i++){
      LWE::CipherText* onebitsum = sum[i];
      int outputonebit = LWE::Decrypt(*SK, *onebitsum);
      cout << i <<"th outputbit " << outputonebit << "\n";
  }
  
//   for (int i=0; i<numBits; i++){
//   
//       LWE::CipherText* input1ct;
//       LWE::CipherText* input2ct;
//       input1ct = ctarray1[i];
//       input2ct =  ctarray2[i];
//       
//       
//       
//       int inputonebit = LWE::Decrypt(*SK, *input1ct);
//       
//       int inputonebit2 = LWE::Decrypt(*SK, *input2ct);
//       cout << i <<"th inputbit for array1 " << inputonebit << "\n";
//       
//       cout << i <<"th inputbit for array2 " << inputonebit2 << "\n";
//   }
  
  //
  //
  
  
//   int outputsum = LWE::Decrypt(*SK, *sumResult);
//   int outputcarry = LWE::Decrypt(*SK, *carryResult);
//   cout << "outputsum = " << outputsum << "\n";
//   cout << "outputcarry = " << outputcarry << "\n";
//   
  
  
  
}

