#include "mex.h"
#include "BIPL_LIB_SDNN.h"
#pragma comment(lib,"BIPL.SDNN.lib")

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void sdnnTrain(const std::string &setting_filename,mwSize input_number, double *train_sample, double *train_target, mwSize train_n, const std::string &save_filename)
{
    bipl::sdnn::SDNN sdnn;
    sdnn.InitSDNN(setting_filename);   
    sdnn.Train4Matlab(train_sample,train_target,train_n);
    sdnn.Save(save_filename);
}

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    if(nrhs!=4) 
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:nrhs",
                      "Four inputs required.");
    }

    if(nlhs!=0)
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:nlhs",
                      "No output required.");
    }    
    
    if ( mxIsChar(prhs[0]) != 1)
    {
      mexErrMsgIdAndTxt( "bipl:sdnn:SDNN:sdnnTrainAndEstimate:inputNotString",
              "Input must be a string.");
    }
        
    if( !mxIsDouble(prhs[1]) || 
        mxIsComplex(prhs[1]))
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:sdnnTrainAndEstimate:notDouble",
            "Input matrix must be type double.");
    }
    
    if( !mxIsDouble(prhs[2]) || 
        mxIsComplex(prhs[2]))
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:sdnnTrainAndEstimate:notDouble",
            "Input matrix must be type double.");
    }
    
    if ( mxIsChar(prhs[3]) != 1)
    {
      mexErrMsgIdAndTxt( "bipl:sdnn:SDNN:sdnnTrainAndEstimate:inputNotString",
              "Input must be a string.");
    }
    
    if(mxGetM(prhs[1]) < 2)
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:sdnnTrainAndEstimate:input_number < 2",
            "Input number must be 2 and more.");       
    }
    
    if(mxGetN(prhs[1])!=mxGetN(prhs[2]))
    {
        mexErrMsgIdAndTxt("bipl:sdnn:SDNN:sdnnTrainAndEstimate:sample_number_collumped",
                      "input_sample_number != target_sample_number.");
    }
    
    double *train_sample;
    train_sample = mxGetPr(prhs[1]);
    double *train_target;
    train_target = mxGetPr(prhs[2]);
        
    mwSize train_n = (mwSize)mxGetN(prhs[1]);
   
    char *setting_filename = mxArrayToString(prhs[0]);   
    char *save_filename = mxArrayToString(prhs[3]);

    std::ifstream test_file(setting_filename);
    if(test_file.is_open() != 1)
    {
         mxFree(setting_filename);
         mexErrMsgIdAndTxt( "bipl:sdnn:SDNN:sdnnTrainAndEstimate:is_not_open",
              "Could not open setting file");
    }
    test_file.close();
    
    const std::string buffer(bipl::sdnn::CheckParameterFile(setting_filename));
    if(buffer != "OK")
    {
         mexErrMsgIdAndTxt( "bipl:sdnn:SDNN:sdnnTrainAndEstimate:incorrect_parameter",
              buffer.c_str());
    }
    
    sdnnTrain(setting_filename,mxGetM(prhs[1]),train_sample,train_target,train_n,save_filename);   
    mxFree(setting_filename);
    mxFree(save_filename);
}