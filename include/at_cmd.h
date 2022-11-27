#ifndef AT_CMD_H
#define AT_CMD_H

/* R = Rx*/
/* T = Tx*/
/* Q = Request (denoted with ?) */
/* S = Response (indicated by =) */
/* C = Command (no response, denoted by !) */
/* D = Contains raw data*/

#define AT_CMD_TS_CMD_INVALID       "AT+CMD_INVALID\n"      // invalid command

#define AT_CMD_RQ_STATUS          "AT+STATUS?\n"
#define AT_CMD_TS_STATUS_READY    "AT+STATUS=READY\n"
#define AT_CMD_TS_STATUS_BUSY     "AT+STATUS=BUSY\n"
#define AT_CMD_TS_STATUS_ERROR    "AT+STATUS=ERROR\n"   // problem with system

#define AT_CMD_RD_IMAGE "AT+IMAGE=\n"    // send image data in raw format, for now the length is fized to 28 * 28 * 3 = 2352 bytes

#define AT_CMD_RC_RESET "AT+RESET!\n"     // reset the device

#endif