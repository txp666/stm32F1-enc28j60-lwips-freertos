#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"

#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

typedef enum { FALSE   = 0, TRUE    = !FALSE } boolean, bool_t;

/* MII and RMII mode selection, for STM324xG-EVAL Board(MB786) RevB ***********/
#define RMII_MODE  	// User have to provide the 50 MHz clock by soldering a 50 MHz
                     // oscillator (ref SM7745HEV-50.0M or equivalent) on the U3
                     // footprint located under CN3 and also removing jumper on JP5. 
                     // This oscillator is not provided with the board. 
                     // For more details, please refer to STM3240G-EVAL evaluation
                     // board User manual (UM1461).

                                     
//#define MII_MODE

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef 	MII_MODE
 #define PHY_CLOCK_MCO
#endif

/**
 * @brief TCP Client netconn or socket
 *
 */
#define TCP_CLINET_NETCONN		1
#define TCP_CLINET_SOCKET		0

/**
 * @brief TCP Server netconn or socket
 *
 */
#define TCP_SERVER_NETCONN		1
#define TCP_SERVER_SOCKET		0

#endif // __MAIN_H__


