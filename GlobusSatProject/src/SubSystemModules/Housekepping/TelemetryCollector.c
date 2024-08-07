#include "SubSystemModules/Maintenance/Maintenance.h"
#include "TelemetryCollector.h"
#include "hal/Storage/FRAM.h"
#include "satellite-subsystems/IsisTRXVU.h"
#include "utils.h"
void WriteTelem(void *data, int size, const char *ext) {
  Time time;
  Time_get(&time);
  char buffer[50];

  snprintf(buffer, 50, "/log/%02d/%02d/%02d.%s", time.year + 2000, time.month,
           time.date, ext);
  printf("writing telem to file: %s", buffer);
}

void TelemetrySaveEPS() {
  gom_eps_hkparam_t data;
  gom_eps_hk_t data1;
  gom_eps_hk_vi_t data2;
  gom_eps_hk_out_t data3;
  gom_eps_hk_wdt_t data4;
  gom_eps_hk_basic_t data5;

  GomEpsGetHkData_param(0, &data);
  GomEpsGetHkData_general(0, &data1);
  GomEpsGetHkData_vi(0, &data2);
  GomEpsGetHkData_out(0, &data3);
  GomEpsGetHkData_wdt(0, &data4);
  GomEpsGetHkData_basic(0, &data5);
}

time_unix prev_trx_time;
time_unix trx_period;
void TelemetrySaveTRXVU() {
  ISIStrxvuTxTelemetry telemetry;
  IsisTrxvu_tcGetTelemetryAll(0, &telemetry);
  WriteTelem(&telemetry, sizeof(ISIStrxvuTxTelemetry), "trx");
}

void TelemetryCollectorLogic() {
  FRAM_read((unsigned char *)&trx_period, EPS_SAVE_TLM_PERIOD_ADDR,
            sizeof(time_unix));
  Time_getUnixEpoch(&prev_trx_time);
  if (CheckExecutionTime(prev_trx_time, trx_period)) {
    TelemetrySaveTRXVU();
  }
}
////////
TelemetrySaveTRXVU(){

	ISIStrxvuTxTelemetry telemetry_all;
	 IsisTrxvu_tcGetTelemetryAll(0, &telemetry_all);
	 WriteTelem(&telemetry_all, sizeof(ISIStrxvuTxTelemetry), END_FILE_NAME_TX	);

	 ISIStrxvuTxTelemetry_revC telemetry_all_revC;
	 IsisTrxvu_tcGetTelemetryAll_revC(0, &telemetry_all_revC);
	 WriteTelem(&telemetry_all_revC, sizeof( ISIStrxvuTxTelemetry_revC), END_FILE_NAME_TX);

	 ISIStrxvuTxTelemetry last_telemetry;
	 IsisTrxvu_tcGetLastTxTelemetry(0, &last_telemetry);
	 WriteTelem(&last_telemetry, sizeof(  ISIStrxvuTxTelemetry), END_FILE_NAME_TX);

	 ISIStrxvuTxTelemetry_revC last_telemetry_revC;
	 IsisTrxvu_tcGetLastTxTelemetry_revC(0, &last_telemetry_revC);
	 WriteTelem(&last_telemetry_revC, sizeof( ISIStrxvuTxTelemetry_revC ), END_FILE_NAME_TX);

	 ISIStrxvuRxTelemetry telemetry_Rx;
	 IsisTrxvu_rcGetTelemetryAll(0, &telemetry_Rx);
	 WriteTelem(&telemetry_Rx, sizeof(	 ISIStrxvuRxTelemetry),END_FILE_NAME_RX);

	 ISIStrxvuRxTelemetry_revC telemetry_All_revC_Rx;
      IsisTrxvu_rcGetTelemetryAll_revC(0, telemetry_All_revC_Rx);
      WriteTelem(&telemetry_All_revC_Rx, sizeof( ISIStrxvuRxTelemetry_revC),END_FILE_NAME_RX);

}
////////
TelemetrySaveANT(){

	ISISantsTelemetry alltelemetry_A;
	IsisAntS_getAlltelemetry(ANTS_I2C_SIDE_A_ADDR, isisants_sideA, &alltelemetry_A);
	WriteTelem(&alltelemetry_A, sizeof( ISISantsTelemetry),END_FILE_NAME_ANTENNA);

	ISISantsTelemetry alltelemetry_B;
	IsisAntS_getAlltelemetry(ANTS_I2C_SIDE_B_ADDR, isisants_sideB, &alltelemetry_B);
	WriteTelem(&alltelemetry_B, sizeof( ISISantsTelemetry),END_FILE_NAME_ANTENNA);



}
int GetCurrentWODTelemetry(WOD_Telemetry_t *wod) {
  gom_eps_hk_t data_out;
  GomEpsGetHkData_general(0, &data_out);
  wod->vbat = data_out.fields.vbatt;
  gom_eps_hkparam_t eps_param;
  PROPEGATE_ERROR(GomEpsGetHkData_param(0, &eps_param),
                  "error getting eps params");
  wod->bat_temp = eps_param.fields.tempBattery;

  for (int i = 0; i < 6; i++) {
    uint8_t status;
    PROPEGATE_ERROR(
        IsisSolarPanelv2_getTemperature(i, &wod->solar_panels[i], &status),
        "could not get solar panel temp");
  }

  return 0;
}
