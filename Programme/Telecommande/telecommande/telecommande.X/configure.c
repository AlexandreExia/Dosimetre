#include <p33Fxxxx.h>   // A modifier suivant le uC utilis�
#include "defines.h"

extern int Tdelais;

/*--------------------------------------------------------          
  Function   : void InitCPU_Clock(void)
  Description: Initialisation de l'horloge CPU
     - Les bits de configuration (d�ja programm�s) r�alisent les s�lections 
       suivantes : 
         - horloge de base = FRC = 7.37MHz � 2%
         - PLL activ�e (s�lection S1)
     - Les registres du module horloge sont affect�s par cette fonction pour
       obtenir : 
         - FRCDIVN = 7,37MHz
         - Fosc = 73,7 MHz (7,37*10)
         - FCY  = 36,85MHz � 2% pr�s
-----------------------------------------------------------*/
void InitCPU_Clock(void)
{
    CLKDIVbits.FRCDIV=0;    // FRC non divis�e : 7,37MHz
    CLKDIVbits.PLLPRE=0;    // Pr�division par 2 avant PLL : 3,685MHz
    PLLFBDbits.PLLDIV=40-2; // PLL : multiplication par 40 : 147,4MHz
    CLKDIVbits.PLLPOST=0;   // Division par 2 apr�s PLL : Fosc = 73,7MHz
}

/*---------------------------------------------------------------
  Nom          : void InitTimer1 (void)
  Description :  Programme d'initialisation du timer 1 en base de temps 1ms
   Arguments    : aucun
  Valeur renvoy�e : aucune
 * 
 * 
 * Temps d'actualisation : FCY / 256 / 144 = 1 ms
-----------------------------------------------------------------*/
void InitTimer1 (void)
{ 
    
    PR1=144; //base de temps 
    T1CON=0x8030; // TCKPS=10 FcY div 256 ,Timer on, TGATE=0 TCS=0
    T1CONbits.TON=1  ; //timer on
    IEC0bits.T1IE=1  ; // validation des interruptions du Timer 1
    TMR1=0;
}

/*---------------------------------------------------------------
  Nom          : void InitTimer1 (void)
  Description :  Programme d'interruption du timer 1
   Arguments    : aucun
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/
void _ISR _T1Interrupt(void)  //
{
    TMR1=0;  // RAZ de TMR1
    IFS0bits.T1IF=0; // Acquitement interruption
    Tdelais++;
}

/*---------------------------------------------------------------
  Nom          : void Delay(int delais)
  Description :  Programme de temporisation
   Arguments    : dur�e en ms
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/
void Delay(int delais)
{
    Tdelais=0;
    while (Tdelais < delais){}
    
}