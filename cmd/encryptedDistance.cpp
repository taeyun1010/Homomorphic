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
#include <mex.h>
using namespace std;

FHEW::EvalKey* EK;

LWE::SecretKey* SK;

int numBits =10;
// 
// int num_disk = 8;
// int num_sectors = 80;
// 


//int vectorlength = num_disk * num_sectors;

//CAUTION!!!!!!!!!!!!!!!!!!!1   TODO
int vectorlength = 16;


// 
// //initialize
// double vectors[vectorlength]; 

// CAUTION!!!!!!!!!!!! fixed constant  TODO
int fingerprintvector[16];
int fingerprintvector2[16];


// int fingerprintvector[16] = {52,
// 51,
// 57,
// 97,
// 120,
// 137,
// 106,
// 114,
// 51,
// 52,
// 80,
// 47,
// 98,
// 85,
// 83,
// 81};
// int fingerprintvector2[16] = {73,
// 61,
// 58,
// 93,
// 113,
// 121,
// 122,
// 97,
// 107,
// 91,
// 73,
// 74,
// 148,
// 135,
// 110,
// 134};

typedef struct { 
    
    // CAUTION!!!!!!!!!!! TODO,   numBits
    LWE::CipherText* cts[10];
} Encryptedvalue;


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
        
        
        
        //
        //
       //int inputonebit = LWE::Decrypt(*SK, *inputbit1);
      
      //int inputonebit2 = LWE::Decrypt(*SK, *inputbit2);
      //cout << i <<"th inputbit for array1 " << inputonebit << "\n";
      
      //cout << i <<"th inputbit for array2 " << inputonebit2 << "\n";
      //
      //
      
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
    if(x>=0){
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
        
    }
    else{
        int absolute = abs(x);
        //cout << "absolute = " << absolute << "\n";
        while(absolute) {
            if (absolute&1)
                ret.push_back(1);
            else
                ret.push_back(0);
            absolute>>=1;
        }
        
        while(!(ret.size() >= numBits)){
            ret.push_back(0);
        }
        
        //bits for abs calculated
        //cout << "abs bits: \n";
//         for (int j=0; j< ret.size(); j++){
//             cout << "ret[j] = " << ret[j] << "\n";
//           
//             
//         }
        
//         for (vector<int>::const_iterator i = ret.begin(); i != ret.end(); ++i){
//             cout << *i << ' ';
//            
//             
//         }
        
        
        //flip all bits
        
//         for (vector<int>::const_iterator i = ret.begin(); i != ret.end(); ++i){
//             //cout << *i << ' ';
//             if(*i == 0){
//                 *i = 1;
//                 
//             }
//             else{
//                 *i = 0;
//                 
//             }
//             
//         }
         for (int j=0; j< ret.size(); j++){
            //cout << *i << ' ';
            if(ret[j] == 0){
                ret[j] = 1;
                
            }
            else{
                ret[j] = 0;
                
            }
            
        }
        
//         //add one
//         int mustcarry = 0;
//         for (vector<int>::const_iterator i = ret.begin(); i != ret.end(); ++i){
//             //cout << *i << ' ';
//             if(*i == 0){
//                 *i = 1;
//                 mustcarry = 0;
//             }
//             else{
//                 *i = 0;
//                 mustcarry = 1;
//             }
//             if (mustcarry == 0)
//                 break;
//         }
        
          //add one
        int mustcarry = 0;
        for (int j=0; j< ret.size(); j++){
            //cout << *i << ' ';
            if(ret[j] == 0){
                ret[j] = 1;
                mustcarry = 0;
            }
            else{
                ret[j] = 0;
                mustcarry = 1;
            }
            if (mustcarry == 0)
                break;
        }
        
    }
    
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
        if (i == (numBits-1))
            result = result - bit* pow(2,i);
        else
            result = result + bit * pow(2,i);
    }
    return result;
    
}

//encrypts two inputs, adds them, decrypts to see if addition works
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

//encrypts two inputs, adds them, decrypts to see if addition works
void subtractPlaintext(int input1, int input2){
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
  
   subtraction(sum, carryPointer, *EK, encrypt1, encrypt2, *initialcarry);
    
    clock_t end = clock();
    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
    cout << "subtraction time = " << elapsed_secs << "\n";
   int result = decryptInt(sum);
 
//    
//     for (vector<int>::const_iterator i = bits1.begin(); i != bits1.end(); ++i)
//     cout << *i << ' ';
   cout << "subtraction result = " << result << "\n";
   
   //   clock_t begin = clock();
//   
//   subtraction(sum, carryPointer, *EK, ctarray1, ctarray2, *initialcarry);
//   
//   clock_t end = clock();
//   double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
}
// 
// //encrypts all plaintexts, version that does not use Encryptedvalue
// void encryptPlaintexts(LWE::CipherText** result[], int inputs[]){
//     for (int i=0; i<vectorlength; i++){
//         int thisint = inputs[i];
//         LWE::CipherText* encrypt[numBits];
//         encryptInt(encrypt, thisint);
//         result[i] = encrypt;
//     }
// }


