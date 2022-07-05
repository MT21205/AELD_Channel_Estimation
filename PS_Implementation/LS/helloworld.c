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

// #define SNR 0
#define SNR 5
#if(SNR == 0)
#include "LS_includes.h"
#elif(SNR == 5)
#include "LS_includes_5dB.h"
#endif

double mod_err;
float mod_gold;


void LS_Estimate(const flt_cmplx Preamble_In[nUSC], const flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC]);
int resultCheck(flt_cmplx lsOut[nUSC], const flt_cmplx ls_gold[nUSC], const flt_cmplx act_ch_wght[nUSC]);

// The program execution starts from the "main" function
int main()
{
	init_platform();
	printf("\n\rStart\n\r");

	// Array to store the LS_Estimated Data
	flt_cmplx LS_Est[nUSC];

	// Array to temporarily store the received data pertained to each OFDM symbol
	flt_cmplx LS_Data_In[nUSC];

	// Iterate over all OFDM symbols
	for(int k=0; k<SYMBOLS_USED; k++)
	{
		for(int j=0; j<nUSC; j++)
		{
			LS_Data_In[j] = preambleIn[k][j];
		}

		// The LS Estimation is performed using the below API.
		// preambleIn are the sub-carriers received at the receiver and correspond to one OFDM symbol.
		// trainingSym are the training symbols used against each sub-carrier.
		(void)LS_Estimate(LS_Data_In, trainingSym, LS_Est);

		// Compare the result with god data
		int status = resultCheck(LS_Est, ls_gold[k], Act_Channel_weight[k]);

		if(status != 2)
		{
			printf("\n\tFAILED for OFDM symbol %d\n", (k+1));
			return -1;
		}
		printf("\n\tSUCCESS for OFDM symbol %d\n", (k+1));
		/*
		for(int i=0;i<nUSC;i++)
		{
			// If comparison is a success, the LS gold and estimated data are displayed on console.
			printf("\n(%d) : lsOut= %f + %fi\t:\tgold= %f + %fi\t",k,creal(LS_Est[i]),cimag(LS_Est[i]),creal(ls_gold[k][i]),cimag(ls_gold[k][i]));
		}
		*/
		//printf("\nDone");
	}


	printf("\n mod_err is %f\n", (mod_err));
	printf("\n mod_gold 0dB SNR is %f\n", (mod_gold));
	printf("\n NMSE for 0dB SNR is %f\n", (mod_err/mod_gold));


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
void LS_Estimate(const flt_cmplx Preamble_In[nUSC], const flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC])
{
	for(int i=0; i<nUSC; i++)
	{
		// Divide the received data at the receiver with the training symbol.
		// This provides the LS estimated data for the channel.
		LS_est[i] = Preamble_In[i]/Training_Symbol[i];
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
int resultCheck(flt_cmplx lsOut[nUSC], const flt_cmplx ls_gold[nUSC], const flt_cmplx act_ch_wght[nUSC])
{
	// Comparison result of gold and estimated data
	flt_cmplx res = 0+0*I;

	// Error computation between act channel weight and LS estimated weight
	flt_cmplx Err_LS = 0+0*I;
	for(int k=0;k<nUSC;k++)
	{
		res =ls_gold[k]-lsOut[k];
		Err_LS = act_ch_wght[k] - lsOut[k];
		mod_err = mod_err + ((creal(Err_LS) * creal(Err_LS))+((cimag(Err_LS)) * (cimag(Err_LS))));
		mod_gold = mod_gold + ((creal(act_ch_wght[k]) * creal(act_ch_wght[k]))+((cimag(act_ch_wght[k])) * (cimag(act_ch_wght[k]))));
		if(creal(res) > 0.000001 || cimag(res) > 0.000001)
		{
			printf("\n(%d) : lsOut= %f + %fi\t:\tgold= %f + %fi\t:\tdiff= %f + %fi\t",k,creal(lsOut[k]),cimag(lsOut[k]),creal(ls_gold[k]),cimag(ls_gold[k]),creal(res),cimag(res));
			return 1;
		}
	}
	return 2;

}
