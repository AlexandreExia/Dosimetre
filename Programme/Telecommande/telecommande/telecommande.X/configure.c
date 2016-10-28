#include <p33Fxxxx.h>   // A modifier suivant le uC utilisé
#include "defines.h"

extern int Tdelais;

/*--------------------------------------------------------          
  Function   : void InitCPU_Clock(void)
  Description: Initialisation de l'horloge CPU
     - Les bits de configuration (déja programmés) réalisent les sélections 
       suivantes : 
         - horloge de base = FRC = 7.37MHz à 2%
         - PLL activée (sélection S1)
     - Les registres du module horloge sont affectés par cette fonction pour
       obtenir : 
         - FRCDIVN = 7,37MHz
         - Fosc = 73,7 MHz (7,37*10)
         - FCY  = 36,85MHz à 2% près
-----------------------------------------------------------*/
void InitCPU_Clock(void)
{
    CLKDIVbits.FRCDIV=0;    // FRC non divisée : 7,37MHz
    CLKDIVbits.PLLPRE=0;    // Prédivision par 2 avant PLL : 3,685MHz
    PLLFBDbits.PLLDIV=40-2; // PLL : multiplication par 40 : 147,4MHz
    CLKDIVbits.PLLPOST=0;   // Division par 2 après PLL : Fosc = 73,7MHz
}

/*---------------------------------------------------------------
  Nom          : void InitTimer1 (void)
  Description :  Programme d'initialisation du timer 1 en base de temps 1ms
   Arguments    : aucun
  Valeur renvoyée : aucune
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
  Valeur renvoyée : aucune
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
   Arguments    : durée en ms
  Valeur renvoyée : aucune
-----------------------------------------------------------------*/
void Delay(int delais)
{
    Tdelais=0;
    while (Tdelais < delais){}
    
}