//encrypts all plaintexts
void encryptPlaintexts(Encryptedvalue result[], int inputs[]){
    for (int i=0; i<vectorlength; i++){
        Encryptedvalue thisvalue;
        for(int j=0; j<numBits; j++){
            int thisint = inputs[i];
            
            //CAUTION!!!!!!!!!!! TODO
            LWE::CipherText* encrypt[10];
            encryptInt(encrypt, thisint);
            for (int k=0; k<numBits; k++){
                thisvalue.cts[k] = encrypt[k];
            }
        }
       
        result[i] = thisvalue;
    }
}


// calculate absolute value of given encrypted integer
void absolute(const FHEW::EvalKey& EK, LWE::CipherText* ct[]){
    
    LWE::CipherText* msbct = ct[(numBits-1)];
    int msb = LWE::Decrypt(*SK, *msbct);
    
     //calculate 2's complement only if this is a negative number
    if (msb == 1){
        
        //LWE::CipherText* tempresult[numBits];
        
        //flip all bits
        for(int i=0; i<numBits; i++){
            LWE::CipherText* inputbit = ct[i];
            
            //LWE::CipherText *notResult;
           // notResult = new LWE::CipherText;
            FHEW::HomNOT(inputbit, *inputbit);
            //result[i] = inputbit;
            
        }
        
        //add one
        
        LWE::CipherText *initialcarry;
        
        LWE::CipherText* onect[numBits];
        encryptInt(onect, 1);
        
        initialcarry = new LWE::CipherText;
        
        //this was supposed to be an indicator of whether addition returned a carry in most sig bit, but not suppported, for now
        int carryOutput = 0;
        int* carryPointer = &carryOutput;
        
        //initialize carry to 0
        LWE::Encrypt(initialcarry, *SK, 0);
        
        addition(ct, carryPointer, EK, onect, ct, *initialcarry);
    }
    
    //just return the same ct
    else{
         for(int i=0; i<numBits; i++){
            LWE::CipherText* inputbit = ct[i];
            
            //result[i] = inputbit;
            ct[i] = inputbit;
        }
    }
    
    
}

//determine if given encrypted integer is negative, returns 1 if negative
int isNegative(LWE::CipherText* input[]){
    LWE::CipherText* msbct = input[(numBits-1)];
    int msb = LWE::Decrypt(*SK, *msbct);
    return msb;
}


// //given two array of encrypted plaintexts, calculate 1 norm ,version that does not use Encryptedvalue
// int oneNorm(LWE::CipherText** input1[], LWE::CipherText** input2[]){
//     for(int i=0; i<vectorlength; i++){
//         LWE::CipherText** thisinput1 = input1[i];
//         LWE::CipherText** thisinput2 = input2[i];
//         
//         LWE::CipherText* initialcarry;
//         
//         initialcarry = new LWE::CipherText;
//         
//         int carryOutput = 0;
//         int* carryPointer = &carryOutput;
//         
//         //initialize carry to 0
//         LWE::Encrypt(initialcarry, *SK, 0);
//         
//         //4 byte integer
//         LWE::CipherText* subtracted[numBits];
//         
//         subtraction(subtracted, carryPointer, *EK, thisinput1, thisinput2, *initialcarry);
//         
//     }
//     
//     return 1;
// }

//given two array of encrypted plaintexts, calculate 1 norm
int oneNorm(Encryptedvalue encryptedvector[], Encryptedvalue encryptedvector2[]){
    
    LWE::CipherText* result[numBits];
    
    //initialize result to zeros (encrypted)
    for (int i=0; i< numBits; i++){
        LWE::CipherText* encryptedzero;
        encryptedzero = new LWE::CipherText;
        LWE::Encrypt(encryptedzero, *SK, 0);
        result[i] = encryptedzero;
    }
    
//     //constant
//     LWE::CipherText* constant[numBits];
//     encryptInt(constant, 25);
//     
    
    for(int i=0; i<vectorlength; i++){
        Encryptedvalue thisinput1 = encryptedvector[i];
        Encryptedvalue thisinput2 = encryptedvector2[i];
        
        LWE::CipherText* initialcarry;
        
        initialcarry = new LWE::CipherText;
        
        int carryOutput = 0;
        int* carryPointer = &carryOutput;
        
        //initialize carry to 0
        LWE::Encrypt(initialcarry, *SK, 0);
        
        //4 byte integer
        LWE::CipherText* subtracted[numBits];
        
        LWE::CipherText* ct1[numBits];
        LWE::CipherText* ct2[numBits];
        
        for (int j=0; j< numBits; j++){
            ct1[j] = thisinput1.cts[j];
            ct2[j] = thisinput2.cts[j];
        }
        
        //LWE::CipherText* ct1[]
        subtraction(subtracted, carryPointer, *EK, ct1, ct2, *initialcarry);
        
//         //if negative, add predetermined value
//         if (isNegative(subtracted)){
//             addition(result, carryPointer, *EK, constant, result , *initialcarry);
//         }
//         else{
//             addition(result, carryPointer, *EK, subtracted, result , *initialcarry);
//         }
        
        //add absolute value
        absolute(*EK, subtracted);
        addition(result, carryPointer, *EK, subtracted, result , *initialcarry);
        
    }
    
    int distance = decryptInt(result);
    
    return distance;
    
    //return 1;
}


