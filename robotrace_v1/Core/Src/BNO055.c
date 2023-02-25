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
    uint8_t rx_buf[1];
    uint8_t rx_num = 1;

    I2C_BNO055_MEMREAD

    return rx_buf[0];
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readBNO055AxisData
// 処理概要     指定レジスタの読み出し
// 引数         reg:レジスタアドレス
// 戻り値       読み出したデータ
/////////////////////////////////////////////////////////////////////
void readBNO055AxisData(uint8_t reg, uint8_t *rx_buf ) {
    uint8_t rx_num = 6;

    I2C_BNO055_MEMREAD
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readBNO055OneAxisData
// 処理概要     指定レジスタの読み出し
// 引数         reg:レジスタアドレス
// 戻り値       読み出したデータ
/////////////////////////////////////////////////////////////////////
void readBNO055OneAxisData(uint8_t reg, uint8_t *rx_buf ) {
    uint8_t rx_num = 2;

    I2C_BNO055_MEMREAD
}
/////////////////////////////////////////////////////////////////////
// モジュール名 writeBNO055
// 処理概要     指定レジスタへ書き込み
// 引数         reg:レジスタアドレス data: 書き込むデータ
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void writeBNO055(uint8_t reg, uint8_t data) {
    uint8_t tx_buf[1] = {data};
    uint8_t tx_num = 1;

    I2C_BNO055_MEMWRITE
}
/////////////////////////////////////////////////////////////////////
// モジュール名 initBNO055
// 処理概要     初期設定パラメータの書き込み
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
bool initBNO055(void) {
    if(readBNO055(BNO055_CHIP_ID_ADDR) == BNO055_CHIP_ID) {
        // コンフィグ設定
        writeBNO055(BNO055_OPR_MODE_ADDR, 0x00);    // コンフィグモードに変更
        writeBNO055(BNO055_PAGE_ID_ADDR, 0x01);     // ページ1に変更

        // 加速
        writeBNO055(BNO055_ACC_CONFIG_ADDR, 0x17);    // 16G 250Hz
        // ジャイロ
        writeBNO055(BNO055_GYR_CONFIG_0_ADDR, 0x00);  // 2000dps 523Hz

        // 書き込んだパラメータを確認
        printf("0x%x\n",readBNO055(BNO055_GYR_CONFIG_0_ADDR));

        // モード変更
        writeBNO055(BNO055_PAGE_ID_ADDR, 0x00);     // ページ0に変更
        writeBNO055(BNO055_UNIT_SEL_ADDR, 0x82);    // 角速度の単位を[rad/s]に変更
        writeBNO055(BNO055_OPR_MODE_ADDR, 0x05);    // 加速度、角速度計測モードに変更
        HAL_Delay(50);    //変更まで待つ

        // 書き込んだパラメータを確認
        printf("0x%x\n",readBNO055(BNO055_UNIT_SEL_ADDR));

        return true;
    } else {
        return false;
    }
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getAcceleration
// 処理概要     加速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getBNO055Acceleration(void) {
	uint8_t rawData[6];
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
    uint8_t rawData[6];
    // readBNO055AxisData(BNO055_GYRO_DATA_X_LSB_ADDR, rawData);

    // gyroVal[INDEX_X] = (int16_t)( rawData[1] * 0x100) + rawData[0] ;
    // gyroVal[INDEX_Y] = (int16_t)( rawData[3] * 0x100) + rawData[2] ;
    // gyroVal[INDEX_Z] = (int16_t)( rawData[5] * 0x100) + rawData[4] ;
    // angularVelocity[INDEX_X] = (float)gyroVal[INDEX_X] / GYROLSB * 1.2;
    // angularVelocity[INDEX_Y] = (float)gyroVal[INDEX_Y] / GYROLSB * 1.2;
    // angularVelocity[INDEX_Z] = (float)gyroVal[INDEX_Z] / GYROLSB * 1.2;

    readBNO055OneAxisData(BNO055_GYRO_DATA_Z_LSB_ADDR, rawData);
    
    gyroVal[INDEX_Z] = (int16_t)( rawData[1] * 0x100) + rawData[0] ;
    angularVelocity[INDEX_Z] = (float)gyroVal[INDEX_Z] / GYROLSB * 1.07;

    // readBNO055(BNO055_GYRO_DATA_Z_LSB_ADDR);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 calcDegrees
// 処理概要     角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcDegrees(void) {
    angle[INDEX_X] += angularVelocity[INDEX_X] * DEFF_TIME;
    angle[INDEX_Y] += angularVelocity[INDEX_Y] * DEFF_TIME;
    angle[INDEX_Z] += angularVelocity[INDEX_Z] * DEFF_TIME;   
}
