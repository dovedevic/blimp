/* Copyright (c) 2007, Stanford University
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Stanford University nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#include "MapReduceScheduler.h"
#include "stddefines.h"
#include "stopwatch.h"

typedef struct {
   int row_num;
   int *matrix_A;
   int *matrix_B;
   int matrix_len;
   int *output;
} mm_data_t;

typedef struct {
	int x_loc;
	int y_loc;
	int value;
} mm_key_t;

void matrixmult_map(void *args_in);

/** matrixmul_splitter()
 *  Assign a set of rows of the output matrix to each thread
 */
void matrixmult_splitter(void *data_in)
{
   //pthread_attr_t attr;
   //pthread_t * tid;
   int i, num_procs;

	/* Make a copy of the mm_data structure */
   mm_data_t * data = (mm_data_t *)data_in; 

   /* Check whether the various terms exist */
   assert(data_in);   
   assert(data->matrix_len >= 0);
   assert(data->matrix_A);
   assert(data->matrix_B);
   assert(data->output);

   //CHECK_ERROR((num_procs = sysconf(_SC_NPROCESSORS_ONLN)) <= 0);
   num_procs = 128;
   printf("The number of processors is %d\n", num_procs);

   //tid = (pthread_t *)MALLOC(num_procs * sizeof(pthread_t));
   /* Thread must be scheduled systemwide */
   //pthread_attr_init(&attr);
   //pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

   int req_rows = data->matrix_len / num_procs;


   for(i=0; i<1; i++)
   {
	   map_args_t* out = (map_args_t*)malloc(sizeof(map_args_t));
	   mm_data_t* data_out = (mm_data_t*)malloc(sizeof(mm_data_t));
	   memcpy(data_out,data, sizeof(mm_data_t));

	   int available_rows = data->matrix_len - data->row_num;
	   out->length = (req_rows < available_rows)? req_rows:available_rows;
	   out->data = data_out;

		   if(i == (num_procs - 1))
			   out->length = data->matrix_len - i*req_rows;

	   data->row_num += out->length;
	   dprintf("Allocated rows is %d data->row_num is %d\n",out->length, data->row_num);
	   fflush(stdout);

	   //CHECK_ERROR(pthread_create(&tid[i], &attr, matrixmult_map, (void*)out) != 0);
       matrixmult_map((void*)out);

   }
   /* Barrier, wait for all threads to finish */
   for (i = 0; i < 1; i++)
   {
      int ret_val;
      //CHECK_ERROR(pthread_join(tid[i], (void **)(void*)&ret_val) != 0);
	  //CHECK_ERROR(ret_val != 0);
   }
   //free(tid);
}

/** matrixmul_map()
 * Function that computes the products for each of the portions assigned to the thread
 */
void matrixmult_map(void *args_in)
{
   map_args_t* args = (map_args_t*)args_in;

   int row_count = 0;
   int i,j, x_loc, y_loc, value;
   int * a_ptr,* b_ptr;   

   assert(args);
   
   mm_data_t* data = (mm_data_t*)(args->data);
   assert(data);

   while(row_count < args->length)
   {
	   a_ptr = data->matrix_A + (data->row_num + row_count)*data->matrix_len;

	   for(i=0; i < data->matrix_len ; i++)
	   {
		   b_ptr = data->matrix_B + i;
		   value = 0;

		   for(j=0;j<data->matrix_len ; j++)
		   {
			   value += ( a_ptr[j] * (*b_ptr));
			   b_ptr+= data->matrix_len;
		   }
		   x_loc = (data->row_num + row_count);
		   y_loc = i;
		   //printf("THe location is %d %d, value is %d\n",x_loc, y_loc, value);
		   data->output[x_loc*data->matrix_len + i] = value;
	   }
	   row_count++;	
   }
   free(args->data);
   free(args);
   return;
}

int main(int argc, char *argv[]) {
   START_PROGRAM();
   int i,j, create_files;
   FILE* fd_A;
   FILE* fd_B;
   FILE* fd_out;
   int file_size;
   char * fdata_A, *fdata_B;
   int matrix_len;
   
   char * fname_A, *fname_B, *fname_out;
   int *matrix_A_ptr, *matrix_B_ptr;

   
   
   srand( (unsigned)time( NULL ) );

   // Make sure a filename is specified
   if (argv[1] == NULL)
   {
      printf("USAGE: %s [side of matrix] [size of Row block]\n", argv[0]);
      exit(1);
   }

   fname_A = "matrix_file_A.txt";
   fname_B = "matrix_file_B.txt";
   fname_out = "matrix_file_out_pthreads.txt";
   CHECK_ERROR ( (matrix_len = atoi(argv[1])) < 0);
   file_size = ((matrix_len*matrix_len))*sizeof(int);

   fprintf(stderr, "***** file size is %d\n", file_size);

   printf("MatrixMult_pthreads: Side of the matrix is %d\n", matrix_len);
   printf("MatrixMult_pthreads: Running...\n");

START_CHRONO(0);
    
    
    fd_out = fopen(fname_out, "w");

   fd_A = fopen(fname_A, "r");
   fseek(fd_A, 0, SEEK_END);
   long finfo_A_st_size = ftell(fd_A);
   fseek(fd_A, 0, SEEK_SET);
   fdata_A = (char*)malloc(sizeof(char) * finfo_A_st_size + 1);
   fread(fdata_A, sizeof(char), finfo_A_st_size, fd_A);
   
   
   fd_B = fopen(fname_B, "r");
   fseek(fd_B, 0, SEEK_END);
   long finfo_B_st_size = ftell(fd_B);
   fseek(fd_B, 0, SEEK_SET);
   fdata_B = (char*)malloc(sizeof(char) * finfo_B_st_size + 1);
   fread(fdata_B, sizeof(char), finfo_B_st_size, fd_B);
    

STOP_CHRONO(0);
   // Setup splitter args
   mm_data_t mm_data;
   mm_data.matrix_len = matrix_len;
   mm_data.matrix_A = NULL;
   mm_data.matrix_B = NULL;
   mm_data.row_num = 0;

   mm_data.output = (int*)malloc(matrix_len*matrix_len*sizeof(int));
   
   mm_data.matrix_A = matrix_A_ptr = ((int *)fdata_A);
   mm_data.matrix_B = matrix_B_ptr = ((int *)fdata_B);

   printf("MatrixMult_pthreads: Calling MapReduce Scheduler Matrix Multiplication\n");

   //gettimeofday(&starttime,0);
   
   START_CHRONO(1);
   matrixmult_splitter(&mm_data);
   STOP_CHRONO(1);

   //gettimeofday(&endtime,0);

   //printf("MatrixMult_pthreads: Multiply Completed time = %ld\n", (endtime.tv_sec - starttime.tv_sec));

   //for(i=0;i<matrix_len*matrix_len;i++)
   //{
	//   if(i%matrix_len == 0)
	//	   dprintf("\n");

	//   dprintf("%d ",mm_data.output[i]);

	   //write(fd_out,&(mm_data.output[i]),sizeof(int));
   //}
   //dprintf("\n");

   dprintf("MatrixMult_pthreads: MapReduce Completed\n");

   free(mm_data.output);

   fclose(fd_A);
   fclose(fd_B);
   fclose(fd_out);

STOP_PROGRAM();
   return 0;
}
