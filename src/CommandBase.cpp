#include "CommandBase.h"

#include "Commands/Scheduler.h"

// Initialize a single static instance of all of your subsystems to NULL

OI* CommandBase::oi = NULL;
Chassis* CommandBase::chassis = NULL;
CommandBase::CommandBase(char const *name) :
		Command(name)
{
}

CommandBase::CommandBase() :
		Command()
{

}

void CommandBase::init()
{
	// Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	chassis = new Chassis();
	oi = new OI();
}
