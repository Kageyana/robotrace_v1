//====================================//
// インクルード
//====================================//
#include "setup.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t 	start = 0;		// 0:セットアップ中	1:セットアップ完了

// タイマ関連
uint16_t 		cntSetup1 = 0;			// セットアップで使用
uint16_t 		cntSetup2 = 0;			// セットアップで使用
uint16_t 		cntSwitchUD = 0;		// スイッチ判定用右
uint16_t 		cntSwitchLR = 0;		// スイッチ判定用左
uint16_t		cntSwitchUDLong = 0;	// スイッチ長押し判定用右
uint16_t		cntSwitchLRLong = 0;	// スイッチ長押し判定用左

// スイッチ関連
int8_t pushLR = 0;
int8_t pushUD = 0;

// パターン関連
uint8_t push = 0;
uint8_t push1 = 0;
uint8_t patternSensors = 1;
uint8_t patternSensorLine = 1;
uint8_t patternSensorAccele = 1;
uint8_t patternSensorGyro = 1;
uint8_t patternParameter1 = 1;
uint8_t patternParameter2 = 1;
uint8_t patternParameter3 = 1;
uint8_t patternParameter4 = 1;
uint8_t patternGain = 1;
uint8_t patternSpeedseting = 1;
uint8_t patternLog = 1;
uint8_t patternCalibration = 1;

// フラグ関連
uint8_t setting1meter;
uint8_t setting2meter;
uint8_t setting3meter;
uint8_t motor_test = 0;
uint8_t trace_test = 0;
uint8_t fixSpeed = 0;
uint8_t	calTimes = 1;
uint8_t	calTimesNow = 0;
uint8_t str[8];

// パラメータ関連
int16_t motorTestPwm = 200;

//====================================//
// プロトタイプ宣言
//====================================//
void data_select ( uint8_t *data , uint8_t button );
void dataTuningUD ( void *data, uint8_t add );
void dataTuningLR ( void *data, uint8_t add );

