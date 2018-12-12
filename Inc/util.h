#ifndef _USER_INCLUDE_UTIL_H_
#define _USER_INCLUDE_UTIL_H_

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))

#define container_of(ptr, type, member) ({			        \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


#define BIT(nr)			(1UL << (nr))

/*
 * Check at compile time that something is of a particular type.
 * Always evaluates to 1 so you may use it easily in comparisons.
 */
#define typecheck(type,x)           \
({	type __dummy;                   \
	typeof(x) __dummy2;             \
	(void)(&__dummy == &__dummy2);  \
	1;                              \
})

/*
 * Check at compile time that 'function' is a certain type, or is a pointer
 * to that type (needs to use typedef for the function type.)
 */
#define typecheck_fn(type,function) \
({	typeof(type) __tmp = function;  \
	(void)__tmp;                    \
})

/*
 *	These inlines deal with timer wrapping correctly. You are 
 *	strongly encouraged to use them
 *	1. Because people otherwise forget
 *	2. Because if the timer wrap changes in future you won't have to
 *	   alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a,b)		        \
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)((b) - (a)) < 0))
#define time_before(a,b)	time_after(b,a)

#define time_after_eq(a,b)	        \
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)((a) - (b)) >= 0))

#define time_before_eq(a,b)	time_after_eq(b,a)

#define __cmp(x, y, op)	((x) op (y) ? (x) : (y))

#define __careful_cmp(x, y, op)  __cmp(x, y, op)

/**
 * min - return minimum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define min(x, y)	__careful_cmp(x, y, <)

/**
 * max - return maximum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define max(x, y)	__careful_cmp(x, y, >)

/**
 * min3 - return minimum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#define min3(x, y, z) min((typeof(x))min(x, y), z)

/**
 * max3 - return maximum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#define max3(x, y, z) max((typeof(x))max(x, y), z)

/**
 * min_t - return minimum of two values, using the specified type
 * @type: data type to use
 * @x: first value
 * @y: second value
 */
#define min_t(type, x, y)	__careful_cmp((type)(x), (type)(y), <)

/**
 * max_t - return maximum of two values, using the specified type
 * @type: data type to use
 * @x: first value
 * @y: second value
 */
#define max_t(type, x, y)	__careful_cmp((type)(x), (type)(y), >)

void util_critical_section_enter(void);

void util_critical_section_exit(void);

#endif /* #ifndef _USER_INCLUDE_UTIL_H_ */
