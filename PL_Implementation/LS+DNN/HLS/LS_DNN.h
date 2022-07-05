#include <hls_stream.h>
#include "complex"
#include <ap_fixed.h>

#define IMPL_FLOAT 1
#define IMPL_FIXED 2
#define SOLUTION_FLOAT 3
#define SOLUTION_OPT_WL1 4
#define SOLUTION_OPT_WL2 5

#define IMPL_USED SOLUTION_OPT_WL2

// Number of used sub-carriers in one OFDM symbol
#define NUM_USED_SUB_CARRIERS 52
#define nUSC NUM_USED_SUB_CARRIERS
#define SYMBOLS_USED 200


#if(IMPL_USED == IMPL_FLOAT)
#include <ap_int.h>
typedef std::complex<float> flt_cmplx;
struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};


void LS_DNN_Estimate(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif


#if(IMPL_USED == IMPL_FIXED)
#include <ap_fixed.h>
typedef std::complex<float> flt_cmplx;
// 24 bit value with 10 integer bits
typedef ap_fixed<24,10> DType_fixed;
//typedef float DType_fixed;
typedef std::complex<DType_fixed> flt_cmplx_fixed;
struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};


void LS_DNN_Estimate_FixType(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif


#if(IMPL_USED == SOLUTION_FLOAT)
#include <ap_int.h>
typedef std::complex<float> flt_cmplx;
struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};


void LS_DNN_Estimate_Opt(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif

#if(IMPL_USED == SOLUTION_OPT_WL1)
#include <ap_int.h>

typedef std::complex<float> flt_cmplx;
typedef ap_fixed<20,4> fixed_cmplx_base;
typedef std::complex<fixed_cmplx_base> fixed_cmplx;

//typedef float fixed_cmplx_base;
//typedef std::complex<fixed_cmplx_base> fixed_cmplx;

struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};


void LS_DNN_Opt_WL1(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif

#if(IMPL_USED == SOLUTION_OPT_WL2)
#include <ap_int.h>

typedef std::complex<float> flt_cmplx;
typedef ap_fixed<20,4> fixed_cmplx_base;
typedef std::complex<fixed_cmplx_base> fixed_cmplx;

//typedef float fixed_cmplx_base;
//typedef std::complex<fixed_cmplx_base> fixed_cmplx;

struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};


void LS_DNN_Opt_WL2(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif
