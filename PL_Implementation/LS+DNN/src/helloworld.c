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
#include "math.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xtime_l.h"
#define SNR 5

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

#define DMA_MM2S_RegOffset 0x04
#define DMA_S2MM_RegOffset 0x34

#define SW_NMSE_ENABLE 1

void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] );
int resultCheck(flt_cmplx lsOut[nUSC], const flt_cmplx ls_gold[nUSC]);
void Normalize(float* , float* , float* , float* );
void DeNormalize(float* , float* , float* , float* );
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC]);
void LS_Estimate(flt_cmplx Preamble_In[nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC]);

// Definition of a ReLU function
#define ReLU(z) ((z > 0)? (z) : (0))

// Global variables to evaluate the NMSE
float mod_err;
float mod_gold;

#if(SW_NMSE_ENABLE == 1)
float mod_err_sw;
float mod_gold_sw;
#endif


int main()
{
	//int i,k;
	int i;
	// Store the Normalized and De-Normalized data
	float Norm_In[2*nUSC], De_Norm_Out[2*nUSC];

	// Store the LS data predicted using DNN algorithm
	flt_cmplx LS_DNN_Est_Out[SYMBOLS_USED][nUSC];

	// Array to store the LS_Estimated Data
	flt_cmplx LS_Est[nUSC];

	// Array to temporarily store the received data pertained to each OFDM symbol
	//flt_cmplx LS_Data_In[nUSC];

	// Store the Normalized DNN Input and Output
	float DNN_In[nUSC*2], DNN_Out[nUSC*2];

	// Input and Outputs of the configured DMA
	//flt_cmplx Data_In[nUSC];
	flt_cmplx Data_Out[SYMBOLS_USED][nUSC];

	// Variable stores the latency for the 200 channel realization
	float HW_Latency = 0;
	XTime Start_Time, End_Time;

    init_platform();

    XAxiDma Dma_Inst_0;
	XAxiDma_Config *DMA_Config_0;
	u32 status;
    DMA_Config_0 = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	// Initialize the DMA configuration
	XAxiDma_CfgInitialize(&Dma_Inst_0, DMA_Config_0);

	flt_cmplx (*preamble_in)[nUSC];
	flt_cmplx (*Act_Channel_weight)[nUSC];
	//flt_cmplx (*ls_gold)[nUSC];

	printf("\nComputation using IP without word length optimization\n");
	for(int cur_SNR=0; cur_SNR<=SNR_MAX; cur_SNR+=5)
	{
		HW_Latency = 0;
		mod_err = 0;
		mod_gold = 0;

#if(SW_NMSE_ENABLE == 1)
		mod_err_sw = 0;
		mod_gold_sw = 0;
#endif

		if(cur_SNR == 0)
		{
			preamble_in = preamble_in_0dB;
			Act_Channel_weight = Act_Channel_weight_0dB;
		}
		else if(cur_SNR == 5)
		{
			preamble_in = preamble_in_5dB;
			Act_Channel_weight = Act_Channel_weight_5dB;
		}
		else if(cur_SNR == 10)
		{
			preamble_in = preamble_in_10dB;
			Act_Channel_weight = Act_Channel_weight_10dB;
		}
		else if(cur_SNR == 15)
		{
			preamble_in = preamble_in_15dB;
			Act_Channel_weight = Act_Channel_weight_15dB;
		}
		else if(cur_SNR == 20)
		{
			preamble_in = preamble_in_20dB;
			Act_Channel_weight = Act_Channel_weight_20dB;
		}
		else if(cur_SNR == 25)
		{
			preamble_in = preamble_in_25dB;
			Act_Channel_weight = Act_Channel_weight_25dB;
		}
		else if(cur_SNR == 30)
		{
			preamble_in = preamble_in_30dB;
			Act_Channel_weight = Act_Channel_weight_30dB;
		}
		/************************** PS Estimate **************************/

		for(int k=0; k<SYMBOLS_USED; k++)
		{

			//Perform the LS_Estimation
			(void)LS_Estimate(preamble_in[k], training_symbol, LS_Est);

			// Separate the real and Imaginary values of the LS estimated data
			for(i=0; i<nUSC; i++)
			{
				Norm_In[i] = creal(LS_Est[i]);
				Norm_In[nUSC+i] = cimag(LS_Est[i]);
			}

			// Normalize the LS estimated data
			(void)Normalize(Norm_In, Mean_X, SD_X, DNN_In);

			// Pass the Normalized Data into the DNN and extract the DNN predicted data
			LS_DNN(DNN_In,DNN_Out);

			// De-Normalize the DNN Output and combine the real and imaginary values
			(void)DeNormalize(DNN_Out, Mean_Y, SD_Y, De_Norm_Out);

			// Combine the 104 valued De-Normalized data to form a 52 valued complex data
			for(i=0; i<nUSC; i++)
			{
				LS_DNN_Est_Out[k][i] = De_Norm_Out[i] + I*De_Norm_Out[i+nUSC];
				//printf("\n LS Gold(%d) -> %f+%f*i |\t LS_DNN(%d) -> %f+%f*i",(i+1),creal(ls_DNN_gold[i]), cimag(ls_DNN_gold[i]), (i+1), creal(LS_DNN_Est_Out[i]), cimag(LS_DNN_Est_Out[i]));
			}

			//(void)Evaluate_Result(ls_DNN_gold[k], LS_DNN_Est_Out);

			/************************** PS Estimate **************************/
		}

		/************************** PL Estimate **************************/

		for(int k=0; k<SYMBOLS_USED; k++)
		{
			XTime_SetTime(0);
			XTime_GetTime(&Start_Time);

			status = XAxiDma_SimpleTransfer(&Dma_Inst_0, (UINTPTR)Data_Out[k], (sizeof(flt_cmplx))*(nUSC), XAXIDMA_DEVICE_TO_DMA);
			status = XAxiDma_SimpleTransfer(&Dma_Inst_0, (UINTPTR)preamble_in[k], (sizeof(flt_cmplx))*(nUSC), XAXIDMA_DMA_TO_DEVICE);

			status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,DMA_MM2S_RegOffset) & 0x02;

			while(status != 0x02)
			{
				status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,DMA_MM2S_RegOffset) & 0x02;
			}

			// Check the status of the Stream to MemoryMap (S2MM) transfer and wait un-till it becomes idle indicating a completion of processing
			status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,DMA_S2MM_RegOffset) & 0x02;
			while(status != 0x2)
			{
				status = XAxiDma_ReadReg(XPAR_AXI_DMA_0_BASEADDR,DMA_S2MM_RegOffset) & 0x02;
			}
			XTime_GetTime(&End_Time);
			HW_Latency = HW_Latency + (float)1.0 * (End_Time - Start_Time) / (COUNTS_PER_SECOND/1000000);
		}


		/************************** PL Estimate **************************/

		flt_cmplx Err_LS = 0+0*I;

