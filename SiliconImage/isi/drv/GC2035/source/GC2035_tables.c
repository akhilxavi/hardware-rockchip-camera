#include <ebase/types.h>
#include <ebase/trace.h>
#include <ebase/builtins.h>

#include <common/return_codes.h>

#include "isi.h"
#include "isi_iss.h"
#include "isi_priv.h"
#include "GC2035_priv.h"


/*****************************************************************************
 * DEFINES
 *****************************************************************************/


/*****************************************************************************
 * GLOBALS
 *****************************************************************************/

// Image sensor register settings default values taken from data sheet OV8810_DS_1.1_SiliconImage.pdf.
// The settings may be altered by the code in IsiSetupSensor.
const IsiRegDescription_t GC2035_g_aRegDescription[] =
{
	{0xfe , 0x80,"",eReadWrite},
	{0xfe , 0x80,"",eReadWrite},
	{0xfe , 0x80,"",eReadWrite},  
	{0xfc , 0x06,"",eReadWrite},
	{0xf2 , 0x00,"",eReadWrite},
	{0xf3 , 0x00,"",eReadWrite},
	{0xf4 , 0x00,"",eReadWrite},
	{0xf5 , 0x00,"",eReadWrite},
	{0xf9 , 0xfe,"",eReadWrite}, //[0] pll enable
	{0xfa , 0x00,"",eReadWrite},
	{0xf6 , 0x00,"",eReadWrite},
	{0xf7 , 0x15,"",eReadWrite}, //pll enable

	{0xf8 , 0x85,"",eReadWrite},
	{0xfe , 0x00,"",eReadWrite},
	{0x82 , 0x00,"",eReadWrite},
	{0xb3 , 0x60,"",eReadWrite},
	{0xb4 , 0x40,"",eReadWrite},
	{0xb5 , 0x60,"",eReadWrite},

	{0x03 , 0x02,"",eReadWrite},
	{0x04 , 0x80,"",eReadWrite},

	//////////measure window  ///////////
	{0xfe , 0x00,"",eReadWrite},
	{0xec , 0x06,"",eReadWrite},//04 
	{0xed , 0x06,"",eReadWrite},//04 
	{0xee , 0x62,"",eReadWrite},//60 
	{0xef , 0x92,"",eReadWrite},//90 

	///////////analog/////////////
	{0x0a , 0x00,"",eReadWrite}, //row start
	{0x0c , 0x00,"",eReadWrite}, //col start
	{0x0d , 0x04,"",eReadWrite},
	{0x0e , 0xc0,"",eReadWrite},
	{0x0f , 0x06,"",eReadWrite}, //Window setting
	{0x10 , 0x58,"",eReadWrite}, 
	{0x17 , 0x14,"",eReadWrite}, //[0]mirror [1]flip


	{0x18 , 0x0e,"",eReadWrite}, //sdark 4 row in even frame??
	{0x19 , 0x0c,"",eReadWrite}, //AD pipe number

	/*
	///  毛刺 现象
	{0x18 , 0x0a,"",eReadWrite}, //sdark 4 row in even frame??
	{0x19 , 0x0a,"",eReadWrite}, //AD pipe number
	*/
	
	{0x1a , 0x01,"",eReadWrite}, //CISCTL mode4
	{0x1b , 0x8b,"",eReadWrite},
	{0x1e , 0x88,"",eReadWrite}, //analog mode1 [7] tx-high en [5:3]COL_bias
	{0x1f , 0x08,"",eReadWrite}, //[3] tx-low en//
	{0x20 , 0x05,"",eReadWrite}, //[0]adclk mode , 0x[1]rowclk_MODE [2]rsthigh_en
	{0x21 , 0x0f,"",eReadWrite}, //[6:4]rsg
	{0x22 , 0xf0,"",eReadWrite}, //[3:0]vref
	{0x23 , 0xc3,"",eReadWrite}, //f3//ADC_r
	{0x24 , 0x17,"",eReadWrite}, //pad drive  16

	//AEC
	{0xfe , 0x01,"",eReadWrite},
	{0x11 , 0x20,"",eReadWrite},//AEC_out_slope , 0x
	{0x1f , 0xc0,"",eReadWrite},//max_post_gain
	{0x20 , 0x60,"",eReadWrite},//max_pre_gain
	{0x47 , 0x30,"",eReadWrite},//AEC_outdoor_th
	{0x0b , 0x10,"",eReadWrite},//
	{0x13 , 0x75,"",eReadWrite},//y_target
	{0xfe , 0x00,"",eReadWrite},


	{0x05 , 0x01,"",eReadWrite},//hb
	{0x06 , 0x11,"",eReadWrite},
	{0x07 , 0x00,"",eReadWrite},//vb
	{0x08 , 0x50,"",eReadWrite},
	{0xfe , 0x01,"",eReadWrite},
	{0x27 , 0x00,"",eReadWrite},//step
	{0x28 , 0xa0,"",eReadWrite},
	{0x29 , 0x05,"",eReadWrite},//level1
	{0x2a , 0x00,"",eReadWrite},
	{0x2b , 0x05,"",eReadWrite},//level2
	{0x2c , 0x00,"",eReadWrite},
	{0x2d , 0x06,"",eReadWrite},//6e8//level3
	{0x2e , 0xe0,"",eReadWrite},
	{0x2f , 0x0a,"",eReadWrite},//level4
	{0x30 , 0x00,"",eReadWrite},
	{0x3e , 0x40,"",eReadWrite},
	{0xfe , 0x00,"",eReadWrite},
	{0xfe , 0x00,"",eReadWrite},  //0x , 0x , 0x , 0x , 0x 
	{0xb6 , 0x03,"",eReadWrite}, //AEC enable
	{0xfe , 0x00,"",eReadWrite},

	/////////BLK,"",eReadWrite//////
	{0x3f , 0x00,"",eReadWrite}, //prc close
	{0x40 , 0x77,"",eReadWrite},//
	{0x42 , 0x7f,"",eReadWrite},
	{0x43 , 0x30,"",eReadWrite},
	{0x5c , 0x08,"",eReadWrite},
	{0x5e , 0x20,"",eReadWrite},
	{0x5f , 0x20,"",eReadWrite},
	{0x60 , 0x20,"",eReadWrite},
	{0x61 , 0x20,"",eReadWrite},
	{0x62 , 0x20,"",eReadWrite},
	{0x63 , 0x20,"",eReadWrite},
	{0x64 , 0x20,"",eReadWrite},
	{0x65 , 0x20,"",eReadWrite},

	///block////////////
	{0x80 , 0xff,"",eReadWrite},
	{0x81 , 0x26,"",eReadWrite},//38 , 0x//skin_Y 8c_debug
	{0x87 , 0x90,"",eReadWrite}, //[7]middle gamma 
	{0x84 , 0x00,"",eReadWrite}, //output put foramat
	{0x86 , 0x07,"",eReadWrite}, //02 //sync plority 
	{0x8b , 0xbc,"",eReadWrite},
	{0xb0 , 0x80,"",eReadWrite}, //globle gain
	{0xc0 , 0x40,"",eReadWrite},//Yuv bypass

	//////lsc/////////////
	{0xfe , 0x01,"",eReadWrite},
	{0xc2 , 0x38,"",eReadWrite},
	{0xc3 , 0x25,"",eReadWrite},
	{0xc4 , 0x21,"",eReadWrite},
	{0xc8 , 0x19,"",eReadWrite},
	{0xc9 , 0x12,"",eReadWrite},
	{0xca , 0x0e,"",eReadWrite},
	{0xbc , 0x43,"",eReadWrite},
	{0xbd , 0x18,"",eReadWrite},
	{0xbe , 0x1b,"",eReadWrite},
	{0xb6 , 0x40,"",eReadWrite},
	{0xb7 , 0x2e,"",eReadWrite},
	{0xb8 , 0x26,"",eReadWrite},
	{0xc5 , 0x05,"",eReadWrite},
	{0xc6 , 0x03,"",eReadWrite},
	{0xc7 , 0x04,"",eReadWrite},
	{0xcb , 0x00,"",eReadWrite},
	{0xcc , 0x00,"",eReadWrite},
	{0xcd , 0x00,"",eReadWrite},
	{0xbf , 0x14,"",eReadWrite},
	{0xc0 , 0x22,"",eReadWrite},
	{0xc1 , 0x1b,"",eReadWrite},
	{0xb9 , 0x00,"",eReadWrite},
	{0xba , 0x05,"",eReadWrite},
	{0xbb , 0x05,"",eReadWrite},
	{0xaa , 0x35,"",eReadWrite},
	{0xab , 0x33,"",eReadWrite},
	{0xac , 0x33,"",eReadWrite},
	{0xad , 0x25,"",eReadWrite},
	{0xae , 0x22,"",eReadWrite},
	{0xaf , 0x27,"",eReadWrite},
	{0xb0 , 0x1d,"",eReadWrite},
	{0xb1 , 0x20,"",eReadWrite},
	{0xb2 , 0x22,"",eReadWrite},
	{0xb3 , 0x14,"",eReadWrite},
	{0xb4 , 0x15,"",eReadWrite},
	{0xb5 , 0x16,"",eReadWrite},
	{0xd0 , 0x00,"",eReadWrite},
	{0xd2 , 0x07,"",eReadWrite},
	{0xd3 , 0x08,"",eReadWrite},
	{0xd8 , 0x00,"",eReadWrite},
	{0xda , 0x13,"",eReadWrite},
	{0xdb , 0x17,"",eReadWrite},
	{0xdc , 0x00,"",eReadWrite},
	{0xde , 0x0a,"",eReadWrite},
	{0xdf , 0x08,"",eReadWrite},
	{0xd4 , 0x00,"",eReadWrite},
	{0xd6 , 0x00,"",eReadWrite},
	{0xd7 , 0x0c,"",eReadWrite},
	{0xa4 , 0x00,"",eReadWrite},
	{0xa5 , 0x00,"",eReadWrite},
	{0xa6 , 0x00,"",eReadWrite},
	{0xa7 , 0x00,"",eReadWrite},
	{0xa8 , 0x00,"",eReadWrite},
	{0xa9 , 0x00,"",eReadWrite},
	{0xa1 , 0x80,"",eReadWrite},
	{0xa2 , 0x80,"",eReadWrite},

	//////////cc//////////////
	{0xfe , 0x02,"",eReadWrite},
	{0xc0 , 0x01,"",eReadWrite},
	{0xc1 , 0x40,"",eReadWrite}, //Green_cc for d
	{0xc2 , 0xfc,"",eReadWrite},
	{0xc3 , 0x05,"",eReadWrite},
	{0xc4 , 0xec,"",eReadWrite},
	{0xc5 , 0x42,"",eReadWrite},
	{0xc6 , 0xf8,"",eReadWrite},
	{0xc7 , 0x40,"",eReadWrite},//for cwf 
	{0xc8 , 0xf8,"",eReadWrite},
	{0xc9 , 0x06,"",eReadWrite},
	{0xca , 0xfd,"",eReadWrite},
	{0xcb , 0x3e,"",eReadWrite},
	{0xcc , 0xf3,"",eReadWrite},
	{0xcd , 0x36,"",eReadWrite},//for A
	{0xce , 0xf6,"",eReadWrite},
	{0xcf , 0x04,"",eReadWrite},
	{0xe3 , 0x0c,"",eReadWrite},
	{0xe4 , 0x44,"",eReadWrite},
	{0xe5 , 0xe5,"",eReadWrite},
	{0xfe , 0x00,"",eReadWrite},

	///////awb start ////////////////
	//AWB clear
	{0xfe , 0x01,"",eReadWrite},
	{0x4f , 0x00,"",eReadWrite},
	{0x4d , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x10,"",eReadWrite}, // 10
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x20,"",eReadWrite}, // 20
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x30,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite}, // 30
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x40,"",eReadWrite}, // 40
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x50,"",eReadWrite}, // 50
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x60,"",eReadWrite}, // 60
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x70,"",eReadWrite}, // 70
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x80,"",eReadWrite}, // 80
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0x90,"",eReadWrite}, // 90
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0xa0,"",eReadWrite}, // a0
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0xb0,"",eReadWrite}, // b0
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0xc0,"",eReadWrite}, // c0
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4d , 0xd0,"",eReadWrite}, // d0
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4f , 0x01,"",eReadWrite},
	/////// awbvalue////////
	{0xfe , 0x01,"",eReadWrite},
	{0x4f , 0x00,"",eReadWrite},
	{0x4d , 0x30,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x80,"",eReadWrite},
	{0x4e , 0x80,"",eReadWrite},
	{0x4e , 0x02,"",eReadWrite},
	{0x4e , 0x02,"",eReadWrite},
	{0x4d , 0x40,"",eReadWrite},
	{0x4e , 0x00,"",eReadWrite},
	{0x4e , 0x80,"",eReadWrite},
	{0x4e , 0x80,"",eReadWrite},
	{0x4e , 0x02,"",eReadWrite},
	{0x4e , 0x02,"",eReadWrite},
	{0x4e , 0x02,"",eReadWrite},
	{0x4d , 0x53,"",eReadWrite},
	{0x4e , 0x08,"",eReadWrite},
	{0x4e , 0x04,"",eReadWrite},
	{0x4d , 0x62,"",eReadWrite},
	{0x4e , 0x10,"",eReadWrite},
	{0x4d , 0x72,"",eReadWrite},
	{0x4e , 0x20,"",eReadWrite},
	{0x4f , 0x01,"",eReadWrite},

	/////awb////
	{0xfe , 0x01,"",eReadWrite},
	{0x50 , 0x88,"",eReadWrite},//c0//[6]green mode
	{0x52 , 0x40,"",eReadWrite},
	{0x54 , 0x60,"",eReadWrite},
	{0x56 , 0x06,"",eReadWrite},
	{0x57 , 0x20,"",eReadWrite}, //pre adjust
	{0x58 , 0x01,"",eReadWrite}, 
	{0x5b , 0x02,"",eReadWrite}, //AWB_gain_delta
	{0x61 , 0xaa,"",eReadWrite},//R/G stand
	{0x62 , 0xaa,"",eReadWrite},//R/G stand
	{0x71 , 0x00,"",eReadWrite},
	{0x74 , 0x10,"",eReadWrite},  //0x//AWB_C_max
	{0x77 , 0x08,"",eReadWrite}, // 0x//AWB_p2_x
	{0x78 , 0xfd,"",eReadWrite}, //AWB_p2_y
	{0x86 , 0x30,"",eReadWrite},
	{0x87 , 0x00,"",eReadWrite},
	{0x88 , 0x04,"",eReadWrite},//06 , 0x//[1]dark mode
	{0x8a , 0xc0,"",eReadWrite},//awb move mode
	{0x89 , 0x75,"",eReadWrite},
	{0x84 , 0x08,"",eReadWrite},  //0x//auto_window
	{0x8b , 0x00,"",eReadWrite}, // 0x//awb compare luma
	{0x8d , 0x70,"",eReadWrite}, //awb gain limit R 
	{0x8e , 0x70,"",eReadWrite},//G
	{0x8f , 0xf4,"",eReadWrite},//B
	{0xfe , 0x00,"",eReadWrite},
	{0x82 , 0x02,"",eReadWrite},//awb_en
       /////////awb end /////////////
       
	///==========asde
	{0xfe , 0x01,"",eReadWrite},
	{0x21 , 0xbf,"",eReadWrite},
	{0xfe , 0x02,"",eReadWrite},
	{0xa4 , 0x00,"",eReadWrite},//
	{0xa5 , 0x40,"",eReadWrite}, //lsc_th
	{0xa2 , 0xa0,"",eReadWrite}, //lsc_dec_slope
	{0xa6 , 0x80,"",eReadWrite}, //dd_th
	{0xa7 , 0x80,"",eReadWrite}, //ot_th
	{0xab , 0x31,"",eReadWrite}, //
	{0xa9 , 0x6f,"",eReadWrite}, //
	{0xb0 , 0x99,"",eReadWrite}, //0x//edge effect slope low
	{0xb1 , 0x34,"",eReadWrite},//edge effect slope low
	{0xb3 , 0x80,"",eReadWrite}, //saturation dec slope
	{0xde , 0xb6,"",eReadWrite},  //
	{0x38 , 0x0f,"",eReadWrite}, // 
	{0x39 , 0x60,"",eReadWrite}, //
	{0xfe , 0x00,"",eReadWrite},
	{0x81 , 0x26,"",eReadWrite},
	{0xfe , 0x02,"",eReadWrite},
	{0x83 , 0x00,"",eReadWrite},//
	{0x84 , 0x45,"",eReadWrite},//
	////////////YCP//////////
	{0xd1 , 0x38,"",eReadWrite},//saturation_cb
	{0xd2 , 0x38,"",eReadWrite},//saturation_Cr
	{0xd3 , 0x40,"",eReadWrite},//contrast ?
	{0xd4 , 0x80,"",eReadWrite},//contrast center 
	{0xd5 , 0x00,"",eReadWrite},//luma_offset 
	{0xdc , 0x30,"",eReadWrite},
	{0xdd , 0xb8,"",eReadWrite},//edge_sa_g,b
	{0xfe , 0x00,"",eReadWrite},
	///////dndd///////////
	{0xfe , 0x02,"",eReadWrite},
	{0x88 , 0x15,"",eReadWrite},//dn_b_base
	{0x8c , 0xf6,"",eReadWrite}, //[2]b_in_dark_inc
	{0x89 , 0x03,"",eReadWrite}, //dn_c_weight
	////////EE ///////////
	{0xfe , 0x02,"",eReadWrite},
	{0x90 , 0x6c,"",eReadWrite},// EEINTP mode1
	{0x97 , 0x45,"",eReadWrite},// edge effect
	////==============RGB Gamma 
	{0xfe , 0x02,"",eReadWrite},
	{0x15 , 0x0a,"",eReadWrite},
	{0x16 , 0x12,"",eReadWrite},
	{0x17 , 0x19,"",eReadWrite},
	{0x18 , 0x1f,"",eReadWrite},
	{0x19 , 0x2c,"",eReadWrite},
	{0x1a , 0x38,"",eReadWrite},
	{0x1b , 0x42,"",eReadWrite},
	{0x1c , 0x4e,"",eReadWrite},
	{0x1d , 0x63,"",eReadWrite},
	{0x1e , 0x76,"",eReadWrite},
	{0x1f , 0x87,"",eReadWrite},
	{0x20 , 0x96,"",eReadWrite},
	{0x21 , 0xa2,"",eReadWrite},
	{0x22 , 0xb8,"",eReadWrite},
	{0x23 , 0xca,"",eReadWrite},
	{0x24 , 0xd8,"",eReadWrite},
	{0x25 , 0xe3,"",eReadWrite},
	{0x26 , 0xf0,"",eReadWrite},
	{0x27 , 0xf8,"",eReadWrite},
	{0x28 , 0xfd,"",eReadWrite},
	{0x29 , 0xff,"",eReadWrite},

	///=================y gamma
	{0xfe , 0x02,"",eReadWrite},
	{0x2b , 0x00,"",eReadWrite},
	{0x2c , 0x04,"",eReadWrite},
	{0x2d , 0x09,"",eReadWrite},
	{0x2e , 0x18,"",eReadWrite},
	{0x2f , 0x27,"",eReadWrite},
	{0x30 , 0x37,"",eReadWrite},
	{0x31 , 0x49,"",eReadWrite},
	{0x32 , 0x5c,"",eReadWrite},
	{0x33 , 0x7e,"",eReadWrite},
	{0x34 , 0xa0,"",eReadWrite},
	{0x35 , 0xc0,"",eReadWrite},
	{0x36 , 0xe0,"",eReadWrite},
	{0x37 , 0xff,"",eReadWrite},
	/////1600x1200size// 
	{0xfe , 0x00,"",eReadWrite},//
	{0x90 , 0x01,"",eReadWrite}, //0x//crop enable
	{0x95 , 0x04,"",eReadWrite},  //0x//1600x1200
	{0x96 , 0xb0,"",eReadWrite},
	{0x97 , 0x06,"",eReadWrite},
	{0x98 , 0x40,"",eReadWrite},

	{0xfe , 0x03,"",eReadWrite},
	{0x42 , 0x40,"",eReadWrite}, 
	{0x43 , 0x06,"",eReadWrite}, //output buf width
	{0x41 , 0x02,"",eReadWrite}, // Pclk_polarity
	{0x40 , 0x40,"",eReadWrite},  //00  
	{0x17 , 0x00,"",eReadWrite}, //widv 
	{0xfe , 0x00,"",eReadWrite},
	////output DVP/////
	{0xfe , 0x00,"",eReadWrite},
	{0xb6 , 0x03,"",eReadWrite},
	{0xf7 , 0x15,"",eReadWrite},

	{0xc8 , 0x00,"",eReadWrite},//close scaler
	{0x99 , 0x22,"",eReadWrite},// 1/2 subsample
	{0x9a , 0x06,"",eReadWrite},
	{0x9b , 0x00,"",eReadWrite},
	{0x9c , 0x00,"",eReadWrite},
	{0x9d , 0x00,"",eReadWrite},
	{0x9e , 0x00,"",eReadWrite},
	{0x9f , 0x00,"",eReadWrite},
	{0xa0 , 0x00,"",eReadWrite},  
	{0xa1 , 0x00,"",eReadWrite},
	{0xa2  ,0x00,"",eReadWrite},
	
	{0x90 , 0x01,"",eReadWrite},  //crop enable
	{0x94 , 0x02,"",eReadWrite},
	{0x95 , 0x02,"",eReadWrite},
	{0x96 , 0x58,"",eReadWrite},
	{0x97 , 0x03,"",eReadWrite},
	{0x98 , 0x20,"",eReadWrite},
	{0xfe , 0x00,"",eReadWrite},
	{0x82 , 0xfe,"",eReadWrite},  // fe
	{0xf2 , 0x70,"",eReadWrite}, 
	{0xf3 , 0xff,"",eReadWrite},
	{0xf4 , 0x00,"",eReadWrite},
	{0xf5 , 0x30,"",eReadWrite},
	
	  #if 0   
        /////////  re zao///
	{0xfe,0x00,"",eReadWrite},
	{0x22,0xd0,"",eReadWrite},
	{0xfe,0x01,"",eReadWrite},
	{0x21,0xff,"",eReadWrite},
	{0xfe,0x02,"",eReadWrite},  
	{0x8a,0x33,"",eReadWrite},
	{0x8c,0x76,"",eReadWrite},
	{0x8d,0x85,"",eReadWrite},
	{0xa6,0xf0,"",eReadWrite},	
	{0xae,0x9f,"",eReadWrite},
	{0xa2,0x90,"",eReadWrite},
	{0xa5,0x40,"",eReadWrite},  
	{0xa7,0x30,"",eReadWrite},
	{0xb0,0x88,"",eReadWrite},
	{0x38,0x0b,"",eReadWrite},
	{0x39,0x30,"",eReadWrite},
	{0xfe,0x00,"",eReadWrite},  
	{0x87,0xb0,"",eReadWrite},

       //// small  RGB gamma////
	{0xfe , 0x02,"",eReadWrite},
	{0x15 , 0x0b,"",eReadWrite},
	{0x16 , 0x0e,"",eReadWrite},
	{0x17 , 0x10,"",eReadWrite},
	{0x18 , 0x12,"",eReadWrite},
	{0x19 , 0x19,"",eReadWrite},
	{0x1a , 0x21,"",eReadWrite},
	{0x1b , 0x29,"",eReadWrite},
	{0x1c , 0x31,"",eReadWrite},
	{0x1d , 0x41,"",eReadWrite},
	{0x1e , 0x50,"",eReadWrite},
	{0x1f , 0x5f,"",eReadWrite},
	{0x20 , 0x6d,"",eReadWrite},
	{0x21 , 0x79,"",eReadWrite},
	{0x22 , 0x91,"",eReadWrite},
	{0x23 , 0xa5,"",eReadWrite},
	{0x24 , 0xb9,"",eReadWrite},
	{0x25 , 0xc9,"",eReadWrite},
	{0x26 , 0xe1,"",eReadWrite},
	{0x27 , 0xee,"",eReadWrite},
	{0x28 , 0xf7,"",eReadWrite},
	{0x29 , 0xff,"",eReadWrite},
	
 	////dark sun/////
	{0xfe , 0x02,"",eReadWrite},
	{0x40 , 0x06,"",eReadWrite},
	{0x41 , 0x23,"",eReadWrite},
	{0x42 , 0x3f,"",eReadWrite},
	{0x43 , 0x06,"",eReadWrite},
	{0x44 , 0x00,"",eReadWrite},
	{0x45 , 0x00,"",eReadWrite},
	{0x46 , 0x14,"",eReadWrite},
	{0x47 , 0x09,"",eReadWrite},
 
  #endif
    {0x0000 ,0x00,"eTableEnd",eTableEnd}

};

