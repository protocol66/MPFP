#ifndef COMMON_H
#define COMMON_H


#include <stdint.h>


#ifndef PRIVATE
	#define PRIVATE static
#endif

#ifndef NULL
	#define NULL 0x0U						// define NULL as 0
#endif

#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE static inline
#endif


/*
* 	MACROS
*/
#define INF_LOOP(x)	(while(1))					// infinite loop macro

/*
* comes from real world testing 1sec = 7sec real world
* this is dependent on the compiler version, unfortunitly
*/
#define DELAY_DIV 7		


/*
*	The purpose of this enum is consolidate different types
*	of error into one data type
*/
typedef enum {
	NONE = 0,
	EMPTY,
	FULL,
} STATUS;

/*
* GLOBAL VARIABLES
*/
uint32_t rcc_ahb_frequency;
uint32_t rcc_apb1_frequency;
uint32_t rcc_apb2_frequency;


/*
*	Precondition:	None
*	Input:			u64 cycles to waist
*	Output:			None
*	Impact:			Will WAIST CPU cycles
*
* 	Description:
* 		this function uses nop (no operation) to create a delay
*		this function is very ineffecient and should be avoided
*/
void fn_delay_loop(uint64_t loops)
{
    while(loops > 0)
    {
       asm("nop");
       loops--;
    }
}

/*
*	Precondition:	None
*	Input:			u32 us to waist
*	Output:			None
*	Impact:			Will WAIST CPU cycles for # of us
*
* 	Description:
* 		this function uses nop (no operation) to create a delay
*		this function is very ineffecient and should be avoided
*/
// extern inline
void DELAY_US(uint32_t u32_us)	{
	fn_delay_loop(u32_us * ((rcc_ahb_frequency / ((uint32_t)1e6 * DELAY_DIV)) + 1));	// +1 to prevent 0
}

/*
*	Precondition:	None
*	Input:			u32 ms to waist
*	Output:			None
*	Impact:			Will WAIST CPU cycles for # of ms
*
* 	Description:
* 		this function uses nop (no operation) to create a delay
*		this function is very ineffecient and should be avoided
*/
extern inline
void DELAY_MS(uint32_t u32_ms)	{
	fn_delay_loop(u32_ms * (rcc_ahb_frequency / ((uint32_t)1e3 * DELAY_DIV)));
}


__STATIC_INLINE
uint8_t NUM2ASCII(uint8_t u8_num) {
	return '0' + u8_num;
}

__STATIC_INLINE
uint8_t ASCII2NUM(uint8_t u8_ascii) {
	return u8_ascii - '0';
}

__STATIC_INLINE
void MEMCOPY(uint8_t *pu8_dest, uint8_t *pu8_src, uint32_t u32_len) {
	for(uint32_t i = 0; i < (u32_len * sizeof(u32_len)); i++) {
		pu8_dest[i] = pu8_src[i];
	}
}

__STATIC_INLINE
int STRING_CHK_EQ(char *pc_str1, char *pc_str2) {
	for(uint32_t i = 0; (pc_str1[i] != 0) && (pc_str2[i] != 0); i++) {
		if(pc_str1[i] != pc_str2[i]) {
			return 0;
		}
	}
	return 1;
}


#endif