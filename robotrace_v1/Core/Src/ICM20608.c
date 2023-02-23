//==========================//
// インクルード
//==========================//
#include "ICM20608.h"
//==========================//
// グローバル変数の宣言
//==========================//
volatile int16_t	xa, ya, za; // 加速度(16bitデータ)
volatile int16_t	xg, yg, zg;	// 角加速度(16bitデータ)
volatile uint8_t	who_am_i,ret,imuflag;
volatile int16_t	offset[3] = { 0,0,0 };	// オフセット値
volatile char 	caribration;

float 		TurningAngleIMU;	// yaw軸角度
float		RollAngleIMU;		// Roll角度
float 		PichAngleIMU;		// Pich角度
/////////////////////////////////////////////////////////////////////
// モジュール名 readByte
// 処理概要     指定レジスタの値を読み出す
// 引数         reg: レジスタのアドレス
// 戻り値       読み出した値
////////////////////////////////////////////////////////////////////
uint8_t readByte( uint8_t reg ) {
	uint8_t ret,val;
	
	ret = reg | 0x80;
	CS_RESET;
	SPITRANSFER_REGISTER;
	SPIRECEIVE_SINGLE;
	CS_SET;
	
	return val;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 writeByte
// 処理概要     指定レジスタに値を書き込む
// 引数         reg: レジスタのアドレス val: 書き込む値
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void writeByte( uint8_t reg, uint8_t val )  {
	uint8_t ret;
	
	ret = reg & 0x7F;
	CS_RESET;
	SPITRANSFER_REGISTER;
	SPITRANSFER_DATA;
	CS_SET;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readArry
// 処理概要     指定レジスタから連続読み出し
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void readArry( uint8_t reg, uint8_t *val2 ) {
	uint16_t ret, val;
	
	ret = reg | 0x80;
	CS_RESET;
	SPITRANSFER_REGISTER;
	SPIRECEIVE_ARRY;
	CS_SET;
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 initIMU
// 処理概要     IMUの初期化
// 引数         なし
// 戻り値       1: 成功 0: 失敗
////////////////////////////////////////////////////////////////////
uint8_t initICM20608() {
    uint8_t ret;
	ret = readByte(0x75);	//IMU動作確認　0xE0が送られてくればおｋ
	
	return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readGyroData
// 処理概要     角速度データの読み出し
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void readGyroData() {
	uint8_t val[6];
	readArry(GYRO_XOUT_H, val);
	
	xg = (int16_t)((val[0] << 8 & 0xff00 ) | val[1]);
	yg = (int16_t)((val[2] << 8 & 0xff00 ) | val[3]);
	zg = (int16_t)((val[4] << 8 & 0xff00 ) | val[5]);
	/*
	if ( xg < 0 ) xg += offset[0];
	else		xg -= offset[0];
	if ( yg < 0 ) yg += offset[1];
	else		yg -= offset[1];
	if ( zg < 0 ) zg += offset[2];
	else		zg -= offset[2];*/
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readAccelData
// 処理概要     加速度データの読み出し
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void readAccelData() {
	uint8_t val[6];
	readArry(ACCEL_XOUT_H, val);
	
	xa = ((int16_t)val[0] << 8) | ((int16_t)val[1]);
	ya = ((int16_t)val[2] << 8) | ((int16_t)val[3]);
	za = ((int16_t)val[4] << 8) | ((int16_t)val[5]);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getTurningAngleIMU
// 処理概要   	IMUから旋回角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getTurningAngleIMU(void)
{
	float angularVelocity_zg;
	
	angularVelocity_zg = (float)zg / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	TurningAngleIMU += angularVelocity_zg * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getRollAngleIMU
// 処理概要   	IMUからロール角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getRollAngleIMU(void)
{
	float angularVelocity_yg;
	
	angularVelocity_yg = (float)yg / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	RollAngleIMU -= angularVelocity_yg * DELTATIMU;
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getPichAngleIMU
// 処理概要     IMUからピッチ角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getPichAngleIMU( void )
{
	float angularVelocity_xg;
	
	angularVelocity_xg = (float)xg / GYROLSB;	// IMUのデータを角速度[deg/s]に変換
	
	PichAngleIMU -= angularVelocity_xg * DELTATIMU;
	
}
