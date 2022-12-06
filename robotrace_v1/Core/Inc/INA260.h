#ifndef INA260_H_
#define INA260_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
// スレーブアドレス
#define INA260_SLAVEADDRESS_R	0x0080
#define INA260_SLAVEADDRESS_L	0x0088

#define LSB_CURRENT 1.25    //[mA]
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t rawCurrentR, rawCurrentL;
extern float CurrntL, CurrntR;

/******************************** 自動生成関数 *********************************/
#define 	I2C_INA260_SEND	    HAL_I2C_Master_Transmit(&hi2c1,addr,tx_buf,1,100);
#define 	I2C_INA260_SEND2    HAL_I2C_Master_Transmit(&hi2c1,addr,tx_buf,2,100);
#define	    I2C_INA260_READ	    HAL_I2C_Master_Receive(&hi2c1,addr | 0x0001,rx_buf,2,100);
/******************************************************************************/
//====================================//
// プロトタイプ宣言
//====================================//
uint16_t readINA260( uint16_t addr, uint8_t reg );
void writeINA260( uint16_t addr, uint8_t reg, uint16_t data );
void initINA260( void );
void getCurrent( void );

#endif // INA260_H_
