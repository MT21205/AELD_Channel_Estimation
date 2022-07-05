#include "LS_includes.h"

void LS_Est_benchmark(flt_cmplx preamble_in[nUSC], flt_cmplx train_symb[nUSC], flt_cmplx LS_Est[nUSC]);

int main()
{
	flt_cmplx LS_Est[nUSC];
	axis_data local_read, local_write;
	hls::stream<axis_data> Data_In, LS_Est_HW;
	flt_cmplx train_symb[nUSC] = {1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, -1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, -1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i, 1.0+0.0i};
	//flt_cmplx preamble_in[nUSC] = {0.5459772-0.4880519i,0.8790944-0.8822657i,-0.6479526-0.7465139i,-0.2717896-0.2826404i,-0.1217755-2.019557i,-0.719882-1.672069i,1.194433-1.213394i,-0.6458694-0.2443696i,0.1272094-2.289653i,0.9480821-0.1561816i,1.406977-0.9470038i,1.203361-1.871369i,0.4062152-2.429436i,-0.5790945-2.298569i,-1.227149-1.524593i,1.422152-1.677826i,0.69964-2.407079i,0.5671915+0.2797354i,1.368559-0.3674096i,-1.293211-0.8004617i,0.9776554-2.245352i,0.2449852+0.3340301i,-0.9156283-2.06653i,-1.289778-1.131082i,-0.9473457-0.1936199i,-0.07689207+0.2698267i,1.401448-0.752307i,-1.037165-0.5073627i,-0.3461401+0.08104564i,-0.295201-2.243995i,-0.918331-1.664733i,1.238208-1.334357i,-0.5413172-0.2072798i,0.04349336-2.152192i,0.8410231-0.3657208i,-0.8626714-1.168902i,-0.6457508-0.5326736i,-0.0880438-0.205132i,0.5236132-0.3180105i,0.8948071-0.7853363i,-0.627568-0.8285561i,-0.2489989-0.4425727i,-0.01869058-1.773948i,-0.3880223-1.483399i,0.6996596-1.121107i,-0.2592294-0.739746i,0.1827878-1.537053i,0.3060538-0.7576446i,-0.1667529-1.232578i,-0.1452889-1.072442i,-0.08224314-0.9781799i,-0.02111639-0.9146066i};

	for(int k=0; k<SYMBOLS_USED; k++)
	{

		for(int j=0; j<nUSC; j++)
		{
			#if(SOLUTION_USED == SOLUTION_1)
				local_write.data = preamble_in[j];
				Data_In.write(local_write);
				//printf("\nData_in[%d] = %f + %f*i", j, local_write.data.real(), local_write.data.imag());
			#endif

			#if(SOLUTION_USED == SOLUTION_2)
				local_write.data = preamble_in[j].real();
				Data_In.write(local_write);
				local_write.data = preamble_in[j].imag();
				Data_In.write(local_write);
			#endif

			#if((SOLUTION_USED == SOLUTION_FLOAT) || (SOLUTION_USED == SOLUTION_OPT_WL1) || (SOLUTION_USED == SOLUTION_OPT_WL2))
				local_write.data = preambleIn[k][j];
				Data_In.write(local_write);
				//printf("\nData_in[%d] = %f + %f*i", j, local_write.data.real(), local_write.data.imag());
			#endif

		}

		LS_Est_benchmark(preambleIn[k],train_symb, LS_Est);

	#if(SOLUTION_USED == SOLUTION_1)
		LS_Estimate(Data_In, LS_Est_HW);
	#endif

	#if(SOLUTION_USED == SOLUTION_2)
		LS_Estimate_Real(Data_In, LS_Est_HW);
	#endif

	#if(SOLUTION_USED == SOLUTION_FLOAT)
		LS_Estimate_Opt(Data_In, LS_Est_HW);
	#endif

	#if(SOLUTION_USED == SOLUTION_OPT_WL1)
		LS_Estimate_Opt_WL1(Data_In, LS_Est_HW);
	#endif

	#if(SOLUTION_USED == SOLUTION_OPT_WL2)
		LS_Estimate_Opt_WL2(Data_In, LS_Est_HW);
	#endif


		for(int j=0; j<nUSC; j++)
		{

	#if(SOLUTION_USED == SOLUTION_1)
			local_read = LS_Est_HW.read();
	#endif

	#if(SOLUTION_USED == SOLUTION_2)
			flt_cmplx temp;
			local_read = LS_Est_HW.read();
			temp.real(local_read.data);
			local_read = LS_Est_HW.read();
			temp.imag(local_read.data);
	#endif

	#if((SOLUTION_USED == SOLUTION_FLOAT) || (SOLUTION_USED == SOLUTION_OPT_WL1) || (SOLUTION_USED == SOLUTION_OPT_WL2))
			local_read = LS_Est_HW.read();
	#endif


	#if(SOLUTION_USED == SOLUTION_1)
			if(((local_read.data.real() - LS_Est[j].real()) > 0.001) || ((local_read.data.imag() - LS_Est[j].imag()) > 0.001 ))
	#endif
	#if(SOLUTION_USED == SOLUTION_2)
			if(((temp.real() - LS_Est[j].real()) > 0.001) || ((temp.imag() - LS_Est[j].imag()) > 0.001 ))
	#endif
	#if((SOLUTION_USED == SOLUTION_FLOAT) || (SOLUTION_USED == SOLUTION_OPT_WL1) || (SOLUTION_USED == SOLUTION_OPT_WL2))
			if((((float)(local_read.data.real()) - LS_Est[j].real()) > 0.001) || (((float)local_read.data.imag() - LS_Est[j].imag()) > 0.001 ))
	#endif
			{
				printf("\n ****** Error !! ******\n");
				printf("\nError in iteration %d\n", (j+1));
	#if(SOLUTION_USED == SOLUTION_1)
				printf("LS_Est_SW = %f + %f*i ; LS_Est_HW = %f + %f*i\n",LS_Est[j].real(), LS_Est[j].imag(), local_read.data.real(), local_read.data.imag());
	#endif
	#if(SOLUTION_USED == SOLUTION_2)
				printf("LS_Est_SW = %f + %f*i ; LS_Est_HW = %f + %f*i\n",LS_Est[j].real(), LS_Est[j].imag(), temp.real(), temp.imag());
	#endif
	#if((SOLUTION_USED == SOLUTION_FLOAT) || (SOLUTION_USED == SOLUTION_OPT_WL1) || (SOLUTION_USED == SOLUTION_OPT_WL2))
				printf("LS_Est_SW = %f + %f*i ; LS_Est_HW = %f + %f*i\n",LS_Est[j].real(), LS_Est[j].imag(), local_read.data.real(), local_read.data.imag());
	#endif
				return 1;
			}
		}
		printf("\n***** No errors ***** in %d\n", (k));
		}

	return 0;
}

void LS_Est_benchmark(flt_cmplx preamble_in[nUSC], flt_cmplx train_symb[nUSC],  flt_cmplx LS_Est[nUSC])
{
	for(int i=0; i<nUSC; i++)
	{
		LS_Est[i] = preamble_in[i]/train_symb[i];
		//printf("\n%d. %f + %fI",(i+1), LS_Est[i].real(), LS_Est[i].imag());
	}
}