#if(SW_NMSE_ENABLE == 1)
	flt_cmplx Err_LS_sw = 0+0*I;
#endif

		for(int k=0; k<SYMBOLS_USED; k++)
		{
			for(int i=0; i<nUSC; i++)
			{
				if(((abs)(creal(LS_DNN_Est_Out[k][i]) - creal(Data_Out[k][i])) > 0.001) || ((abs)(cimag(LS_DNN_Est_Out[k][i]) - cimag(Data_Out[k][i])) > 0.001))
				{
					printf("Error at %d\n", (i+1));
					break;
				}
				// Computation for the NMSE
				Err_LS = Act_Channel_weight[k][i] - Data_Out[k][i];
				mod_err = mod_err + ((creal(Err_LS) * creal(Err_LS))+((cimag(Err_LS)) * (cimag(Err_LS))));
				mod_gold = mod_gold + ((creal(Act_Channel_weight[k][i]) * creal(Act_Channel_weight[k][i]))+((cimag(Act_Channel_weight[k][i])) * (cimag(Act_Channel_weight[k][i]))));

#if(SW_NMSE_ENABLE == 1)
				Err_LS_sw = Act_Channel_weight[k][i] - LS_DNN_Est_Out[k][i];
				mod_err_sw = mod_err_sw + ((creal(Err_LS_sw) * creal(Err_LS_sw))+((cimag(Err_LS_sw)) * (cimag(Err_LS_sw))));
				mod_gold_sw = mod_gold_sw + ((creal(Act_Channel_weight[k][i]) * creal(Act_Channel_weight[k][i]))+((cimag(Act_Channel_weight[k][i])) * (cimag(Act_Channel_weight[k][i]))));
#endif
			}
			//printf("\n HW computation of DNN aided LS on HW success for symbol %d ", (k+1));
		}
		//printf("\nHW SW Comparison %ddb SNR success\n", cur_SNR);

		printf("\nNMSE for %ddB SNR on HW is %f", cur_SNR, (mod_err/mod_gold));

