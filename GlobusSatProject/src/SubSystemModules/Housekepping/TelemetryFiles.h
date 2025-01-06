
#ifndef TELEMETRYFILES_H_
#define TELEMETRYFILES_H_
//	---general

#define END_FILE_NAME_TX_REVC			"TXR"
#define END_FILE_NAME_TX				"TX"
#define END_FILE_NAME_RX				"RX"
#define END_FILE_NAME_RX_REVC           "RXR"
#define END_FILE_NAME_RX_FRAME 			"RXF"
#define END_FILE_NAME_ANTENNA			"ANT"
#define END_FILENAME_WOD_TLM			"WOD"
#define	END_FILENAME_EPS_RAW_MB_TLM		"ERM"
#define END_FILENAME_EPS_TLM			"EPS"
#define END_FILENAME_EPS_RAW_CDB_TLM	"ERC"
#define END_FILENAME_EPS_ENG_CDB_TLM	"ECC"
#define	END_FILENAME_SOLAR_PANELS_TLM	"SLR"
#define	END_FILENAME_LOGS				"LOG"


typedef enum tlm_type{
	tlm_eps,
	tlm_tx,
	tlm_antenna,
	tlm_solar,
	tlm_wod,
	tlm_rx,
	tlm_rx_frame,
	tlm_log
}tlm_type_t;
#endif /* TELEMETRYFILES_H_ */
