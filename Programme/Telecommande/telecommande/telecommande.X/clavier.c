/*------------------------------------------------------------------------------
                    Librairies et fichiers inclus
------------------------------------------------------------------------------*/
#include <p33Fxxxx.h>   
#include "defines.h"

#define C1 _RC8
#define C2 _RB10
#define C3 _RB11

#define L1 _LATB12
#define L2 _LATB13
#define L3 _LATB14
#define L4 _LATB15


void InitClav()
{
    /*
        Configuration des bits B11,10 et C8 en entrée
     */
    _TRISB10 = 1, _TRISB11 = 1;
    _TRISC8=1;
    
    _CN20PUE=1;_CN16PUE=1; _CN15PUE=1; // Mise à 1 des résistances pullup du clavier
}

char Lecture_Clav(void)
{
    _TRISB12 = 0;
    Delay(20);
    
    if(C1==0) { _TRISB12 = 1; return('1'); }
    if(C2==0) { _TRISB12 = 1; return('2'); }
    if(C3==0) { _TRISB12 = 1; return('3'); }
    _TRISB12 = 1;

    _TRISB13 = 0;
    Delay(20);   // antirebonds
    
    if(C1==0) { _TRISB13 = 1; return('4'); }
    if(C2==0) { _TRISB13 = 1; return('5'); }
    if(C3==0) { _TRISB13 = 1; return('6'); }
    _TRISB13 = 1;
    
    _TRISB14 = 0;
    Delay(20);   // antirebonds
    
    if(C1==0) { _TRISB14 = 1;  return('7'); }
    if(C2==0) { _TRISB14 = 1;  return('8'); }
    if(C3==0) { _TRISB14 = 1;  return('9'); }
    
    _TRISB14 = 1;
    
    _TRISB15 = 0;
    Delay(20);   // antirebonds
    
    if(C1==0) { _TRISB15 = 1;  return('*'); }
    if(C2==0) { _TRISB15 = 1;  return('0'); }
    if(C3==0) { _TRISB15 = 1;  return('#'); }
    
    _TRISB15 = 1;
    return 'n';
}

