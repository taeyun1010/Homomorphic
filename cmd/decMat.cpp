#include <iostream>
#include <cstdlib>
#include "../FHEW.h"
#include "common.h"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <mex.h>

using namespace std;

// decrypt given plaintext (0 or 1), using sec.key previously generated
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    
    if (nrhs != 2){
        cout << "requires 2 parameters: a b\n";
        return;
    }
    //secret key file name
    char *SKfilename = "sec.key";
    int b;

    int plaintext;
    LWE::CipherText ct; 
    
    int *a = (int*) mxGetPr(prhs[0]);
    b = mxGetScalar(prhs[1]);
    
    for (int i =0; i< n ; i++){
        //cout << "a[i] = " << a[i] << "\n";
        ct.a[i] = a[i];
    }
   // cout << "b = " << b << "\n";
    
//     if(plaintext == NULL) 
//       mexErrMsgIdAndTxt( "MATLAB:revord:conversionFailed",
//               "Could not convert input to string.");
    
    
    
    FHEW::Setup();
    
    //actual secret key
    LWE::SecretKey* SK = LoadSecretKey(SKfilename);
    

    
    ct.b = b;
//    
//     
//     mxFree(SKfilename);
//     
    
    int decrypted = LWE::Decrypt(*SK,ct);
    plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *answer = (int*) mxGetData(plhs[0]);
    //int *a = (int*) mxGetPr(plhs[0]);
    *answer = decrypted;
    
    
    
    //answer = &ct;
    
    
    //cout << "decrypted = "<< decrypted << "\n";
    
    //plhs[0] = answer;
    
    return;


}
