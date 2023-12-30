/*
 * hx711.c
 *
 *  Created on: Dec 30, 2023
 *      Author: Marcin Kosela (KoSik)
 *		e-mail: kosik84@gmail.com
 *		
 *	   version: 1.0
 */


#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "main.h"
#include "hx711.h"

struct HX711 hx711;


HAL_StatusTypeDef hx711_init(GPIO_TypeDef *dtPort, uint16_t dtPin, GPIO_TypeDef *sckPort, uint16_t sckPin){
	int32_t data;

	hx711.DtPort = dtPort;
	hx711.DtPin = dtPin;
	hx711.SckPort = sckPort;
	hx711.SckPin = sckPin;
	HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 0);
	hx711_readyForRetrieval();
	data = hx711_read();
	hx711_turnOff();
	if(data != 0){
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

HAL_StatusTypeDef hx711_tara(uint16_t zeroCycles){
	int32_t zeroWeight = 0;

	hx711_readyForRetrieval();
	zeroWeight = hx711_read();
	for(uint16_t i=0; i<zeroCycles; i++){
		zeroWeight += hx711_read();
		zeroWeight = zeroWeight / 2;
	}

	hx711.zeroWeight = zeroWeight;
	return HAL_OK;
}

int32_t hx711_read(void){
	uint32_t data = 0;

	hx711_readyForRetrieval();
	for (uint8_t i = 0; i < 24; i++) {
		HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 1);
		delayUs(1);
		HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 0);
		if(HAL_GPIO_ReadPin(hx711.DtPort, hx711.DtPin) == 1){
			data |= (1 << (24-i));
		}
		delayUs(1);
	}
	HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 1);
	delayUs(1);
	HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 0);
	if (data & 0x800000) {
		data = data | 0xFF000000;
	}
	return data;
}

int32_t hx711_getWeight(void){
	int32_t weight = hx711_read();
	return weight - hx711.zeroWeight;
}

void hx711_turnOff(void){
	HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 1);
}

void hx711_turnOn(void){
	int32_t data;

	hx711_readyForRetrieval();
	data = hx711_read();
	HAL_Delay(1);
}

HAL_StatusTypeDef hx711_readyForRetrieval(void){
	uint16_t timeout = 0;

	HAL_GPIO_WritePin(hx711.SckPort, hx711.SckPin, 0);
	while(HAL_GPIO_ReadPin(hx711.DtPort, hx711.DtPin) == 1){ //check data readiness
		delayUs(1);
		timeout++;
		if(timeout > 60000){
			return HAL_ERROR;
		}
	}
	return HAL_OK;
}

uint32_t getUs(void){
	uint32_t usTicks = HAL_RCC_GetSysClockFreq() / 1000000;
	register uint32_t ms, cycle_cnt;
	do {
		ms = HAL_GetTick();
		cycle_cnt = SysTick->VAL;
	} while (ms != HAL_GetTick());
	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void delayUs(uint16_t micros){
	uint32_t start = getUs();
	while (getUs() - start < (uint32_t) micros) {
		asm("nop");
	}
}
