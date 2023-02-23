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

// ID
#define INA260_Manufacturer_ID  0x5449
#define INA260_Die_ID            0x2270

#define LSB_CURRENT 1.25    //[mA]
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t rawCurrentR, rawCurrentL;
extern float CurrntL, CurrntR;

/******************************** 自動生成関数 *********************************/
#define I2C_INA260_MEMWRITE HAL_I2C_Mem_Write(&hi2c1,addr,reg,I2C_MEMADD_SIZE_8BIT,tx_buf,tx_num,10);
#define I2C_INA260_MEMREAD  HAL_I2C_Mem_Read(&hi2c1,addr | 0x0001,reg,I2C_MEMADD_SIZE_8BIT,rx_buf,rx_num,10);
/******************************************************************************/
//====================================//
// プロトタイプ宣言
//====================================//
uint16_t readINA260( uint16_t addr, uint8_t reg );
void writeINA260( uint16_t addr, uint8_t reg, uint16_t data );
bool initINA260( void );
void getCurrent( void );

#endif // INA260_H_
