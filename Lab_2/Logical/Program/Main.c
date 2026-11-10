
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

static RTInfo_typ rtInfo;

void _INIT ProgramInit(void)
{
	rtInfo.enable = 1;
	RTInfo(&rtInfo);
	ledSM.cycleTime = rtInfo.cycle_time / 1000;
	if (ledSM.cycleTime == 0)
		ledSM.cycleTime = 100;

	stateMachine.enable   = 0;
	stateMachine.speedSet = 0;
	stateMachine.command  = CMD_SHUTDOWN;
	stateMachine.speed    = 0;

	doorSM.doorState = ST_INIT;
	doorSM.speed     = 0;

	ledSM.state = ST_INIT;
	ledSM.led1  = 0;
	ledSM.led2  = 0;
	ledSM.led3  = 0;
	ledSM.led4  = 0;
}

void _CYCLIC ProgramCyclic(void)
{
	doorSM.state = stateMachine.state;
	DoorStateMashine(&doorSM);

	stateMachine.enable   = 1;
	stateMachine.speedSet = doorSM.speed;
	DriveStateMashine(&stateMachine);

	ledSM.state = (UINT)doorSM.doorState;
	LedStateMashine(&ledSM);
}
