
#include "LS_DNN.h"

#if((IMPL_USED == IMPL_FLOAT) || (IMPL_USED == IMPL_FIXED) || (IMPL_USED == SOLUTION_FLOAT))
flt_cmplx training_symbol[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};

#endif

#if(IMPL_USED == SOLUTION_OPT_WL1)
fixed_cmplx_base training_symbol[nUSC] = {1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0};
#endif

#if(IMPL_USED == SOLUTION_OPT_WL2)
//flt_cmplx training_Symbol[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};
ap_int<3> training_symbol[nUSC] = {1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0};
#endif

#if(IMPL_USED == IMPL_FLOAT)
#include "DNN_Data.h"

void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] );
void Normalize(float* , float* , float* , float* );
void DeNormalize(float* , float* , float* , float* );
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC]);
void LS_Estimate(flt_cmplx Preamble_In[nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC]);

// Definition of a ReLU function
#define ReLU(z) ((z > 0)? (z) : (0))

void LS_DNN_Estimate(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_DNN_Out)
{
#pragma HLS INTERFACE axis register both port=LS_DNN_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return

	// The received data is a combination of 52 training symbols and 52 preamble inputs
		axis_data local_read, local_write;
		//std::complex<float> training_symbol[nUSC];
		std::complex<float> preamble_in[nUSC], LS_DNN_Est_Out[nUSC];

		int i,k;
		// Store the Normalized and De-Normalized data
		float Norm_In[2*nUSC], De_Norm_Out[2*nUSC];

		// Array to store the LS_Estimated Data
		flt_cmplx LS_Est[nUSC];

		// Array to temporarily store the received data pertained to each OFDM symbol
		flt_cmplx LS_Data_In[nUSC];

		// Store the Normalized DNN Input and Output
		float DNN_In[nUSC*2], DNN_Out[nUSC*2];

		// Iterate for all the 200 OFDM Symbols Used
		// For test, consider only 10 symbols
		/*
		Loop_Train_Sym:for(int i=0; i<nUSC; i++)
		{
			local_read= Data_In.read();
			training_symbol[i] = local_read.data;
		}
		*/

		Loop_Preamb_In_2:for(int i=0; i<nUSC; i++)
		{
			local_read= Data_In.read();
			preamble_in[i] = local_read.data;
		}

		for(int j=0; j<nUSC; j++)
		{
			LS_Data_In[j] = preamble_in[j];
		}
		//Perform the LS_Estimation
		(void)LS_Estimate(LS_Data_In, training_symbol, LS_Est);

		// Separate the real and Imaginary values of the LS estimated data
		for(i=0; i<nUSC; i++)
		{
			Norm_In[i] = LS_Est[i].real();
			Norm_In[nUSC+i] = LS_Est[i].imag();
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
			//LS_DNN_Est_Out[i] = De_Norm_Out[i] + I*De_Norm_Out[i+nUSC];
			LS_DNN_Est_Out[i].real(De_Norm_Out[i]);
			LS_DNN_Est_Out[i].imag(De_Norm_Out[i+nUSC]);
			//printf("\n LS Gold(%d) -> %f+%f*i |\t LS_DNN(%d) -> %f+%f*i",(i+1),creal(ls_DNN_gold[i]), cimag(ls_DNN_gold[i]), (i+1), creal(LS_DNN_Est_Out[i]), cimag(LS_DNN_Est_Out[i]));
		}

		//(void)Evaluate_Result(ls_DNN_gold[k], LS_DNN_Est_Out, Act_Channel_weight[k]);

			Loop_HW_Data_Out_2:for(int i=0; i<nUSC; i++)
			{
				local_write.data = LS_DNN_Est_Out[i];
				if(i == nUSC-1)
					local_write.last = 1;
				else
					local_write.last = 0;
				LS_DNN_Out.write(local_write);
			}

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
#endif


#if(IMPL_USED == SOLUTION_FLOAT)
#include "DNN_Data.h"

void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] );
void Normalize(float* , float* , float* , float* );
void DeNormalize(float* , float* , float* , float* );
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC]);
void LS_Estimate(flt_cmplx Preamble_In[nUSC], flt_cmplx Training_Symbol[nUSC], flt_cmplx LS_est[nUSC]);

// Definition of a ReLU function
#define ReLU(z) ((z > 0)? (z) : (0))

