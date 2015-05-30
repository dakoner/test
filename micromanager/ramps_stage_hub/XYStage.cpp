#include "DeviceUtils.h"

#include "RAMPS.h"
#include "XYStage.h"
const char* g_StepSizeProp = "Step Size";
const char* g_XYVelocityProp = "Maximum Velocity";
const char* g_XYAccelerationProp = "Acceleration";
const char* g_XYSettleTimeProp = "Settle Time";

///////////////////////////////////////////////////////////////////////////////
// CRAMPSXYStage implementation
// ~~~~~~~~~~~~~~~~~~~~~~~~~

CRAMPSXYStage::CRAMPSXYStage() :
    CXYStageBase<CRAMPSXYStage>(),
    stepSize_um_(0.025),
    velocity_(10000),
    acceleration_(10000),
    posX_um_(0.0),
    posY_um_(0.0),
    busy_(false),
    timeOutTimer_(0),
    initialized_(false),
    lowerLimit_(0.0),
    upperLimit_(20000.0),
	status_(""),
	is_moving_(false),
	settle_time_(250)
{
  InitializeDefaultErrorMessages();

  // parent ID display
  CreateHubIDProperty();
}

CRAMPSXYStage::~CRAMPSXYStage()
{
  Shutdown();
}

extern const char* g_XYStageDeviceName;
const char* NoHubError = "Parent Hub not defined.";



void CRAMPSXYStage::GetName(char* Name) const
{
  CDeviceUtils::CopyLimitedString(Name, g_XYStageDeviceName);
}

int CRAMPSXYStage::Initialize()
{
  LogMessage("XYStage: initialize");
  RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  if (pHub)
  {
    char hubLabel[MM::MaxStrLength];
    pHub->GetLabel(hubLabel);
    SetParentID(hubLabel); // for backward comp.
  }
  else
    LogMessage(NoHubError);

  if (initialized_)
    return DEVICE_OK;

  // set property list
  // -----------------

  // Name
  int ret = CreateStringProperty(MM::g_Keyword_Name, g_XYStageDeviceName, true);
  if (DEVICE_OK != ret)
    return ret;

  // Description
  ret = CreateStringProperty(MM::g_Keyword_Description, "RAMPS XY stage driver", true);
  if (DEVICE_OK != ret)
    return ret;

  CPropertyAction* pAct = new CPropertyAction (this, &CRAMPSXYStage::OnStepSize);
  CreateProperty(g_StepSizeProp, CDeviceUtils::ConvertToString(stepSize_um_), MM::Float, false, pAct);

  // Max Speed
  pAct = new CPropertyAction (this, &CRAMPSXYStage::OnVelocity);
  CreateProperty(g_XYVelocityProp, CDeviceUtils::ConvertToString(velocity_), MM::Float, false, pAct);
  SetPropertyLimits(g_XYVelocityProp, 0.0, 10000000.0);

  // Acceleration
  pAct = new CPropertyAction (this, &CRAMPSXYStage::OnAcceleration);
  CreateProperty(g_XYAccelerationProp, CDeviceUtils::ConvertToString(acceleration_), MM::Float, false, pAct);
  SetPropertyLimits(g_XYAccelerationProp, 0.0, 1000000000);
  
  pAct = new CPropertyAction (this, &CRAMPSXYStage::OnSettleTime);
  CreateProperty(g_XYSettleTimeProp, CDeviceUtils::ConvertToString(settle_time_), MM::Integer, false, pAct);
  SetPropertyLimits(g_XYSettleTimeProp, 0, 5000);
  
  pHub->PurgeComPortH();
  pHub->SendCommand("G28.3 X0 Y0");
    pHub->PurgeComPortH();
  SetVelocity(velocity_);
    pHub->PurgeComPortH();
  SetAcceleration(acceleration_);
    pHub->PurgeComPortH();
  ret = UpdateStatus();
  if (ret != DEVICE_OK)
    return ret;

  initialized_ = true;

  return DEVICE_OK;
}

int CRAMPSXYStage::Shutdown()
{
  if (initialized_)
  {
    initialized_ = false;
  }
  return DEVICE_OK;
}

bool CRAMPSXYStage::Busy()
{
  LogMessage("XYStage: Busy called");
  
  RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  std::string status = pHub->GetState();

  LogMessage("Status is:");
  LogMessage(status);
  std::string oldstatus = status_;
  status_ = status;

  if (status_ == "Idle") {
	  LogMessage("idle, return false.");
    return false;

  }
  else if (status_ == "Running") {
	  LogMessage("Running, return true.");
	  return true;
  }
  else if (is_moving_) {
	  if (timeOutTimer_ == 0) {
          LogMessage("Stage transitioned from moving to stopped.");
	      LogMessage("Enabling post-stop timer.");
	      timeOutTimer_ = new MM::TimeoutMs(GetCurrentMMTime(),  settle_time_);
		  return true;
	  } else if (timeOutTimer_->expired(GetCurrentMMTime())) {
         LogMessage("Timer expired. return false.");
         delete(timeOutTimer_);
         timeOutTimer_ = 0;
	     is_moving_ = false;
         return false;
      } else {
         LogMessage("Timer has not expired.  Return true");
         return true;
      }
  }
  else {
	  LogMessage("Unexpected status.");
	  return false;
  }
}

double CRAMPSXYStage::GetStepSize() {return stepSize_um_;}

