#include "LS.h"

#if((SOLUTION_USED == SOLUTION_1) || (SOLUTION_USED == SOLUTION_2) || (SOLUTION_USED == SOLUTION_FLOAT))
flt_cmplx training_Symbol[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};
#endif

#if(SOLUTION_USED == SOLUTION_OPT_WL1)
//flt_cmplx training_Symbol[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};
fixed_cmplx_base training_Symbol[nUSC] = {1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0};
#endif

#if(SOLUTION_USED == SOLUTION_OPT_WL2)
//flt_cmplx training_Symbol[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};
ap_int<3> training_Symbol[nUSC] = {1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0};
#endif


#if(SOLUTION_USED == SOLUTION_1)

// Solution 1 reads the preamble input in the complex format

void LS_Estimate(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out)
{
#pragma HLS INTERFACE axis register both port=LS_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return
	// The received data is a combination of 52 training symbols and 52 preamble inputs
	axis_data local_read, local_write;
	//std::complex<float> training_Symbol[nUSC];
	flt_cmplx preamble_In[nUSC], LS_Est[nUSC];

	Loop_Pre_In:for(int i=0; i<nUSC; i++)
	{
		local_read= Data_In.read();
		preamble_In[i] = local_read.data;

		LS_Est[i] = preamble_In[i]/training_Symbol[i];

		local_write.data = LS_Est[i];
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
	}

}

#endif


#if(SOLUTION_USED == SOLUTION_2)

// Solution 2 reads the preamble input in the real format
// --> a[0].real , a[0].imag, a[1].real , a[1].imag

void LS_Estimate_Real(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out)
{
#pragma HLS INTERFACE axis register both port=LS_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return
	// The received data is a combination of 52 training symbols and 52 preamble inputs
	axis_data local_read, local_write;
	//std::complex<float> training_Symbol[nUSC];
	flt_cmplx preamble_In[nUSC], LS_Est[nUSC];



	Loop_Pre_In:for(int i=0; i<nUSC; i++)
	{
		local_read= Data_In.read();
		preamble_In[i].real(local_read.data);

		local_read= Data_In.read();
		preamble_In[i].imag(local_read.data);

		LS_Est[i] = preamble_In[i]/training_Symbol[i];

#if(SOLUTION_USED == SOLUTION_1)
		local_write.data = LS_Est[i];
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
#endif

#if(SOLUTION_USED == SOLUTION_2)
		local_write.data = LS_Est[i].real();
		LS_Out.write(local_write);

		local_write.data = LS_Est[i].imag();
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
#endif
	}
}

#endif


#if(SOLUTION_USED == SOLUTION_FLOAT)

// Solution 3 reads the preamble input in the complex format
// Optimized only for pipelining and unrolling

void LS_Estimate_Opt(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out)
{
#pragma HLS INTERFACE axis register both port=LS_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return
	// The received data is a combination of 52 training symbols and 52 preamble inputs
	axis_data local_read, local_write;
	//std::complex<float> training_Symbol[nUSC];
	flt_cmplx preamble_In[nUSC], LS_Est[nUSC];
//#pragma HLS ARRAY_PARTITION variable=LS_Est block factor=2 dim=1
//#pragma HLS ARRAY_PARTITION variable=preamble_In block factor=2 dim=1
#pragma HLS ARRAY_PARTITION variable=LS_Est complete dim=1
#pragma HLS ARRAY_PARTITION variable=preamble_In complete dim=1


	Loop_Pre_In:for(int i=0; i<nUSC; i++)
	{
#pragma HLS PIPELINE II=1
		local_read= Data_In.read();
		preamble_In[i] = local_read.data;

		LS_Est[i] = preamble_In[i]/training_Symbol[i];

		local_write.data = LS_Est[i];
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
	}

}

#endif


#if(SOLUTION_USED == SOLUTION_OPT_WL1)

// Solution 4 reads the preamble input in the complex format
// Optimized only for pipelining, unrolling and WL optimization (Part 1)
void LS_Estimate_Opt_WL1(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out)
{
#pragma HLS INTERFACE axis register both port=LS_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return
	// The received data is a combination of 52 training symbols and 52 preamble inputs
	axis_data local_read, local_write;
	//std::complex<float> training_Symbol[nUSC];
	fixed_cmplx preamble_In[nUSC];
	fixed_cmplx LS_Est[nUSC];
//#pragma HLS ARRAY_PARTITION variable=LS_Est block factor=2 dim=1
#pragma HLS ARRAY_PARTITION variable=preamble_In complete dim=1
#pragma HLS ARRAY_PARTITION variable=LS_Est complete dim=1
//#pragma HLS ARRAY_PARTITION variable=preamble_In complete dim=1


	Loop_Pre_In:for(int i=0; i<nUSC; i++)
	{
//#pragma HLS UNROLL factor=5
#pragma HLS PIPELINE
		local_read= Data_In.read();
		preamble_In[i] =(fixed_cmplx) local_read.data;

//		LS_Est[i] = preamble_In[i]/((fixed_cmplx)training_Symbol[i]);
		LS_Est[i].real(preamble_In[i].real()/training_Symbol[i]);
		LS_Est[i].imag(preamble_In[i].imag()/training_Symbol[i]);

//		local_write.data = (flt_cmplx)LS_Est[i];
		local_write.data.real((fixed_cmplx_base)LS_Est[i].real());
		local_write.data.imag((fixed_cmplx_base)LS_Est[i].imag());
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
	}

}

#endif

#if(SOLUTION_USED == SOLUTION_OPT_WL2)

// Solution 4 reads the preamble input in the complex format
// Optimized only for pipelining, unrolling and WL optimization (Part 1)

void LS_Estimate_Opt_WL2(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out)
{
#pragma HLS INTERFACE axis register both port=LS_Out
#pragma HLS INTERFACE axis register both port=Data_In
#pragma HLS INTERFACE ap_ctrl_none port=return
	// The received data is a combination of 52 training symbols and 52 preamble inputs
	axis_data local_read, local_write;
	//std::complex<float> training_Symbol[nUSC];
	fixed_cmplx preamble_In[nUSC];
	fixed_cmplx  LS_Est[nUSC];
#pragma HLS ARRAY_PARTITION variable=LS_Est block factor=2 dim=1
#pragma HLS ARRAY_PARTITION variable=preamble_In block factor=2 dim=1
//#pragma HLS ARRAY_PARTITION variable=LS_Est complete dim=1
//#pragma HLS ARRAY_PARTITION variable=preamble_In complete dim=1


	Loop_Pre_In:for(int i=0; i<nUSC; i++)
	{
//#pragma HLS UNROLL factor=2
#pragma HLS PIPELINE
		local_read= Data_In.read();
		preamble_In[i] =(fixed_cmplx) local_read.data;

//		LS_Est[i] = preamble_In[i]/((fixed_cmplx)training_Symbol[i]);
		LS_Est[i].real(preamble_In[i].real()/training_Symbol[i]);
		LS_Est[i].imag(preamble_In[i].imag()/training_Symbol[i]);

		local_write.data = (fixed_cmplx)LS_Est[i];
		if(i == nUSC-1)
			local_write.last = 1;
		else
			local_write.last = 0;
		LS_Out.write(local_write);
	}

}

#endif
