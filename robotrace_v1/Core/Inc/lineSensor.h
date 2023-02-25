#ifndef LINESENSOR_H_
#define LINESENSOR_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
#define NUM_SENSORS      12
#define THITA_SENSOR     11.0F       // ラインセンサの間隔(角度)

//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t		lSensor[12];
extern float        angleSensor;
extern float		lSensorf[NUM_SENSORS];
extern uint8_t      modeCalLinesensors;

//====================================//
// プロトタイプ宣言
//====================================//
void powerLinesensors(uint8_t onoff);
void getLineSensor(void);
void getAngleSensor(void);
void calibrationLinesensor (void);

#endif // LINESENSOR_H_
