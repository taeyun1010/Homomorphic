#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>

#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

FHEW::EvalKey* EK;

LWE::SecretKey* SK;

int numBits =10;

int num_disk = 8;
int num_sectors = 80;

int vectorlength = num_disk * num_sectors;
// 
// //initialize
// double vectors[vectorlength]; 

int fingerprintvector[640] = {0};

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

// calculates ct1 - ct2
void subtractBit(LWE::CipherText* difference, LWE::CipherText* bout, const FHEW::EvalKey& EK, const LWE::CipherText& ct1, const LWE::CipherText& ct2, const LWE::CipherText& bin){
    // ~ct1
    LWE::CipherText *notct1Result = new LWE::CipherText;
    // (~ct1) AND ct2
    LWE::CipherText *notct1andct2Result = new LWE::CipherText;
    // ct1 XOR ct2
    LWE::CipherText *ct1xorct2Result = new LWE::CipherText;
//     // ct1 XOR ct2 XOR bin
//     LWE::CipherText *diffResult = new LWE::CipherText;
    // ~(ct1 XOR ct2)
    LWE::CipherText *notct1xorct2Result = new LWE::CipherText;
    // ~(ct1 XOR ct2) AND bin
    LWE::CipherText *notct1xorct2andbinResult = new LWE::CipherText;
//     // (~(ct1 XOR ct2) AND bin) OR ((~ct1) AND ct2)
//     LWE::CipherText *boutResult = new LWE::CipherText;
    
    
    BinGate gate;
   
    
  FHEW::HomNOT(notct1Result, ct1);
  //
  
  gate = AND;
  
  FHEW::HomGate(notct1andct2Result, gate, EK,*notct1Result,ct2);
  
  XorGate(ct1xorct2Result, EK, ct1, ct2);
  
  int ct1xorct2 = LWE::Decrypt(*SK, *ct1xorct2Result);
//   cout << "ct1xorct2 = " << ct1xorct2 << "\n";
  
  XorGate(difference, EK, *ct1xorct2Result, bin);
   int binvalue = LWE::Decrypt(*SK, bin);
//   cout << "binvalue = " << binvalue << "\n";
  
  
  FHEW::HomNOT(notct1xorct2Result, *ct1xorct2Result);
  
  FHEW::HomGate(notct1xorct2andbinResult, gate, EK,*notct1xorct2Result, bin);
  
  gate = OR;
  FHEW::HomGate(bout, gate, EK, *notct1xorct2andbinResult, *notct1andct2Result);
  
  int outputdifference = LWE::Decrypt(*SK, *difference);
  int outputbout = LWE::Decrypt(*SK, *bout);
//   cout << "outputdifference = " << outputdifference << "\n";
//   cout << "outputbout = " << outputbout << "\n";
  
 
}

// compute ct1[] - ct2[]
void subtraction(LWE::CipherText* result[], int* borrowOutput, const FHEW::EvalKey& EK, LWE::CipherText* ct1[], LWE::CipherText* ct2[], LWE::CipherText& initialborrow){

    LWE::CipherText *borrowResult, *borrow;
    borrowResult = new LWE::CipherText;

    borrow = new LWE::CipherText;
    
  
    
    //sumResult = new LWE::CipherText;
    for(int i=0; i<numBits; i++){
        LWE::CipherText* inputbit1 = ct1[i];
        LWE::CipherText* inputbit2 = ct2[i];
       int inputonebit = LWE::Decrypt(*SK, *inputbit1);
      
      int inputonebit2 = LWE::Decrypt(*SK, *inputbit2);
      //cout << i <<"th inputbit for array1 " << inputonebit << "\n";
      
      //cout << i <<"th inputbit for array2 " << inputonebit2 << "\n";
        LWE::CipherText *thissubResult;
        thissubResult = new LWE::CipherText;
        if (i==0){
            
            subtractBit(thissubResult, borrowResult, EK, *inputbit1, *inputbit2, initialborrow);
             //int sumresultbit = LWE::Decrypt(*SK, *sumResult);
             
             //cout << "0th sum bit = " << sumresultbit << "\n";
        }
        else{
            subtractBit(thissubResult, borrowResult, EK, *inputbit1, *inputbit2, *borrow);
        }
        result[i] = thissubResult;
        borrow = borrowResult;
    }
}

//least significant ones first
vector<int> convert(int x) {
  vector<int> ret;
  while(x) {
    if (x&1)
      ret.push_back(1);
    else
      ret.push_back(0);
    x>>=1;  
  }
  
  while(!(ret.size() >= numBits)){
    ret.push_back(0);
  }
  
  //reverse(ret.begin(),ret.end());
  return ret;
}

