
#ifndef SPL_H_
#define SPL_H_

typedef enum __attribute__((__packed__)) spl_command_type_t {
  trxvu_cmd_type,
  eps_cmd_type,
  telemetry_cmd_type, // 2
  filesystem_cmd_type,
  managment_cmd_type,
  ack_type, // 5
  dump_type,
} spl_command_type;

typedef enum __attribute__((__packed__)) ack_subtype_t {
  ACK_RECEIVE_COMM = 0,
  ACK_COMD_EXEC = 1,
  ACK_RESET_WAKEUP = 2,
  ACK_FS_RESET = 3,
  ACK_TRXVU_SOFT_RESET = 4,
  ACK_TRXVU_HARD_RESET = 5,
  ACK_ANTS_RESET = 6,
  ACK_EPS_RESET = 7,
  ACK_SOFT_RESET = 8,
  ACK_HARD_RESET = 9,
  ACK_MEMORY_DELETE = 10,
  ACK_UPDATE_TIME = 11,
  ACK_UPDATE_BEACON_BIT_RATE = 12,
  ACK_UPDATE_BEACON_TIME_DELAY = 13,
  ACK_UPDATE_EPS_VOLTAGES = 14,
  ACK_UPDATE_EPS_HEATER_VALUES = 15,
  ACK_UPDATE_EPS_ALPHA = 16,
  ACK_MUTE = 17,
  ACK_UNMUTE = 18,
  ACK_ALLOW_TRANSPONDER = 19,
  ACK_DUMP_START = 20,
  ACK_DUMP_ABORT = 21,
  ACK_DUMP_FINISHED = 22,
  ACK_GENERIC_I2C_CMD = 23,
  ACK_ARM_DISARM = 24,
  ACK_REDEPLOY = 25,
  ACK_RESET_DELAYED_CMD = 26,
  ACK_FRAM_RESET = 27,
  ACK_DELETE_TLM = 28,
  ACK_PING = 29,
  ACK_UNKNOWN_SUBTYPE = 30,
  ACK_ERROR_MSG = 31
} ack_subtype_t;

typedef enum __attribute__((__packed__)) trxvu_subtypes_t {
  PING = 0,
  BEACON_SUBTYPE = 1,
  MUTE_TRXVU = 2,
  UNMUTE_TRXVU = 3,
  TRXVU_IDLE = 4,
  DUMP_DAYS = 5,
  DUMP_TIME_RANGE = 6,
  ABORT_DUMP_SUBTYPE = 7,
  GET_BAUD_RATE = 8,
  GET_BEACON_INTERVAL = 9,
  SET_BEACON_INTERVAL = 10,
  TRANSMIT_BEACON = 11,
  SET_BAUD_RATE = 12,
  SET_BEACON_CYCLE_TIME = 13,
  GET_TX_UPTIME = 14,
  GET_RX_UPTIME = 15,
  GET_NUM_OF_ONLINE_CMD = 16,
  SET_RSSI_TRANSPONDER = 17,
  SET_TRANSPONDER = 18,
  ANT_SET_ARM_STATUS = 19,
  ANT_GET_ARM_STATUS = 20,
  ANT_GET_UPTIME = 21,
  ANT_CANCEL_DEPLOY = 22,
  ANT_DEPLOY = 23,
  ANT_STOP_REDEPLOY = 24,
  FORCE_ABORT_DUMP_SUBTYPE = 25,
  DELETE_DUMP_TASK = 26
} trxvu_subtypes_t;

typedef enum __attribute__((__packed__)) eps_subtypes_t {
  UPDATE_ALPHA = 1,
  GET_HEATER_VALUES = 2,
  SET_HEATER_VALUES = 3,
  RESET_EPS_WDT = 4,
  GET_THRESHOLD_VOLTAGES = 5,
  UPDATE_THRESHOLD_VOLTAGES = 6,
  GET_SOLAR_PANEL_STATE = 7,
  SOLAR_PANEL_SLEEP = 8,
  SOLAR_PANEL_WAKE = 9,

  ENTER_FULL_MODE = 10,
  ENTER_CRITICAL_MODE = 11,
  ENTER_SAFE_MODE = 12,
  GET_CURRENT_MODE = 13,

  SET_3V3_CHANNELS_ON = 14,
  SET_3V3_CHANNELS_OFF = 15,
  SET_5V_CHANNELS_ON = 16,
  SET_5V_CHANNELS_OFF = 17
} eps_subtypes_t;

typedef enum __attribute__((__packed__)) telemetry_subtypes_t {
  DELETE_FILE = 1,      // 0b10101010
  DELETE_ALL_FILES = 2, // 0b10101010
  GET_LAST_FS_ERROR = 3,
  SET_TLM_PERIOD = 4,
  GET_TLM_PERIOD = 5,
  GET_IMAGE_INFO = 6,
  GET_IMAGE_DATA = 7
} telemetry_subtypes_t;

typedef enum __attribute__((__packed__)) management_subtypes_t {
  SOFT_RESET_SUBTYPE = 0,       // 0b10101010
  HARD_RESET_SUBTYPE = 1,       // 0b10101010
  TRXVU_SOFT_RESET_SUBTYPE = 2, // 0b11000011
  TRXVU_HARD_RESET_SUBTYPE = 3, // 0b00111100
  EPS_RESET_SUBTYPE = 4,        // 0b10111011
  FS_RESET_SUBTYPE = 5,         // 0b11001100
  UPDATE_SAT_TIME = 6,          // 0b‭11011101‬
  GENERIC_I2C_CMD = 7,          // 0b‭11011111‬
  RESET_COMPONENT = 8,          // 0b‭11011110
  FRAM_WRITE_AND_TRANSMIT = 9,  // 0b11100000
  FRAM_READ_AND_TRANSMIT = 10,
  FRAM_RESTART = 11,
  GET_SAT_UPTIME = 12
} management_subtypes_t;

#endif /* SPL_H_ */