#if(SW_NMSE_ENABLE == 1)
		printf("\nNMSE for %ddB SNR on SW is %f\n", (int)cur_SNR, (mod_err_sw/mod_gold_sw));
#endif
	}

	printf("\nTime Consumed for HW computation of LS+DNN = %f us", HW_Latency);
	printf("\nTime Consumed for one symbol HW computation of LS+DNN = %f us", (HW_Latency/SYMBOLS_USED));
	printf("\n********************************************************************\n");

    cleanup_platform();
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
void LS_Estimate(flt_cmplx Preamble_In[nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC])
{
	for(int i=0; i<nUSC; i++)
	{
		// Divide the received data at the receiver with the training symbol.
		// This provides the LS estimated data for the channel.
		LS_est[i] = Preamble_In[i]/Training_Symbol[i];
	}
}

/*
 * Function		: LS_DNN
 * Description	: Perform the prediction of the normalized data
 * Parameters	: LS_Est_DNN_In -> Normalized Input
 * 				: LS_Est_DNN_Out -> Predicted Data from the 2 Layered DNN Algorithm
 *
 * Return		: void
 */
void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] )
{
	int i,j;
	float DNN_Layer2[LAYER_2_COUNT];

	// Iterate over each neuron in Layer 2 and accumulate the 104 weights with the bias.
	for(i=0; i<LAYER_2_COUNT; i++)
	{
		// First, assign the bias to the node in layer-2 from layer-1
		DNN_Layer2[i] = 0;
		DNN_Layer2[i] = Layer1_bias[i];
		for(j=0; j<LAYER_1_COUNT; j++)
		{
			// Layer 2 perceptron evaluation for each neuron
			// Sum up for all the 104 values from the Layer-1 to a node in Layer-2
			// This is done for 52 nodes in layer-2
			DNN_Layer2[i] = DNN_Layer2[i] + (LS_Est_DNN_In[j] * Layer1_weight[j][i]);
		}

		// Post summing up the bias and the weight data, pass this data through ReLU function.
		DNN_Layer2[i] = ReLU(DNN_Layer2[i]);
	}

	for(i=0; i<LAYER_3_COUNT; i++)
	{
		// First, assign the bias to the node in layer-3 from layer-2
		// Each of 104 nodes in layer 3 has a bias value
		LS_Est_DNN_Out[i] = 0;
		LS_Est_DNN_Out[i] = Layer2_bias[i];

		for(j=0; j<LAYER_2_COUNT; j++)
		{
			// Layer 3 perceptron evaluation for each neuron
			// Sum up for all the 52 values from the Layer-2 to a node in Layer-3
			// This is done for 104 nodes in layer-3
			LS_Est_DNN_Out[i] = LS_Est_DNN_Out[i] + (DNN_Layer2[j] * Layer2_weight[j][i]);
		}
	}
}


