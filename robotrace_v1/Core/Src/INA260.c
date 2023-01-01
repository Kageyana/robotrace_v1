//======================================//
// インクルード
//======================================//
#include "INA260.h"
//====================================//
// グローバル変数の宣言
//====================================//
float CurrntL = 0, CurrntR = 0;
int16_t rawCurrentR, rawCurrentL;
//////////////////////////////////////////////////////////////////////////
// モジュール名 readINA260
// 処理概要     データ受信
// 引数         reg:レジスタのアドレス
// 戻り値       読み取ったデータ(16bit)
//////////////////////////////////////////////////////////////////////////
uint16_t readINA260( uint16_t addr, uint8_t reg )
{
    uint8_t rx_buf[2], tx_buf[1] = { reg };
	I2C_INA260_SEND
    I2C_INA260_READ

    return (uint16_t)( rx_buf[0] * 0x100 + rx_buf[1] );
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 writeINA260
// 処理概要     データ受信
// 引数         reg:レジスタのアドレス
// 戻り値       読み取ったデータ(16bit)
//////////////////////////////////////////////////////////////////////////
void writeINA260( uint16_t addr, uint8_t reg, uint16_t data )
{
    uint8_t tx_buf[2] = { reg, data};
	I2C_INA260_SEND2
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 initINA260
// 処理概要     INA260の初期化
// 引数         なし
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
void initINA260( void ) {
    // Current  conversion time = 8.244 ms
    // Vbus     conversion time = 1.1 ms
    writeINA260( INA260_SLAVEADDRESS_R, 0x00, 0x6b27 );
    HAL_Delay(50);
	writeINA260( INA260_SLAVEADDRESS_L, 0x00, 0x6b27 );
    HAL_Delay(50);
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 readCurrent
// 処理概要     電流値の取得
// 引数         なし
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
void getCurrent( void )
{
    // Rawデータ取得
    rawCurrentR = readINA260(INA260_SLAVEADDRESS_R, 0x01);
	if (rawCurrentR > 32767) rawCurrentR = ~rawCurrentR+0x8000;     // 2の補数計算(16bitの半分を超えたとき)
    rawCurrentL = readINA260(INA260_SLAVEADDRESS_L, 0x01);
	if (rawCurrentL > 32767) rawCurrentL = ~rawCurrentL+0x8000;  // 2の補数計算(16bitの半分を超えたとき)

    CurrntR = LSB_CURRENT * rawCurrentR;
    CurrntL = LSB_CURRENT * rawCurrentL;
}
