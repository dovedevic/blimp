//
// Vector Array Addition / vadd
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

set_elements:         // Set the number of elements to process
    addi t2, x0, 1        // define elements to process
    #ifdef S_32MB
        slli t2, t2, 23   // 8388608 Elements, or 32MB
    #endif
    #ifdef S_16MB
        slli t2, t2, 22   // 4194304 Elements, or 16MB
    #endif
    #ifdef S_8MB
        slli t2, t2, 21   // 2097152 Elements, or 8MB
    #endif
    #ifdef S_4MB
        slli t2, t2, 20   // 1048576 Elements, or 4MB
    #endif
    #ifdef S_1MB
        slli t2, t2, 18   // 262144 Elements, or 1MB
    #endif
	#ifdef S_1KB
        slli t2, t2, 8    // 256 Elements, or 1KB
    #endif

load_offsets:         // Load addresses   
    la    t4, data_a    // set a[i]
    #ifndef S_32MB
        slli t3, t2, 2      // Region size elements * 4 (32bit int is 4 bytes)
        
        #ifndef S_16MB      // Tri-region
            add  t5, t4, t3     // define b[i] to be a[i] + region size
            add  t6, t5, t3     // define c[i] to be b[i] + region size
        #endif
	    #ifdef S_16MB       // Duo-region
            add  t5, t4, t3     // define b[i] to be a[i] + region size
        #endif
    #endif

//
//
// Begin BLIMP-V Benchmark
//
//

vvadd32:
    vsetvli t0, t2, e32   // Set vector length based on 32-bit vectors
	
    vle32.v v0, (t4)      // Get first vector
	
    #ifdef S_32MB             // Solo-region
        vadd.vv v2, v0, v0    // Sum vectors
		vse32.v v2, (t4)      // Store result
    #endif
    #ifndef S_32MB
        vle32.v v1, (t5)      // Get second vector
        
        #ifndef S_16MB        // Tri-region
		    vadd.vv v2, v0, v1    // Sum vectors
			vse32.v v2, (t6)      // Store result
        #endif
        #ifdef S_16MB         // Duo-region
            vadd.vv v2, v0, v1    // Sum vectors
			vse32.v v2, (t5)      // Store result
        #endif
    #endif
    
    sub t2, t2, t0        // Decrement number done
	slli t0, t0, 2        // Get the number done in bytes
    add  t4, t4, t0       // Bump a pointer by a RB
    add  t5, t5, t0       // Bump b pointer by a RB
    add  t6, t6, t0       // Bump c pointer by a RB

    bnez t2, vvadd32      // Loop back

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
