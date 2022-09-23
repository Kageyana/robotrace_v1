//====================================//
// インクルード
//====================================//
#include "setup.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t 	start = 0;		// 0:セットアップ中	1:セットアップ完了

// タイマ関連
uint16_t 		cntSetup1 = 0;		// セットアップで使用
uint16_t 		cntSetup2 = 0;		// セットアップで使用
uint16_t 		cntSetup3 = 0;		// セットアップで使用
uint16_t		cntSwitchUD;	// スイッチ長押し判定用右
uint16_t		cntSwitchLR;	// スイッチ長押し判定用左

// スイッチ関連
int8_t pushLR = 0;
int8_t pushUD = 0;

// パターン関連
uint8_t push = 0;
uint8_t push1 = 0;
uint8_t pattern_sensor = 1;
uint8_t pattern_parameter = 1;
uint8_t pattern_parameter2 = 1;
uint8_t pattern_parameter3 = 1;
uint8_t pattern_parameter4 = 1;
uint8_t pattern_gain = 1;
uint8_t pattern_gain2 = 1;
uint8_t pattern_gain3 = 1;
uint8_t pattern_speedseting = 1;
uint8_t pattern_msd = 1;
uint8_t pattern_flash = 1;
uint8_t pattern_scanf = 0;

// フラグ関連
uint8_t setting_1meter;
uint8_t setting_2meter;
uint8_t setting_3meter;
uint8_t motor_test = 0;
uint8_t trace_test = 0;
uint8_t fixSpeed = 0;
uint8_t str[8];

// パラメータ関連
int16_t motorTestPwm = 200;

//====================================//
// プロトタイプ宣言
//====================================//
void data_select ( uint8_t *data , uint8_t button );
void data_tuningUD ( void *data, uint8_t add );
void data_tuningLR ( void *data, uint8_t add );