void LS_DNN_Estimate_Opt(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_DNN_Out)
{
#pragma HLS INTERFACE axis register both port=LS_DNN_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return

	// The received data is a combination of 52 training symbols and 52 preamble inputs
		axis_data local_read, local_write;
		//std::complex<float> training_symbol[nUSC];
		std::complex<float> preamble_in[nUSC], LS_DNN_Est_Out[nUSC];

		int i,k;
		// Store the Normalized and De-Normalized data
		float Norm_In[2*nUSC], De_Norm_Out[2*nUSC];

		// Array to store the LS_Estimated Data
		flt_cmplx LS_Est[nUSC];

		// Array to temporarily store the received data pertained to each OFDM symbol
		flt_cmplx LS_Data_In[nUSC];

		// Store the Normalized DNN Input and Output
		float DNN_In[nUSC*2], DNN_Out[nUSC*2];

		Loop_Preamb_In:for(int i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			local_read= Data_In.read();
			LS_Data_In[i] = local_read.data;
		}

		//Perform the LS_Estimation
		(void)LS_Estimate(LS_Data_In, training_symbol, LS_Est);

		// Separate the real and Imaginary values of the LS estimated data
		Loop_Sep_Re_Im:for(i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			Norm_In[i] = LS_Est[i].real();
			Norm_In[nUSC+i] = LS_Est[i].imag();
		}

		// Normalize the LS estimated data
		(void)Normalize(Norm_In, Mean_X, SD_X, DNN_In);

		// Pass the Normalized Data into the DNN and extract the DNN predicted data

		LS_DNN(DNN_In,DNN_Out);
		/*
		float DNN_Layer2[LAYER_2_COUNT];

		// Iterate over each neuron in Layer 2 and accumulate the 104 weights with the bias.
		Loop_DNN_Layer1_1:for(int i=0; i<LAYER_2_COUNT; i++)
		{

			// First, assign the bias to the node in layer-2 from layer-1
			DNN_Layer2[i] = 0;
			DNN_Layer2[i] = Layer1_bias[i];
			Loop_DNN_Layer1_2:for(int j=0; j<LAYER_1_COUNT; j++)
			{
				// Layer 2 perceptron evaluation for each neuron
				// Sum up for all the 104 values from the Layer-1 to a node in Layer-2
				// This is done for 52 nodes in layer-2
				DNN_Layer2[i] = DNN_Layer2[i] + (DNN_In[j] * Layer1_weight[j][i]);
			}

			// Post summing up the bias and the weight data, pass this data through ReLU function.
			DNN_Layer2[i] = ReLU(DNN_Layer2[i]);
		}

		Loop_DNN_Layer2_1:for(int i=0; i<LAYER_3_COUNT; i++)
		{
			// First, assign the bias to the node in layer-3 from layer-2
			// Each of 104 nodes in layer 3 has a bias value
			DNN_Out[i] = 0;
			DNN_Out[i] = Layer2_bias[i];

			Loop_DNN_Layer2_2:for(int j=0; j<LAYER_2_COUNT; j++)
			{
				// Layer 3 perceptron evaluation for each neuron
				// Sum up for all the 52 values from the Layer-2 to a node in Layer-3
				// This is done for 104 nodes in layer-3
				DNN_Out[i] = DNN_Out[i] + (DNN_Layer2[j] * Layer2_weight[j][i]);
			}
		}
		*/

		// De-Normalize the DNN Output and combine the real and imaginary values
		//(void)DeNormalize(DNN_Out, Mean_Y, SD_Y, De_Norm_Out);
		// De-Normalize the data using the formula -> (x*sig)+u
		Loop_DeNorm:for(int i=0; i<(2*nUSC); i++)
		{
//#pragma HLS PIPELINE enable_flush rewind
#pragma HLS PIPELINE
			De_Norm_Out[i] = (DNN_Out[i] * ((SD_Y[i])))+(Mean_Y[i]);
		}


			Loop_HW_Data_Out_2:for(int i=0; i<nUSC; i++)
			{
//#pragma HLS PIPELINE rewind
				#pragma HLS PIPELINE

				//local_write.data = LS_DNN_Est_Out[i];
				local_write.data.real(De_Norm_Out[i]);
				local_write.data.imag(De_Norm_Out[i+nUSC]);
				if(i == nUSC-1)
					local_write.last = 1;
				else
					local_write.last = 0;
				LS_DNN_Out.write(local_write);
			}

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
	LS_Est_Loop:for(int i=0; i<nUSC; i++)
	{
#pragma HLS PIPELINE
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
	float DNN_Layer2[LAYER_2_COUNT];
//	ap_fixed<18,4> DNN_Layer2[LAYER_2_COUNT];
	// DNN_Layer2 is partitioned to achieve the initiation interval of 1 for the inner loop "Loop_DNN_Layer1_2"
//#pragma HLS ARRAY_PARTITION variable=DNN_Layer2 complete

	// Iterate over each neuron in Layer 2 and accumulate the 104 weights with the bias.
	Loop_DNN_Layer1_1:for(int i=0; i<LAYER_2_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE

		// First, assign the bias to the node in layer-2 from layer-1
		DNN_Layer2[i] = 0;
		DNN_Layer2[i] = Layer1_bias[i];
		Loop_DNN_Layer1_2:for(int j=0; j<LAYER_1_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=104

			// Layer 2 perceptron evaluation for each neuron
			// Sum up for all the 104 values from the Layer-1 to a node in Layer-2
			// This is done for 52 nodes in layer-2
			DNN_Layer2[i] = DNN_Layer2[i] + (LS_Est_DNN_In[j] * Layer1_weight[j][i]);
		}

		// Post summing up the bias and the weight data, pass this data through ReLU function.
		DNN_Layer2[i] = ReLU(DNN_Layer2[i]);
	}

	Loop_DNN_Layer2_1:for(int i=0; i<LAYER_3_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE rewind
		// First, assign the bias to the node in layer-3 from layer-2
		// Each of 104 nodes in layer 3 has a bias value
		LS_Est_DNN_Out[i] = 0;
		LS_Est_DNN_Out[i] = Layer2_bias[i];

		Loop_DNN_Layer2_2:for(int j=0; j<LAYER_2_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=52
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
	Loop_Norm_Func:for(int i=0; i<(2*nUSC); i++)
	{
#pragma HLS PIPELINE enable_flush rewind

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
	Loop_DeNorm_Func:for(int i=0; i<(2*nUSC); i++)
	{
		Out[i] = (In[i] * ((SD[i])))+(Mean[i]);
	}
}
#endif


#if(IMPL_USED == SOLUTION_OPT_WL1)
#include "DNN_Data.h"

void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] );
void Normalize(float* , float* , float* , float* );
void DeNormalize(float* , float* , float* , float* );
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC]);
void LS_Estimate(fixed_cmplx Preamble_In[nUSC], fixed_cmplx_base Training_Symbol[nUSC], fixed_cmplx LS_est[nUSC]);

