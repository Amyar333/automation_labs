
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif

void DoorStateMashine(struct DoorStateMashine* inst)
{
	BOOL s0 = inst->sw1;
	BOOL s1 = inst->sw2;
	BOOL s2 = inst->sw3;
	BOOL s3 = inst->sw4;

	BOOL s0Changed = (s0 != inst->s0Prev);
	BOOL s1Changed = (s1 != inst->s1Prev);
	BOOL s2Changed = (s2 != inst->s2Prev);
	BOOL s3Changed = (s3 != inst->s3Prev);

	BOOL driveOn = ((inst->state & DRV_STATE_MASK) == STATE_ENABLED);
	DoorStates stateAtStart;

	if (!driveOn && inst->doorState != ST_INIT)
		inst->doorState = ST_INIT;

	stateAtStart = inst->doorState;

	switch (inst->doorState)
	{
		case ST_INIT:
			inst->speed = SPEED_STOP;
			if (driveOn)
				inst->doorState = ST_UNKNOWN;
			break;

		case ST_UNKNOWN:
			inst->speed = inst->direction ? SPEED_SLOW : -SPEED_SLOW;
			if (!inst->direction && s0Changed)
				inst->doorState = ST_OPEN;
			else if (inst->direction && s3Changed)
				inst->doorState = ST_CLOSE;
			break;

		case ST_OPEN:
			inst->speed = SPEED_STOP;
			if (inst->direction)
				inst->doorState = ST_ACC_POS;
			break;

		case ST_ACC_POS:
			inst->speed = SPEED_SLOW;
			if (!inst->direction)
				inst->doorState = ST_DEC_NEG;
			else if (s1Changed)
				inst->doorState = ST_POS;
			break;

		case ST_POS:
			inst->speed = SPEED_FAST;
			if (!inst->direction)
				inst->doorState = ST_NEG;
			else if (s2Changed)
				inst->doorState = ST_DEC_POS;
			break;

		case ST_DEC_POS:
			inst->speed = SPEED_SLOW;
			if (!inst->direction)
				inst->doorState = ST_ACC_NEG;
			else if (s3Changed)
				inst->doorState = ST_CLOSE;
			break;

		case ST_CLOSE:
			inst->speed = SPEED_STOP;
			if (!inst->direction)
				inst->doorState = ST_ACC_NEG;
			break;

		case ST_ACC_NEG:
			inst->speed = -SPEED_SLOW;
			if (inst->direction)
				inst->doorState = ST_DEC_POS;
			else if (s2Changed)
				inst->doorState = ST_NEG;
			break;

		case ST_NEG:
			inst->speed = -SPEED_FAST;
			if (inst->direction)
				inst->doorState = ST_POS;
			else if (s1Changed)
				inst->doorState = ST_DEC_NEG;
			break;

		case ST_DEC_NEG:
			inst->speed = -SPEED_SLOW;
			if (inst->direction)
				inst->doorState = ST_ACC_POS;
			else if (s0Changed)
				inst->doorState = ST_OPEN;
			break;

		default:
			inst->speed = SPEED_STOP;
			inst->doorState = ST_INIT;
			break;
	}

	if (stateAtStart == ST_INIT || inst->doorState == ST_INIT)
	{
		inst->s0Prev = 0;
		inst->s1Prev = 0;
		inst->s2Prev = 0;
		inst->s3Prev = 0;
	}
	else
	{
		inst->s0Prev = s0;
		inst->s1Prev = s1;
		inst->s2Prev = s2;
		inst->s3Prev = s3;
	}
}