void printvector(vector<int> bits){
    for (vector<int>::const_iterator i = bits.begin(); i != bits.end(); ++i)
    cout << *i << ' ';
}

void encryptInt(LWE::CipherText* result[], int input){
    
    vector<int> bits = convert(input);
    
    int j=0;
    for (vector<int>::const_iterator i = bits.begin(); i != bits.end(); ++i){
//         if (j >= numBits)
//             break;
        //cout << *i << ' ';
        LWE::CipherText* ct = new LWE::CipherText;
        LWE::Encrypt(ct, *SK, *i);
        result[j] = ct;
        j++;
    }
    
    
}

int decryptInt(LWE::CipherText* encrypted[]){
    int result = 0;
//     int j=0;
//     for (vector<int>::const_iterator i = bits.begin(); i != bits.end(); ++i){
//         cout << *i << ' ';
//         LWE::CipherText* ct = new LWE::CipherText;
//         LWE::Encrypt(ct, *SK, *i);
//         result[j] = ct;
//         j++;
//     }
//     
    for (int i=0; i< numBits; i++){
        LWE::CipherText* ct = encrypted[i];
        int bit = LWE::Decrypt(*SK, *ct);
        result = result + bit * pow(2,i);
    }
    return result;
    
}

// TODO: encrypts two inputs, adds them, decrypts to see if addition works
void addPlaintext(int input1, int input2){
   LWE::CipherText* initialcarry;

    initialcarry = new LWE::CipherText;
  LWE::CipherText* encrypt1[numBits];
  LWE::CipherText* encrypt2[numBits];
   encryptInt(encrypt1, input1);
   encryptInt(encrypt2, input2);
   
  //int decrypted = decryptInt(encrypt);
  //cout << "decrypted = " << decrypted << "\n";

    
  int carryOutput = 0;
  int* carryPointer = &carryOutput;
   
  //initialize carry to 0 
  LWE::Encrypt(initialcarry, *SK, 0);
  
  //4 byte integer
  LWE::CipherText* sum[numBits];
  LWE::CipherText* ctarray1[numBits];
  LWE::CipherText* ctarray2[numBits];
   
  clock_t begin = clock();
  
   addition(sum, carryPointer, *EK, encrypt1, encrypt2, *initialcarry);
    
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    cout << "addition time = " << elapsed_secs << "\n";
   int result = decryptInt(sum);
 
//    
//     for (vector<int>::const_iterator i = bits1.begin(); i != bits1.end(); ++i)
//     cout << *i << ' ';
   cout << "addition result = " << result << "\n";
   
   //   clock_t begin = clock();
//   
//   subtraction(sum, carryPointer, *EK, ctarray1, ctarray2, *initialcarry);
//   
//   clock_t end = clock();
//   double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
}

//encrypts all plaintexts
void encryptPlaintexts(LWE::CipherText** result[], int inputs[]){
    for (int i=0; i<vectorlength; i++){
        int thisint = inputs[i];
        LWE::CipherText* encrypt[numBits];
        encryptInt(encrypt, thisint);
        result[i] = encrypt;
    }
}

//given two array of encrypted plaintexts, calculate 1 norm
int oneNorm(LWE::CipherText** input1[], LWE::CipherText** input2[]){

}


