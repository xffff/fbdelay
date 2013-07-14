//--------------------------------------------------------------------------------------------
// 14/07/13
// Filename     : fbdelay.cpp
// Created by   : Michael Murphy
// Description  : Feedback delay line
//
//-------------------------------------------------------------------------------------------

#define DEBUG
#include <cmath>
#include "fbdelay.h"

//-------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
  return new FBdelay (audioMaster);
}

//-------------------------------------------------------------------------------------------
FBdelay::FBdelay (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, 1, 3)	
{
  setNumInputs (1);		// stereo in
  setNumOutputs (1);		// stereo out
  setUniqueID ('delay');	// identify
  canProcessReplacing ();	// supports replacing output
  canDoubleReplacing ();	// supports double precision processing

  i0		= 1;    // gain
  i1		= 0.5f; // delay
  i2		= 0.7f; // fb
  maxdlyms      = 10000;
  maxbufsize	= maxdlyms * getSampleRate() * 0.001;
  rpos		= 0;
  wpos		= 0;
  dlybuf        = new double[maxbufsize];
  lsamp         = 0;
  memset(dlybuf, 0, maxbufsize * sizeof(double));
  
  vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//--------------------------------------------------------------------------------------------
FBdelay::~FBdelay ()
{
  if(dlybuf)
    { delete [] dlybuf; }
}

//-------------------------------------------------------------------------------------------
void FBdelay::setProgramName (char* name)
{
  vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void FBdelay::getProgramName (char* name)
{
  vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void FBdelay::setParameter (VstInt32 index, float value)
{
  int temp;
  
  switch(index) {
  case 0: i0 = value; break;
  case 1: i1 = value; break;
  case 2: i2 = value; break;
  }
  
  gain	= i0;
  dly	= (int)(i1 * maxdlyms);
  fb	= 0.9f * i2;

  temp  = (int)(wpos - dly * getSampleRate() * 0.001);
  if(rpos != temp){
    rpos = temp;
    if(rpos < 0)
      { rpos += maxbufsize; }
  }
}

//-----------------------------------------------------------------------------------------
float FBdelay::getParameter (VstInt32 index)
{
  switch(index) {
  case 0: return i0; break;
  case 1: return i1; break;
  case 2: return i2; break;
  }
}

//-----------------------------------------------------------------------------------------
void FBdelay::getParameterName (VstInt32 index, char* label)
{
  switch(index) {
  case 0: vst_strncpy (label, "Gain", kVstMaxParamStrLen); break;
  case 1: vst_strncpy (label, "Delay", kVstMaxParamStrLen); break;
  case 2: vst_strncpy (label, "Feedback", kVstMaxParamStrLen); break;
  }
}

//-----------------------------------------------------------------------------------------
void FBdelay::getParameterDisplay (VstInt32 index, char* text)
{
  switch(index) {
  case 0: dB2string (gain, text, kVstMaxParamStrLen); break;
  case 1: int2string (dly, text, kVstMaxParamStrLen); break;
  case 2: int2string (99 * i2, text, kVstMaxParamStrLen); break;
  }
}

//-----------------------------------------------------------------------------------------
void FBdelay::getParameterLabel (VstInt32 index, char* label)
{
  switch(index) {
  case 0: vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
  case 1: vst_strncpy (label, "ms", kVstMaxParamStrLen); break;
  case 2: vst_strncpy (label, "%", kVstMaxParamStrLen); break;
  }
}

//------------------------------------------------------------------------
bool FBdelay::getEffectName (char* name)
{
  vst_strncpy (name, "delay", kVstMaxEffectNameLen);
  return true;
}

//------------------------------------------------------------------------
bool FBdelay::getProductString (char* text)
{
  vst_strncpy (text, "delay", kVstMaxProductStrLen);
  return true;
}

//------------------------------------------------------------------------
bool FBdelay::getVendorString (char* text)
{
  vst_strncpy (text, "xffff", kVstMaxVendorStrLen);
  return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 FBdelay::getVendorVersion ()
{ 
  return 1000; 
}

//-----------------------------------------------------------------------------------------
void FBdelay::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
  float* in  =  inputs[0];
  float* out = outputs[0];
  float s;
  
  for(int i=0; i<sampleFrames; i++) {
    dlybuf[wpos++] = in[i] + lsamp;
    if(wpos == maxbufsize)
      { wpos = 0; }    
    s = dlybuf[rpos];
    lsamp = s * fb;
    rpos++;
    if(rpos >= maxbufsize)
      { rpos -= maxbufsize; }
    out[i] = s * gain;
  }
}

//-----------------------------------------------------------------------------------------
void FBdelay::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
  double* in  =  inputs[0];
  double* out = outputs[0];
  double s;
  
  for(int i=0; i<sampleFrames; i++) {
    dlybuf[wpos++] = in[i] + lsamp;
    if(wpos == maxbufsize)
      { wpos = 0; }    
    s = dlybuf[rpos];
    lsamp = s * fb;
    rpos++;
    if(rpos >= maxbufsize)
      { rpos -= maxbufsize; }
    out[i] = s * gain;
  }
}
