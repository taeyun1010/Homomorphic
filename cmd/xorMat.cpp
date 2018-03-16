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
    LWE::CipherText* nandresult;
    
    LWE::CipherText* orresult;
    
    LWE::CipherText* finalresult;
    
    nandresult = new LWE::CipherText;
    orresult = new LWE::CipherText;
    finalresult = new LWE::CipherText;
    
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
    
    
    //calculate xor using nand, and, or
    FHEW::Setup();
    
    BinGate gate=NAND;
    
    EK = LoadEvalKey(EKfilename);
    
    for (int i =0; i< n ; i++){
        ct1.a[i] = a[i];
    }
    
    ct1.b = b;
    
    for (int i =0; i< n ; i++){
        ct2.a[i] = a2[i];
    }
    
    ct2.b = b2;
 
    FHEW::HomGate(nandresult, gate, *EK,ct1,ct2);
    gate = OR;
    FHEW::HomGate(orresult, gate, *EK, ct1,ct2);
    gate = AND;
    FHEW::HomGate(finalresult,gate, *EK, *nandresult, *orresult);
    LWE::CipherText output;
    
    output = *finalresult;
    
    plhs[0] = mxCreateNumericMatrix(1, n, mxINT32_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *outputb = (int*) mxGetData(plhs[1]);
    int *outputa = (int*) mxGetData(plhs[0]);
    
    *outputb = output.b;
    
    
    //n =500
    for (int i=0; i< n;i++){
        outputa[i] = output.a[i];
        
    }
    
}