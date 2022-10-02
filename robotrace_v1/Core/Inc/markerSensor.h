#ifndef MARKERSENSOR_H_
#define MARKERSENSOR_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
#define RIGHTMARKER      0x1
#define LEFTMARKER       0x2
#define CROSSLINE       0x3

#define STARTMARKER      0x1
#define GOALMARKER       0x2
//====================================//
// グローバル変数の宣言
//====================================//
extern  uint8_t     SGmarker;
extern  int32_t		encMarker;
//====================================//
// プロトタイプ宣言
//====================================//
uint8_t getMarkerSensor ( void );
uint8_t checkMarker( void );

#endif // MARKERSENSOR_H_
