#define _POSIX_C_SOURCE 200809L //Suggested by *AI* to avoid errors, recall being metioned in class
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <alloca.h>

// Helper to get monotonic time in nanoseconds
static unsigned long long monotonic_ns(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        fprintf(stderr, "clock_gettime error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return (unsigned long long)ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

// Function to perform one iteration of the alloca test.
// This is done to ensure 'alloca' memory is freed upon return (stack cleanup).
// Iam following Model 1: Allocate + memcpy + touch per iteration
static unsigned long long alloca_iteration(size_t size, volatile int *sink) {
    unsigned long long start, end;
    
    start = monotonic_ns();
    
    // Allocate on the stack will be automatically freed upon return
    char *a = alloca(size); 
    char *b = alloca(size); 
    
    // Check for potential alloca failure
    if (!a || !b) {
        fprintf(stderr, "alloca failed or stack exhausted for size %zu\n", size);
        exit(EXIT_FAILURE);
    }
    
    // Fill buffer A and copy A to B
    memset(a, 1, size);
    memcpy(b, a, size);
    
    // "Touch" / "Consume" a byte sum to prevent dead-code elimination
    int sum = 0;
    for (size_t k = 0; k < size; k++) {
        sum += b[k];
    }
    *sink = sum; // Use volatile to prevent optimization
    
    end = monotonic_ns();
    
    return end - start;
}

int main(void) {
    const int iterations = 100;
    
    printf("MEMORY_PERF_START SIZE_RANGE=64-1048576 ITERATIONS=%d\n", iterations);
    
    int num_malloc_tests = 0;
    int num_alloca_tests = 0;
    volatile int sink = 0; // Sink for consuming byte sums *Sugessted by GeminiAI to fix errors*
    
    // Loop through all sizes from 2^6 to 2^20
    for (int exp = 6; exp <= 20; exp++) {
        size_t size = 1ull << exp;
        
        // --- MALLOC BENCHMARK ---
        unsigned long long total_time_malloc = 0;
        for (int i = 0; i < iterations; i++) {
            unsigned long long start, end;
            
            // Using Model 1: Allocate + memcpy + touch + free per iteration
            start = monotonic_ns();
            
            char *a = malloc(size);
            char *b = malloc(size);
            
            if (!a || !b) {
                fprintf(stderr, "malloc failed for size %zu\n", size);
                exit(EXIT_FAILURE);
            }
            
            // Fill buffer A and copy A to B
            memset(a, 1, size);
            memcpy(b, a, size);
            
            // "Touch" / "Consume" a byte sum to prevent dead-code elimination
            int sum = 0;
            for (size_t k = 0; k < size; k++) {
                sum += b[k];
            }
            sink = sum; // Use volatile to prevent optimization
            
            free(a); // Free
            free(b);
            
            end = monotonic_ns();
            total_time_malloc += (end - start);
        }
        
        // Print MALLOC results
        double avg_time_malloc = (double)total_time_malloc / iterations;
        printf("ALLOCATION_METHOD=malloc SIZE=%zu AVG_TIME_NS=%.0f\n", size, avg_time_malloc);
        num_malloc_tests++;
        
        // --- ALLOCA BENCHMARK ---
        unsigned long long total_time_alloca = 0;
        for (int i = 0; i < iterations; i++) {
            // Each iteration calls a function that returns, clearing the stack for alloca
            total_time_alloca += alloca_iteration(size, &sink); 
        }
        
        // Print ALLOCA results
        double avg_time_alloca = (double)total_time_alloca / iterations;
        printf("ALLOCATION_METHOD=alloca SIZE=%zu AVG_TIME_NS=%.0f\n", size, avg_time_alloca);
        num_alloca_tests++;
    }
    
    // Final line
    printf("MEMORY_PERF_COMPLETE MALLOC_TESTS=%d ALLOCA_TESTS=%d TOTAL_TESTS=%d\n", 
           num_malloc_tests, num_alloca_tests, num_malloc_tests + num_alloca_tests);
    
    fflush(stdout);
    return 0;
}
