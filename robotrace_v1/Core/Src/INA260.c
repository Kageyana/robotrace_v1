//======================================//
// インクルード
//======================================//
#include "INA260.h"
//====================================//
// グローバル変数の宣言
//====================================//
double CurrntL = 0, CurrntR = 0;

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

    return (uint16_t)( (uint8_t)rx_buf[0] * 0x100 + (uint8_t)rx_buf[1] );
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 lcdPut
// 処理概要     データ送信
// 引数         data
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
void readCurrent( void )
{
    int16_t rawCurrentR, rawCurrentL;
    
    // Rawデータ取得
    rawCurrentR = readINA260(INA260_SLAVEADDRESS_R, 0x01);
	if (rawCurrentR > 32767) rawCurrentR = ~rawCurrentR+0x8000;     // 2の補数計算(16bitの半分を超えたとき)
    rawCurrentL = readINA260(INA260_SLAVEADDRESS_L, 0x01);
	if (rawCurrentL > 32767) rawCurrentL = ~rawCurrentL+0x8000;  // 2の補数計算(16bitの半分を超えたとき)

    CurrntR = LSB_CURRENT * rawCurrentR;
    CurrntL = LSB_CURRENT * rawCurrentL;
}