/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <xparameters.h>
#include <xaxidma.h>
#include <xtime_l.h>
#define SNR 30

#if 0
#if(SNR == 0)
#include "LS_includes.h"
#elif(SNR == 5)
#include "LS_includes_5dB.h"
#elif(SNR == 10)
#include "LS_includes_10dB.h"
#elif(SNR == 15)
#include "LS_includes_15dB.h"
#elif(SNR == 20)
#include "LS_includes_20dB.h"
#elif(SNR == 25)
#include "LS_includes_25dB.h"
#elif(SNR == 30)
#include "LS_includes_30dB.h"
#endif
#endif

#include "LS_includes.h"
#include "LS_includes_5dB.h"
#include "LS_includes_10dB.h"
#include "LS_includes_15dB.h"
#include "LS_includes_20dB.h"
#include "LS_includes_25dB.h"
#include "LS_includes_30dB.h"
#include "LS_includes_common.h"



void LS_Estimate(flt_cmplx Preamble_In[TEST_RUN][nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[TEST_RUN][nUSC]);
int resultCheck(flt_cmplx lsOut[TEST_RUN][nUSC], flt_cmplx ls_gold[TEST_RUN][nUSC], int cur_SNR);

// Global variables to evaluate the NMSE
float mod_err;
float mod_gold;
// Variable stores the latency for the 200 channel realization
float HW_Latency = 0;


// The program execution starts from the "main" function
int main()
{
	init_platform();
	printf("\n\rStart\n\r");

	// Array stores the LS estimated value
	flt_cmplx lsOut[TEST_RUN][nUSC];

	flt_cmplx (*preambleIn)[nUSC], (*ls_gold)[nUSC];

	// The LS Estimation is performed using the below API.
	// preambleIn are the sub-carriers received at the receiver and correspond to one OFDM symbol.
	// trainingSym are the training symbols used against each sub-carrier.
	for(int k=0; k<=SNR_MAX; k+=5)
	{
		HW_Latency = 0;
		mod_err = 0;
		mod_gold = 0;

		if(k == 0)
		{
			preambleIn = preambleIn_0dB;
			ls_gold = ls_gold_0dB;
		}
		else if(k == 5)
		{
			preambleIn = preambleIn_5dB;
			ls_gold = ls_gold_5dB;
		}
		else if(k == 10)
		{
			preambleIn = preambleIn_10dB;
			ls_gold = ls_gold_10dB;
		}
		else if(k == 15)
		{
			preambleIn = preambleIn_15dB;
			ls_gold = ls_gold_15dB;
		}
		else if(k == 20)
		{
			preambleIn = preambleIn_20dB;
			ls_gold = ls_gold_20dB;
		}
		else if(k == 25)
		{
			preambleIn = preambleIn_25dB;
			ls_gold = ls_gold_25dB;
		}
		else if(k == 30)
		{
			preambleIn = preambleIn_30dB;
			ls_gold = ls_gold_30dB;
		}

		(void)LS_Estimate(preambleIn, trainingSym, lsOut);

		// The below API compares the estimated value against gold value (obtained from Matlab).
		int status = resultCheck(lsOut, ls_gold, k);

		if(status != 2)
		{
			printf("\n\tFAILED\n");
			return -1;
		}
		//printf("\n\tSUCCESS\n");

		printf("\n NMSE for %ddB SNR is %f", (int)k, (mod_err/mod_gold));

	}

	printf("\nTime Consumed for HW computation of LS = %f us", HW_Latency);
	printf("\nTime Consumed for one symbol HW computation of LS = %f us", (HW_Latency/SYMBOLS_USED));

	return 0;
}


/*
 * Function		: LS_Estimate
 * Description	: Perform the LS Estimation on the given data.
 * Parameters	: Preamble_In		-> Input at the receiver end.
 * 				: Training_Symbol 	-> Used Training Symbols
 * 				: LS_est			-> Array containing the estimated LS for the channel used.
 *
 * Return		: void
 */
void LS_Estimate(flt_cmplx Preamble_In[TEST_RUN][nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[TEST_RUN][nUSC])
{
	int status;
	XAxiDma_Config *Dma_Cfg_ptr; //DMA configuration pointer
	XAxiDma Dma_Inst; // DMA instance

	// LS_IP_In contains -> Preamble_In(R,I)
	// LS_IP_Out contains -> LS_est(R,I)
	//flt_cmplx LS_IP_In[nUSC], LS_IP_Out[nUSC];

	//XTime time_PLACP_start , time_PLACP_end; // PL time calculations


	//float time_FPGA_ACP_Poll, time_FPGA_ACP_Intr;

	XTime Start_Time, End_Time;

	// Copy the DMA configuration
	Dma_Cfg_ptr = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	// Initialize the DMA with the read configuration
	status = XAxiDma_CfgInitialize(&Dma_Inst, Dma_Cfg_ptr);

	for(int j=0; j<TEST_RUN; j++)
	{
		XTime_SetTime(0);
		XTime_GetTime(&Start_Time);

		status = XAxiDma_SimpleTransfer(&Dma_Inst, (UINTPTR)LS_est[j], (sizeof(flt_cmplx)*(nUSC)),XAXIDMA_DEVICE_TO_DMA);
		status = XAxiDma_SimpleTransfer(&Dma_Inst, (UINTPTR)Preamble_In[j], (sizeof(flt_cmplx)*(nUSC)),XAXIDMA_DMA_TO_DEVICE);
		// We have only configure the DMA to perform these two transactions..DMA might not have started the transactions.
		//Xil_DCacheInvalidateRange((UINTPTR)FFT_output_PLACP, (sizeof(float complex)*FFT_Size));

		// Continuously read the DMA status register to check for the completion of the transfer from both the sides
		status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,0x04) & 0x00000002;
		while(status!=0x00000002)
		{
		status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,0x04) & 0x00000002;
		}
		status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,0x34) & 0x00000002;
		while(status!=0x00000002)
		{
		status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,0x34) & 0x00000002;
		}

		XTime_GetTime(&End_Time);
		HW_Latency = HW_Latency + (float)1.0 * (End_Time - Start_Time) / (COUNTS_PER_SECOND/1000000);
	}

}


