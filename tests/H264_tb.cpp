//==============================================================================
//      File:           $URL$
//      Version:        1.0  worked will on fluid pipeline
//      Author:         Bipeng Zhang
//                      Xiaoyuan Lu
//      Copyright:      Copyright 2005 UC Santa Cruz
//==============================================================================

//==============================================================================
//      Section:        License
//==============================================================================
//      Copyright (c) 2005, Regents of the University of California
//      All rights reserved.
//
//      Redistribution and use in source and binary forms, with or without modification,
//      are permitted provided that the following conditions are met:
//
//              - Redistributions of source code must retain the above copyright notice,
//                      this list of conditions and the following disclaimer.
//              - Redistributions in binary form must reproduce the above copyright
//                      notice, this list of conditions and the following disclaimer
//                      in the documentation and/or other materials provided with the
//                      distribution.
//              - Neither the name of the University of California, Santa Cruz nor the
//                      names of its contributors may be used to endorse or promote
//                      products derived from this software without specific prior
//                      written permission.
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//      ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//      WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
//      ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//      (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//      ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

/****************************************************************************
    Description:
  //Start DUT variable definition
  BoolType		reset_n;
  BitstreamType		BitStream_buffer_input;
  BoolType		pin_disable_DF;
  BoolType		freq_ctrl0;
  BoolType		freq_ctrl1;
  BoolType		BitStream_ram_ren;
  BitstreamAddrType		BitStream_ram_addr;
  PicnumType		pic_num;
  BoolType		ext_frame_RAM0_cs_n;
  BoolType		ext_frame_RAM0_wr;
  RamAddrType		ext_frame_RAM0_addr;
  RamDataType		ext_frame_RAM0_data;
  BoolType		ext_frame_RAM1_cs_n;
  BoolType		ext_frame_RAM1_wr;
  RamAddrType		ext_frame_RAM1_addr;
  RamDataType		ext_frame_RAM1_data;
  RamDataType		dis_frame_RAM_din;
  BoolType		slice_header_s6;



****************************************************************************/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "vpi_user.h"
#include "veriuser.h"

#include "atc_fns.h"

#include "H264_tb.h"
#include "H264_ttb.h"

#ifdef MODELSIM
typedef PLI_INT32 PLIType;
#else
typedef uint32_t PLIType;
#endif

#define GLOBAL_PRINT  0
#define DEBUG_PRINT   0
#define CYCLES_HOLD_RESET   10
#define Beha_Bitstream_ram_size	131071  //Beha_Bitstream_ram size

using namespace std;

uint64_t NUM_EXECUTE_CYCLES = 57300*10;
uint64_t num_total_ops = 0;

//Useful debugging macros, debugging and error message macros so we can pin down message provenance at a glance
#define DEBUGPRINT2(...)       vpi_printf(__VA_ARGS__)
#define ALWAYSPRINT(_fmt, ...)  DEBUGPRINT2(DBGSTR _fmt, WHEREARG, __VA_ARGS__)
#define ERRORPRINT(_fmt, ...)  DEBUGPRINT2(ERRSTR _fmt, WHEREARG, __VA_ARGS__)

uint32_t            ext_frame_RAM0[9504];
uint32_t            ext_frame_RAM1[9504];
uint16_t            BitStream_ram[Beha_Bitstream_ram_size];

void H264_tb_init() {

	int32_t time_seed = time(0);
 	vpi_printf("@D Running for %d cycles\n", NUM_EXECUTE_CYCLES);
	char *opt = mc_scan_plusargs("print=");
	// Handle RNG Seeding
	opt = mc_scan_plusargs("seed=");
	if (opt) {
		time_seed = atoi(opt);
	}
 	time_seed = 1240608344; //FIXME: remove, just for debugging
	vpi_printf("\t\t\tTestbench Seeded with %d\n\n", time_seed);

	FILE *ifp;
    char *mode = "r";
    char buffer[6];
    int i;
    int j;

    //BitStream_ram[Beha_Bitstream_ram_size]= {0};

    //read from the txt file
    ifp = fopen("/mada/users/xyuanlu/projs/mascrtl/code/tapeout/H264/bitstream/akiyo300_1ref.txt", mode);
    //ifp = fopen("bitstream/akiyo300_1ref.txt", mode);


    //read bit stream from file, and change it from char to int (hex)

	for(i=0;i<Beha_Bitstream_ram_size;i++){
        fgets(buffer, sizeof(buffer), ifp);
        sscanf(buffer,"%x", &j);
        BitStream_ram[i]=j;
    }

}// /H264_tb_init()


// Check if operation produced the correct result
void H264_tb_check(H264_ * dut) {
	vpi_printf("@D *************H264_tb check()*************\n");


}// /H264_tb_check()


/* The main function, set all the data inputs
   This fn is independant of H264_tb_check(), to increase verification and isolation of check and set functions*/
