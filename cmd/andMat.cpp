#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <mex.h>

using namespace std;

FHEW::EvalKey* EK;


void help() {
  cerr << "\nusage: " << " InCTInput1's a InCTInput1's b InCTInput2's a InCTInput2's b  \n\n" 
       << "  Perform Homomorphic Gate computation where Gate={AND, OR, NAND, NOR, XOR}\n\n";
  exit(0);
}


//int main(int argc, char *argv[]) {
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if (nrhs != 4) help();
    //secret key file name
    char *EKfilename = "ev.key";
    int b, b2;
    //char *buf;
    //size_t buflen;
    //int status;
    
    //Input ciphertexts to perform gate operation on: ct1, ct2
    LWE::CipherText ct1, ct2;
    LWE::CipherText* ct3;
    
    ct3 = new LWE::CipherText;
    
    //these a and b are for ciphertext input 1
    int *a = (int*) mxGetPr(prhs[0]);
    b = mxGetScalar(prhs[1]);
    
    //these a and b are for ciphertext input 1
    int *a2 = (int*) mxGetPr(prhs[2]);
    b2 = mxGetScalar(prhs[3]);
    
    
//     /* Get number of characters in the input string.  Allocate enough
//        memory to hold the converted string. */
//     
//     buflen = mxGetN(prhs[0]) + 1;
//     buf = (char*) mxMalloc(buflen);
//     
//     /* Copy the string data into buf. */ 
//     status = mxGetString(prhs[0], buf, (mwSize) buflen);
// 
//     if (status != 0) {
//         mexErrMsgIdAndTxt( "MATLAB:mxmalloc:mxGetString", 
//                            "Failed to copy input string into allocated memory.");
//     }
// 
//     mexPrintf("The input gate is:  %s\n", buf);
    /* NOTE: You could add your own code here to manipulate 
       the string */
    
//     /* When finished using the string, deallocate it. */
//     mxFree(buf);
    
    BinGate gate=AND;
//     if (buf=="OR") gate = OR;
//     else if (buf=="AND") gate = AND;
//     else if (buf=="NOR") gate = NOR;
//     else if (buf=="NAND") gate = NAND;
//     else if (buf=="XOR") {
//         //TODO
//         return;
//     }
//     else
//     {
//         cerr << "\n This gate does not exists (please choose {AND, OR, NAND, NOR, XOR}) \n";
//         exit(1);
//     }
//     
    
    
    FHEW::Setup();
    
    EK = LoadEvalKey(EKfilename);
    
    for (int i =0; i< n ; i++){
        //cout << "a[i] = " << a[i] << "\n";
        ct1.a[i] = a[i];
    }
    
    ct1.b = b;
    //cout << "ct1.b = " << ct1.b << "\n";
    
    for (int i =0; i< n ; i++){
        //cout << "a[i] = " << a[i] << "\n";
        ct2.a[i] = a2[i];
    }
    
    ct2.b = b2;
    //cout << "ct2.b = " << ct2.b << "\n";
    
//     
//     LWE::CipherText ct3 = new LWE::CipherText;
    
    
    FHEW::HomGate(ct3, gate, *EK,ct1,ct2);
    
    LWE::CipherText output;
    
    output = *ct3;
    
    plhs[0] = mxCreateNumericMatrix(1, n, mxINT32_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *outputb = (int*) mxGetData(plhs[1]);
    int *outputa = (int*) mxGetData(plhs[0]);
    
    *outputb = output.b;
    
    
    //n =500
    for (int i=0; i< n;i++){
        //cout << "output.a[i] = " << output.a[i] << "\n";
        outputa[i] = output.a[i];
        //cout << "a[i] = " << a[i] << "\n";
        
    }
    
    //cout << "output.b = " << output.b << "\n";
}