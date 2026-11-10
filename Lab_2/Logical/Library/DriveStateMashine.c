
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "Library.h"
#ifdef __cplusplus
	};
#endif

void DriveStateMashine(struct DriveStateMashine* inst)
{
	UINT st = inst->state & DRV_STATE_MASK;

	if (!inst->enable)
	{
		inst->command = CMD_SHUTDOWN;
		inst->speed   = 0;
		return;
	}

	if (inst->state & STATE_FAULT_BIT)
	{
		inst->command = CMD_FAULT_RESET;
		inst->speed   = 0;
		return;
	}

	if (st == STATE_ENABLED)
	{
		inst->command = CMD_ENABLE;
		inst->speed   = inst->speedSet;
	}
	else if (st == STATE_READY)
	{
		inst->command = CMD_ENABLE;
		inst->speed   = 0;
	}
	else if (st == STATE_SWITCHED_ON)
	{
		inst->command = CMD_SWITCH_ON;
		inst->speed   = 0;
	}
	else
	{
		inst->command = CMD_SHUTDOWN;
		inst->speed   = 0;
	}
}