void H264_tb_set(H264_ * dut) {
	//////////////////////////////////////////////////////
	vpi_printf("\n\n		    ********Cycle: %d********\n", num_total_ops);
	vpi_printf("@D *********************H264_tb set()*********************\n");

	///////////////////////////////////output
	uint32_t            ext_frame_RAM0_addr;
	uint32_t            ext_frame_RAM1_addr;
	unsigned char		pic_num;
	unsigned char       ext_frame_RAM0_cs_n;
	unsigned char       ext_frame_RAM0_wr;
	unsigned char       ext_frame_RAM1_cs_n;
	unsigned char       ext_frame_RAM1_wr;
	unsigned char       slice_header_s6;
	uint32_t            dis_frame_RAM_din;
	unsigned char       reset_n;

	unsigned char		BitStream_ram_ren;  //the signal
	uint32_t            BitStream_ram_addr;    //input [16:0]
	uint32_t            BitStream_buffer_input;
	int j = 0;



	if (num_total_ops <= CYCLES_HOLD_RESET)
	{
		if (num_total_ops<CYCLES_HOLD_RESET/2)
			dut->reset_n                      	= 1;
		else
			dut->reset_n                      	= 0;

        //dut->BitStream_buffer_input			= 0;
        //dut->ext_frame_RAM0_data			= 4294967295;
        //dut->ext_frame_RAM1_data			= 4294967295;

		vpi_printf("@D *************H264_tb set()*************hold reset\n");
		}
		else
		{
			dut->reset_n		= 1;
 		}

			ext_frame_RAM0_addr		= dut->ext_frame_RAM0_addr.to_i();
			ext_frame_RAM1_addr		= dut->ext_frame_RAM1_addr.to_i();
			pic_num		= dut->pic_num.to_i();
			ext_frame_RAM0_cs_n		= dut->ext_frame_RAM0_cs_n.to_i();
			ext_frame_RAM0_wr		= dut->ext_frame_RAM0_wr.to_i();
			ext_frame_RAM1_cs_n		= dut->ext_frame_RAM1_cs_n.to_i();
			ext_frame_RAM1_wr		= dut->ext_frame_RAM1_wr.to_i();
			slice_header_s6		= dut->slice_header_s6.to_i();
			dis_frame_RAM_din		= dut->dis_frame_RAM_din.to_i();
			reset_n 				= dut->reset_n.to_i();


			vpi_printf("ext_frame_RAM0_cs_n: %d ext_frame_RAM0_wr: %d ext_frame_RAM0_addr: %d \n", ext_frame_RAM0_cs_n, ext_frame_RAM0_wr, ext_frame_RAM0_addr);
			vpi_printf("ext_frame_RAM1_cs_n: %d ext_frame_RAM1_wr: %d ext_frame_RAM1_addr: %d \n", ext_frame_RAM1_cs_n, ext_frame_RAM1_wr, ext_frame_RAM1_addr);

			//init
            BitStream_ram_addr=dut->BitStream_ram_addr.to_i();
			BitStream_ram_ren=dut->BitStream_ram_ren.to_i();

			//check the signal
			if (BitStream_ram_ren == 0){
				//waits for 2 units of timescale
				// Write data at the 32 bit offset 0
				dut->BitStream_buffer_input = BitStream_ram[BitStream_ram_addr];
                vpi_printf("BitStream_ram[BitStream_ram_addr]: %x \n", BitStream_ram[BitStream_ram_addr]);

			}
	        //ram0
            if (!ext_frame_RAM0_cs_n  && ext_frame_RAM0_wr)
			{
                ext_frame_RAM0[ext_frame_RAM0_addr] = dis_frame_RAM_din;
			}

            if (!ext_frame_RAM0_cs_n  && !ext_frame_RAM0_wr)
			{
                dut->ext_frame_RAM0_data = ext_frame_RAM0[ext_frame_RAM0_addr];
			}

            if (slice_header_s6 == 1 && (pic_num%2==1))
			{
				FILE* tracefile_display;
				tracefile_display = fopen("nova_display.log","a");
				fwrite(ext_frame_RAM0, sizeof(uint32_t), 9504, tracefile_display);
				fclose(tracefile_display);
				vpi_printf("@D *************H264_tb set()*************write in ram0\n");
			}

	        // ram1
            if (!ext_frame_RAM1_cs_n && ext_frame_RAM1_wr)
			{
				ext_frame_RAM1[ext_frame_RAM1_addr] = dis_frame_RAM_din;
			}

            if (!ext_frame_RAM1_cs_n && !ext_frame_RAM1_wr)
			{
                dut->ext_frame_RAM1_data = ext_frame_RAM1[ext_frame_RAM1_addr];
			}

            if (slice_header_s6 == 1 && (pic_num%2==0) && reset_n!=0)
			{
				FILE* tracefile_display;
				tracefile_display = fopen("nova_display.log","a");
				fwrite(ext_frame_RAM1, sizeof(uint32_t), 9504, tracefile_display);
				fclose(tracefile_display);
				vpi_printf("@D *************H264_tb set()*************write in ram1 now\n");
			}

}// /H264_tb_set()


// Print all errors, summary, free memory
void H264_tb_end() {
	vpi_printf("\n\n");

	vpi_printf("\n%d Clock Cycles Total H264_tb FINISHED\n", num_total_ops);

}// /H264_tb_end()


