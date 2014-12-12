/*******************************************************************************
* ClubElek Robot motion control software for Eurobot 2007
* Copyright (C) 2006-2007 ClubElek
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*******************************************************************************/
//! \file motor_positionCommand.c
//! \author Julien Rouviere <gmail address : julien.rouviere@...>
//! \author ClubElek <http://clubelek.insa-lyon.fr>
// svn :
// $LastChangedBy$
// $LastChangedDate$
/******************************************************************************/

#include "motor_positionCommand.h"

#include "../types.h"
#include "../global.h"

#include <math.h>


EXPORTED_FUNCTION void ComputePositionCommand(PositionCommand *out_cmd, int32 dist, int32 VMax, int32 Accel, int32 Decel)
{
	out_cmd->period0 = 0;
	out_cmd->order0 = 0;
	out_cmd->lastOrder = 0;
	out_cmd->phase = TR_PRE_PHASE;
	out_cmd->order3 = out_cmd->order0 + dist;

	Accel = ABS(Accel);
	Decel = ABS(Decel);
	VMax = ABS(VMax);

	//avoid erroneous case
	if(dist == 0 || VMax == 0 || Accel == 0 || Decel == 0)
	{
		out_cmd->T01 = 0;
		out_cmd->T12 = 0;
		out_cmd->T23 = 0;
		out_cmd->phase = TR_END_PHASE;
			
		return;
	}

	//convert all acceleration
	if(dist > 0.0f)
	{
		out_cmd->Acceleration = Accel;
		out_cmd->Deceleration = Decel;
	
		//convert maximum speed
		out_cmd->VMax = VMax;
	}
	else
	{
		out_cmd->Acceleration = -Accel;
		out_cmd->Deceleration = -Decel;
	
		//convert maximum speed
		out_cmd->VMax = -VMax;
	}

	//compute acceleration / deceleration phases duration
	out_cmd->T01 = out_cmd->VMax/out_cmd->Acceleration;
	out_cmd->T23 = out_cmd->VMax/out_cmd->Deceleration;

	//test if the maximum speed is possible for this distance and acceleration
	if(fabs(out_cmd->order0 + out_cmd->Acceleration*out_cmd->T01*out_cmd->T01/2.0f + out_cmd->VMax*out_cmd->T23 - out_cmd->Deceleration*out_cmd->T23*out_cmd->T23/2.0f) > fabs(out_cmd->order3))
	{
		//case 1 : maximum speed isn't reachable
		int32 order02b;
		out_cmd->T23 = round(sqrtf((out_cmd->order3-out_cmd->order0) / ((out_cmd->Deceleration/(2.0f*out_cmd->Acceleration)+0.5f)*out_cmd->Deceleration)));
		out_cmd->VMax = out_cmd->T23*out_cmd->Deceleration;
	
		order02b = out_cmd->order3-((out_cmd->VMax*out_cmd->T23)-(out_cmd->T23*out_cmd->T23*out_cmd->Deceleration/2))-out_cmd->order0;
		out_cmd->T01 = round(sqrtf(2*order02b/out_cmd->Acceleration));
	
		out_cmd->T12 = 0;
	}
	else
	{
		//case 2 : maximum speed is reachable
		int32 order1,order2,order23b,delta;
	
		order1 = out_cmd->Acceleration * out_cmd->T01*out_cmd->T01/2 + out_cmd->order0;
		order2 = out_cmd->order3 - (out_cmd->Deceleration * out_cmd->T23*out_cmd->T23/2);
	
		//try to avoid numerical error
		out_cmd->T12 = abs((order2 - order1) / out_cmd->VMax);
	
		//recalculate deceleration time
		//knowing integer value for speed and time
		//=> reduce numerical error
	
		//(goal position) - (position after int constant phase)
		order23b = out_cmd->order3-(out_cmd->T12*out_cmd->VMax)-order1;
		delta = out_cmd->VMax*out_cmd->VMax - 2*out_cmd->Deceleration*order23b;
	
		if(delta > 0)
		{
			if(dist>0.0f)
				out_cmd->T23 = abs((-out_cmd->VMax + sqrtf(delta))/out_cmd->Deceleration);
			else
				out_cmd->T23 = abs((-out_cmd->VMax - sqrtf(delta))/out_cmd->Deceleration);
		}
	}
}


void LoadPositionCommand( PositionCommand *out_cmd, MOTOR *motor, int32 periodNb)
{
	out_cmd->period0 = periodNb;

	out_cmd->lastOrder = motor->lastPos;
	out_cmd->order0 = motor->lastPos;
	out_cmd->order3 += motor->lastPos;
}


/*
// 3 phases trapezoidal trajectory (acceleration - constant speed - deceleration)
//    T1 --- T2
//   /        \
//  T0         T3
*/
EXPORTED_FUNCTION BOOL GetPositionOrder(PositionCommand *posCommand, uint32 Tn, int32 *out_Order)
{
	BOOL finished = FALSE;
	
	switch(posCommand->phase)
	{
	case TR_PRE_PHASE:
		if(Tn >= posCommand->period0)
		{
			posCommand->phase = TR_ACCEL_PHASE;
		}
		else
		{
			*out_Order = posCommand->order0;
			break;
		}

	//acceleration phase
	case TR_ACCEL_PHASE:
		Tn -= posCommand->period0;
	
		if(Tn >= posCommand->T01)
		{
			Tn += posCommand->period0;
			posCommand->period0 += posCommand->T01;
			posCommand->phase = TR_CONSTANT_PHASE;
		}
		else
		{
			//For a fixed integration period, DeltaT = 1 period
			//units for acceleration and speed are in ticks/sample� and ticks/sample
			//so we can simplify the formula :
			//Acceleration*DeltaT*(Tx+DeltaT/2.0f) +V0*DeltaT + codo0;
			*out_Order = posCommand->Acceleration*Tn + posCommand->Acceleration/2 + posCommand->lastOrder;
			break;
		}

	//constant speed phase
	case TR_CONSTANT_PHASE:
		Tn -= posCommand->period0;
	
		if(Tn >= posCommand->T12)
		{
			Tn += posCommand->period0;
			posCommand->period0 += posCommand->T12;
			posCommand->phase = TR_DECEL_PHASE;
		}
		else
		{
			//Same computation than first phase with acceleration = 0
			*out_Order = posCommand->VMax + posCommand->lastOrder;
			break;
		}

	//deceleration phase
	case TR_DECEL_PHASE:
		Tn -= posCommand->period0;
	
		if(Tn >= posCommand->T23)
		{
			posCommand->phase = TR_END_PHASE;
		}
		else
		{
			//Same computation than acceleration
			*out_Order = -posCommand->Deceleration*Tn - posCommand->Deceleration/2 + posCommand->VMax + posCommand->lastOrder;
			break;
		}

	case TR_END_PHASE:
		*out_Order = posCommand->order3;
		finished = TRUE;
		break;
	};

	posCommand->lastOrder = *out_Order;
	return finished;
}