int16_t cnttest = 0;
///////////////////////////////////////////////////////////////
// モジュール名 setup
// 処理概要     走行前設定
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////
void setup( void )
{
	uint8_t cnt_led, sd_sw;
	int16_t i, j, k;
	
	// ディップスイッチで項目選択
	// if ( cntSW >= 100 ) {
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
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
		// case 0x1:
			
		// 	break;
			
		//------------------------------------------------------------------
		// パラメータ調整(クランク)
		//------------------------------------------------------------------
		// case 0x2:
			
		// 	break;
		//------------------------------------------------------------------
		// パラメータ調整(レーンチェンジ)
		//------------------------------------------------------------------
		// case 0x3:
			
		// 	break;
		//------------------------------------------------------------------
		// パラメータ調整(坂道、角度)
		//------------------------------------------------------------------
		// case 0x4:
			
		// 	break;
		//------------------------------------------------------------------
		// ゲイン調整(トレース)
		//------------------------------------------------------------------
		case 0x5:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			data_select( &trace_test, SW_PUSH );
			// PUSHでトレースON/OFF
			if ( trace_test == 1 ) {
				motorPwmOutSynth( tracePwm, 0 );
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
			} else {
				motorPwmOutSynth( 0, 0 );
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
			}
			
			data_tuningLR( &pattern_gain, 1 );
			if ( pattern_gain == 4 ) pattern_gain = 1;
			else if ( pattern_gain == 0 ) pattern_gain = 3;
			
			switch( pattern_gain ) {
				case 1:
					// kp
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "   %2d %2d", ki1_buff, kd1_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp1_buff, ki1_buff, kd1_buff);
					}
					
					data_tuningUD ( &kp1_buff, 1 );
					break;
				case 2:
					// ki
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "%2d    %2d", kp1_buff, kd1_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp1_buff, ki1_buff, kd1_buff);
					}
					
					data_tuningUD ( &ki1_buff, 1 );
					break;
				case 3:
					// kd
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "%2d %2d   ", kp1_buff, ki1_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp1_buff, ki1_buff, kd1_buff);
					}
					
					data_tuningUD ( &kd1_buff, 1 );
					break;
			}
			break;
		//------------------------------------------------------------------
		// ゲイン調整(速度)
		//------------------------------------------------------------------
		case 0x6:
			lcdRowPrintf(UPROW, "kp ki kd");
			
			// data_select( &trace_test, SW_PUSH );
			
			data_tuningLR( &pattern_gain, 1 );
			if ( pattern_gain == 4 ) pattern_gain = 1;
			else if ( pattern_gain == 0 ) pattern_gain = 3;
			
			switch( pattern_gain ) {
				case 1:
					// kp
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "   %2d %2d", ki2_buff, kd2_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuningUD ( &kp2_buff, 1 );
					break;
				case 2:
					// ki
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "%2d    %2d", kp2_buff, kd2_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuningUD ( &ki2_buff, 1 );
					break;
				case 3:
					// kd
					//値を点滅
					if ( cntSetup1 >= 500 ) cntSetup1 = 0;
					if ( cntSetup1 < 250 ) {
						lcdRowPrintf(LOWROW, "%2d %2d   ", kp2_buff, ki2_buff);
					} else {
						lcdRowPrintf(LOWROW, "%2d %2d %2d", kp2_buff, ki2_buff, kd2_buff);
					}
					
					data_tuningUD ( &kd2_buff, 1 );
					break;
			}
			break;
		//------------------------------------------------------------------
		// 
		//------------------------------------------------------------------
		// case 0x7:
			
		// 	break;
		//------------------------------------------------------------------
		// プリセットパラメータ
		//------------------------------------------------------------------
		// case 0x8:
			
		// 	break;
		//------------------------------------------------------------------
		// Motor_test
		//------------------------------------------------------------------
		case 0x9:
			data_tuningLR( &pattern_sensor, 1 );
			
			if ( pattern_sensor == 12 ) pattern_sensor = 1;
			else if ( pattern_sensor == 0 ) pattern_sensor = 11;

			switch( pattern_sensor ) {
				case 1:
					// LED
					lcdRowPrintf(UPROW, "LED     ");
					lcdRowPrintf(LOWROW, "        ");
					cnt_led = 0x00;
					if ( swValTact == SW_PUSH) {
						cnt_led = 0x00;
						while( cnt_led <= 0x7 ) {
							ledOut( cnt_led );
							cnt_led++;
							HAL_Delay(400);
						}
						ledOut( 0 );
					}
					break;
									
				case 2:
					// エンコーダ
					lcdRowPrintf(UPROW, "R %6d",encTotalR);
					lcdRowPrintf(LOWROW, "L %6d",encN);
					break;
							
				case 3:
					// モーターテスト
					lcdRowPrintf(UPROW, "Motor   ");
					lcdRowPrintf(LOWROW, "   %4d%%",motorTestPwm);
					data_tuningUD ( &motorTestPwm, 100 );
					if ( motor_test == 1 ) {
						motorPwmOut(motorTestPwm,motorTestPwm);
					} else {
						motorPwmOut(0, 0);
					}
					
					data_select( &motor_test, SW_PUSH );
					break;

				case 4:
					// マーカーセンサ
					lcdRowPrintf(UPROW, "Marker  ");
					lcdRowPrintf(LOWROW, "     0x%x", getMarksensor());
					break;

				case 5:
					lcdRowPrintf(UPROW, "L1  %4d",lsensor[0]);
					lcdRowPrintf(LOWROW, "L2  %4d",lsensor[1]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;

				case 6:
					lcdRowPrintf(UPROW, "L3  %4d",lsensor[2]);
					lcdRowPrintf(LOWROW, "L4  %4d",lsensor[3]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;

				case 7:
					lcdRowPrintf(UPROW, "L5  %4d",lsensor[4]);
					lcdRowPrintf(LOWROW, "L6  %4d",lsensor[5]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;
				
				case 8:
					lcdRowPrintf(UPROW, "R1  %4d",lsensor[11]);
					lcdRowPrintf(LOWROW, "R2  %4d",lsensor[10]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;

				case 9:
					lcdRowPrintf(UPROW, "R3  %4d",lsensor[9]);
					lcdRowPrintf(LOWROW, "R4  %4d",lsensor[8]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;

				case 10:
					lcdRowPrintf(UPROW, "R5  %4d",lsensor[7]);
					lcdRowPrintf(LOWROW, "R6  %4d",lsensor[6]);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;
				
				case 11:
					targetSpeed = 120;
					lcdRowPrintf(UPROW, "tra%5d",tracePwm);
					lcdRowPrintf(LOWROW, "vel%5d",speedPwm);
					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
					break;
			} // switch
			break;
		//------------------------------------------------------------------
		// 位置固定デモ
		//------------------------------------------------------------------
		// case 0xa:
			
		// 	break;
		//------------------------------------------------------------------
		// MicroSD
		//------------------------------------------------------------------
		// case 0xb:
			
		// 	break;
		//------------------------------------------------------------------
		// キャリブレーション
		//------------------------------------------------------------------
		// case 0xc:
			
		// 	break;
		//------------------------------------------------------------------
		// フラッシュ
		//------------------------------------------------------------------
		// case 0xd:
			
		// 	break;

	default:
		lcdRowPrintf(UPROW, "%#x     ", getSWrotary());
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
// モジュール名 data_tuningUD
// 処理概要     タクトスイッチでdataを加減する
// 引数         data: 加減させる変数 add: 0: 変化量 dir: 0:上下 1:左右
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////////////////
void data_tuningUD ( void *data, uint8_t add )
{
	int16_t *data2 = (int16_t*)data;	// short型ポインタにキャスト
	
	if ( cntSetup2 >= 50 ) {
		if ( swValTact == SW_UP || swValTact == SW_DOWN ) {
			cntSwitchUD++; // 長押し時間計測
			if ( swValTact == SW_UP  ) {
				// インクリメント
				if ( cntSwitchUD >= 20 ) {	// 長押し処理
					*data2 += add;
				} else if (pushUD == 0) {	// 1回押し処理
					pushUD = 1;
					*data2 += add;
				}
			} else if ( swValTact == SW_DOWN  ) {
				// デクリメント
				if ( cntSwitchUD >= 20 ) {	// 長押し処理
					*data2 -= add;
				} else if (pushUD == 0) {	// 1回押し処理
					pushUD = 1;
					*data2 -= add;
				}
			}
		} else {
			pushUD = 0;
			cntSwitchUD = 0;
		}
		cntSetup2 = 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
// モジュール名 data_tuningLR
// 処理概要     タクトスイッチでdataを加減する
// 引数         data: 加減させる変数 add: 0: 変化量 dir: 0:上下 1:左右
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////////////////
void data_tuningLR ( void *data, uint8_t add )
{
	int16_t *data2 = (int16_t*)data;	// short型ポインタにキャスト

	if ( cntSetup3 >= 50 ) {
		if ( swValTact == SW_LEFT || swValTact == SW_RIGHT ) {
			cntSwitchLR++; // 長押し時間計測
			if ( swValTact == SW_RIGHT  ) {
				// インクリメント
				if ( cntSwitchLR >= 20 ) {	// 長押し処理
					*data2 += add;
				} else if (pushLR == 0) {	// 1回押し処理
					pushLR = 1;
					*data2 += add;
				}
			} else if ( swValTact == SW_LEFT  ) {
				// デクリメント
				if ( cntSwitchLR >= 20 ) {	// 長押し処理
					*data2 -= add;
				} else if (pushLR == 0) {	// 1回押し処理
					pushLR = 1;
					*data2 -= add;
				}
			}
		} else {
			pushLR = 0;
			cntSwitchLR = 0;
		}
		cntSetup3 = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////
// モジュール名 fixSpeedSetting
// 処理概要     速度パラメータを固定値にする
// 引数         なし
// 戻り値       0: 速度一定にしない　1: 速度一定にする
/////////////////////////////////////////////////////////////////////////////////
uint8_t fixSpeedSetting ( void )
{
	uint8_t ret = 0;
	
	// if ( setting_1meter == 1 ) {
	// 	speed_straight		= 10;
	// 	speed_curve_brake	= 10;
	// 	speed_curve_r600	= 10;
	// 	speed_curve_r450	= 10;
	// 	speed_curve_straight	= 10;
		
	// 	speed_crossline		= 10;
	// 	speed_ckank_trace	= 10;
	// 	speed_rightclank_curve	= 10;
	// 	speed_rightclank_escape	= 10;
	// 	speed_leftclank_curve	= 10;
	// 	speed_leftclank_escape	= 10;
		
	// 	speed_halfine		= 10;
	// 	speed_rightchange_trace = 10;
	// 	speed_rightchange_curve	= 10;
	// 	speed_rightchange_escape= 10;
	// 	speed_leftchange_trace 	= 10;
	// 	speed_leftchange_curve	= 10;
	// 	speed_leftchange_escape	= 10;
		
	// 	speed_slope_brake	= 10;
	// 	speed_slope_trace	= 10;
		
	// 	fixSpeed = 1;
	// 	ret = 1;
	// } else if ( setting_2meter == 1 ) {
	// 	speed_straight		= 20;
	// 	speed_curve_brake	= 20;
	// 	speed_curve_r600	= 20;
	// 	speed_curve_r450	= 20;
	// 	speed_curve_straight	= 20;
		
	// 	speed_crossline		= 20;
	// 	speed_ckank_trace	= 16;
	// 	speed_rightclank_curve	= 16;
	// 	speed_rightclank_escape	= 20;
	// 	speed_leftclank_curve	= 16;
	// 	speed_leftclank_escape	= 20;
		
	// 	speed_halfine		= 20;
	// 	speed_rightchange_trace = 20;
	// 	speed_rightchange_curve	= 20;
	// 	speed_rightchange_escape= 20;
	// 	speed_leftchange_trace 	= 20;
	// 	speed_leftchange_curve	= 20;
	// 	speed_leftchange_escape	= 20;
		
	// 	speed_slope_brake	= 20;
	// 	speed_slope_trace	= 20;
		
	// 	fixSpeed = 1;
	// 	ret = 1;
	// } else if ( setting_3meter == 1 ) {
	// 	speed_straight		= 30;
	// 	speed_curve_brake	= 30;
	// 	speed_curve_r600	= 30;
	// 	speed_curve_r450	= 30;
	// 	speed_curve_straight	= 30;
		
	// 	speed_crossline		= 25;
	// 	speed_ckank_trace	= 16;
	// 	speed_rightclank_curve	= 16;
	// 	speed_rightclank_escape	= 30;
	// 	speed_leftclank_curve	= 16;
	// 	speed_leftclank_escape	= 30;
		
	// 	speed_halfine		= 30;
	// 	speed_rightchange_trace = 30;
	// 	speed_rightchange_curve	= 30;
	// 	speed_rightchange_escape= 30;
	// 	speed_leftchange_trace 	= 30;
	// 	speed_leftchange_curve	= 30;
	// 	speed_leftchange_escape	= 30;
		
	// 	speed_slope_brake	= 20;
	// 	speed_slope_trace	= 30;
		
	// 	fixSpeed = 1;
	// 	ret = 1;
	// }
	
	return ret;
}
