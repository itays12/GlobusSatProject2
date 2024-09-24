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

void TelemetryCollectorLogic() {

	time_unix curTime;
	Time_getUnixEpoch(&curTime);

	time_unix trxTimer;
	FRAM_READ_FIELD(&trxTimer, teleSaveTime_trx);
	if (curTime > trxTimer) {
		TelemetrySaveTRXVU();
		time_unix telePeriod_trx;
		FRAM_READ_FIELD(&telePeriod_trx, telePeriod_trx);
		trxTimer += telePeriod_trx;
		FRAM_WRITE_FIELD(&trxTimer, teleSaveTime_trx);
	}
}


void TelemetrySaveTRXVU(){
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
