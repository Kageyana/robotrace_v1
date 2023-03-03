//====================================//
// インクルード
//====================================//
#include "BNO055.h"
//====================================//
// グローバル変数の宣言
//====================================//
axis accele = { 0.0F, 0.0F, 0.0F};
axis gyro = { 0.0F, 0.0F, 0.0F};
axis angle = { 0.0F, 0.0F, 0.0F};
IMUval 	BNO055val;

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

        // モード変更
        writeBNO055(BNO055_PAGE_ID_ADDR, 0x00);     // ページ0に変更
        writeBNO055(BNO055_UNIT_SEL_ADDR, 0x82);    // 角速度の単位を[rad/s]に変更
        writeBNO055(BNO055_OPR_MODE_ADDR, 0x05);    // 加速度、角速度計測モードに変更
        HAL_Delay(50);    //変更まで待つ

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
    int16_t accelVal[3];

    // 加速度の生データを取得
    readBNO055AxisData(BNO055_ACCEL_DATA_X_LSB_ADDR, rawData);

    accelVal[0] = (int16_t)( rawData[1] * 0x100 + rawData[0] );
    accelVal[1] = (int16_t)( rawData[3] * 0x100 + rawData[2] );
    accelVal[2] = (int16_t)( rawData[5] * 0x100 + rawData[4] );
    BNO055val.accele.x = (float)accelVal[0] / ACCELELSB;
    BNO055val.accele.y = (float)accelVal[1] / ACCELELSB;
    BNO055val.accele.z = (float)accelVal[2] / ACCELELSB;  
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getGyro
// 処理概要     角速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getBNO055Gyro(void) {
    uint8_t rawData[6];
    int16_t gyroVal[3];

    // 角速度の生データを取得
    // readBNO055AxisData(BNO055_GYRO_DATA_X_LSB_ADDR, rawData);

    // gyroVal[0] = (int16_t)( rawData[1] * 0x100) + rawData[0] ;
    // gyroVal[1] = (int16_t)( rawData[3] * 0x100) + rawData[2] ;
    // gyroVal[2] = (int16_t)( rawData[5] * 0x100) + rawData[4] ;
    // BNO055val.gyro.x = (float)gyroVal[0] / GYROLSB * 1.2;
    // BNO055val.gyro.y = (float)gyroVal[1] / GYROLSB * 1.2;
    // BNO055val.gyro.z = (float)gyroVal[2] / GYROLSB * 1.2;

    readBNO055OneAxisData(BNO055_GYRO_DATA_Z_LSB_ADDR, rawData);
    
    gyroVal[2] = (int16_t)( rawData[1] * 0x100) + rawData[0] ;
    BNO055val.gyro.z = (float)gyroVal[2] / GYROLSB * 1.07;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 calcDegrees
// 処理概要     角度の計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcDegrees(void) {
    BNO055val.angle.x += BNO055val.gyro.x * DEFF_TIME;
    BNO055val.angle.y += BNO055val.gyro.y * DEFF_TIME;
    BNO055val.angle.z += BNO055val.gyro.z * DEFF_TIME;   
}
