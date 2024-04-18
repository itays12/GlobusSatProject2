#include "EPS.h"
#include <satellite-subsystems/GomEPS.h>

voltage_t last_voltage;
int voltage_tend;

int EPS_Conditioning()
{
	int ret=0;
	voltage_t t;
	voltage_tend = getFilteredVolt(t);
	ret = GetBatteryVoltage(&t);
	last_voltage = t;
	if(ret!=0)
	{
		return -1;
	}
	if(voltage_tend==-1)
	{
		if(t>=FULL_MODE_DOWN_TEND)
		{
			return EnterFullMode();
		}

		if(t>=CRUISE_MODE_DOWN_TEND)
		{
			return EnterCruiseMode();
		}
		if(t>=SAFE_MODE_DOWN_TEND)
		{
			return EnterCruiseMode();
		}
		else
		{
			return EnterCriticalMode();
		}
	}
	else
	{
		if(t>=FULL_MODE_UP_TEND)
		{
			return EnterFullMode();
		}

		if(t>=CRUISE_MODE_UP_TEND)
		{
			return EnterCruiseMode();
		}
		if(t>=SAFE_MODE_UP_TEND)
		{
			return EnterCruiseMode();
		}
		else
		{
			return EnterCriticalMode();
		}
	}
}

int GetBatteryVoltage(voltage_t *vbat)
{
	gom_eps_hk_t response;
	int err = (GomEpsGetHkData_general(0,&response));
	print_error(err);
	*vbat = response.fields.vbatt;
	return err;
}

int getFilteredVolt(voltage_t t)
{
	if(t>=last_voltage)
	{
		return 1;
	}
	return -1;
}

