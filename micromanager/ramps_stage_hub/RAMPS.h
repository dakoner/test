///////////////////////////////////////////////////////////////////////////////
// FILE:          RAMPS.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Device adapter that turns a RAMPS stepper controller with
//                3-axis stage into a Micro-Manager stage
//                
// AUTHOR:        David Konerding, dakoner@gmail.com, 06/01/2015
//
// COPYRIGHT:     Google Inc., 2015
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.

#ifndef _RAMPS_H_
#define _RAMPS_H_

#include "DeviceBase.h"
#include "DeviceThreads.h"
#include <string>
#include <map>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//
// #define ERR_UNKNOWN_MODE         102
// #define ERR_UNKNOWN_POSITION     103
// #define ERR_IN_SEQUENCE          104
// #define ERR_SEQUENCE_INACTIVE    105
#define ERR_STAGE_MOVING         110

#define ERR_UNKNOWN_POSITION 101
#define ERR_INITIALIZE_FAILED 102
#define ERR_WRITE_FAILED 103
#define ERR_CLOSE_FAILED 104
#define ERR_BOARD_NOT_FOUND 105
#define ERR_PORT_OPEN_FAILED 106
#define ERR_COMMUNICATION 107
#define ERR_NO_PORT_SET 108
#define ERR_VERSION_MISMATCH 109


////////////////////////
// RAMPSHub
//////////////////////

class RAMPSHub : public HubBase<RAMPSHub>
{
 public:
  RAMPSHub();
  ~RAMPSHub();

  // Device API
  // ---------
  int Initialize();
  int Shutdown();
  void GetName(char* pName) const; 
  bool Busy();

  // property handlers
  int OnVersion(MM::PropertyBase* pProp, MM::ActionType pAct);
  int OnPort(MM::PropertyBase* pPropt, MM::ActionType eAct);
  int OnCommand(MM::PropertyBase* pProp, MM::ActionType pAct);
  int OnSettleTime(MM::PropertyBase* pProp, MM::ActionType eAct);
  int SetVelocity(double velocity);
  int SetAcceleration(double acceleration);
  int OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct);
  int OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct);

  // HUB api
  int DetectInstalledDevices();

  int SendCommand(std::string command, std::string terminator="\r");
  int ReadResponse(std::string& returnString, float timeout=300.);
  int SetAnswerTimeoutMs(double timout);
  MM::DeviceDetectionStatus DetectDevice(void);
  int PurgeComPortH();
  int WriteToComPortH(const unsigned char* command, unsigned len);
  int ReadFromComPortH(unsigned char* answer, unsigned maxLen, unsigned long& bytesRead);
  int SetCommandComPortH(const char* command, const char* term);
  int GetSerialAnswerComPortH (std::string& ans,  const char* term);
  int GetStatus();
  int SetTargetXY(double x, double y);
  int SetTargetZ(double z);
  int GetXYPosition(double *x, double *y);
  std::string GetState();
  int GetControllerVersion(std::string& version);

 private:
  void GetPeripheralInventory();
  std::vector<std::string> peripherals_;
  bool initialized_;
  bool busy_;
  std::string version_;
  MMThreadLock lock_;
  MMThreadLock executeLock_;
  std::string port_;
  bool portAvailable_;
  std::string commandResult_;
  double MPos[3];
  double WPos[3];
  double target_x_, target_y_, target_z_;
  std::string status_;
  MM::TimeoutMs* timeOutTimer_;
  long settle_time_;
  double velocity_;
  double acceleration_;
};


#endif //_RAMPS_H_
