//====================================//
// インクルード
//====================================//
#include "BNO055.h"

double gyroValX, gyroValY, gyroValZ;
double acceleValX, acceleValY, acceleValZ;
double angleX=0, angleY=0, angleZ=0;
static uint8_t rawAcceleVal[8], rawGyroVal[8];
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
    uint8_t rx_buf[1], tx_buf[1] = {reg, data};
    uint8_t rx_num = 1, tx_num = 2;

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
        writeBNO055(BNO055_GYR_CONFIG_0_ADDR, 0x08);  // 2000dps 230Hz 

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
    readBNO055AxisData(BNO055_ACCEL_DATA_X_LSB_ADDR, rawAcceleVal);
    acceleValZ = (double)((int16_t)( rawAcceleVal[1] * 0x100 + rawAcceleVal[0] )) / ACCELELSB;
    acceleValY = (double)((int16_t)( rawAcceleVal[3] * 0x100 + rawAcceleVal[2] )) / ACCELELSB;
    acceleValX = (double)((int16_t)( rawAcceleVal[5] * 0x100 + rawAcceleVal[4] )) / ACCELELSB;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getGyro
// 処理概要     角速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getBNO055Gyro(void) {
    int16_t x,y,z;
    readBNO055AxisData(BNO055_GYRO_DATA_X_LSB_ADDR, rawGyroVal);
    x = (int16_t)( rawGyroVal[1] * 0x100 + rawGyroVal[0] );
    y = (int16_t)( rawGyroVal[3] * 0x100 + rawGyroVal[2] );
    z = (int16_t)( rawGyroVal[5] * 0x100 + rawGyroVal[4] );
    gyroValX = ((double)x) / GYROLSB;
    gyroValY = ((double)y) / GYROLSB;
    gyroValZ = ((double)z) / GYROLSB;   
}
/////////////////////////////////////////////////////////////////////
// モジュール名 calcDegrees
// 処理概要     角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcDegrees(void) {
    angleX += gyroValX * 0.0056;
    angleY += gyroValY * 0.0056;
    angleZ += gyroValZ * 0.0056;   
}