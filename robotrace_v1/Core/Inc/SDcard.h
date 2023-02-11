#ifndef SDCARD_H_
#define SDCARD_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
#include "fatfs_sd.h"
//====================================//
// シンボル定義
//====================================//
#define PERIOD_LOG  2
#define BUFFER_SIZW_LOG  1024
//====================================//
// グローバル変数の宣言
//====================================//
extern uint8_t   insertMSD;
//====================================//
// プロトタイプ宣言
//====================================//
// MicroSD
void initMicroSD(void);
void initLog(void);
void endLog(void);
void setLogStr(uint8_t* column, uint8_t* format);
void writeLogBuffer (uint8_t valNum, ...);
void writeLogPut(void);
void readLog(void);

#endif // SDCARD_H_