/*
 * Function		: resultCheck
 * Description	: Compare between the lsOut data and the ls_gold data
 * Parameters	: ls_gold	-> Gold data of comprising the predicted LS using algorithm on Matlab
 * 				: lsOut 	-> LS estimated data evaluated after running the LS algorithm on PS
 *
 * Return		: void
 */
int resultCheck(flt_cmplx lsOut[TEST_RUN][nUSC], flt_cmplx ls_gold[TEST_RUN][nUSC], int cur_SNR)
{
	flt_cmplx res = 0+0*I;
	flt_cmplx Err_LS = 0+0*I;
	flt_cmplx (*Act_Channel_weight)[nUSC];

	if(cur_SNR == 0)
		Act_Channel_weight = Act_Channel_weight_0dB;
	else if(cur_SNR == 5)
		Act_Channel_weight = Act_Channel_weight_5dB;
	else if(cur_SNR == 10)
		Act_Channel_weight = Act_Channel_weight_10dB;
	else if(cur_SNR == 15)
		Act_Channel_weight = Act_Channel_weight_15dB;
	else if(cur_SNR == 20)
		Act_Channel_weight = Act_Channel_weight_20dB;
	else if(cur_SNR == 25)
		Act_Channel_weight = Act_Channel_weight_25dB;
	else if(cur_SNR == 30)
		Act_Channel_weight = Act_Channel_weight_30dB;

	for(int j=0; j<TEST_RUN; j++)
	{
		for(int k=0;k<nUSC;k++)
		{
			res = lsOut[j][k]-ls_gold[j][k];
			if(creal(res) > 0.000001 || cimag(res) > 0.000001)
			{
				printf("\n(%d) : lsOut= %f + %fi\t:\tgold= %f + %fi\t:\tdiff= %f + %fi\t",k,creal(lsOut[j][k]),cimag(lsOut[j][k]),creal(ls_gold[j][k]),cimag(ls_gold[j][k]),creal(res),cimag(res));
				return 1;
			}
			// Computation for the NMSE
			Err_LS = Act_Channel_weight[k][j] - lsOut[k][j];
			mod_err = mod_err + ((creal(Err_LS) * creal(Err_LS))+((cimag(Err_LS)) * (cimag(Err_LS))));
			mod_gold = mod_gold + ((creal(Act_Channel_weight[j][k]) * creal(Act_Channel_weight[j][k]))+((cimag(Act_Channel_weight[j][k])) * (cimag(Act_Channel_weight[j][k]))));
		}

		//printf("\n LS Estimation for OFDM symbol %d Success", (j+1));
	}

	return 2;

}