// Definition of a ReLU function
#define ReLU(z) ((z > 0)? (z) : (0))

void LS_DNN_Opt_WL1(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_DNN_Out)
{
#pragma HLS INTERFACE axis register both port=LS_DNN_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return

	// The received data is a combination of 52 training symbols and 52 preamble inputs
		axis_data local_read, local_write;
		//std::complex<float> training_symbol[nUSC];
		fixed_cmplx preamble_in[nUSC], LS_DNN_Est_Out[nUSC];

		int i,k;
		// Store the Normalized and De-Normalized data
		float Norm_In[2*nUSC], De_Norm_Out[2*nUSC];

		// Array to store the LS_Estimated Data
		fixed_cmplx LS_Est[nUSC];

		// Array to temporarily store the received data pertained to each OFDM symbol
		fixed_cmplx LS_Data_In[nUSC];

		// Store the Normalized DNN Input and Output
		float DNN_In[nUSC*2], DNN_Out[nUSC*2];

		Loop_Preamb_In:for(int i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			local_read= Data_In.read();
			LS_Data_In[i] = (fixed_cmplx)local_read.data;
		}

		//Perform the LS_Estimation
		(void)LS_Estimate(LS_Data_In, training_symbol, LS_Est);

		// Separate the real and Imaginary values of the LS estimated data
		Loop_Sep_Re_Im:for(i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			Norm_In[i] = LS_Est[i].real();
			Norm_In[nUSC+i] = LS_Est[i].imag();
		}

		// Normalize the LS estimated data
		(void)Normalize(Norm_In, Mean_X, SD_X, DNN_In);

		// Pass the Normalized Data into the DNN and extract the DNN predicted data

		LS_DNN(DNN_In,DNN_Out);
				Loop_DeNorm:for(int i=0; i<(2*nUSC); i++)
		{
//#pragma HLS PIPELINE enable_flush rewind
#pragma HLS PIPELINE
			De_Norm_Out[i] = (DNN_Out[i] * ((SD_Y[i])))+(Mean_Y[i]);
		}


			Loop_HW_Data_Out_2:for(int i=0; i<nUSC; i++)
			{
//#pragma HLS PIPELINE rewind
				#pragma HLS PIPELINE

				//local_write.data = LS_DNN_Est_Out[i];
				local_write.data.real(De_Norm_Out[i]);
				local_write.data.imag(De_Norm_Out[i+nUSC]);
				if(i == nUSC-1)
					local_write.last = 1;
				else
					local_write.last = 0;
				LS_DNN_Out.write(local_write);
			}

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
void LS_Estimate(fixed_cmplx Preamble_In[nUSC], fixed_cmplx_base Training_Symbol[nUSC], fixed_cmplx LS_est[nUSC])
{
	LS_Est_Loop:for(int i=0; i<nUSC; i++)
	{
#pragma HLS PIPELINE
		// Divide the received data at the receiver with the training symbol.
		// This provides the LS estimated data for the channel.
//		LS_est[i] = Preamble_In[i]/Training_Symbol[i];

		LS_est[i].real(Preamble_In[i].real()/Training_Symbol[i]);
		LS_est[i].imag(Preamble_In[i].imag()/Training_Symbol[i]);
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
	float DNN_Layer2[LAYER_2_COUNT];
//	ap_fixed<18,4> DNN_Layer2[LAYER_2_COUNT];
	// DNN_Layer2 is partitioned to achieve the initiation interval of 1 for the inner loop "Loop_DNN_Layer1_2"
//#pragma HLS ARRAY_PARTITION variable=DNN_Layer2 complete

	// Iterate over each neuron in Layer 2 and accumulate the 104 weights with the bias.
	Loop_DNN_Layer1_1:for(int i=0; i<LAYER_2_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE

		// First, assign the bias to the node in layer-2 from layer-1
		DNN_Layer2[i] = 0;
		DNN_Layer2[i] = Layer1_bias[i];
		Loop_DNN_Layer1_2:for(int j=0; j<LAYER_1_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=104

			// Layer 2 perceptron evaluation for each neuron
			// Sum up for all the 104 values from the Layer-1 to a node in Layer-2
			// This is done for 52 nodes in layer-2
			DNN_Layer2[i] = DNN_Layer2[i] + (LS_Est_DNN_In[j] * Layer1_weight[j][i]);
		}

		// Post summing up the bias and the weight data, pass this data through ReLU function.
		DNN_Layer2[i] = ReLU(DNN_Layer2[i]);
	}

	Loop_DNN_Layer2_1:for(int i=0; i<LAYER_3_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE rewind
		// First, assign the bias to the node in layer-3 from layer-2
		// Each of 104 nodes in layer 3 has a bias value
		LS_Est_DNN_Out[i] = 0;
		LS_Est_DNN_Out[i] = Layer2_bias[i];

		Loop_DNN_Layer2_2:for(int j=0; j<LAYER_2_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=52
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
	Loop_Norm_Func:for(int i=0; i<(2*nUSC); i++)
	{
#pragma HLS PIPELINE enable_flush rewind

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
	Loop_DeNorm_Func:for(int i=0; i<(2*nUSC); i++)
	{
		Out[i] = (In[i] * ((SD[i])))+(Mean[i]);
	}
}
#endif


#if(IMPL_USED == SOLUTION_OPT_WL2)
#include "DNN_Data.h"

void LS_DNN(float LS_Est_DNN_In[nUSC*2], float LS_Est_DNN_Out[nUSC*2] );
void Normalize(float* , float* , float* , float* );
void DeNormalize(float* , float* , float* , float* );
void Evaluate_Result(flt_cmplx LS_DNN_Ref[nUSC], flt_cmplx LS_DNN_eval[nUSC]);
void LS_Estimate(fixed_cmplx Preamble_In[nUSC], ap_int<3> Training_Symbol[nUSC], fixed_cmplx LS_est[nUSC]);

// Definition of a ReLU function
#define ReLU(z) ((z > 0)? (z) : (0))

void LS_DNN_Opt_WL2(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_DNN_Out)
{
#pragma HLS INTERFACE axis register both port=LS_DNN_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return

	// The received data is a combination of 52 training symbols and 52 preamble inputs
		axis_data local_read, local_write;
		//std::complex<float> training_symbol[nUSC];
		fixed_cmplx preamble_in[nUSC], LS_DNN_Est_Out[nUSC];

		int i,k;
		// Store the Normalized and De-Normalized data
		float Norm_In[2*nUSC], De_Norm_Out[2*nUSC];

		// Array to store the LS_Estimated Data
		fixed_cmplx LS_Est[nUSC];

		// Array to temporarily store the received data pertained to each OFDM symbol
		fixed_cmplx LS_Data_In[nUSC];

		// Store the Normalized DNN Input and Output
		float DNN_In[nUSC*2], DNN_Out[nUSC*2];

		Loop_Preamb_In:for(int i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			local_read= Data_In.read();
			LS_Data_In[i] = (fixed_cmplx)local_read.data;
		}

		//Perform the LS_Estimation
		(void)LS_Estimate(LS_Data_In, training_symbol, LS_Est);

		// Separate the real and Imaginary values of the LS estimated data
		Loop_Sep_Re_Im:for(i=0; i<nUSC; i++)
		{
#pragma HLS PIPELINE
			Norm_In[i] = LS_Est[i].real();
			Norm_In[nUSC+i] = LS_Est[i].imag();
		}

		// Normalize the LS estimated data
		(void)Normalize(Norm_In, Mean_X, SD_X, DNN_In);

		// Pass the Normalized Data into the DNN and extract the DNN predicted data

		LS_DNN(DNN_In,DNN_Out);
				Loop_DeNorm:for(int i=0; i<(2*nUSC); i++)
		{
//#pragma HLS PIPELINE enable_flush rewind
#pragma HLS PIPELINE
			De_Norm_Out[i] = (DNN_Out[i] * ((SD_Y[i])))+(Mean_Y[i]);
		}


			Loop_HW_Data_Out_2:for(int i=0; i<nUSC; i++)
			{
//#pragma HLS PIPELINE rewind
				#pragma HLS PIPELINE

				//local_write.data = LS_DNN_Est_Out[i];
				local_write.data.real(De_Norm_Out[i]);
				local_write.data.imag(De_Norm_Out[i+nUSC]);
				if(i == nUSC-1)
					local_write.last = 1;
				else
					local_write.last = 0;
				LS_DNN_Out.write(local_write);
			}

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
void LS_Estimate(fixed_cmplx Preamble_In[nUSC], ap_int<3> Training_Symbol[nUSC], fixed_cmplx LS_est[nUSC])
{
	LS_Est_Loop:for(int i=0; i<nUSC; i++)
	{
#pragma HLS PIPELINE
		// Divide the received data at the receiver with the training symbol.
		// This provides the LS estimated data for the channel.
//		LS_est[i] = Preamble_In[i]/Training_Symbol[i];

		LS_est[i].real(Preamble_In[i].real()/Training_Symbol[i]);
		LS_est[i].imag(Preamble_In[i].imag()/Training_Symbol[i]);
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
	float DNN_Layer2[LAYER_2_COUNT];
//	ap_fixed<18,4> DNN_Layer2[LAYER_2_COUNT];
	// DNN_Layer2 is partitioned to achieve the initiation interval of 1 for the inner loop "Loop_DNN_Layer1_2"
//#pragma HLS ARRAY_PARTITION variable=DNN_Layer2 complete

	// Iterate over each neuron in Layer 2 and accumulate the 104 weights with the bias.
	Loop_DNN_Layer1_1:for(int i=0; i<LAYER_2_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE

		// First, assign the bias to the node in layer-2 from layer-1
		DNN_Layer2[i] = 0;
		DNN_Layer2[i] = Layer1_bias[i];
		Loop_DNN_Layer1_2:for(int j=0; j<LAYER_1_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=104

			// Layer 2 perceptron evaluation for each neuron
			// Sum up for all the 104 values from the Layer-1 to a node in Layer-2
			// This is done for 52 nodes in layer-2
			DNN_Layer2[i] = DNN_Layer2[i] + (LS_Est_DNN_In[j] * Layer1_weight[j][i]);
		}

		// Post summing up the bias and the weight data, pass this data through ReLU function.
		DNN_Layer2[i] = ReLU(DNN_Layer2[i]);
	}

	Loop_DNN_Layer2_1:for(int i=0; i<LAYER_3_COUNT; i++)
	{
#pragma HLS UNROLL factor=4
//#pragma HLS PIPELINE rewind
		// First, assign the bias to the node in layer-3 from layer-2
		// Each of 104 nodes in layer 3 has a bias value
		LS_Est_DNN_Out[i] = 0;
		LS_Est_DNN_Out[i] = Layer2_bias[i];

		Loop_DNN_Layer2_2:for(int j=0; j<LAYER_2_COUNT; j++)
		{
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=52
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
	Loop_Norm_Func:for(int i=0; i<(2*nUSC); i++)
	{
#pragma HLS PIPELINE enable_flush rewind

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
	Loop_DeNorm_Func:for(int i=0; i<(2*nUSC); i++)
	{
		Out[i] = (In[i] * ((SD[i])))+(Mean[i]);
	}
}
#endif
