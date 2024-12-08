#include "SubSystemModules/Maintenance/Maintenance.h"
#include "TelemetryCollector.h"
#include "hal/Storage/FRAM.h"
#include "satellite-subsystems/IsisTRXVU.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

int namesIndex = 0;

void WriteTelem(void *data, int size, const char *ext) {
  Time time;
  Time_get(&time);
  char buffer[50];

  snprintf(buffer, 50, "/log/%02d/%02d/%02d.%s", time.year + 2000, time.month,
           time.date, ext);
  printf("writing telem to file: %s", buffer);
}

char* getFallenName(){
	char* names[] = {
			"Davidovich Shlomi",
			"Hajabi Ziv",
			"Elikim Noam",
			"Barosh Shoshana",
			"Zohar Yaniv",
			"Zohar Yasmin",
			"Zohar Keshet",
			"Zohar Tichlet",
			"Libna Chaim",
			"Idan Maayan",
			"Arava Eliezer Tomer",
			"Arava Dekel",
			"Poslushni Ran",
			"Fiorentino Ilan",
			"Ron Shlomo Tzvi",
			"Avi'ani Shachar",
			"Admoni Guy",
			"Admoni Michal",
			"Adari Aviad",
			"Orgad Eliyahu",
			"Eylon Tal",
			"Itamari Ram",
			"Itamari Lili",
			"Alqavtz Sivan",
			"Amir Mordechai (Mudi)",
			"Apstein Blaha",
			"Apstein Neta",
			"Bardichovski Itai",
			"Badichovski Hadr",
			"Bouyum Yuval",
			"Bergstein Shahaf",
			"Bar'am Aviv",
			"Goldstein Nadav",
			"Goldstein Yam",
			"Goren Aran",
			"Goren Tova",
			"Dgani Naomi",
			"Handy Avi",
			"Weiner Yahav",
			"Wertheim Dorit",
			"Wertheim Aviv",
			"Zini Nir'al",
			"Ziv Eitan",
			"Ziv Peleg Tami",
			"Chaim Yotam",
			"Harmash Omer",
			"Hasidim Naor",
			"Libshtein Ofir",
			"Libshtein Nitzan",
			"Idan Smadar",
			"Idan Roy",
			"Amikam Nadav",
			"Salomon Yuval",
			"Feld Daniel",
			"Feld Gila",
			"Feld Yizhar",
			"Flash Yigal",
			"Flash Cynthia Tamar",
			"Tzadikovich Omer",
			"Kutz Aviv",
			"Kutz Yonatan",
			"Kutz Yiftach",
			"Kutz Livnat",
			"Kutz Rotem",
			"Katzir David",
			"Raviv Niv",
			"Ronen Nira",
			"Russo Uri Shimon",
			"Reichenstein Eliyahu",
			"Schwartzman Pinko Orli",
			"Schwartzman Alberto David",
			"Shoshani Ofir",
			"Shtahl Mira",
			"Shmariz Alon",
			"Hajabi Elitzur Tzuriel",
			"Hajabi Yehonatan",
			"Hajabi Yizhar",
			"Salam Peleg",
			"Glazki Yevgeny",
			"Naim Amir",
			"Shapira Ziv Pape"};
	if (namesIndex >= sizeof(names) / sizeof(names[0])) {
		namesIndex = 0; // Reset to start if all names are used
	    }
	 return strcat("In memory of ", names[namesIndex++]);

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
  char* fallenName = getFallenName();
  strcpy(wod->fallenName, fallenName);
  free(fallenName);
  return 0;
}

void TelemetrySaveSolarPanels(){
	int32_t array_paneltemp[9];
	array_paneltemp[0]=ISIS_SOLAR_PANEL_0;
	array_paneltemp[1]=ISIS_SOLAR_PANEL_1;
	array_paneltemp[2]=ISIS_SOLAR_PANEL_2;
	array_paneltemp[3]=ISIS_SOLAR_PANEL_3;
	array_paneltemp[4]=ISIS_SOLAR_PANEL_4;
	array_paneltemp[5]=ISIS_SOLAR_PANEL_5;
	array_paneltemp[6]=ISIS_SOLAR_PANEL_6;
	array_paneltemp[7]=ISIS_SOLAR_PANEL_7;
	array_paneltemp[8]=ISIS_SOLAR_PANEL_8;
	uint8_t status;

	int error;
	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_0,&array_paneltemp,&status,"TelemetrySaveSolarPanels"));
	if(error != 0){
		return;

	}
	 WriteTelem(&array_paneltemp, sizeof(array_paneltemp),END_FILENAME_SOLAR_PANELS_TLM);

}
void TelemetrySaveTRXVU(){
	unsigned char index=0;
	ISIStrxvuTxTelemetry telemetry_tc;
	int error;
	error=logError(IsisTrxvu_tcGetTelemetryAll(index, &telemetry_tc), "TelemetrySaveTRXVU1");
	if(error != 0){
		return;
	}
	WriteTelem(&telemetry_tc, sizeof(telemetry_tc),END_FILE_NAME_TX);



	unsigned char index=0;
	ISIStrxvuRxTelemetry telemetry_rc;
	int error1;
	error = logError(IsisTrxvu_rcGetTelemetryAll(index, &telemetry_rc), "TelemetrySaveTRXVU2");
	if(error != 0){
		return;
	}
	 WriteTelem(&telemetry_rc, sizeof(telemetry_rc), END_FILE_NAME_RX);
}

void TelemetrySaveWOD(){



}

void  InitSavePeriodTimes(){


}


