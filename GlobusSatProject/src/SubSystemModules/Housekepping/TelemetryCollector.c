#include "stdio.h"

void TelemetrySaveEPS(){
	gom_eps_hkparam_t data;
	gom_eps_hk_t data1;
	gom_eps_hk_vi_t data2;
	 gom_eps_hk_out_t  data3;
	 gom_eps_hk_wdt_t data4;
	 gom_eps_hk_basic_t data5;

	 GomEpsGetHkData_param(0,  &data);
	 GomEpsGetHkData_general(0,&data1);
	 GomEpsGetHkData_vi(0,  &data2);
	 GomEpsGetHkData_out(0, &data3);
	 GomEpsGetHkData_wdt(0,  &data4);
	 GomEpsGetHkData_basic(0, &data5);
}

void TelemetrySaveTRXVO(){
	 ISIStrxvuTxTelemetry telemetry;
	 ISIStrxvuTxTelemetry_revC telemetry1;
	 ISIStrxvuTxTelemetry telemetry2;
	 ISIStrxvuTxTelemetry_revC telemetry3;
	IsisTrxvu_tcGetTelemetryAll(0,telemetry);
	IsisTrxvu_tcGetTelemetryAll_revC(0,telemetry1);
	IsisTrxvu_tcGetLastTxTelemetry(0,telemetry2);
	IsisTrxvu_tcGetLastTxTelemetry_revC(0, telemetry3);
}

void WriteTelem(){
			Time time;
     Time_get (&time);

}