//int main(int argc, char *argv[]) {
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
     int *inputvector = (int*) mxGetPr(prhs[0]);
    int *inputvector2 = (int*) mxGetPr(prhs[1]);
    
//     cout << "*inputvector = " << *inputvector << "\n";
//     
    for (int i =0; i< vectorlength ; i++){
//         cout << "*inputvector = " << *inputvector << "\n";
       // cout << "inputvector[i] = " << inputvector[i] << "\n";
        fingerprintvector[i] = inputvector[i];
        
        fingerprintvector2[i] = inputvector2[i];
    }
    
    
  char *EKfilename = "ev.key";
  char *SKfilename = "sec.key";
  
  //int input1 = atoi(argv[1]);
  //int input2 = atoi(argv[2]);
  
  FHEW::Setup();

  SK = LoadSecretKey(SKfilename);
  EK = LoadEvalKey(EKfilename);

//   LWE::CipherText *ct1,*ct2,*ct3, *ct4, *carry, *initialcarry, 
//            *sumResult, 
//           *carryResult, *initialbin, *difference, *bout;
// 
//   
//   
//   ct1 = new LWE::CipherText;
//   ct2 = new LWE::CipherText;
//   ct3 = new LWE::CipherText;
//   ct4 = new LWE::CipherText;
//   carry = new LWE::CipherText;
//   initialcarry = new LWE::CipherText;
//   initialbin = new LWE::CipherText;
//   difference = new LWE::CipherText;
//   bout = new LWE::CipherText;
// 
//   carryResult = new LWE::CipherText;
// 
//   sumResult = new LWE::CipherText;
//   
//   //initialize input
//   LWE::Encrypt(ct1, *SK, 0);
//   LWE::Encrypt(ct2, *SK, 1);
//    LWE::Encrypt(ct3, *SK, 1);
//   LWE::Encrypt(ct4, *SK, 0);
//   LWE::Encrypt(initialbin, *SK, 0);
//   //LWE::Encrypt(carry, *SK, 1);
//   
//   //initialize carry to 0 
//   LWE::Encrypt(initialcarry, *SK, 0);
//   
//   //
//   // test encryptint and decryptint, absolute
//        //integer
//   LWE::CipherText* encrypt[numBits];
//   //LWE::CipherText* result[numBits];
//    encryptInt(encrypt, input1);
//    absolute(*EK, encrypt);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
//   int decrypted = decryptInt(encrypt);
//   cout << "abs value = " << decrypted << "\n";
// //
//   //
  
  
  
  //
  //test subtractPlaintext
  //subtractPlaintext(input1, input2);
  //
  //
  
  
  
  //
  //test encryptPlaintexts
  
//   for (int i=0; i<vectorlength; i++){
//   cout << "fingerprintvector[i] = "<<fingerprintvector[i] <<"\n";
//             cout<<"fingerprintvector2[i] = " << fingerprintvector2[i] << "\n";
//   }
  
  //test oneNorm
  //LWE::CipherText** encryptedvector[vectorlength];
  
  Encryptedvalue encryptedvector[vectorlength];
  
 
  encryptPlaintexts(encryptedvector, fingerprintvector);
  
  
  //LWE::CipherText** encryptedvector2[vectorlength];
  
  Encryptedvalue encryptedvector2[vectorlength];
  
  encryptPlaintexts(encryptedvector2, fingerprintvector2);
  
  clock_t begin = clock();
  
  int distance = oneNorm(encryptedvector, encryptedvector2);
 
  clock_t end = clock();
  double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;
  cout << "elapsed sec to calculate norm = " << elapsed_secs << "\n";
  
  cout << "1 norm distance = " << distance <<"\n";
  
  plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
  int *answer = (int*) mxGetData(plhs[0]);
    //int *a = (int*) mxGetPr(plhs[0]);
  *answer = distance;
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

