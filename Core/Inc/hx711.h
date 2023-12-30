/*
 * hx711.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Marcin Kosela (KoSik)
 *		e-mail: kosik84@gmail.com
 *		
 *	   version: 1.0
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

struct HX711 {
	int32_t zeroWeight;
	GPIO_TypeDef *DtPort;
	uint16_t DtPin;
	GPIO_TypeDef *SckPort;
	uint16_t SckPin;
};

HAL_StatusTypeDef hx711_init(GPIO_TypeDef *dtPort, uint16_t dtPin, GPIO_TypeDef *sckPort, uint16_t sckPin);
HAL_StatusTypeDef hx711_tara(uint16_t zeroCount);
int32_t hx711_read(void);
int32_t hx711_getWeight(void);
void hx711_turnOff(void);
void hx711_turnOn(void);
HAL_StatusTypeDef hx711_readyForRetrieval(void);
uint32_t getUs(void);
void delayUs(uint16_t micros);

#endif /* INC_HX711_H_ */
