#ifndef BNO055_H_
#define BNO055_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
// スレーブアドレス
#define BNO055_SLAVEADDRESS	    0x0050
// チップID
#define BNO055_CHIP_ID          0xa0

// レジスタ
/* Page id register definition */
#define BNO055_PAGE_ID_ADDR         0X07

/* PAGE0 REGISTER DEFINITION START*/
#define BNO055_CHIP_ID_ADDR         0x00
#define BNO055_ACCEL_REV_ID_ADDR    0x01
#define BNO055_MAG_REV_ID_ADDR      0x02
#define BNO055_GYRO_REV_ID_ADDR     0x03
#define BNO055_SW_REV_ID_LSB_ADDR   0x04
#define BNO055_SW_REV_ID_MSB_ADDR   0x05
#define BNO055_BL_REV_ID_ADDR       0X06

/* Accel data register */
#define BNO055_ACCEL_DATA_X_LSB_ADDR   0X08
#define BNO055_ACCEL_DATA_X_MSB_ADDR   0X09
#define BNO055_ACCEL_DATA_Y_LSB_ADDR   0X0A
#define BNO055_ACCEL_DATA_Y_MSB_ADDR   0X0B
#define BNO055_ACCEL_DATA_Z_LSB_ADDR   0X0C
#define BNO055_ACCEL_DATA_Z_MSB_ADDR   0X0D

/* Mag data register */
#define BNO055_MAG_DATA_X_LSB_ADDR   0X0E
#define BNO055_MAG_DATA_X_MSB_ADDR   0X0F
#define BNO055_MAG_DATA_Y_LSB_ADDR   0X10
#define BNO055_MAG_DATA_Y_MSB_ADDR   0X11
#define BNO055_MAG_DATA_Z_LSB_ADDR   0X12
#define BNO055_MAG_DATA_Z_MSB_ADDR   0X13

/* Gyro data registers */
#define BNO055_GYRO_DATA_X_LSB_ADDR   0X14
#define BNO055_GYRO_DATA_X_MSB_ADDR   0X15
#define BNO055_GYRO_DATA_Y_LSB_ADDR   0X16
#define BNO055_GYRO_DATA_Y_MSB_ADDR   0X17
#define BNO055_GYRO_DATA_Z_LSB_ADDR   0X18
#define BNO055_GYRO_DATA_Z_MSB_ADDR   0X19

/* Euler data registers */
#define BNO055_EULER_H_LSB_ADDR   0X1A
#define BNO055_EULER_H_MSB_ADDR   0X1B
#define BNO055_EULER_R_LSB_ADDR   0X1C
#define BNO055_EULER_R_MSB_ADDR   0X1D
#define BNO055_EULER_P_LSB_ADDR   0X1E
#define BNO055_EULER_P_MSB_ADDR   0X1F

/* Quaternion data registers */
#define BNO055_QUATERNION_DATA_W_LSB_ADDR   0X20
#define BNO055_QUATERNION_DATA_W_MSB_ADDR   0X21
#define BNO055_QUATERNION_DATA_X_LSB_ADDR   0X22
#define BNO055_QUATERNION_DATA_X_MSB_ADDR   0X23
#define BNO055_QUATERNION_DATA_Y_LSB_ADDR   0X24
#define BNO055_QUATERNION_DATA_Y_MSB_ADDR   0X25
#define BNO055_QUATERNION_DATA_Z_LSB_ADDR   0X26
#define BNO055_QUATERNION_DATA_Z_MSB_ADDR   0X27

/* Linear acceleration data registers */
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR   0X28
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR   0X29
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR   0X2A
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR   0X2B
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR   0X2C
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR   0X2D

/* Gravity data registers */
#define BNO055_GRAVITY_DATA_X_LSB_ADDR   0X2E
#define BNO055_GRAVITY_DATA_X_MSB_ADDR   0X2F
#define BNO055_GRAVITY_DATA_Y_LSB_ADDR   0X30
#define BNO055_GRAVITY_DATA_Y_MSB_ADDR   0X31
#define BNO055_GRAVITY_DATA_Z_LSB_ADDR   0X32
#define BNO055_GRAVITY_DATA_Z_MSB_ADDR   0X33

/* Temperature data register */
#define BNO055_TEMP_ADDR   0X34,

/* Status registers */
#define BNO055_CALIB_STAT_ADDR          0X35
#define BNO055_SELFTEST_RESULT_ADDR     0X36
#define BNO055_INTR_STAT_ADDR           0X37

#define BNO055_SYS_CLK_STAT_ADDR    0X38
#define BNO055_SYS_STAT_ADDR        0X39
#define BNO055_SYS_ERR_ADDR         0X3A

/* Unit selection register */
#define BNO055_UNIT_SEL_ADDR   0X3B

/* Mode registers */
#define BNO055_OPR_MODE_ADDR   0X3D
#define BNO055_PWR_MODE_ADDR   0X3E

#define BNO055_SYS_TRIGGER_ADDR   0X3F
#define BNO055_TEMP_SOURCE_ADDR   0X40

/* Axis remap registers */
#define BNO055_AXIS_MAP_CONFIG_ADDR     0X41
#define BNO055_AXIS_MAP_SIGN_ADDR       0X42

