#include "TelemetryCollector.h"
#include "GlobalStandards.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryFiles.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "TLM_management.h"
#include "hal/Storage/FRAM.h"
#include "satellite-subsystems/isis_ants.h"
#include "satellite-subsystems/isis_vu_e.h"
#include "satellite-subsystems/isismepsv2_ivid7_piu.h"
#include "utils.h"
#include <hcc/api_fat.h>
#include <stdlib.h>
#include <string.h>
#include "fallenNames.h"

unsigned int namesIndex = 0;
time_unix save_time[NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS];
time_unix last_tlem_save[NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS];

int CMD_GetTLMPeriodTimes(sat_packet_t *cmd) { // done
  int error;
  error = TransmitDataAsSPL_Packet(
      cmd, save_time, sizeof(time_unix) * NUM_OF_SUBSYSTEMS_SAVE_FUNCTIONS);
  return error;
}

int CMD_SetTLMPeriodTimes(sat_packet_t *cmd) { // done
  int err;
  time_unix newSaveTime;
  memcpy(&newSaveTime, cmd->data + 1, sizeof(newSaveTime));
  switch (cmd->data[0]) {
  case tlm_eps:
    err = FRAM_WRITE_FIELD(&newSaveTime, telePeriod_eps);
    save_time[tlm_eps] = newSaveTime;
    break;
  case tlm_tx:
    err = FRAM_WRITE_FIELD(&newSaveTime, telePeriod_trx);
    save_time[tlm_tx] = newSaveTime;
    break;
  case tlm_antenna:
    err = FRAM_WRITE_FIELD(&newSaveTime, telePeriod_ant);
    save_time[tlm_antenna] = newSaveTime;
    break;
  case tlm_solar:
    err = FRAM_WRITE_FIELD(&newSaveTime, telePeriod_solar);
    save_time[tlm_solar] = newSaveTime;
    break;
  case tlm_wod:
    err = FRAM_WRITE_FIELD(&newSaveTime, telePeriod_wod);
    save_time[tlm_wod] = newSaveTime;
    break;
  default:
    err = -4;
  }
  if (err != 0) {
    return err;
  }
  SendAckPacket(ACK_COMD_EXEC, cmd->ID, NULL, 0);

  return err;
}

void TelemetryCollectorLogic() { // done
  time_unix telem_time;
  if (CheckExecutionTime(last_tlem_save[tlm_eps], save_time[tlm_eps])) {
    TelemetrySaveEPS();
    if (Time_getUnixEpoch(&telem_time) == 0) {
      last_tlem_save[tlm_eps] = telem_time;
    }
  }
  if (CheckExecutionTime(last_tlem_save[tlm_tx], save_time[tlm_tx])) {
    TelemetrySaveTRXVU();
    if (Time_getUnixEpoch(&telem_time) == 0) {
      last_tlem_save[tlm_tx] = telem_time;
    }
  }
/*  if (CheckExecutionTime(last_tlem_save[tlm_solar], save_time[tlm_solar])) {
    TelemetrySaveSolarPanels();
    if (Time_getUnixEpoch(&telem_time) == 0) {
      last_tlem_save[tlm_solar] = telem_time;
    }
  }*/
  if (CheckExecutionTime(last_tlem_save[tlm_wod], save_time[tlm_wod])) {
    TelemetrySaveWOD();
    if (Time_getUnixEpoch(&telem_time) == 0) {
      last_tlem_save[tlm_wod] = telem_time;
    }
  }

  /*if (CheckExecutionTime(last_tlem_save[tlm_antenna], save_time[tlm_antenna])) {
    TelemetrySaveANT();
    if (Time_getUnixEpoch(&telem_time) == 0) {
      last_tlem_save[tlm_antenna] = telem_time;
    }
  }*/
}

void TelemetrySaveEPS() { // done
  isismepsv2_ivid7_piu__gethousekeepingeng__from_t response;
  int error;
  error = logError(isismepsv2_ivid7_piu__gethousekeepingeng(0, &response),
                   "TelemetrySaveEPS");
  if (error == 0)
    writeToFile(&response, sizeof(response), tlm_eps);
}

void getFallenName(char buffer[]) {
/*  char *names[] = {"Davidovich Shlomi",
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
                   "Shapira Ziv Pape"};*/

  namesIndex %= sizeof(names) / sizeof(names[0]);
  strcpy(buffer, names[namesIndex++]);
}

