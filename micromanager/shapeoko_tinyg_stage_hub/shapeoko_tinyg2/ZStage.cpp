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
#include "ShapeokoTinyG.h"

#include "ZStage.h"
using namespace std;

#include "MMDevice.h"
#include "DeviceBase.h"

extern const char* g_ZStageDeviceName;
extern const char* g_Keyword_LoadSample;
const char* g_ZVelocityProp = "Velocity";
const char* g_ZAccelerationProp = "Acceleration";

CShapeokoTinyGZStage::CShapeokoTinyGZStage() :
    // http://www.shapeoko.com/wiki/index.php/Zaxis_ACME
    stepSize_um_ (5.),
    posZ_um_(0.0),
	velocity_(1000),
    acceleration_(1000),
	timeOutTimer_(0),
    initialized_ (false)
{
  InitializeDefaultErrorMessages();

  SetErrorText(ERR_SCOPE_NOT_ACTIVE, "Zeiss Scope is not initialized.  It is needed for the Focus drive to work");
  SetErrorText(ERR_NO_FOCUS_DRIVE, "No focus drive found in this microscopes");
}

CShapeokoTinyGZStage::~CShapeokoTinyGZStage()
{
  Shutdown();
}

void CShapeokoTinyGZStage::GetName(char* Name) const
{
  CDeviceUtils::CopyLimitedString(Name, g_ZStageDeviceName);
}

int CShapeokoTinyGZStage::Initialize()
{
  
  InitializeDefaultErrorMessages();

  // parent ID display
  CreateHubIDProperty();

    ShapeokoTinyGHub* pHub = static_cast<ShapeokoTinyGHub*>(GetParentHub());
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
  CPropertyAction* pAct = new CPropertyAction(this, &CShapeokoTinyGZStage::OnPosition);
  ret = CreateProperty(MM::g_Keyword_Position, "0", MM::Float,false, pAct);
  if (ret != DEVICE_OK)
    return ret;

  
  // Max Speed
  pAct = new CPropertyAction (this, &CShapeokoTinyGZStage::OnVelocity);
  CreateProperty(g_ZVelocityProp, CDeviceUtils::ConvertToString(velocity_), MM::Float, false, pAct);
  SetPropertyLimits(g_ZVelocityProp, 0.0, 10000000.0);

  // Acceleration
  pAct = new CPropertyAction (this, &CShapeokoTinyGZStage::OnAcceleration);
  CreateProperty(g_ZAccelerationProp, CDeviceUtils::ConvertToString(acceleration_), MM::Float, false, pAct);
  SetPropertyLimits(g_ZAccelerationProp, 0.0, 10000000);

  
  
  pHub->PurgeComPortH();
  pHub->SendCommand("G28.3 Z0");
    pHub->PurgeComPortH();
  SetVelocity(velocity_);
    pHub->PurgeComPortH();
  SetAcceleration(acceleration_);
    pHub->PurgeComPortH();

  // Update lower and upper limits.  These values are cached, so if they change during a session, the adapter will need to be re-initialized
  ret = UpdateStatus();
  if (ret != DEVICE_OK)
    return ret;

  initialized_ = true;

  return DEVICE_OK;
}

int CShapeokoTinyGZStage::Shutdown()
{
  initialized_ = false;

  return DEVICE_OK;
}

bool CShapeokoTinyGZStage::Busy()
{
	LogMessage("ZStage: Busy called");
  if (timeOutTimer_ == 0)
      return false;
  LogMessage("1");
   if (timeOutTimer_->expired(GetCurrentMMTime()))
   {
	    LogMessage("1.1");
	   timeOutTimer_ = 0;
      //delete(timeOutTimer_);
      
   } else {
	   LogMessage("1.2");
	   return true;
   }
  LogMessage("2");
  ShapeokoTinyGHub* pHub = static_cast<ShapeokoTinyGHub*>(GetParentHub());
  std::string status = pHub->GetState();
    LogMessage("3");
  LogMessage("Status is:");
  LogMessage(status);
  if (status == "Ready" || status == "Stop")
    return false;
  else
    return true;

}

int CShapeokoTinyGZStage::SetPositionUm(double pos)
{
  long steps = (long)(pos / stepSize_um_ + 0.5);
  int ret = SetPositionSteps(steps);
  if (ret != DEVICE_OK)
    return ret;

  return DEVICE_OK;
}

int CShapeokoTinyGZStage::GetPositionUm(double& pos)
{
  long steps;
  int ret = GetPositionSteps(steps);
  if (ret != DEVICE_OK)
    return ret;
  pos = steps * stepSize_um_;

  return DEVICE_OK;
}

double CShapeokoTinyGZStage::GetStepSize() const {return stepSize_um_;}

/*
 * Requests movement to new z postion from the controller.  This function does the actual communication
 */
