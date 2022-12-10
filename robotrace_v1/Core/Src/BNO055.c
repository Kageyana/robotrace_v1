//====================================//
// インクルード
//====================================//
#include "BNO055.h"

float acceleration[3] = {0,0,0};
float angularVelocity[3] = {0,0,0};
float angle[3] = {0,0,0};
int16_t accelVal[3], gyroVal[3];
/////////////////////////////////////////////////////////////////////
// モジュール名 readBNO055
// 処理概要     指定レジスタの読み出し
// 引数         reg:レジスタアドレス
// 戻り値       読み出したデータ
/////////////////////////////////////////////////////////////////////
uint8_t readBNO055(uint8_t reg) {
    uint8_t rx_buf[1], tx_buf[1] = {reg};
    uint8_t rx_num = 1, tx_num = 1;

    I2C_BNO055_SEND
    I2C_BNO055_READ

    return rx_buf[0];
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readBNO055
// 処理概要     指定レジスタの読み出し
// 引数         reg:レジスタアドレス
// 戻り値       読み出したデータ
/////////////////////////////////////////////////////////////////////
void readBNO055AxisData(uint8_t reg, uint8_t *rx_buf ) {
    uint8_t tx_buf[1] = {reg};
    uint8_t rx_num = 6, tx_num = 1;

    I2C_BNO055_SEND
    I2C_BNO055_READ
}
/////////////////////////////////////////////////////////////////////
// モジュール名 writeBNO055
// 処理概要     指定レジスタへ書き込み
// 引数         reg:レジスタアドレス data: 書き込むデータ
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void writeBNO055(uint8_t reg, uint8_t data) {
    uint8_t tx_buf[2] = {reg, data};
    uint8_t tx_num = 2;

    I2C_BNO055_SEND
}
/////////////////////////////////////////////////////////////////////
// モジュール名 initBNO055
// 処理概要     指定レジスタへ書き込み
// 引数         reg:レジスタアドレス data: 書き込むデータ
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void initBNO055(void) {
    if(readBNO055(BNO055_CHIP_ID_ADDR) == BNO055_CHIP_ID) {
        // コンフィグ設定
        writeBNO055(BNO055_PAGE_ID_ADDR, 0x01); // ページ1に変更

        // 加速
        writeBNO055(BNO055_ACC_CONFIG_ADDR, 0x17);    // 16G 250Hz
        // ジャイロ
        writeBNO055(BNO055_GYR_CONFIG_0_ADDR, 0x00);  // 2000dps 523Hz 

        // モード変更
        writeBNO055(BNO055_PAGE_ID_ADDR, 0x00);     // ページ0に変更
        writeBNO055(BNO055_OPR_MODE_ADDR, 0x05);    // ノーマルモード
        HAL_Delay(50);    //変更まで待つ
    }
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getAcceleration
// 処理概要     加速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getBNO055Acceleration(void) {
	uint8_t rawData[8];
    readBNO055AxisData(BNO055_ACCEL_DATA_X_LSB_ADDR, rawData);

    accelVal[INDEX_X] = (int16_t)( rawData[1] * 0x100 + rawData[0] );
    accelVal[INDEX_Y] = (int16_t)( rawData[3] * 0x100 + rawData[2] );
    accelVal[INDEX_Z] = (int16_t)( rawData[5] * 0x100 + rawData[4] );
    acceleration[INDEX_X] = (float)accelVal[INDEX_X] / ACCELELSB;
    acceleration[INDEX_Y] = (float)accelVal[INDEX_Y] / ACCELELSB;
    acceleration[INDEX_Z] = (float)accelVal[INDEX_Z] / ACCELELSB;  
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getGyro
// 処理概要     角速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getBNO055Gyro(void) {
    uint8_t rawData[8];
    readBNO055AxisData(BNO055_GYRO_DATA_X_LSB_ADDR, rawData);

    gyroVal[INDEX_X] = (int16_t)( rawData[1] * 0x100 + rawData[0] );
    gyroVal[INDEX_Y] = (int16_t)( rawData[3] * 0x100 + rawData[2] );
    gyroVal[INDEX_Z] = (int16_t)( rawData[5] * 0x100 + rawData[4] );
    angularVelocity[INDEX_X] = (float)gyroVal[INDEX_X] / GYROLSB;
    angularVelocity[INDEX_Y] = (float)gyroVal[INDEX_Y] / GYROLSB;
    angularVelocity[INDEX_Z] = (float)gyroVal[INDEX_Z] / GYROLSB;   
}
/////////////////////////////////////////////////////////////////////
// モジュール名 calcDegrees
// 処理概要     角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcDegrees(void) {
    angle[INDEX_X] += angularVelocity[INDEX_X] * 0.0056;
    angle[INDEX_Y] += angularVelocity[INDEX_Y] * 0.0056;
    angle[INDEX_Z] += angularVelocity[INDEX_Z] * 0.0056;   
}