/*
 * Function		: Normalize
 * Description	: Normalize the data using the formula -> (x-u)/sig.
 * Parameters	: In	-> Input Data
 * 				: Out 	-> Normalized Output
 * 				: Mean	-> Array containing the mean value corresponding to each 104 elements corresponding to the 52 used Sub-carriers.
 * 				: SD	-> Array containing the SD value corresponding to each 104 elements corresponding to the 52 used Sub-carriers.
 *
 * Return		: void
 */
void Normalize(float In[2*nUSC], float Mean[2*nUSC], float SD[2*nUSC], float Out[2*nUSC])
{
	// Normalize the data using the formula -> (x-u)/sig.
	for(int i=0; i<(2*nUSC); i++)
	{
		//Out[i] = (In[i] - Mean[i])/(sqrt(Var[i]));
		Out[i] = (In[i] - Mean[i])/((SD[i]));
	}
}

/*
 * Function		: DeNormalize
 * Description	: De-Normalize the data using the formula -> (x*sig)+u
 * Parameters	: In	-> Input Data
 * 				: Out 	-> Normalized Output
 * 				: Mean	-> Array containing the mean value corresponding to each 104 elements corresponding to the 52 used Sub-carriers.
 * 				: SD	-> Array containing the SD value corresponding to each 104 elements corresponding to the 52 used Sub-carriers.
 *
 * Return		: void
 */
void DeNormalize(float In[2*nUSC], float Mean[2*nUSC], float SD[2*nUSC], float Out[2*nUSC])
{
	// De-Normalize the data using the formula -> (x*sig)+u
	for(int i=0; i<(2*nUSC); i++)
	{
		//Out[i] = (In[i] * (sqrt(Var[i])))+(Mean[i]);
		Out[i] = (In[i] * ((SD[i])))+(Mean[i]);
	}
}

/*
 * Function		: Evaluate_Result
 * Description	: Compare between the DNN gold data and the evaluated data
 * Parameters	: LS_DNN_Ref	-> Gold data of comprising the predicted LS using DNN algorithm
 * 				: LS_DNN_eval 	-> LS estimated data evaluated after running the DNN algorithm on PS
 *
 * Return		: void
 */
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC])
{
	static int DNN_Iter;
	float diff_Re, diff_Im;
	int error = 0;

	printf("\n ************** DNN Operation %d **************\n",(DNN_Iter+1));
	for(int i=0; i<nUSC; i++)
	{
		diff_Re = creal(LS_DNN_Ref[i]) - creal(LS_DNN_eval[i]);
		diff_Im = cimag(LS_DNN_Ref[i]) - cimag(LS_DNN_eval[i]);

		if((diff_Re > 0.0001) || (diff_Im > 0.0001))
		{
			printf("\n **** Error occurred in comparison %d of DNN Iteration %d ****",(i+1) ,(DNN_Iter+1));
			error =1;
			break;
		}
		else
		{
			// Compare the de-normalized value with the DNN gold against each sample.
			//print the gold and estimated values for 1st sample only

			// For visual verification, print only last 5 LS_DNN outputs and the respective gold value.
			if((DNN_Iter+1) > 195)
			{
				printf("\n LS_DNN_Gold(%d) -> %f+%f*i |\t LS_DNN_Eval(%d) -> %f+%f*i",(i+1),creal(LS_DNN_Ref[i]), cimag(LS_DNN_Ref[i]), (i+1), creal(LS_DNN_eval[i]), cimag(LS_DNN_eval[i]));
			}
		}
	}
	if(error != 1)
	{
		printf("\n DNN Operation %d Successful",(DNN_Iter+1) );
	}
	printf("\n ******************************************\n");
	DNN_Iter++;
}