int16_t cnttest = 0;
///////////////////////////////////////////////////////////////
// モジュール名 setup
// 処理概要     走行前設定
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////
void setup( void )
{
	uint8_t cntLed;

	// ディップスイッチで項目選択
	switch ( swValRotary ) {
		//------------------------------------------------------------------
		// スタート待ち
		//------------------------------------------------------------------
		case 0x0:
			data_select( &start, SW_PUSH );
			lcdRowPrintf(UPROW, "start   ");
			lcdRowPrintf(LOWROW, "        ");
			break;
		//------------------------------------------------------------------
		// パラメータ調整(通常トレース)
		//------------------------------------------------------------------
		case 0x1:
			dataTuningLR( &patternParameter1, 1 );
			
			if ( patternParameter1 == 12 ) patternParameter1 = 1;
			else if ( patternParameter1 == 0 ) patternParameter1 = 11;
			
			switch( patternParameter1 ) {
				case 1:
					// 通常走行速度
					dataTuningUD( &targetParam.straight, 1 );
					lcdRowPrintf(UPROW, "STRAIGHT");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.straight / 10);
					break;
				case 2:
					// 停止速度
					dataTuningUD( &targetParam.curve, 1 );
					lcdRowPrintf(UPROW, "CURVE   ");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.curve / 10);
					break;
				case 3:
					// 停止速度
					dataTuningUD( &targetParam.stop, 1 );
					lcdRowPrintf(UPROW, "STOP    ");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.stop / 10);
					break;
				case 4:
					// 2次走行_直線
					dataTuningUD( &targetParam.boostStraight, 1 );
					lcdRowPrintf(UPROW, "BST STRT");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boostStraight / 10);
					break;
				case 5:
					// 2次走行_R1500
					dataTuningUD( &targetParam.boost1500, 1 );
					lcdRowPrintf(UPROW, "BST 1500");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boost1500 / 10);
					break;
				case 6:
					// 2次走行_R800
					dataTuningUD( &targetParam.boost800, 1 );
					lcdRowPrintf(UPROW, "BST  800");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boost800 / 10);
					break;
				case 7:
					// 2次走行_R1600
					dataTuningUD( &targetParam.boost600, 1 );
					lcdRowPrintf(UPROW, "BST  600");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boost600 / 10);
					break;
				case 8:
					// 2次走行_R400
					dataTuningUD( &targetParam.boost400, 1 );
					lcdRowPrintf(UPROW, "BST  400");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boost400 / 10);
					break;
				case 9:
					// 2次走行_R200
					dataTuningUD( &targetParam.boost200, 1 );
					lcdRowPrintf(UPROW, "BST  200");
					lcdRowPrintf(LOWROW, "  %3gm/s", (float)targetParam.boost200 / 10);
					break;
				
			}
			break;
		//------------------------------------------------------------------
		// ゲイン調整(直線トレース)
		//------------------------------------------------------------------
		case 0x2:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			data_select( &trace_test, SW_PUSH );
			// PUSHでトレースON/OFF
			if ( trace_test == 1 ) {
				motorPwmOutSynth( lineTraceCtrl.pwm, 0 );
				powerLinesensors(1);
			} else {
				motorPwmOutSynth( 0, 0 );
				powerLinesensors(0);
			}
			
			dataTuningLR( &patternGain, 1 );
			if ( patternGain == 4 ) patternGain = 1;
			else if ( patternGain == 0 ) patternGain = 3;
			
			switch( patternGain ) {
				case 1:
					// kp
					dataTuningUD ( &lineTraceCtrl.kp, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "   %2d %2d", lineTraceCtrl.ki, lineTraceCtrl.kd);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", lineTraceCtrl.kp, lineTraceCtrl.ki, lineTraceCtrl.kd);
					break;
				case 2:
					// ki
					dataTuningUD ( &lineTraceCtrl.ki, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "%2d    %2d", lineTraceCtrl.kp, lineTraceCtrl.kd);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", lineTraceCtrl.kp, lineTraceCtrl.ki, lineTraceCtrl.kd);
					break;
				case 3:
					// kd
					dataTuningUD ( &lineTraceCtrl.kd, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "%2d %2d   ", lineTraceCtrl.kp, lineTraceCtrl.ki);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", lineTraceCtrl.kp, lineTraceCtrl.ki, lineTraceCtrl.kd);
					break;
			}
			break;
		//------------------------------------------------------------------
		// ゲイン調整(速度)
		//------------------------------------------------------------------
		case 0x3:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			// data_select( &trace_test, SW_PUSH );
			
			dataTuningLR( &patternGain, 1 );
			if ( patternGain == 4 ) patternGain = 1;
			else if ( patternGain == 0 ) patternGain = 3;
			
			switch( patternGain ) {
				case 1:
					// kp
					dataTuningUD ( &veloCtrl.kp, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "   %2d %2d", veloCtrl.ki, veloCtrl.kd);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", veloCtrl.kp, veloCtrl.ki, veloCtrl.kd);
					break;
				case 2:
					// ki
					dataTuningUD ( &veloCtrl.ki, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "%2d    %2d", veloCtrl.kp, veloCtrl.kd);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", veloCtrl.kp, veloCtrl.ki, veloCtrl.kd);
					break;
				case 3:
					// kd
					dataTuningUD ( &veloCtrl.kd, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) 	lcdRowPrintf(LOWROW, "%2d %2d   ", veloCtrl.kp, veloCtrl.ki);
					else 					lcdRowPrintf(LOWROW, "%2d %2d %2d", veloCtrl.kp, veloCtrl.ki, veloCtrl.kd);
					break;
			}
			break;
		//------------------------------------------------------------------
		// Motor_test
		//------------------------------------------------------------------
		case 0x4:
			dataTuningLR( &patternSensors, 1 );
			
			if ( patternSensors == 11 ) patternSensors = 1;
			else if ( patternSensors == 0 ) patternSensors = 10;

			if(patternSensors != 8) useIMU = false;
			if(patternSensors == 6 || patternSensors == 9 || patternSensors == 10) powerLinesensors(1);
			else powerLinesensors(0);

			switch( patternSensors ) {
				case 1:
					// LED
					lcdRowPrintf(UPROW, "LED     ");
					lcdRowPrintf(LOWROW, "        ");
					cntLed = 0x00;
					if ( swValTact == SW_PUSH) {
						cntLed = 0x00;
						while( cntLed <= 0x7 ) {
							ledOut( cntLed );
							cntLed++;
							HAL_Delay(400);
						}
						ledOut( 0 );
					}
					break;
									
				case 2:
					// エンコーダ
					lcdRowPrintf(UPROW, "R %6d",encTotalR);
					lcdRowPrintf(LOWROW, "L %6d",encTotalN);

					if(swValTact == SW_PUSH) encTotalN = 0;
					break;
							
				case 3:
					// モーターテスト
					lcdRowPrintf(UPROW, "Motor   ");
					lcdRowPrintf(LOWROW, "   %4d%%",motorTestPwm);
					// モータ回転
					dataTuningUD ( &motorTestPwm, 100 );
					if ( motor_test == 1 ) motorPwmOut(motorTestPwm,motorTestPwm);
					else motorPwmOut(0, 0);
										
					data_select( &motor_test, SW_PUSH );
					break;

				case 4:
					// マーカーセンサ
					lcdRowPrintf(UPROW, "Marker  ");
					lcdRowPrintf(LOWROW, "     0x%x", getMarkerSensor());
					break;

				case 5:
					// 電流センサ
					lcdRowPrintf(UPROW, "   %3.0fmA", CurrntR);
					lcdRowPrintf(LOWROW, "   %3.0fmA", CurrntL);

					motorTestPwm = 80;
					data_select( &motor_test, SW_PUSH );
					if ( motor_test == 1 ) motorPwmOut(motorTestPwm,motorTestPwm);
					else motorPwmOut(0, 0);
					break;

				case 6:
					// ラインセンサ
					dataTuningUD( &patternSensorLine, 1 );
					if ( patternSensorLine == 7 ) patternSensorLine = 1;
					else if ( patternSensorLine == 0 ) patternSensorLine = 6;

					switch( patternSensorLine ) {
						case 1:
							// lcdRowPrintf(UPROW, "L1  %4d",lSensor[0]);
							lcdRowPrintf(UPROW, "L1 %5d",lSensor[5]+lSensor[6]);
							lcdRowPrintf(LOWROW, "L2  %4d",lSensor[1]);
							break;

						case 2:
							lcdRowPrintf(UPROW, "L3  %4d",lSensor[2]);
							lcdRowPrintf(LOWROW, "L4  %4d",lSensor[3]);
							break;
						case 3:
							lcdRowPrintf(UPROW, "L5  %4d",lSensor[4]);
							lcdRowPrintf(LOWROW, "L6  %4d",lSensor[5]);
							break;
						
						case 4:
							lcdRowPrintf(UPROW, "R1  %4d",lSensor[11]);
							lcdRowPrintf(LOWROW, "R2  %4d",lSensor[10]);
							break;

						case 5:
							lcdRowPrintf(UPROW, "R3  %4d",lSensor[9]);
							lcdRowPrintf(LOWROW, "R4  %4d",lSensor[8]);
							break;

						case 6:
							lcdRowPrintf(UPROW, "R5  %4d",lSensor[7]);
							lcdRowPrintf(LOWROW, "R6  %4d",lSensor[6]);
							break;
					}
					break;
				case 7:
					// 加速度
					dataTuningUD( &patternSensorAccele, 1 );
					if ( patternSensorAccele == 4 ) patternSensorAccele = 1;
					else if ( patternSensorAccele == 0 ) patternSensorAccele = 3;

					switch( patternSensorAccele ) {
						case 1:
							lcdRowPrintf(UPROW, "X accele");
							lcdRowPrintf(LOWROW, "    %4.0f",acceleration[INDEX_X]);
							break;
						case 2:
							lcdRowPrintf(UPROW, "Y accele");
							lcdRowPrintf(LOWROW, "    %4.0f",acceleration[INDEX_Y]);
							break;
						case 3:
							lcdRowPrintf(UPROW, "Z accele");
							lcdRowPrintf(LOWROW, "    %4.0f",acceleration[INDEX_Z]);
							break;
					}
					break;
				case 8:
					// 角速度
					dataTuningUD( &patternSensorGyro, 1 );
					if ( patternSensorGyro == 7 ) patternSensorGyro = 1;
					else if ( patternSensorGyro == 0 ) patternSensorGyro = 6;
					if (swValTact == SW_PUSH) {
						angle[INDEX_Z] = 0.0;
					}
					useIMU = true;		// IMU 使用開始

					switch( patternSensorGyro ) {
						case 1:
							lcdRowPrintf(UPROW, "X gyro  ");
							lcdRowPrintf(LOWROW, "   %3.2f",angularVelocity[INDEX_X]);
							break;
						case 2:
							lcdRowPrintf(UPROW, "Y gyro  ");
							lcdRowPrintf(LOWROW, "   %3.2f",angularVelocity[INDEX_Y]);
							break;
						case 3:
							lcdRowPrintf(UPROW, "Z gyro  ");
							lcdRowPrintf(LOWROW, "   %3.2f",angularVelocity[INDEX_Z]);
							break;
						case 4:
							lcdRowPrintf(UPROW, "X angle ");
							lcdRowPrintf(LOWROW, "   %3.2f",angle[INDEX_X]);
							break;
						case 5:
							lcdRowPrintf(UPROW, "Y angle ");
							lcdRowPrintf(LOWROW, "   %3.2f",angle[INDEX_Y]);
							break;
						case 6:
							lcdRowPrintf(UPROW, "Z angle ");
							lcdRowPrintf(LOWROW, "   %3.2f",angle[INDEX_Z]);
							break;
					}
					break;
				case 9:
					// PID制御量
					targetSpeed = 120;
					lcdRowPrintf(UPROW, "tra%5d",lineTraceCtrl.pwm);
					lcdRowPrintf(LOWROW, "vel%5d",yawCtrl.pwm);
					break;
				case 10:
					// 仮想センサ角度
					targetSpeed = 120;
					lcdRowPrintf(UPROW, "Anglesen");
					lcdRowPrintf(LOWROW, "   %3.1f", angleSensor);
					break;
			} // switch patternSensors
			break;
		//------------------------------------------------------------------
		// Log
		//------------------------------------------------------------------
		case 0x5:
			lcdRowPrintf(UPROW, "LOG     ");
			if (initMSD) {
				lcdRowPrintf(LOWROW, "    %4d", fileNumbers[fileIndexLog]);

				dataTuningLR( &fileIndexLog, 1 );
				// fileIndexLog = endFileIndex;
				if(fileIndexLog > endFileIndex) fileIndexLog = 0;
				if(fileIndexLog < 0) fileIndexLog = endFileIndex;

				switch (patternLog) {
					case 1:
						// ログ解析
						if (swValTact == SW_UP) {
							numMarkerLog = readLog(fileNumbers[fileIndexLog]);
							if (numMarkerLog > 0) {
								lcdRowPrintf(LOWROW, " success");
								optimalTrace = true;
							} else {
								lcdRowPrintf(LOWROW, "   false");
							}
							HAL_Delay(1000);
						}
						break;
				
					default:
						break;
				}
			} else {
				lcdRowPrintf(LOWROW, "  no MSD");
			}
			break;
		//------------------------------------------------------------------
		// キャリブレーション(ラインセンサ) 
		//------------------------------------------------------------------
		case 0x6:
			switch (patternCalibration) {
				case 1:
					// スイッチ入力待ち
					dataTuningUD( &calTimes, 1 );

					targetSpeed = 0;
					motorPwmOutSynth( 0, veloCtrl.pwm );
					lcdRowPrintf(UPROW, "Calibrat");
					lcdRowPrintf(LOWROW, "%d  times", calTimes);
					if (swValTact == SW_PUSH) {
						cntSetup1 = 0;
						enc1 = 0;
						patternCalibration = 2;
					}
					break;

				case 2:
					// 開始準備
					lcdRowPrintf(LOWROW, "%4d",cntSetup1);
					if (cntSetup1 > 1000) {
						powerLinesensors(1);
						cntSetup1 = 0;
						enc1 = 0;
						modeCalLinesensors = 1; 	// キャリブレーション開始
						patternCalibration = 3;
					}
					break;
				
				case 3:
					// 前進
					targetSpeed = 0.4*PALSE_MILLIMETER;
					motorPwmOutSynth( 0, veloCtrl.pwm );
					if (enc1 > encMM(100)) {
						cntSetup1 = 0;
						enc1 = 0;
						patternCalibration = 4;
					}
					break;

				case 4:
					// 後退
					targetSpeed = -0.4*PALSE_MILLIMETER;
					motorPwmOutSynth( 0, veloCtrl.pwm );
					if (enc1 < -encMM(100)) {
						modeCalLinesensors = 0;		// キャリブレーション停止
						powerLinesensors(0);
						cntSetup1 = 0;
						patternCalibration = 5;
					}
					break;

				case 5:
					// 停止
					targetSpeed = 0;
					motorPwmOutSynth( 0, veloCtrl.pwm );
					if (abs(encCurrentN) < 2) {
						calTimesNow++;
						enc1 = 0;
						if (calTimesNow >= calTimes) {
							calTimesNow = 0;
							patternCalibration = 1;
						} else {
							patternCalibration = 3;
						}
					}
					break;
				
				default:
					break;
				}
			break;

		//------------------------------------------------------------------
		// ゲイン調整(角速度)
		//------------------------------------------------------------------
		case 0x7:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			targetAngularVelocity = 0;
			targetSpeed = 0;
			data_select( &trace_test, SW_PUSH );
			// PUSHでトレースON/OFF
			if ( trace_test == 1 ) {
				motorPwmOutSynth( yawRateCtrl.pwm, veloCtrl.pwm );
			} else {
				motorPwmOutSynth( 0, 0 );
			}
			
			dataTuningLR( &patternGain, 1 );
			if ( patternGain == 4 ) patternGain = 1;
			else if ( patternGain == 0 ) patternGain = 3;
			
			switch( patternGain ) {
				case 1:
					// kp
					dataTuningUD ( &yawRateCtrl.kp, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "   %2d %2d", yawRateCtrl.ki, yawRateCtrl.kd);
					else 										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawRateCtrl.kp, yawRateCtrl.ki, yawRateCtrl.kd);
					break;
				case 2:
					// ki
					dataTuningUD ( &yawRateCtrl.ki, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "%2d    %2d", yawRateCtrl.kp, yawRateCtrl.kd);
					else 										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawRateCtrl.kp, yawRateCtrl.ki, yawRateCtrl.kd);
					break;
				case 3:
					// kd
					dataTuningUD ( &yawRateCtrl.kd, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "%2d %2d   ", yawRateCtrl.kp, yawRateCtrl.ki);
					else										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawRateCtrl.kp, yawRateCtrl.ki, yawRateCtrl.kd);
					break;
			}
			break;
		//------------------------------------------------------------------
		// ゲイン調整(角度)
		//------------------------------------------------------------------
		case 0x8:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			if (cntSetup2 < 1000) 	targetAngle = 45.0;
			else 					targetAngle = -45.0;
			if (cntSetup2 > 2000)	cntSetup2 = 0;
			targetSpeed = 0;
			data_select( &trace_test, SW_PUSH );
			// PUSHでトレースON/OFF
			if ( trace_test == 1 ) {
				motorPwmOutSynth( yawCtrl.pwm, veloCtrl.pwm );
			} else {
				motorPwmOutSynth( 0, 0 );
			}
			
			dataTuningLR( &patternGain, 1 );
			if ( patternGain == 4 ) patternGain = 1;
			else if ( patternGain == 0 ) patternGain = 3;
			
			switch( patternGain ) {
				case 1:
					// kp
					dataTuningUD ( &yawCtrl.kp, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "   %2d %2d", yawCtrl.ki, yawCtrl.kd);
					else 										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawCtrl.kp, yawCtrl.ki, yawCtrl.kd);
					break;
				case 2:
					// ki
					dataTuningUD ( &yawCtrl.ki, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "%2d    %2d", yawCtrl.kp, yawCtrl.kd);
					else 										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawCtrl.kp, yawCtrl.ki, yawCtrl.kd);
					break;
				case 3:
					// kd
					dataTuningUD ( &yawCtrl.kd, 1 );
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 && trace_test == 0 ) 	lcdRowPrintf(LOWROW, "%2d %2d   ", yawCtrl.kp, yawCtrl.ki);
					else 										lcdRowPrintf(LOWROW, "%2d %2d %2d", yawCtrl.kp, yawCtrl.ki, yawCtrl.kd);
					break;
			}
			break;

	default:
		lcdRowPrintf(UPROW, "%#x     ", swValRotary);
		lcdRowPrintf(LOWROW, "none    ");
		break;
	} // switch
	// } // cntSW
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 data_select
// 処理概要     タクトスイッチで0,1に変化させる
// 引数         data: 変化させる変数 button: どのスイッチで変化させるか
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////////////////
void data_select ( uint8_t *data , uint8_t button )
{
	if ( swValTact == button ) {
		if ( *data == 1 && push == 0) {
			push = 1;
			*data = 0;
		} else if ( *data == 0 && push == 0) {
			push = 1;
			*data = 1;
		}
	} else {
		push = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 dataTuningUD
// 処理概要     タクトスイッチでdataを加減する
// 引数         data: 加減させる変数 add: 0: 変化量 dir: 0:上下 1:左右
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////////////////
void dataTuningUD ( void *data, uint8_t add )
{
	int16_t *data2 = (int16_t*)data;	// short型ポインタにキャスト
	
	if ( cntSwitchUD >= 50 ) {
		if ( swValTact == SW_UP || swValTact == SW_DOWN ) {
			cntSwitchUDLong++; // 長押し時間計測
			if ( swValTact == SW_UP  ) {
				// インクリメント
				if ( cntSwitchUDLong >= 20 ) {	// 長押し処理
					*data2 += add;
				} else if (pushUD == 0) {	// 1回押し処理
					pushUD = 1;
					*data2 += add;
				}
			} else if ( swValTact == SW_DOWN  ) {
				// デクリメント
				if ( cntSwitchUDLong >= 20 ) {	// 長押し処理
					*data2 -= add;
				} else if (pushUD == 0) {	// 1回押し処理
					pushUD = 1;
					*data2 -= add;
				}
			}
		} else {
			pushUD = 0;
			cntSwitchUDLong = 0;
		}
		cntSwitchUD = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 dataTuningLR
// 処理概要     タクトスイッチでdataを加減する
// 引数         data: 加減させる変数 add: 0: 変化量 dir: 0:上下 1:左右
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////////////////
void dataTuningLR ( void *data, uint8_t add )
{
	int16_t *data2 = (int16_t*)data;	// short型ポインタにキャスト

	if ( cntSwitchLR >= 50 ) {
		if ( swValTact == SW_LEFT || swValTact == SW_RIGHT ) {
			cntSwitchLRLong++; // 長押し時間計測
			if ( swValTact == SW_RIGHT  ) {
				// インクリメント
				if ( cntSwitchLRLong >= 20 ) {	// 長押し処理
					*data2 += add;
				} else if (pushLR == 0) {	// 1回押し処理
					pushLR = 1;
					*data2 += add;
				}
			} else if ( swValTact == SW_LEFT  ) {
				// デクリメント
				if ( cntSwitchLRLong >= 20 ) {	// 長押し処理
					*data2 -= add;
				} else if (pushLR == 0) {	// 1回押し処理
					pushLR = 1;
					*data2 -= add;
				}
			}
		} else {
			pushLR = 0;
			cntSwitchLRLong = 0;
		}
		cntSwitchLR = 0;
	}
}
