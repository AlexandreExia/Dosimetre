/******************************************************************************
 ***                        Projet XXXXXXXXXXXXXXXX                         ***
 ***                 Mettre ici le type de microcontroleur                  ***
 ***  Description :                                                         ***
 ***    D�crire ici le programme en quelques mots                           ***
 ***  Auteur  :                                                             ***
 ***  Version :                                                             ***
 ***  Date de cr�ation :                                                    ***
 ***  Derni�re mise � jour :                                                ***
 ******************************************************************************/

/*------------------------------------------------------------------------------
                    Librairies et fichiers inclus
------------------------------------------------------------------------------*/
#include <p33Fxxxx.h>   
#include <stdlib.h>
#include <string.h>
#include "wifi.h"
#include "defines.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~ Bits de configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
_FBS(RBS_LARGE_RAM & BSS_NO_FLASH & BWRP_WRPROTECT_OFF);
     // Large Sized Boot Ram
     // No Boot Segment Program Memory
     // Write Protect Disabled
_FSS(RSS_NO_RAM & SSS_NO_FLASH & SWRP_WRPROTECT_OFF);
     // Secure Segment Data Ram: No Secure Ram
     // Secure Segment Program Memory: No Secure Segment
     // Write Protect Disabled 
_FGS(GSS_OFF & GCP_OFF & GWRP_OFF);
     // Code Protect off
     // Write Protect Disabled
_FOSCSEL(FNOSC_FRCPLL & IESO_OFF); 
     // Fast RC oscillator w/ divide and PLL
     // Two-speed Oscillator Startup disabled
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_OFF & POSCMD_NONE);
     // Clock Switching is enabled and Fail Safe Clock Monitor is disabled
     // Single configuration for remappable I/O disabled
     // OSC2 Pin Function: OSC2 is RA3
     // Horloge externe inhib�e
_FWDT(FWDTEN_OFF & WINDIS_OFF); 	
     // Watchdog Timer disabled by user software
     // Windowed WDT disabled
_FPOR(ALTI2C_OFF & FPWRT_PWR128);
     // I2C mapped to SDA1/SCL1
     // Power-on Reset Value: 128mS
_FICD(JTAGEN_OFF & ICS_PGD1);
     // JTAG is disabled
     // ICD communicate on PGEC1/EMUEC1
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------------------------------------------------------------------
                      D�clarations des �quivalences
------------------------------------------------------------------------------*/
#define FCY         36850000 // Fosc= ... MHz et Fcy= ... MHz
/*------------------------------------------------------------------------------
                     D�clarations des variables globales
------------------------------------------------------------------------------*/

char espResponse[SIZE_ESPBUFF]={0};      // Tableau contenant la r�ponse aux commandes AT (RESET = 270 car.)
int countU1=0;                  // Compteur permettant d'acc�der au tableau espResponse

char seqResponse[SIZE_SEQBUFF]={0};      // Tableau contenant la r�ponse Le s�quence envoy� par le raspberry
int countU2=0;                  // Compteur permettant d'acc�der au tableau seqReponse

int Tdelais=0;                  // Compteur pour le timer

/*
    Structure contenant les variables pour configurer le wifi.
*/
Wifi configurationWifi; // Structure pour configurer le wifi

/*
 * Variable contenant l'activit� charg�
*/
char sequence[SIZE_SEQBUFF] = {0};

/*
 * Tableau contenant la liste des dosimetres
*/
int dosimetres[MAX_DOSIMETRES] = {0};



/*---------------------------------------------------------------
    Vider le tableau contenant la s�quence charg� dans la t�l�commande                   
-----------------------------------------------------------------*/
    
void ClearSeqBuffer(void)
{
    int i;
    for (i=0;i<sizeof(sequence);i++) sequence[i]=0;
}

/*---------------------------------------------------------------
    Retourne 1 si le caract�re est un chiffre sinon 0                   
-----------------------------------------------------------------*/
    
int IsCharDigit(char c)
{
    switch(c)
    {
        case '1': return 1;
        case '2': return 1;
        case '3': return 1;
        case '4': return 1;
        case '5': return 1;
        case '6': return 1;
        case '7': return 1;
        case '8': return 1;
        case '9': return 1;
        default : return 0;
    }
    return 0;
}

/*---------------------------------------------------------------
                Fonction principale                      
-----------------------------------------------------------------*/

int main(void)
{
    InitCPU_Clock();
    InitTimer1();
    AD1PCFGL=0x01FF; // Pas d'entr�e analogique
    
    InitClav();
    InitUART1(57600);
    InitUART2(57600);
    LcdInit();
    InitWifi();
    
  
    ShowMenu();
    
    while(1)
    {}   
    
 }  


        