const IsiRegDescription_t GC2035_g_svga[] =
{
	{0xfe , 0x00,"",eReadWrite},
	{0xb6 , 0x03,"",eReadWrite},
	{0xf7 , 0x15,"",eReadWrite},

	{0xc8 , 0x00,"",eReadWrite},//close scaler
	{0x99 , 0x22,"",eReadWrite},// 1/2 subsample
	{0x9a , 0x06,"",eReadWrite},
	{0x9b , 0x00,"",eReadWrite},
	{0x9c , 0x00,"",eReadWrite},
	{0x9d , 0x00,"",eReadWrite},
	{0x9e , 0x00,"",eReadWrite},
	{0x9f , 0x00,"",eReadWrite},
	{0xa0 , 0x00,"",eReadWrite},  
	{0xa1 , 0x00,"",eReadWrite},
	{0xa2 , 0x00,"",eReadWrite},
	
	{0x90 , 0x01,"",eReadWrite},  //crop enable
	{0x94 , 0x02,"",eReadWrite},
	{0x95 , 0x02,"",eReadWrite},
	{0x96 , 0x58,"",eReadWrite},
	{0x97 , 0x03,"",eReadWrite},
	{0x98 , 0x20,"",eReadWrite},
    {0x0000 ,0x00,"eTableEnd",eTableEnd}
};

const IsiRegDescription_t GC2035_g_1600x1200[] =
{
	{0xfe , 0x00,"",eReadWrite},
	{0xc8 , 0x00,"",eReadWrite},
	{0xf7 , 0x17,"",eReadWrite},

	{0x99 , 0x11,"",eReadWrite}, // disable sambsample
	{0x9a , 0x06,"",eReadWrite},
	{0x9b , 0x00,"",eReadWrite},
	{0x9c , 0x00,"",eReadWrite},
	{0x9d , 0x00,"",eReadWrite},
	{0x9e , 0x00,"",eReadWrite},
	{0x9f , 0x00,"",eReadWrite},
	{0xa0 , 0x00,"",eReadWrite},  
	{0xa1 , 0x00,"",eReadWrite},
	{0xa2 , 0x00,"",eReadWrite},
	        
	{0x90 , 0x01,"",eReadWrite},
	{0x95 , 0x04,"",eReadWrite},
	{0x96 , 0xb0,"",eReadWrite},  
	{0x97 , 0x06,"",eReadWrite},
	{0x98 , 0x40,"",eReadWrite},
                
    {0x0000 ,0x00,"eTableEnd",eTableEnd}

};

