#ifndef AQM0802_H_
#define AQM0802_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
//====================================//
// シンボル定義
//====================================//
// スレーブアドレス
#define LCD_SLAVEADDRESS	0x007c
// 液晶関連変数
#define LCD_MAX_X	17		// 表示文字数 横 16 or 20
#define LCD_MAX_Y	2		// 表示文字数 縦  2 or  4
#define RSBIT0		0x00	// コマンド送信ビット
#define RSBIT1		0x40	// データ送信ビット
#define UPROW       0       // 1行目
#define LOWROW      1       // 2行目

/******************************** 自動生成関数 *********************************/
#define I2C_LCD_SEND	HAL_I2C_Master_Transmit(&hi2c1,LCD_SLAVEADDRESS,tx_buf,2,1);
/******************************************************************************/
//====================================//
// グローバル変数の宣言
//====================================//

//====================================//
// プロトタイプ宣言
//====================================//
// LCD関連
void lcdShowProcess (void);
void lcdPosition (uint8_t x ,uint8_t y);
bool intiLcd (void);
void writeLCDCMD (uint8_t cmd);
void writeLCDData (uint8_t data);
int lcdPrintf (uint8_t *format, ...);
void lcdcursol (void);
int lcdRowPrintf (uint8_t step, char *format, ...);

#endif // AQM0802_H_
