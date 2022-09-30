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

typedef struct {
    uint8_t index;     // ソート前のインデックス（要素番号）
    uint16_t value;     // ソート対象の値
} lSensors;
//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t		lSensor[12];
extern lSensors     lSensors_list[12];
extern double       angleSensor;
//====================================//
// プロトタイプ宣言
//====================================//
void getLineSensor(void);
int cmpare_lSensors(const void *n1, const void *n2);
void getAngleSensor(void);
void print_lSensors_list(const lSensors *list, const uint16_t n);

#endif // LINESENSOR_H_
