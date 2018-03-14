#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <mex.h>

using namespace std;

// encrypt given plaintext, using sec.key previously generated
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    if (nrhs != 1){
        cout << "requires 1 parameter: plaintext\n";
        return;
    }
    //evaluation key file name
    char *SKfilename = "sec.key";
    int plaintext;

    plaintext = mxGetScalar(prhs[0]);
    cout << "plaintext = " << plaintext << "\n";
//     if(plaintext == NULL) 
//       mexErrMsgIdAndTxt( "MATLAB:revord:conversionFailed",
//               "Could not convert input to string.");
    
    
    
    FHEW::Setup();
    
    //actual secret key
    LWE::SecretKey* SK = LoadSecretKey(SKfilename);
    
    LWE::CipherText ct;
    LWE::Encrypt(&ct, *SK, plaintext);  
//    
//     
//     mxFree(SKfilename);
//     
    plhs[0] = mxCreateNumericMatrix(1, n, mxINT32_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *b = (int*) mxGetData(plhs[1]);
    int *a = (int*) mxGetData(plhs[0]);
    //int *a = (int*) mxGetPr(plhs[0]);
    *b = ct.b;
    
    
    cout << "&ct = " << &ct << "\n";
    cout << "ct.b = " << ct.b << "\n";
    //n =500
    for (int i=0; i< n;i++){
        //cout << "ct.a[i] = " << ct.a[i] << "\n";
        a[i] = ct.a[i];
        //cout << "a[i] = " << a[i] << "\n";
        
    }
    
    //answer = &ct;
    
    int decrypted = LWE::Decrypt(*SK,ct);
    cout << "decrypted = "<< decrypted << "\n";
    
    //plhs[0] = answer;
    
    return;


}