int CShapeokoTinyGZStage::SetPositionSteps(long steps)
{
  LogMessage("ZStage: SetPositionSteps");
   if (timeOutTimer_ != 0)
     {
     if (!timeOutTimer_->expired(GetCurrentMMTime()))
     return ERR_STAGE_MOVING;
	 timeOutTimer_ = 0;
     //delete (timeOutTimer_);
     }
  
  posZ_um_ = steps * stepSize_um_;
   double newPosZ = steps * stepSize_um_;
  double difZ = newPosZ - posZ_um_;


  char buff[100];
  sprintf(buff, "G0 Z%f", posZ_um_/1000.);
  std::string buffAsStdStr = buff;
  ShapeokoTinyGHub* pHub = static_cast<ShapeokoTinyGHub*>(GetParentHub());
  int ret = pHub->SendCommand(buffAsStdStr);
  if (ret != DEVICE_OK)
    return ret;
  ret = pHub->ReadResponse(buffAsStdStr);
   if (ret != DEVICE_OK)
    return ret;

     
  long timeOut = (long) (difZ / velocity_);
  timeOutTimer_ = new MM::TimeoutMs(GetCurrentMMTime(),  timeOut);
  //ret = OnZStagePositionChanged(posZ_um_);
   

  // CDeviceUtils::SleepMs(100);
  return DEVICE_OK;
}

/*
 * Requests current z postion from the controller.  This function does the actual communication
 */
int CShapeokoTinyGZStage::GetPositionSteps(long& steps)
{
  steps = (long)(posZ_um_ / stepSize_um_);

  // TODO(dek): implement status to get Z position
  return DEVICE_OK;
}

int CShapeokoTinyGZStage::SetOrigin()
{
  // const char* cmd ="HPZP0" ;
  // int ret = g_hub.ExecuteCommand(*this, *GetCoreCallback(),  cmd);
  // if (ret != DEVICE_OK)
  //    return ret;

  // TODO(dek): run gcode to set origin to current location (G28.3 ?)

  return DEVICE_OK;
}

int CShapeokoTinyGZStage::GetLimits(double& lower, double& upper)
{
  lower = lowerLimit_;
  upper = upperLimit_;
  return DEVICE_OK;
}

int CShapeokoTinyGZStage::SetVelocity(double velocity) {
	ShapeokoTinyGHub* pHub = static_cast<ShapeokoTinyGHub*>(GetParentHub());
	
	std::string command = "$zvm=";
	std::string result;
	command += CDeviceUtils::ConvertToString(velocity);
	int ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;

	return ret;
}

int CShapeokoTinyGZStage::SetAcceleration(double acceleration) {
	ShapeokoTinyGHub* pHub = static_cast<ShapeokoTinyGHub*>(GetParentHub());
	
	std::string command = "$zjm=";
	std::string result;
	command += CDeviceUtils::ConvertToString(acceleration);
	int ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;


	return ret;
}


bool CShapeokoTinyGZStage::IsContinuousFocusDrive() const {return false;}

// TODO(dek): implement GetUpperLimit and GetLowerLimit

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////
/*
 * Uses the Get and Set PositionUm functions to communicate with controller
 */
int CShapeokoTinyGZStage::OnPosition(MM::PropertyBase* pProp, MM::ActionType eAct)
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
int CShapeokoTinyGZStage::IsStageSequenceable(bool& isSequenceable) const {isSequenceable = true; return DEVICE_OK;}
int CShapeokoTinyGZStage::GetStageSequenceMaxLength(long& nrEvents) const  {nrEvents = 0; return DEVICE_OK;}
int CShapeokoTinyGZStage::StartStageSequence() {return DEVICE_OK;}
int CShapeokoTinyGZStage::StopStageSequence() {return DEVICE_OK;}
int CShapeokoTinyGZStage::ClearStageSequence() {return DEVICE_OK;}
int CShapeokoTinyGZStage::AddToStageSequence(double /*position*/) {return DEVICE_OK;}
int CShapeokoTinyGZStage::SendStageSequence() {return DEVICE_OK;}


// TODO(dek): these should send actual commands to update the device
int CShapeokoTinyGZStage::OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("TinyG XYStage OnVelocity");
  if (eAct == MM::BeforeGet)
  {
        

    pProp->Set(velocity_);
  }
  else if (eAct == MM::AfterSet)
  {
    if (initialized_)
    {
      double velocity;
      pProp->Get(velocity);
      velocity_ = velocity;
	  SetVelocity(velocity);
    }

  }

   
  LogMessage("Set velocity");

  return DEVICE_OK;
}

int CShapeokoTinyGZStage::OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("TinyG XYStage OnAcceleration");
  if (eAct == MM::BeforeGet)
  {
        

    pProp->Set(acceleration_);
  }
  else if (eAct == MM::AfterSet)
  {
    if (initialized_)
    {
      double acceleration;
      pProp->Get(acceleration);
      acceleration_ = acceleration;
	  SetAcceleration(acceleration);
    }

  }

   
  LogMessage("Set acceleration");

  return DEVICE_OK;
}