int main(int argc, char *argv[]) {

  char *EKfilename = "ev.key";
  char *SKfilename = "sec.key";
  
  //int input1 = atoi(argv[1]);
  // int input2 = atoi(argv[2]);
  
  FHEW::Setup();

  SK = LoadSecretKey(SKfilename);
  EK = LoadEvalKey(EKfilename);

  LWE::CipherText *ct1,*ct2,*ct3, *ct4, *carry, *initialcarry, 
           *sumResult, 
          *carryResult, *initialbin, *difference, *bout;

  
  
  ct1 = new LWE::CipherText;
  ct2 = new LWE::CipherText;
  ct3 = new LWE::CipherText;
  ct4 = new LWE::CipherText;
  carry = new LWE::CipherText;
  initialcarry = new LWE::CipherText;
  initialbin = new LWE::CipherText;
  difference = new LWE::CipherText;
  bout = new LWE::CipherText;

  carryResult = new LWE::CipherText;

  sumResult = new LWE::CipherText;
  
  //initialize input
  LWE::Encrypt(ct1, *SK, 0);
  LWE::Encrypt(ct2, *SK, 1);
   LWE::Encrypt(ct3, *SK, 1);
  LWE::Encrypt(ct4, *SK, 0);
  LWE::Encrypt(initialbin, *SK, 0);
  //LWE::Encrypt(carry, *SK, 1);
  
  //initialize carry to 0 
  LWE::Encrypt(initialcarry, *SK, 0);
  
//   //
//   // test encryptint and decryptint
//        //integer
//   LWE::CipherText* encrypt[numBits];
//    encryptInt(encrypt, input1);
//   int decrypted = decryptInt(encrypt);
//   cout << "decrypted = " << decrypted << "\n";
// //
//   //
  
  
  //
  //test encryptPlaintexts
  
  
  
  
  LWE::CipherText** encryptedvector[vectorlength];
  clock_t begin = clock();
  encryptPlaintexts(encryptedvector, fingerprintvector);
  clock_t end = clock();
  double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
  cout << "encryptPlaintexts elapsed sec = " << elapsed_secs << "\n";
  //
  //
  
  //
  //test addPlaintext
  //cout << "input1 = " << input1 << "\n";
  //addPlaintext(input1,input2);
  //
  //
  
//     //
//   //test subtraction
//   int carryOutput = 0;
//   int* carryPointer = &carryOutput;
//   
// //   //4 byte integer
//   LWE::CipherText* sum[numBits];
//   LWE::CipherText* ctarray1[numBits];
//   LWE::CipherText* ctarray2[numBits];
//   
//   ctarray1[0] = ct1;
//   ctarray2[0] = ct2;
//   ctarray1[1] = ct3;
//   ctarray2[1] = ct4;
//   for (int i=2; i<numBits; i++){
//       LWE::CipherText *newct1,*newct2;
//   
//       newct1 = new LWE::CipherText;
//       newct2 = new LWE::CipherText;
//       LWE::Encrypt(newct1, *SK, 0);
//       LWE::Encrypt(newct2, *SK, 0);
//       ctarray1[i] = newct1;
//       ctarray2[i] = newct2;
//   }
//   
//   clock_t begin = clock();
//   
//   subtraction(sum, carryPointer, *EK, ctarray1, ctarray2, *initialcarry);
//   
//   clock_t end = clock();
//   double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
//   cout << "elapsed sec = " << elapsed_secs << "\n";
//   for (int i=0; i<numBits; i++){
//       LWE::CipherText* onebitsum = sum[i];
//       int outputonebit = LWE::Decrypt(*SK, *onebitsum);
//       cout << i <<"th outputbit " << outputonebit << "\n";
//   }
//   //
//   //
  
  
  // test addBit
  //addBit(sumResult, carryResult, *EK, *ct1, *ct2, *carry);
  //
  
//   //
//   // test subtractBit 
//   subtractBit(difference, bout, *EK, *ct1, *ct2, *initialbin);
//   int outputdifference = LWE::Decrypt(*SK, *difference);
//   int outputbout = LWE::Decrypt(*SK, *bout);
//   cout << "outputdifference = " << outputdifference << "\n";
//   cout << "outputbout = " << outputbout << "\n";
//   //
//   //
//   
//   //
//   //test addition
//   int carryOutput = 0;
//   int* carryPointer = &carryOutput;
//   
//   //4 byte integer
//   LWE::CipherText* sum[numBits];
//   LWE::CipherText* ctarray1[numBits];
//   LWE::CipherText* ctarray2[numBits];
//   
//   ctarray1[0] = ct1;
//   ctarray2[0] = ct2;
//   ctarray1[1] = ct3;
//   ctarray2[1] = ct4;
//   for (int i=2; i<numBits; i++){
//       LWE::CipherText *newct1,*newct2;
//   
//       newct1 = new LWE::CipherText;
//       newct2 = new LWE::CipherText;
//       LWE::Encrypt(newct1, *SK, 0);
//       LWE::Encrypt(newct2, *SK, 0);
//       ctarray1[i] = newct1;
//       ctarray2[i] = newct2;
//   }
//   
//   clock_t begin = clock();
//   
//   addition(sum, carryPointer, *EK, ctarray1, ctarray2, *initialcarry);
//   
//   clock_t end = clock();
//   double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
//   cout << "elapsed sec = " << elapsed_secs << "\n";
//   for (int i=0; i<numBits; i++){
//       LWE::CipherText* onebitsum = sum[i];
//       int outputonebit = LWE::Decrypt(*SK, *onebitsum);
//       cout << i <<"th outputbit " << outputonebit << "\n";
//   }
//   //
//   //
  
  
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