int GetCurrentWODTelemetry(WOD_Telemetry_t *wod) {
  isismepsv2_ivid7_piu__gethousekeepingeng__from_t response;
  int error;
  error = logError(isismepsv2_ivid7_piu__gethousekeepingeng(0, &response),
                   "GetCurrentWODTelemetry");
  wod->vbat = response.fields.batt_input.fields.volt;
  wod->volt_5V = response.fields.vip_obc01.fields.volt;
  wod->volt_5V = response.fields.vip_obc01.fields.volt;
  wod->volt_3V3 = response.fields.vip_obc05.fields.volt;
  wod->charging_power = response.fields.batt_input.fields.power *
                        10; // todo make sure the response correct batery power
  wod->consumed_power = response.fields.dist_input.fields.power * 10;
  wod->electric_current = response.fields.batt_input.fields.current;
  wod->current_3V3 = response.fields.vip_obc05.fields.current;
  wod->current_5V = response.fields.vip_obc01.fields.current;
  wod->mcu_temp = ((double)response.fields.temp) * 0.01;
  wod->bat_temp = ((double)response.fields.temp2) * 0.01;

/*  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_0, &wod->solar_panels[0], &status),
                   "TelemetryGetSolarPanels0");
  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_1, &wod->solar_panels[1], &status),
                   "TelemetryGetSolarPanels1");
  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_2, &wod->solar_panels[2], &status),
                   "TelemetryGetSolarPanels2");
  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_3, &wod->solar_panels[3], &status),
                   "TelemetryGetSolarPanels3");
  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_4, &wod->solar_panels[4], &status),
                   "TelemetryGetSolarPanels4");

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_5, &wod->solar_panels[5], &status),
                   "TelemetryGetSolarPanels5");*/

  unsigned int epochTime;

  error = Time_getUnixEpoch(&epochTime);
  wod->sat_time = epochTime;

  wod->sat_uptime = Time_getUptimeSeconds();

  F_SPACE space;
  /* get free space on current drive */
  f_getfreespace(f_getdrive(), &space);
  wod->free_memory = space.free;
  wod->corrupt_bytes = space.bad;

  FRAM_READ_FIELD(&wod->number_of_resets, startupCount);

  getFallenName(wod->fallenName);
  return error;
}

void TelemetrySaveSolarPanels() { // done
  uint8_t status;
  int error;
  int32_t array_paneltemp[6];

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_0, &array_paneltemp[0], &status),
                   "TelemetrySaveSolarPanels0");
  if (error != 0) {
    return;
  }

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_1, &array_paneltemp[1], &status),
                   "TelemetrySaveSolarPanels1");
  if (error != 0) {
    return;
  }

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_2, &array_paneltemp[2], &status),
                   "TelemetrySaveSolarPanels2");
  if (error != 0) {
    return;
  }

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_3, &array_paneltemp[3], &status),
                   "TelemetrySaveSolarPanels3");
  if (error != 0) {
    return;
  }
  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_4, &array_paneltemp[4], &status),
                   "TelemetrySaveSolarPanels4");
  if (error != 0) {
    return;
  }

  error = logError(IsisSolarPanelv2_getTemperature(
                       ISIS_SOLAR_PANEL_5, &array_paneltemp[5], &status),
                   "TelemetrySaveSolarPanels5");
  if (error != 0) {
    return;
  }

  writeToFile(&array_paneltemp, sizeof(array_paneltemp), tlm_solar);
}

void TelemetrySaveTRXVU() { // done
  unsigned char index = 0;
  isis_vu_e__get_tx_telemetry__from_t telemetry_tc;
  int error;
  error = logError(isis_vu_e__get_tx_telemetry(index, &telemetry_tc),
                   "TelemetrySaveTRXVU1");
  if (error != 0) {
    return;
  }

  writeToFile(&telemetry_tc, sizeof(telemetry_tc), tlm_tx);

  index = 0;
  isis_vu_e__get_rx_telemetry__from_t telemetry_rc;
  int error1;
  error1 = logError(isis_vu_e__get_rx_telemetry(index, &telemetry_rc),
                    "TelemetrySaveTRXVU2");
  if (error1 != 0) {
    return;
  }

  writeToFile(&telemetry_rc, sizeof(telemetry_rc), tlm_rx);
}

void TelemetrySaveWOD() { // done
  WOD_Telemetry_t wod;
  int error;
  error = GetCurrentWODTelemetry(&wod);
  if (error != 0) {
    return;
  }

  writeToFile(&wod, sizeof(wod), tlm_wod);
}

void InitSavePeriodTimes() {
  FRAM_READ_FIELD(&save_time[tlm_eps], telePeriod_eps);
  FRAM_READ_FIELD(&save_time[tlm_antenna], telePeriod_ant);
  FRAM_READ_FIELD(&save_time[tlm_wod], telePeriod_wod);
  FRAM_READ_FIELD(&save_time[tlm_solar], telePeriod_solar);
  FRAM_READ_FIELD(&save_time[tlm_tx], telePeriod_trx);
}
void TelemetrySaveANT() { // done
  int error;
  isis_ants__get_all_telemetry__from_t response[2];
  error = logError(isis_ants__get_all_telemetry(0, &response[0]),
                   "TelemetrySaveANT");
  error = logError(isis_ants__get_all_telemetry(1, &response[1]),
                   "TelemetrySaveANT");
  if (error != 0) {
    return;
  }
  writeToFile(response, sizeof(response), tlm_antenna);
}

size_t getTlmDataSize(tlm_type_t tlm_type) {
  switch (tlm_type) {
  case tlm_eps:
    return sizeof(isismepsv2_ivid7_piu__gethousekeepingeng__from_t);
  case tlm_tx:
    return sizeof(isis_vu_e__get_tx_telemetry__from_t);
  case tlm_antenna:
    return sizeof(isis_ants__get_all_telemetry__from_t) * 2;
  case tlm_solar:
    return sizeof(int32_t) * 8;
  case tlm_wod:
    return sizeof(WOD_Telemetry_t);
  case tlm_rx:
    return sizeof(isis_vu_e__get_rx_telemetry__from_t);
  case tlm_log:
    return sizeof(logData_t);
  default:
    return 0;
  }
}
