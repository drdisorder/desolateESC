/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

// from https://www.rcgroups.com/forums/showatt.php?attachmentid=8524039&d=1450424877

uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
	uint8_t crc_u, i;
	crc_u = crc;
	crc_u ^= crc_seed;
	for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
	return (crc_u);
}
uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen){
	uint8_t crc = 0, i;
	for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
	return (crc);
}



void sendTelemetry(int16_t *TelemetryData){
	uint8_t tlmDataBuf[9];
	
	//temp
	tlmDataBuf[0] = TelemetryData[0];
	
	//voltage
	tlmDataBuf[1] = TelemetryData[1]>>8;
	tlmDataBuf[2] = TelemetryData[1]&0xFF;
	
	//current
	tlmDataBuf[3] = TelemetryData[2]>>8;
	tlmDataBuf[4] = TelemetryData[2]&0xFF;
	
	//consumption
	tlmDataBuf[5] = TelemetryData[3]>>8;
	tlmDataBuf[6] = TelemetryData[3]&0xFF;
	
	//2pole rpm
	tlmDataBuf[7] = TelemetryData[4]>>8;
	tlmDataBuf[8] = TelemetryData[4]&0xFF;
	
	//crc
	tlmDataBuf[9] = get_crc8(tlmDataBuf, 9);
	
	#ifdef UART_TLM
	TLMUARTsend(tlmDataBuf, 10);
	#endif
}