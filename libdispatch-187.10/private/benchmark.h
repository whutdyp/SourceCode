#ifndef __DISPATCH_BENCHMARK__
#define __DISPATCH_BENCHMARK__

#ifndef __DISPATCH_INDIRECT__
#error "Please #include <dispatch/dispatch.h> instead of this file directly."
#include <dispatch/base.h> // for HeaderDoc
#endif

__BEGIN_DECLS

/*!
 * @function dispatch_benchmark
 *
 * @abstract
 * Count the average number of cycles a given block takes to execute.
 *
 * @param count
 * The number of times to serially execute the given block.
 *
 * @param block
 * The block to execute.
 *
 * @result
 * The approximate number of cycles the block takes to execute.
 *
 * @discussion
 * This function is for debugging and performance analysis work. For the best
 * results, pass a high count value to dispatch_benchmark(). When benchmarking
 * concurrent code, please compare the serial version of the code against the
 * concurrent version, and compare the concurrent version on different classes
 * of hardware. Please look for inflection points with various data sets and
 * keep the following facts in mind:
 *
 * 1) Code bound by computational bandwidth may be inferred by proportional
 *    changes in performance as concurrency is increased.
 * 2) Code bound by memory bandwidth may be inferred by negligible changes in
 *    performance as concurrency is increased.
 * 3) Code bound by critical sections may be inferred by retrograde changes in
 *    performance as concurrency is increased.
 * 3a) Intentional: locks, mutexes, and condition variables.
 * 3b) Accidental: unrelated and frequently modified data on the same
 *     cache-line.
 */
#ifdef __BLOCKS__
__OSX_AVAILABLE_STARTING(__MAC_10_6,__IPHONE_4_0)
DISPATCH_EXPORT DISPATCH_NONNULL2 DISPATCH_NOTHROW
uint64_t
dispatch_benchmark(size_t count, void (^block)(void));
#endif

__OSX_AVAILABLE_STARTING(__MAC_10_6,__IPHONE_4_0)
DISPATCH_EXPORT DISPATCH_NONNULL3 DISPATCH_NOTHROW
uint64_t
dispatch_benchmark_f(size_t count, void *ctxt, void (*func)(void *));

__END_DECLS

#endif
