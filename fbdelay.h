//--------------------------------------------------------------------------------------------
// 14/07/13
// Filename     : fbdelay.cpp
// Created by   : Michael Murphy
// Description  : Feedback delay line
//
//-------------------------------------------------------------------------------------------

#ifndef __again__
#define __again__

#include "public.sdk/source/vst2.x/audioeffectx.h"

//---------------------------------------------------------------------------------------------
class FBdelay : public AudioEffectX
{
public:
  FBdelay (audioMasterCallback audioMaster);
  ~FBdelay ();

  // Processing
  virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
  virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

  // Program
  virtual void setProgramName (char* name);
  virtual void getProgramName (char* name);

  // Parameters
  virtual void setParameter (VstInt32 index, float value);
  virtual float getParameter (VstInt32 index);
  virtual void getParameterLabel (VstInt32 index, char* label);
  virtual void getParameterDisplay (VstInt32 index, char* text);
  virtual void getParameterName (VstInt32 index, char* text);

  virtual bool getEffectName (char* name);
  virtual bool getVendorString (char* text);
  virtual bool getProductString (char* text);
  virtual VstInt32 getVendorVersion ();

protected:
  // std::ofstream dbg;
  double *dlybuf;
  double lsamp;
  double gain, dly, fb;
  int maxbufsize;
  float maxdlyms;
  float i0, i1, i2;
  int wpos, rpos;
  
  char programName[kVstMaxProgNameLen + 1];
};

#endif
