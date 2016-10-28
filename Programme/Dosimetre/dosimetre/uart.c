#include <p33Fxxxx.h>   
#include "defines.h"

extern int countU1;
extern int countU2;
extern char espResponse[SIZE_ESPBUFF];
extern char seqResponse[SIZE_SEQBUFF];
/*------------------------------------------------------------------------------
                                 C�blage
------------------------------------------------------------------------------*/
#define U1_Tx RPOR2=0x0003;    // U1Tx du pic cabl� sur RP4
#define U1_Rx RPINR18=0x1F05;  // U1Rx du pic cabl� sur RP5

#define U2_Tx RPOR1=0x0005;    // U2Tx du pic cabl� sur RP2
#define U2_Rx RPINR19=0x1F03;  // U2Rx du pic cabl� sur RP3

/*---------------------------------------------------------------
  Nom          : void InitUART1 (long Bauds)
  Description :  Programme d'interruption d�clench� � la r�ception
                 d'un caract�re
  Arguments    : Bauds
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/

void InitUART1(long Bauds)
{
    U1_Tx;  // c�blage TxD
    U1_Rx;  // c�blage RxD
    
    U1MODE=0x8000;  // Module UART valid�, 8 bits, pas de parit�, 1 bit stop
                    // Pas d'inversion des signaux RX1 et TX1
                    // RTS et CTS non utilis�s
    U1STA =0x0400; // UART TX valid�
                   // Interruption RX � chaque caract�re
    U1BRG =FCY/((long)16*Bauds)-1; // Vitesse de transmission 
 
    _U1RXIP=7;     // Priorit� maximum car buffer hard de faible capacit�
    _U1RXIF=0;     // Raz indicateur interruption RX
    _U1RXIE=1;     // Validation interruption RX
    
    ClearUartEspBuffer();
}

/*---------------------------------------------------------------
  Nom          : void InitUART1 (long Bauds)
  Description :  Programme d'interruption d�clench� � la r�ception
                 d'un caract�re
  Arguments    : Bauds
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/

void InitUART2(long Bauds)
{
    U2_Tx;  // c�blage TxD
    U2_Rx;  // c�blage RxD
    
    U2MODE=0x8000;  // Module UART valid�, 8 bits, pas de parit�, 1 bit stop
                    // Pas d'inversion des signaux RX1 et TX1
                    // RTS et CTS non utilis�s
    U2STA =0x0400; // UART TX valid�
                   // Interruption RX � chaque caract�re
    U2BRG =FCY/((long)16*Bauds)-1; // Vitesse de transmission 
 
    _U2RXIP=7;     // Priorit� maximum car buffer hard de faible capacit�
    _U2RXIF=0;     // Raz indicateur interruption RX
    _U2RXIE=1;     // Validation interruption RX
    
    ClearUartSeqBuffer();
}


/*---------------------------------------------------------------
  Nom          : void _ISR _U1RXInterrupt (void)
  Description :  Programme d'interruption d�clench� � la r�ception
                 d'un caract�re
  Arguments    : aucun
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/

void _ISR _U1RXInterrupt(void)
{
    _U1RXIF=0;              // Clear interrupt flag
    while (U1STAbits.URXDA) // Boucle pour lire ts les car. ds le buf. de l'UART
    {
        if(countU1 + 1 < sizeof(espResponse))
            espResponse[countU1++]=U1RXREG;
    }
} 

/*---------------------------------------------------------------
  Nom          : void _ISR _U1RXInterrupt (void)
  Description :  Programme d'interruption d�clench� � la r�ception
                 d'un caract�re
  Arguments    : aucun
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/

void _ISR _U2RXInterrupt(void)
{
    _U2RXIF=0;              // Clear interrupt flag
    while (U2STAbits.URXDA) // Boucle pour lire ts les car. ds le buf. de l'UART
    {
        seqResponse[countU2++] = U2RXREG;
    }
} 

void SendData(char* message)
{
    int i=0, var=1;
    
    for(i=0;message[i]!='\0';i++)
    {
        while (U1STAbits.TRMT==0){}
        U1TXREG=message[i];
        int j=0;
        for(j=0;j<1000;j++){var++;} 
    }    
}

void SendData2(char* message)
{
    int i=0, var=1;
    for(i=0;message[i]!='\0';i++)
    {
        while (U2STAbits.TRMT==0){}
        U2TXREG=message[i];
        int j=0;
        for(j=0;j<1000;j++){var++;} 
    }    
}

/*---------------------------------------------------------------
  Nom          : void ClearEspBuffer(void)
  Description :  Programme qui vide le Buffer de r�ception RXD
   Arguments    : dur�e en ms
  Valeur renvoy�e : aucune
-----------------------------------------------------------------*/

void ClearUartEspBuffer(void)
{
    int i;
    for (i=0;i<sizeof(espResponse);i++) espResponse[i]=0;
    countU1=0;
}

void ClearUartSeqBuffer(void)
{
    int i;
    for (i=0;i<sizeof(seqResponse);i++) seqResponse[i]=0;
    countU2=0;
}

int IsUart2Connected()
{
    return !_RA0;
}