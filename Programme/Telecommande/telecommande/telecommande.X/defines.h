/* 
 * File:   defines.h
 * Author: ariehl
 *
 * Created on 16 mars 2016, 16:55
 */

#ifndef DEFINES_H
#define	DEFINES_H

#define DEBUG_MODE 1 // Mode debug desactivé = 0 | activé = 1


#define BIT0  0b0000000000000001
#define BIT1  0b0000000000000010
#define BIT2  0b0000000000000100
#define BIT3  0b0000000000001000
#define BIT4  0b0000000000010000
#define BIT5  0b0000000000100000
#define BIT6  0b0000000001000000
#define BIT7  0b0000000010000000
#define BIT8  0b0000000100000000
#define BIT9  0b0000001000000000
#define BIT10 0b0000010000000000
#define BIT11 0b0000100000000000
#define BIT12 0b0001000000000000
#define BIT13 0b0010000000000000
#define BIT14 0b0100000000000000
#define BIT15 0b1000000000000000

/*--------------
LCD Nokia 3310
----------------*/

#define Lcd_DIN    LATAbits.LATA9   // patte 35
#define Lcd_CLK    LATAbits.LATA4   // patte 34
#define Lcd_DC      LATCbits.LATC3  // patte 36
#define Lcd_CE      LATCbits.LATC5  // patte 38
#define Lcd_RST     LATCbits.LATC4  // patte 37
#define LCD_PinsOut TRISC&=~(BIT3|BIT4|BIT5); TRISA&=~(BIT4|BIT9);
#define LCDInitState LATC|=(BIT4|BIT5); // CE=RST="1"


#define SIZE_ESPBUFF            800
#define SIZE_SEQBUFF            350
#define MAX_DOSIMETRES          50

#define FCY     36850000 // Fosc= ... MHz et Fcy= ... MHz



#endif	/* DEFINES_H */

