#include "SubSystemModules/Housekepping/TelemetryFiles.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "TelemetryCollector.h"
#include "hal/Storage/FRAM.h"
#include "satellite-subsystems/IsisTRXVU.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

#include "satellite-subsystems/imepsv2_piu.h"
#include <hcc/api_fat.h>
#include "GlobalStandards.h"
#include "satellite-subsystems/isis_ants2.h"
int namesIndex = 0;
time_unix save_time[NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS];
time_unix last_tlem_save[NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS];

void TelemetryCollectorLogic() {
	int i=0;
	time_unix telem_time;
	if( CheckExecutionTime( last_tlem_save[tlm_eps],save_time[tlm_eps])){
		TelemetrySaveEPS();
		TelemetrySaveSolarPanels();
		TelemetrySaveTRXVU();
		TelemetrySaveSolarPanels();

		if(Time_getUnixEpoch(&telem_time)==0){
		last_tlem_save[tlm_eps]=telem_time;

		}

	}



}

void TelemetrySaveEPS(){//done
	imepsv2_piu__gethousekeepingeng__from_t response;
	int error;
	error=logError(imepsv2_piu__gethousekeepingeng(0,&response),"TelemetrySaveEPS");
	if( error==0)
	 WriteTelem(&response, sizeof(response),END_FILENAME_EPS_TLM);
}

unsigned int namesIndex = 0;

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



int GetCurrentWODTelemetry(WOD_Telemetry_t *wod) {//done
	imepsv2_piu__gethousekeepingeng__from_t response;
	 int error;
     error = imepsv2_piu__gethousekeepingeng(0,&response);
     wod->vbat=response.fields.batt_input.fields.volt;
     wod->volt_5V=response.fields.vip_obc01.fields.volt;
     wod->volt_5V=response.fields.vip_obc01.fields.volt;
     wod->volt_3V3=response.fields.vip_obc05.fields.volt;
     wod->charging_power=response.fields.batt_input.fields.power * 10;//todo make sure the response correct batery power
     wod->consumed_power=response.fields.dist_input.fields.power * 10;
     wod->electric_current=response.fields.batt_input.fields.current;
     wod->current_3V3=response.fields.vip_obc05.fields.current;
     wod->current_5V=response.fields.vip_obc01.fields.current;
     wod->mcu_temp= ((double)response.fields.temp) * 0.01;
     wod->bat_temp=((double)response.fields.temp2) * 0.01;

     uint8_t status;

     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_0,&wod->solar_panels[0],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     		return error;
     	}
     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_1,&wod->solar_panels[1],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     	     		return error;
     	     	}
     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_2,&wod->solar_panels[2],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     	     		return error;
     	     	}
     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_3,&wod->solar_panels[3],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     	     		return error;
     	     	}
     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_4,&wod->solar_panels[4],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     	     		return error;
     	     	}
     	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_5,&wod->solar_panels[5],&status),"TelemetrySaveSolarPanels");
     	if(error!=0){
     	     		return error;
     	     	}


     unsigned int epochTime;

          error =Time_getUnixEpoch( &epochTime);
     wod->sat_time=epochTime;

     wod->sat_uptime=Time_getUptimeSeconds();

     F_SPACE space;
      /* get free space on current drive */
       f_getfreespace(f_getdrive(),&space);
      wod->free_memory=space.free;
      wod->free_memory=space.bad;

      char* fallenName = getFallenName();
      strcpy(wod->fallenName, fallenName);
      return 0;
}

void TelemetrySaveSolarPanels(){//done
	uint8_t status;
	int error;
	int32_t array_paneltemp[6];

	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_0,&array_paneltemp[0],&status),"TelemetrySaveSolarPanels");
		if(error != 0){
			return;

		}

	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_1,&array_paneltemp[1],&status),"TelemetrySaveSolarPanels");
		if(error != 0){
			return;

		}

	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_2,&array_paneltemp[2],&status),"TelemetrySaveSolarPanels");
		if(error != 0){
			return;
		}

	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_3,&array_paneltemp[3],&status),"TelemetrySaveSolarPanels");
	if(error != 0){
				return;
			}
	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_4,&array_paneltemp[4],&status),"TelemetrySaveSolarPanels");
	if(error != 0){
				return;
			}

	error= logError(IsisSolarPanelv2_getTemperature(ISIS_SOLAR_PANEL_5,&array_paneltemp[5],&status),"TelemetrySaveSolarPanels");
	if(error != 0){
				return;
			}


	 WriteTelem(&array_paneltemp, sizeof(array_paneltemp),END_FILENAME_SOLAR_PANELS_TLM);

}
void TelemetrySaveTRXVU(){//done
	unsigned char index=0;
	ISIStrxvuTxTelemetry telemetry_tc;
	int error;
	error=logError(IsisTrxvu_tcGetTelemetryAll(index, &telemetry_tc), "TelemetrySaveTRXVU1");
	if(error == 0){
		WriteTelem(&telemetry_tc, sizeof(telemetry_tc),END_FILE_NAME_TX);
	}


	index=0;
	ISIStrxvuRxTelemetry telemetry_rc;
	int error1;
	error1 = logError(IsisTrxvu_rcGetTelemetryAll(index, &telemetry_rc), "TelemetrySaveTRXVU2");
	if(error1 == 0){
		 WriteTelem(&telemetry_rc, sizeof(telemetry_rc), END_FILE_NAME_RX);
		}
}

void TelemetrySaveWOD(){//done
	WOD_Telemetry_t wod;
	int error;
	error=GetCurrentWODTelemetry(&wod);
	if(error==0)
	WriteTelem(&wod, sizeof(wod), END_FILENAME_WOD_TLM);

}


void  InitSavePeriodTimes(){//done
	unsigned int SavePeriodTime_eps=5;
	unsigned int SavePeriodTime_ant=5;
	unsigned int SavePeriodTime_wod=5;
	unsigned int SavePeriodTime_trxvu=5;
	int error;

	error=FRAM_WRITE_FIELD(&SavePeriodTime_eps, telePeriod_eps);
	if(error!=0)
		return;
	error=FRAM_WRITE_FIELD(&SavePeriodTime_ant, telePeriod_ant);
	if(error!=0)
			return;

	error=FRAM_WRITE_FIELD(&SavePeriodTime_wod, telePeriod_wod);
	if(error!=0)
			return;
	error=FRAM_WRITE_FIELD(&SavePeriodTime_wod, telePeriod_trx);
	if(error!=0)
			return;

}
void TelemetrySaveANT(){//done
	int error;
	isis_ants2__get_all_telemetry__from_t response;
	error=logError(isis_ants2__get_all_telemetry(0, &response),"TelemetrySaveANT");
	if(error==0)
	WriteTelem(&response, sizeof(response),END_FILE_NAME_ANTENNA);

}


size_t getTlmDataSize(tlm_type_t tlm_type){
  switch (tlm_type) {
  case tlm_eps:
    return sizeof(imepsv2_piu__gethousekeepingeng__from_t);
  case tlm_tx:
    return sizeof(ISIStrxvuTxTelemetry);
  case tlm_antenna:
    return sizeof(isis_ants2__get_all_telemetry__from_t);
  case tlm_solar:
    return sizeof(int32_t) * 8;
  case tlm_wod:
    return sizeof(WOD_Telemetry_t);
  case tlm_rx:
    return sizeof(ISIStrxvuRxTelemetry);
  case tlm_log:
    return sizeof(logData_t);
  default:
    return 0;
  }
}
