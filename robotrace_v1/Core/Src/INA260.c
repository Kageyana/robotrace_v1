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
    uint8_t rx_buf[2], rx_num = 2;
	I2C_INA260_MEMREAD

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
    uint8_t tx_buf[1] = {data}, tx_num = 1;
	I2C_INA260_MEMWRITE
}
//////////////////////////////////////////////////////////////////////////
// モジュール名 initINA260
// 処理概要     INA260の初期化
// 引数         なし
// 戻り値       なし
//////////////////////////////////////////////////////////////////////////
bool initINA260( void ) {
    if (readINA260(INA260_SLAVEADDRESS_L, 0xFE) == INA260_Manufacturer_ID && readINA260(INA260_SLAVEADDRESS_R, 0xFE) == INA260_Manufacturer_ID) {
        // Current  conversion time = 8.244 ms
        // Vbus     conversion time = 1.1 ms
        writeINA260( INA260_SLAVEADDRESS_R, 0x00, 0x6b27 );
        HAL_Delay(50);
        writeINA260( INA260_SLAVEADDRESS_L, 0x00, 0x6b27 );
        HAL_Delay(50);

        printf("Manufacturer ID 0x%x\n",readINA260(INA260_SLAVEADDRESS_R, 0xFE));
        printf("Manufacturer ID 0x%x\n",readINA260(INA260_SLAVEADDRESS_L, 0xFE));

        printf("Die ID Register ID 0x%x\n",readINA260(INA260_SLAVEADDRESS_R, 0xFF));
        printf("Die ID Register ID 0x%x\n",readINA260(INA260_SLAVEADDRESS_L, 0xFF));

        return true;
    } else {
        return false;
    }
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
