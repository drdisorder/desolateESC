/*This file is part of desolateESC CC BY-NC-SA*/
#include "main.h"

uint32_t InputBuf[32];
uint32_t newInput;
uint8_t telemetryRequest = 0;

uint16_t computeDshot(){
	static uint16_t lastInput = 0;
	if(newInput == 1){
		newInput = 0;
		uint8_t dshotBits[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		uint32_t frameLength = InputBuf[30]-InputBuf[0];
		uint32_t DshotBitWidth = frameLength>>4;
		uint32_t DshotBitOne = DshotBitWidth-(DshotBitWidth>>2);
		uint32_t DshotBitZero = DshotBitOne>>1;
		uint32_t DshotBitMargin = DshotBitWidth>>3; 
		for(uint8_t i=0;i<16;i++){
			uint32_t bitDiff = InputBuf[i*2+1]-InputBuf[i*2];
			if(bitDiff < DshotBitZero-DshotBitMargin || bitDiff > DshotBitOne+DshotBitMargin){
				return lastInput;
			}else if(bitDiff > DshotBitZero+DshotBitMargin) dshotBits[15-i] = 1;
		}
		uint8_t checkCRC = (
				(dshotBits[15]^dshotBits[11]^dshotBits[7])<<3
				|(dshotBits[14]^dshotBits[10]^dshotBits[6])<<2
				|(dshotBits[13]^dshotBits[9]^dshotBits[5])<<1
				|(dshotBits[12]^dshotBits[8]^dshotBits[4])
			);
		uint8_t recCRC = (dshotBits[3]<<3 | dshotBits[2]<<2 | dshotBits[1]<<1 | dshotBits[0]);
		
		if(checkCRC == recCRC){
			lastInput = (
				dshotBits[15]<<10 | dshotBits[14]<<9 | dshotBits[13]<<8 | dshotBits[12]<<7
				| dshotBits[11]<<6 | dshotBits[10]<<5 | dshotBits[9]<<4 | dshotBits[8]<<3
				| dshotBits[7]<<2 | dshotBits[6]<<1 | dshotBits[5]
			
			);
			inputFailsave = 0;
		}
	}
	return lastInput;
}