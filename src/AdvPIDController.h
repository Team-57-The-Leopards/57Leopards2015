/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/
#pragma once

#include "Base.h"
#include "Controller.h"
#include "LiveWindow/LiveWindow.h"
#include "HAL/Semaphore.hpp"

class PIDOutput;
class PIDSource;
class DigitalInput;
class Notifier;

/**
 * Class implements a PID Control Loop.
 *
 * Creates a separate thread which reads the given PIDSource and takes
 * care of the integral calculations, as well as writing the given
 * PIDOutput
 */
class AdvPIDController : public LiveWindowSendable, public Controller, public ITableListener
{
public:
	AdvPIDController(float p, float i, float d, PIDSource *source, PIDOutput *output,
			DigitalInput *posLimit = NULL, DigitalInput *negLimit = NULL, bool limitHit = true, float period = 0.05);
	AdvPIDController(float p, float i, float d, float f, PIDSource *source, PIDOutput *output,
			DigitalInput *posLimit = NULL, DigitalInput *negLimit = NULL, bool limitHit = true, float period = 0.05);
	AdvPIDController(float p, float i, float d, float f, float v, PIDSource *source, PIDOutput *output,
			DigitalInput *posLimit = NULL, DigitalInput *negLimit = NULL, bool limitHit = true, float period = 0.05);
	virtual ~AdvPIDController();
	virtual float Get();
	virtual void SetContinuous(bool continuous = true);
	virtual void SetInputRange(float minimumInput, float maximumInput);
	virtual void SetOutputRange(float minimumOutput, float maximumOutput);
	virtual void SetPID(float p, float i, float d);
	virtual void SetPID(float p, float i, float d, float f);
	virtual void SetPID(float p, float i, float d, float f, float v);
	virtual float GetP();
	virtual float GetI();
	virtual float GetD();
	virtual float GetF();
	virtual float GetV();

	virtual void SetSetpoint(float setpoint, float rate = 100);
	virtual void SetJog(float rate);
	virtual void DisableJog();
	virtual int GetJogDir();
	virtual void SetAutoReset(float resetErr);
	virtual float GetAutoReset();
	virtual float GetSetpoint();

	virtual float GetError();

	virtual void SetTolerance(float percent);
	virtual void SetAbsoluteTolerance(float absValue);
	virtual void SetPercentTolerance(float percentValue);
	virtual bool OnTarget();

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();

	virtual void Reset();

	virtual void InitTable(ITable* table);

private:
	float m_P;			// factor for "proportional" control
	float m_I;			// factor for "integral" control
	float m_D;			// factor for "derivative" control
	float m_F;			// factor for "feed forward" control
	float m_V;			// factor for "velocity feed forward" control
	float m_maximumOutput;	// |maximum output|
	float m_minimumOutput;	// |minimum output|
	float m_maximumInput;		// maximum input - limit setpoint to this
	float m_minimumInput;		// minimum input - limit setpoint to this
	bool m_limitHit;	// value indicating a limit is hit
	bool m_continuous;	// do the endpoints wrap around? eg. Absolute encoder
	bool m_enabled; 			//is the pid controller enabled
	bool m_destruct; // should the calculate thread stop running
	float m_prevError;	// the prior sensor input (used to compute velocity)
	double m_totalError; //the sum of the errors for use in the integral calc
	enum
	{
		kAbsoluteTolerance,
		kPercentTolerance,
		kNoTolerance
	} m_toleranceType;
	float m_tolerance;	//the percetage or absolute error that is considered on target
	float m_setpoint;
	float m_autoReset;
	float m_error;
	float m_result;
	float m_period;
	float m_rate;
	float m_targetSetpoint;
	int m_jogDir;

	MUTEX_ID m_semaphore;

	DigitalInput *m_posLimit;
	DigitalInput *m_negLimit;
	PIDSource *m_pidInput;
	PIDOutput *m_pidOutput;

	Notifier* m_controlLoop;

	void Initialize(float p, float i, float d, float f, float v, PIDSource *source, PIDOutput *output,
			 DigitalInput *posLimit = NULL, DigitalInput *negLimit = NULL, bool limitHit = true, float period = 0.05);
	static void CallCalculate(void *controller);

	virtual ITable* GetTable();
	virtual std::string GetSmartDashboardType();
	virtual void ValueChanged(ITable* source, const std::string& key, EntryValue value, bool isNew);
	virtual void UpdateTable();
	virtual void StartLiveWindowMode();
	virtual void StopLiveWindowMode();
protected:
	ITable* m_table;
	void Calculate();

	DISALLOW_COPY_AND_ASSIGN(AdvPIDController);
};