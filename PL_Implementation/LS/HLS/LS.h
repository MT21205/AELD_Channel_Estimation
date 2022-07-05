#include <hls_stream.h>
#include "complex"


#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_half.h>
//#include "complex.h"

#define SOLUTION_1 1
#define SOLUTION_2 2
#define SOLUTION_FLOAT 3
#define SOLUTION_OPT_WL1 4
#define SOLUTION_OPT_WL2 5

#define SOLUTION_USED SOLUTION_OPT_WL2

#define SYMBOLS_USED 200
#define nUSC 52

#if (SOLUTION_USED == SOLUTION_1)

typedef std::complex<float> flt_cmplx;
struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};

void LS_Estimate(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif


#if (SOLUTION_USED == SOLUTION_2)

typedef std::complex<float> flt_cmplx;
struct axis_data
{
	float data;
	ap_uint<1> last;
};

void LS_Estimate_Real(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif

#if (SOLUTION_USED == SOLUTION_FLOAT)

typedef std::complex<float> flt_cmplx;

// Post observing the largest negative number and the fixed point precision, the bits required are
// 18 -> (4,14)
typedef ap_fixed<18,4> fixed_type;
struct axis_data
{
	flt_cmplx data;
	//std::complex<fixed_type> data;
	ap_uint<1> last;
};

void LS_Estimate_Opt(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif

#if (SOLUTION_USED == SOLUTION_OPT_WL1)

typedef std::complex<float> flt_cmplx;

// Post observing the largest negative number and the fixed point precision, the bits required are
// 18 -> (4,14)

typedef ap_fixed<18,4> fixed_cmplx_base;
typedef std::complex<fixed_cmplx_base> fixed_cmplx;
struct axis_data
{
	flt_cmplx data;
	ap_uint<1> last;
};

void LS_Estimate_Opt_WL1(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif

#if (SOLUTION_USED == SOLUTION_OPT_WL2)

typedef std::complex<float> flt_cmplx;

// Post observing the largest negative number and the fixed point precision, the bits required are
// 18 -> (4,14)
typedef ap_fixed<18,4> fixed_type;

typedef ap_fixed<18,4> fixed_cmplx_base;
typedef std::complex<fixed_cmplx_base> fixed_cmplx;
struct axis_data
{
//	std::complex<fixed_type> data;
	flt_cmplx data;
	ap_uint<1> last;
};

void LS_Estimate_Opt_WL2(hls::stream<axis_data> &Data_In, hls::stream<axis_data> &LS_Out);
#endif