/* SIC registers */
#define BNO055_SIC_MATRIX_0_LSB_ADDR   0X43
#define BNO055_SIC_MATRIX_0_MSB_ADDR   0X44
#define BNO055_SIC_MATRIX_1_LSB_ADDR   0X45
#define BNO055_SIC_MATRIX_1_MSB_ADDR   0X46
#define BNO055_SIC_MATRIX_2_LSB_ADDR   0X47
#define BNO055_SIC_MATRIX_2_MSB_ADDR   0X48
#define BNO055_SIC_MATRIX_3_LSB_ADDR   0X49
#define BNO055_SIC_MATRIX_3_MSB_ADDR   0X4A
#define BNO055_SIC_MATRIX_4_LSB_ADDR   0X4B
#define BNO055_SIC_MATRIX_4_MSB_ADDR   0X4C
#define BNO055_SIC_MATRIX_5_LSB_ADDR   0X4D
#define BNO055_SIC_MATRIX_5_MSB_ADDR   0X4E
#define BNO055_SIC_MATRIX_6_LSB_ADDR   0X4F
#define BNO055_SIC_MATRIX_6_MSB_ADDR   0X50
#define BNO055_SIC_MATRIX_7_LSB_ADDR   0X51
#define BNO055_SIC_MATRIX_7_MSB_ADDR   0X52
#define BNO055_SIC_MATRIX_8_LSB_ADDR   0X53
#define BNO055_SIC_MATRIX_8_MSB_ADDR   0X54

/* Accelerometer Offset registers */
#define BNO055_ACCEL_OFFSET_X_LSB_ADDR   0X55
#define BNO055_ACCEL_OFFSET_X_MSB_ADDR   0X56
#define BNO055_ACCEL_OFFSET_Y_LSB_ADDR   0X57
#define BNO055_ACCEL_OFFSET_Y_MSB_ADDR   0X58
#define BNO055_ACCEL_OFFSET_Z_LSB_ADDR   0X59
#define BNO055_ACCEL_OFFSET_Z_MSB_ADDR   0X5A

/* Magnetometer Offset registers */
#define BNO055_MAG_OFFSET_X_LSB_ADDR   0X5B
#define BNO055_MAG_OFFSET_X_MSB_ADDR   0X5C
#define BNO055_MAG_OFFSET_Y_LSB_ADDR   0X5D
#define BNO055_MAG_OFFSET_Y_MSB_ADDR   0X5E
#define BNO055_MAG_OFFSET_Z_LSB_ADDR   0X5F
#define BNO055_MAG_OFFSET_Z_MSB_ADDR   0X60

/* Gyroscope Offset register s*/
#define BNO055_GYRO_OFFSET_X_LSB_ADDR   0X61
#define BNO055_GYRO_OFFSET_X_MSB_ADDR   0X62
#define BNO055_GYRO_OFFSET_Y_LSB_ADDR   0X63
#define BNO055_GYRO_OFFSET_Y_MSB_ADDR   0X64
#define BNO055_GYRO_OFFSET_Z_LSB_ADDR   0X65
#define BNO055_GYRO_OFFSET_Z_MSB_ADDR   0X66

/* Radius registers */
#define BNO055_ACCEL_RADIUS_LSB_ADDR   0X67
#define BNO055_ACCEL_RADIUS_MSB_ADDR   0X68
#define BNO055_MAG_RADIUS_LSB_ADDR     0X69
#define BNO055_MAG_RADIUS_MSB_ADDR     0X6A

/* PAGE0 REGISTER DEFINITION START*/
#define BNO055_ACC_CONFIG_ADDR      0X08
#define BNO055_MAG_CONFIG_ADDR      0X09
#define BNO055_GYR_CONFIG_0_ADDR    0X0A
#define BNO055_GYR_CONFIG_1_ADDR    0X0B

//  unit settings
#define ACCELELSB       100.0F
#define GYROLSB         900.0F
#define MAGPLSB         16.0F

#define INDEX_X         0
#define INDEX_Y         1
#define INDEX_Z         2

#define DEFF_TIME       0.005
/******************************** 自動生成関数 *********************************/
#define I2C_BNO055_SEND HAL_I2C_Master_Transmit(&hi2c1,BNO055_SLAVEADDRESS,tx_buf,tx_num,100);
#define I2C_BNO055_READ HAL_I2C_Master_Receive(&hi2c1,BNO055_SLAVEADDRESS|0x0001,rx_buf,rx_num,100);
/******************************************************************************/
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t accelVal[3], gyroVal[3];
extern float acceleration[3];        // 加速度
extern float angularVelocity[3];     // 角速度
extern float angle[3];               // 角度
//====================================//
// プロトタイプ宣言
//====================================//
uint8_t readBNO055(uint8_t reg);
void readBNO055AxisData(uint8_t reg, uint8_t *rx_buf);
void writeBNO055(uint8_t reg, uint8_t data);
void initBNO055(void);
void getBNO055Acceleration(void);
void getBNO055Gyro(void);
void calcDegrees(void);

#endif // BNO055_H_
