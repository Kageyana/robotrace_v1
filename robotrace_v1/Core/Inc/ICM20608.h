#ifndef SPI_ICM20648_H_
#define SPI_ICM20648_H_
//==========================//
// インクルード
//==========================//
#include "main.h"
#include <math.h>
//==========================//
// シンボル定義
//==========================//
#define WHO_AM_I			0x75
#define GYRO_XOUT_H         0x43
#define ACCEL_XOUT_H        0x3b

/*************************************** 自動生成関数 *************************************/
#define CS_RESET 		        HAL_GPIO_WritePin(CS_IMU_GPIO_Port, CS_IMU_Pin, GPIO_PIN_RESET)
#define CS_SET   		        HAL_GPIO_WritePin(CS_IMU_GPIO_Port, CS_IMU_Pin, GPIO_PIN_SET)
#define SPITRANSFER_REGISTER	HAL_SPI_Transmit(&hspi2,&ret,1,1000)
#define SPITRANSFER_DATA	    HAL_SPI_Transmit(&hspi2,&val,1,1000)
#define SPIRECEIVE_SINGLE       HAL_SPI_Receive(&hspi2,&val,1,1000)
#define SPIRECEIVE_ARRY	        HAL_SPI_Receive(&hspi2,&val,6,1000)
/*****************************************************************************************/
#define ACCELLSB			4096		// 16[g]
// #define GYROLSB			    32.8		// 2000[deg/s]
#define TEMP_LSB			333.87	    // LSB/°C
#define ROOMTEMPOFFSET	    0		    // 21°Cのとき0

#define MAXDATA_RANGE		32764	    // 16bitデータの最大値
#define G_ACCELERATION		9.81		// 重力加速度
#define DELTATIMU			0.01F	    // 取得周期
#define SAMPLE				5000		// キャリブレーション回数
//==========================//
// グローバル変数の宣言
//==========================//
// IMUから取得したデータ
extern volatile int16_t 	xa, ya, za; // 加速度(16bitデータ)
extern volatile int16_t 	xg, yg, zg;	// 角加速度(16bitデータ)
extern volatile uint8_t		who_am_i,ret,imuflag;
extern volatile int16_t		offset[3];	// オフセット値
extern volatile char 		caribration;

extern float 		TurningAngleIMU;	// yaw軸角度
extern float		RollAngleIMU;		// Roll角度
extern float 		PichAngleIMU;		// Pich角度
//==========================//
// プロトタイプ宣言
//==========================//
uint8_t readByte( uint8_t );
void writeByte( uint8_t, uint8_t);
void readArry( uint8_t reg, uint8_t *val);
uint8_t initICM20608(void);
void caribrateIMU(void);
void readGyroData(void);
void readAccelData(void);
void getTurningAngleIMU(void);
void getRollAngleIMU(void);
void getPichAngleIMU( void );

#endif // SPI_ICM20648_H_