int CRAMPSXYStage::SetPositionSteps(long x, long y)
{
  LogMessage("XYStage: SetPositionSteps");
  if (is_moving_) {
      if (!timeOutTimer_->expired(GetCurrentMMTime()))
         return ERR_STAGE_MOVING;
   }
  double newPosX = x * stepSize_um_;
  double newPosY = y * stepSize_um_;
  double difX = newPosX - posX_um_;
  double difY = newPosY - posY_um_;
  double distance = sqrt( (difX * difX) + (difY * difY) );
     
  
  posX_um_ = x * stepSize_um_;
  posY_um_ = y * stepSize_um_;

  // TODO(dek): if no position change, don't send new position.
  char buff[100];
  sprintf(buff, "G0 X%f Y%f", posX_um_/1000., posY_um_/1000.);
  std::string buffAsStdStr = buff;
  RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
  int ret = pHub->SendCommand(buffAsStdStr);
  if (ret != DEVICE_OK)
    return ret;

  is_moving_ = true;
  
  ret = OnXYStagePositionChanged(posX_um_, posY_um_);
  if (ret != DEVICE_OK)
    return ret;

  return DEVICE_OK;
}

int CRAMPSXYStage::GetPositionSteps(long& x, long& y)
{
  LogMessage("XYStage: GetPositionSteps");
  x = (long)(posX_um_ / stepSize_um_);
  y = (long)(posY_um_ / stepSize_um_);
  return DEVICE_OK;
}

int CRAMPSXYStage::SetRelativePositionSteps(long x, long y)
{
  LogMessage("XYStage: SetRelativePositioNSteps");
  long xSteps, ySteps;
  GetPositionSteps(xSteps, ySteps);

  return this->SetPositionSteps(xSteps+x, ySteps+y);
}

int CRAMPSXYStage::Home() {   LogMessage("RAMPS XYStage home.");
return DEVICE_OK; }
int CRAMPSXYStage::Stop() { LogMessage("RAMPS XYStage stop.");
return DEVICE_OK; }

int CRAMPSXYStage::SetOrigin() { LogMessage("RAMPS XYStage set origin."); return DEVICE_OK; }

int CRAMPSXYStage::GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax)
{
  LogMessage("RAMPS XYStage get limits um");
  xMin = lowerLimit_; xMax = upperLimit_;
  yMin = lowerLimit_; yMax = upperLimit_;
  return DEVICE_OK;
}

int CRAMPSXYStage::GetStepLimits(long& /*xMin*/, long& /*xMax*/, long& /*yMin*/, long& /*yMax*/)
{   LogMessage("RAMPS XYStage get limits um");
return DEVICE_UNSUPPORTED_COMMAND; }

double CRAMPSXYStage::GetStepSizeXUm() {   LogMessage("RAMPS XYStage get step size x um");
return stepSize_um_; }
double CRAMPSXYStage::GetStepSizeYUm() {   LogMessage("RAMPS XYStage get step size y um");
return stepSize_um_; }
int CRAMPSXYStage::Move(double /*vx*/, double /*vy*/) {LogMessage("RAMPS XYStage move"); return DEVICE_OK;}

int CRAMPSXYStage::IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = true; return DEVICE_OK;}


int CRAMPSXYStage::SetVelocity(double velocity) {
	RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
	
	std::string command = "$xvm=";
	std::string result;
	command += CDeviceUtils::ConvertToString(velocity);
	int ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;

	command = "$yvm=";
	command += CDeviceUtils::ConvertToString(velocity);
	ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;

	return ret;
}

int CRAMPSXYStage::SetAcceleration(double acceleration) {
	RAMPSHub* pHub = static_cast<RAMPSHub*>(GetParentHub());
	
	std::string command = "$xjm=";
	std::string result;
	command += CDeviceUtils::ConvertToString(acceleration);
	int ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;

	command = "$yjm=";
	command += CDeviceUtils::ConvertToString(acceleration);
	ret = pHub->SendCommand(command);
	if (ret != DEVICE_OK) return ret;
	ret = pHub->ReadResponse(result);
	if (ret != DEVICE_OK) return ret;

	return ret;
}


int CRAMPSXYStage::OnStepSize(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("RAMPS XYStage OnStepSize");
  if (eAct == MM::BeforeGet)
  {
        LogMessage("RAMPS XYStage OnStepSizex");

    pProp->Set(stepSize_um_);
  }
  else if (eAct == MM::AfterSet)
  {
    if (initialized_)
    {
      double stepSize_um;
      pProp->Get(stepSize_um);
      stepSize_um_ = stepSize_um;
    }

  }

   
  LogMessage("Set step size");

  return DEVICE_OK;
}

// TODO(dek): these should send actual commands to update the device
int CRAMPSXYStage::OnVelocity(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("RAMPS XYStage OnVelocity");
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

int CRAMPSXYStage::OnAcceleration(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("RAMPS XYStage OnAcceleration");
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


int CRAMPSXYStage::OnSettleTime(MM::PropertyBase* pProp, MM::ActionType eAct)
{
LogMessage("RAMPS XYStage OnSettleTime");
  if (eAct == MM::BeforeGet)
  {
        

    pProp->Set(settle_time_);
  }
  else if (eAct == MM::AfterSet)
  {
    if (initialized_)
    {
      long settle_time;
      pProp->Get(settle_time);
      settle_time_ = settle_time;
    }
  }

   
  LogMessage("Set settle time");

  return DEVICE_OK;
}
///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////
// none implemented
