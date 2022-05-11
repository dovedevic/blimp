//
// Vector Matrix Multiplication / vmatmul
//

.global _start
_start:

enable:	                       // enable the vector extension
  csrr      t0, mstatus
  li        t1, (0x1 <<  9)    //  VS bit position for v0.9
  or        t1, t1, t0
  csrw      mstatus, t1 


addi t1, x0, 1        // define trials
#ifdef T_10
    slli t1, t1, 5    // 32 Trials
#endif
#ifdef T_100
    slli t1, t1, 9    // 512 Trials
#endif
#ifdef T_1000
    slli t1, t1, 10   // 1024 Trials
#endif
#ifdef T_10000
    slli t1, t1, 14   // 16384 Trials
#endif
#ifdef T_100000
    slli t1, t1, 17   // 131072 Trials
#endif
#ifdef T_1000000
    slli t1, t1, 20   // 1048576 Trials
#endif
#ifdef T_10000000
    slli t1, t1, 24   // 16777216 Trials
#endif


loop:                 // Start the trials

set_elements:         // Set the dimmension of the matrix
    addi t2, x0, 1        // define n = 1
    #ifdef S_256B
        slli t2, t2, 3    // n = 8, or 256B
    #endif
    #ifdef S_1KB
        slli t2, t2, 4    // n = 16, or 1KB
    #endif
    #ifdef S_64KB
        slli t2, t2, 7    // n = 128, or 64KB
    #endif
    #ifdef S_256KB
        slli t2, t2, 8    // n = 256, or 256KB
    #endif
    #ifdef S_1MB
        slli t2, t2, 9    // n = 512, or 1MB
    #endif
    #ifdef S_4MB
        slli t2, t2, 10   // n = 1024, or 4MB
    #endif
    #ifdef S_16MB
        slli t2, t2, 11   // n = 2048, or 16MB
    #endif
    
    addi a7, x0, 1024
    #ifdef R_1024B
        slli t3, t3, 1    // dummy operation to keep everything 4-byte aligned
    #endif
    #ifdef R_2048B
        slli a7, a7, 1
    #endif
    #ifdef R_512B
        srli a7, a7, 1
    #endif

load_offsets:           // Load addresses   
    la      t4, data_a  // set a matrix
    mul     t3, t2, t2  // define number of elements
    slli    t6, t3, 2   // Calculate the region size, elements * 4
    add     t5, t4, t6  // define b matrix to be a matrix + region size
    add     t6, t5, t6  // define c result matrix to be b matrix + region size
    
    

//
//
// Begin BLIMP-V Benchmark
//
//

vmatrix32:    
    mv      a0, t4         // ptr to current row
    mv      a1, t4         // ptr to current local row
    mv      a2, t5         // ptr to current col
    mv      a3, t6         // ptr to current result row

    mv      a4, x0         // i row index
    mv      a5, x0         // j col index

outerloop:
    mv       t3, t2        // define number of elements left to process in the row/col
    vmv.s.x  v9, x0        // Zero the accumulator, set v9[0] = 0
    vsetvli  t0, t3, e32   // Set vector length based on 32-bit vectors
    mv       a1, a0        // Reset the local row pointer to the current row pointer
    vle32.v  v0, (a0)      // Get row vector

innerloop:
    vle32.v    v1, (a2)      // Get column vector
    vmul.vv    v2, v0, v1    // Multiply the vectors
    vredsum.vs v9, v2, v9    // Sum the vector v2[0] = v9[0] + sum(v2[i])
    sub        t3, t3, t0
    
    // Is this a multi RB row and column?
    beqz       t3, finished_multicol
        add      a1, a1, a7      // Go to next local row by a1 + a7 (row buffer size)
        add      a2, a2, a7      // Go to next col by a2 + a7 (row buffer size)
        vle32.v  v0, (a1)        // Get new sub-local-row vector
        vsetvli  t0, t3, e32     // Set vector length based on 32-bit vectors
        j        innerloop
  
finished_multicol:

    vmv.x.s  a6, v9        // Set a6 = v9[0], aka sum(a[row] * b[col])
    sw       a6, (a3)      // Store result
    addi     a3, a3, 4     // Bump res pointer by a 32b integer
    
    add      a2, a1, a7    // Go to next col by a1 + a7 (row buffer size)
    addi     a5, a5, 1     // bump j  
  
    bne      a5, t2, outerloop
  
innerdone:
    addi     a4, a4, 1     // bump i
    mv       a5, x0        // reset j
    mv       a2, t5        // reset ptr to current col
    mv       a0, a1        // set row ptr to the local row ptr
    add      a0, a0, a7    // Go to next row by a0 + a7 (row buffer size)
    bne      a4, t2, outerloop

finally:

//
//
// End BLIMP-V Benchmark 
//
//

addi t1, t1, -1    // Trial done
bnez t1, loop      // Loop back if we still have trials

// Trials ended, exit nicely.

exit:
  li    a0, 0   // Use 0 return code
  li    a7, 93  // Service command code 93 terminates
  ecall         // Call linux to terminate the program
  
code_padding:
  .string "xxxxx" // Update this to align code to 4-byte width
  .set code_padding_size, . - code_padding
