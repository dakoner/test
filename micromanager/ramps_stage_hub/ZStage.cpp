///////////////////////////////////////////////////////////////////////////////
// FILE:       ZStage.cpp
// PROJECT:    MicroManage
// SUBSYSTEM:  DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:
// Zeiss CAN bus controller for Axioscop 2 MOT, Z-stage
//


#ifdef WIN32
#include <windows.h>
#define snprintf _snprintf
#endif
#include "RAMPS.h"

#include "ZStage.h"
using namespace std;

#include "MMDevice.h"
#include "DeviceBase.h"

extern const char* g_ZStageDeviceName;
extern const char* g_Keyword_LoadSample;

CRAMPSZStage::CRAMPSZStage() :
    // http://www.shapeoko.com/wiki/index.php/Zaxis_ACME
    stepSize_um_ (5.),
    posZ_um_(0.0),
    initialized_ (false)
{
  InitializeDefaultErrorMessages();

  SetErrorText(ERR_SCOPE_NOT_ACTIVE, "Zeiss Scope is not initialized.  It is needed for the Focus drive to work");
  SetErrorText(ERR_NO_FOCUS_DRIVE, "No focus drive found in this microscopes");
}

CRAMPSZStage::~CRAMPSZStage()
{
  Shutdown();
}

void CRAMPSZStage::GetName(char* Name) const
{
  CDeviceUtils::CopyLimitedString(Name, g_ZStageDeviceName);
}

int CRAMPSZStage::Initialize()
{
  
  InitializeDefaultErrorMessages();

  // parent ID display
  CreateHubIDProperty();

    RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  // set property list
  // ----------------

  // Name
  int ret = CreateProperty(MM::g_Keyword_Name, g_ZStageDeviceName, MM::String, true);
  if (DEVICE_OK != ret)
    return ret;

  // Description
  ret = CreateProperty(MM::g_Keyword_Description, "Z-drive", MM::String, true);
  if (DEVICE_OK != ret)
    return ret;

  // Position
  CPropertyAction* pAct = new CPropertyAction(this, &CRAMPSZStage::OnPosition);
  ret = CreateProperty(MM::g_Keyword_Position, "0", MM::Float,false, pAct);
  if (ret != DEVICE_OK)
    return ret;

  // Update lower and upper limits.  These values are cached, so if they change during a session, the adapter will need to be re-initialized
  ret = UpdateStatus();
  if (ret != DEVICE_OK)
    return ret;

  initialized_ = true;

  return DEVICE_OK;
}

int CRAMPSZStage::Shutdown()
{
  initialized_ = false;

  return DEVICE_OK;
}

bool CRAMPSZStage::Busy()
{
	LogMessage("ZStage: Busy called");
  RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  std::string status = pHub->GetState();
  LogMessage("Status is:");
  LogMessage(status);
  if (status == "Idle")
    return false;
  else
    return true;

}

int CRAMPSZStage::SetPositionUm(double pos)
{
  long steps = (long)(pos / stepSize_um_ + 0.5);
  int ret = SetPositionSteps(steps);
  if (ret != DEVICE_OK)
    return ret;

  return DEVICE_OK;
}

int CRAMPSZStage::GetPositionUm(double& pos)
{
  long steps;
  int ret = GetPositionSteps(steps);
  if (ret != DEVICE_OK)
    return ret;
  pos = steps * stepSize_um_;

  return DEVICE_OK;
}

double CRAMPSZStage::GetStepSize() const {return stepSize_um_;}

/*
 * Requests movement to new z postion from the controller.  This function does the actual communication
 */
int CRAMPSZStage::SetPositionSteps(long steps)
{
  LogMessage("ZStage: SetPositionSteps");
  RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  std::string status = pHub->GetState();
  if (status == "Running") {
      return ERR_STAGE_MOVING;
  }
  posZ_um_ = steps * stepSize_um_;
  double newPosZ = steps * stepSize_um_;
  double difZ = newPosZ - posZ_um_;


  char buff[100];
  sprintf(buff, "G0 Z%f", posZ_um_/1000.);
  std::string buffAsStdStr = buff;
  int ret = pHub->SendCommand(buffAsStdStr);
  if (ret != DEVICE_OK)
    return ret;
  

  pHub->SetTargetZ(posZ_um_);

  return DEVICE_OK;
}

/*
 * Requests current z postion from the controller.  This function does the actual communication
 */
int CRAMPSZStage::GetPositionSteps(long& steps)
{
  steps = (long)(posZ_um_ / stepSize_um_);

  // TODO(dek): implement status to get Z position
  return DEVICE_OK;
}

int CRAMPSZStage::SetOrigin()
{
  // const char* cmd ="HPZP0" ;
  // int ret = g_hub.ExecuteCommand(*this, *GetCoreCallback(),  cmd);
  // if (ret != DEVICE_OK)
  //    return ret;

  // TODO(dek): run gcode to set origin to current location (G28.3 ?)

  return DEVICE_OK;
}

int CRAMPSZStage::GetLimits(double& lower, double& upper)
{
  lower = lowerLimit_;
  upper = upperLimit_;
  return DEVICE_OK;
}



bool CRAMPSZStage::IsContinuousFocusDrive() const {return false;}

// TODO(dek): implement GetUpperLimit and GetLowerLimit

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////
/*
 * Uses the Get and Set PositionUm functions to communicate with controller
 */
int CRAMPSZStage::OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct)
{
  if (eAct == MM::BeforeGet)
  {
    double pos;
    int ret = GetPositionUm(pos);
    if (ret != DEVICE_OK)
      return ret;
    pProp->Set(pos);
  }
  else if (eAct == MM::AfterSet)
  {
    double pos;
    pProp->Get(pos);
    int ret = SetPositionUm(pos);
    if (ret != DEVICE_OK)
      return ret;
  }

  return DEVICE_OK;
}


// TODO(dek): implement OnStageLoad

// Sequence functions (unimplemented)
int CRAMPSZStage::IsStageSequenceable(bool& isSequenceable) const {isSequenceable = true; return DEVICE_OK;}
int CRAMPSZStage::GetStageSequenceMaxLength(long& nrEvents) const  {nrEvents = 0; return DEVICE_OK;}
int CRAMPSZStage::StartStageSequence() {return DEVICE_OK;}
int CRAMPSZStage::StopStageSequence() {return DEVICE_OK;}
int CRAMPSZStage::ClearStageSequence() {return DEVICE_OK;}
int CRAMPSZStage::AddToStageSequence(double /*position*/) {return DEVICE_OK;}
int CRAMPSZStage::SendStageSequence() {return DEVICE_OK;}
