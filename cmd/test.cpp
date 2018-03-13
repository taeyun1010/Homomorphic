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


// void help(char* cmd) {
//   cerr << "\nusage: " << cmd << "SecretKeyFileName EvalKeyFileName Input1 Input2 OutCTFileName  \n\n" 
//        << "  Perform Homomorphic Addition\n\n";
//   exit(0);
// }


//int main(int argc, char *argv[]) {
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
//void mexFunction(int argc, char *argv[]){
    
    if (nrhs != 3){
        cout << "requires 3 parameters: EvalKeyFileName EncryptedInput1 EncryptedInput2\n";
        return;
    }
    
    char* ek_fn;
    char* ict1;
    char* ict2;
    
    //double* A = 0;
    char** A;
    size_t m =0;
    size_t n = 0;
    int u =0;
    int v =0;
    A = (char**)mxGetData(prhs[0]);
    m = mxGetM(prhs[0]);
    n = mxGetN(prhs[0]);
    
    for (u=0; u<m; ++u)
    {
        for (v=0;v<n;++v){
            mexPrintf("%f ", A[u+v*m]);
            ek_fn = A[u+v*m];
        }
        mexPrintf("\n");
    }
    
    
    A = 0;
    m =0;
    n = 0;
    u =0;
    v =0;
    A = (char**)mxGetPr(prhs[1]);
    m = mxGetM(prhs[1]);
    n = mxGetN(prhs[1]);
    
    for (u=0; u<m; ++u)
    {
        for (v=0;v<n;++v){
            mexPrintf("%f ", A[u+v*m]);
            ict1 = A[u+v*m];
        }
        mexPrintf("\n");
    }
    
     
    A = 0;
    m =0;
    n = 0;
    u =0;
    v =0;
    A = (char**)mxGetPr(prhs[2]);
    m = mxGetM(prhs[2]);
    n = mxGetN(prhs[2]);
    
    for (u=0; u<m; ++u)
    {
        for (v=0;v<n;++v){
            mexPrintf("%f ", A[u+v*m]);
            ict2 = A[u+v*m];
        }
        mexPrintf("\n");
    }
    //cout << *prhs[0] << "\n";
    cout << "nrhs = " << nrhs << "\n";
    cout << "ek_fn = " << *ek_fn << "\n";
    cout << "ict1 = " << *ict1 << "\n";
    cout << "ict2 = " << *ict2 << "\n";
    
    int plaintext1 = 1;
    int plaintext2 = 1;
    char* sk_fn = "sec.key";
    char* ct_fn1 = "a.ct";
    char* ct_fn2 = "b.ct";
   
    LWE::SecretKey* SK = LoadSecretKey(sk_fn);
    
    LWE::CipherText ct;
    LWE::Encrypt(&ct, *SK, plaintext1);  SaveCipherText(&ct,ct_fn1);
    LWE::Encrypt(&ct, *SK, plaintext2);  SaveCipherText(&ct,ct_fn2);
    
    ek_fn = "ev.key";
    
    char* ict1_fn = "a.ct";
    char* ict2_fn = "b.ct";
    char* oct_fn = "d.ct";
    
    FHEW::Setup();
    
    EK = LoadEvalKey(ek_fn);
    
    LWE::CipherText *ct1,*ct2,*ct3;
    
    BinGate gate = AND;
    
    ct1 = LoadCipherText(ict1_fn);
    ct2 = LoadCipherText(ict2_fn);
    ct3 = new LWE::CipherText;
    
    
    FHEW::HomGate(ct3, gate, *EK,*ct1,*ct2);
    
    SaveCipherText(ct3,oct_fn);
    
    
    char* ct_fn = "d.ct";
    
    
    LWE::CipherText* ct4 = LoadCipherText(ct_fn);
    int z = LWE::Decrypt(*SK,*ct2);
    cout << z << "\n";
}

