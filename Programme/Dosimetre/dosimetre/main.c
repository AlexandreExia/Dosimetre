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

char seqResponse[SIZE_SEQBUFF]={0};      // Tableau contenant la r�ponse  envoy� par la telecommande
int countU2=0;                  // Compteur permettant d'acc�der au tableau seqReponse

int Tdelais=0;                  // Compteur pour le timer

/*
    Structure contenant les variables pour configurer le wifi.
*/

extern Wifi configurationWifi; // Structure pour configurer le wifi


/*
 * Variables contenant les informations de l'utilisateur
*/

int userID = 0, activityID = 0;


/*
 Variables contenant les informations de l'activit�es
 
	ded: valeur du d�bit �quivalent dose fix� en ammont par l'instructeur
	coeffExpo: coefficient d'exposition ""
	limitDose: Dose th�orique calcul�e pour l'activit� ""
	
	Alarme sur d�bit : ded > 1.20 * ded
	Alarme sur dose  : dose > 1.20 * dose th�orique
	Pr�-alarme sur dose : dose > 1.05 * dose th�orique
*/

float limitDed, coeffExpo, limitDose, impuls;

/*
 Variables contenant les valeurs de retour de l'activit� et de calcul des valeurs
 * 
 * dedMax : d�bit equivalent dos maximum enregistr� lors de l'activit�
 * userDose : dose accumul� par la personne lors de l'activit�
 * numAlarm: nombre total d'alarme d�clench�e
 * typeAlarm : type d'alarme d�clench�e (DED, PREDOSE, DOSE)
 * alarmState : variable utile au fonctionnement interne, elle d�finit l'�tat de l'alarme
 * alarmPreDose : ""                                    "", elle d�finit si l'alarme predose � d�j� �t� d�clench� ou no
 * alarmTimeout : ""                                    "", elle contient la valeur en ms du temps de fonctionnement de l'alarme
 * time : ""                                            "", elle contient la valeur du temps �coul�s depuis le d�but de l'activit�.
 * timeDed : ""                                            "", elle contient la fr�quence des BIP lors d'une alarme de d�bit
 */
float dedMax, userDose;
int numAlarm, alarmState, alarmPreDose, alarmPreDed, alarmDose;
char typeAlarm[32] = {0};
long alarmTimeout;
float time, timeDed;


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
    void PlaySound(int timeout)
 *  
 *      Joue un son
 *      timeout = temps en ms avant la fin du son (-1 = infini)              
-----------------------------------------------------------------*/
void PlaySound(long timeout)
{
    if(!alarmState)
    {
        RPOR6 = 0x1200;
        alarmTimeout = timeout;
        alarmState = 1;
    }
}

/*---------------------------------------------------------------
    void StopSound()
 *  
 *       Arr�te le son           
-----------------------------------------------------------------*/
void StopSound()
{
    if(alarmState)
    {
        RPOR6 = 0;
        alarmTimeout = 0;
        alarmState = 0;
    }
}

/*---------------------------------------------------------------
    int IsAlarmRing()
 *  
 *       Retourne l'�tat de l'alarme          
-----------------------------------------------------------------*/
int IsAlarmRing()
{
    return (alarmState);
}

/*---------------------------------------------------------------
                Fonction principale                      
-----------------------------------------------------------------*/

int main(void)
{
    InitCPU_Clock();
    InitTimer1();
    InitBuzzer();
   
    RPOR6 = 0; // Eteindre le buzzer, aucun signal n'est inject� sur la patte
    
    AD1PCFGL=0x01FF; // Pas d'entr�e analogique
  
    InitUART1(57600);
    InitUART2(57600);
    LcdInit();
    InitWifi(10);
    
    Delay(1000);
    
    StartServer();
    Delay(2000);
    
    start:
        
    userID = 10, activityID = 10, limitDed  = 15, coeffExpo = 1, limitDose = 10, impuls = 0, numAlarm = 0, dedMax = 0, userDose = 0;
    int i = 0;
    for(i;i < sizeof(typeAlarm);i++) typeAlarm[i] = 0;
    
    StopSound();
    
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString("En attente d'authentification...");
            
          
    ClearUartSeqBuffer();
    
    while(!userID) 
    {
		/*
			Format d''authentification : x AUTH
		*/
        if(strstr(seqResponse, "AUTH"))
        {
            char *end;
            userID = strtol(seqResponse, &end, 10);  // On extrait l'entier
            break;
        }
    }
            
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString("En attente de RTR...");
    
    ClearUartSeqBuffer();
    
    
	while(!activityID || !limitDed || !coeffExpo || !limitDose)
	{
		/*
			Format d'activit� : DED x.xx DOSE x.xxx COEFF x.xxx ACT
		*/
        if(strstr(seqResponse, "ACT"))
        {
            
            /*
                Extraction des parties flottantes dans la chaine de caract�re
            */
            char *end;
            activityID = strtol (seqResponse, &end, 10);
            limitDed = (float)strtod(end, &end);
            dedMax = limitDed;
            limitDose = (float)strtod(end, &end); 
            coeffExpo = (float)strtod(end, &end); 
                   
            Fill_Lcd(0);
            LcdGotoXY(0,0);
            char activity[128] = {0};
            snprintf(activity, sizeof(activity), "%d %f %f %f", activityID, limitDed, limitDose, coeffExpo);
            LcdString(activity);
            Delay(5000);
            break;
        }
	}
    
    ClearUartEspBuffer();
    
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString("Debranchez le dosimetre...");
    
    while(_RA0 == 0) {}
    
    while(1)
    {
        // Si la patte _RA0 est � l'�tat bas, cela signifie que le dosim�tre est branch� en RS232
        if(_RA0 == 0)
        {
			/*	
				Formatage et envoi des valeurs de sortie sur l'UART2
			*/
            Delay(1000);
            char activity[128];
            snprintf(activity, sizeof(activity), "%d %d %f %f %d %s ", userID, activityID, userDose, dedMax, numAlarm, typeAlarm); 
            SendData2(activity);
            Fill_Lcd(0);
            LcdGotoXY(0,0);
            LcdString(activity);
            Delay(5000);
            goto start;
        }
        Fill_Lcd(0);
		// Si un boost est detect� sur la liaison s�rie
        if(strstr(espResponse, "!BOOST"))
        {
            LcdGotoXY(0,0);
            LcdString("Boost");   
                  
            ClearUartEspBuffer();
            impuls++;
        }
		// Si un deboost est detect� sur la liaison s�rie
        else if(strstr(espResponse, "!DEBOOST"))
        {
            LcdGotoXY(0,0);
            LcdString("deboost");   
            
            ClearUartEspBuffer();
            if((impuls-1) >= 0) impuls--;
            
        }
		// Si un raz est detect� sur la liaison s�rie
        else if(strstr(espResponse, "!RAZ"))
        {
            LcdGotoXY(0,0);
            LcdString("raz");   
            
            ClearUartEspBuffer();
            impuls=0; 
        }

        
        time+=0.5;
        
		// Condition permettant de v�rifier et d'affecter le nouveau D�bit max
        if(limitDed * (1 + impuls * 0.05) > dedMax) dedMax = limitDed * (1 + impuls * 0.05);
		
		// Si la dose a d�pass� la limite de dose et qu'elle n'est pas d�clench�e
        if(userDose > (limitDose * 1.20) && !alarmDose)
        {
            PlaySound(-1); // D�clenchement de l'alarme
            numAlarm++;
			alarmDose = 1;
			// Formatage des valeurs pour typeAlarm 
            if(strlen(typeAlarm) == 0) snprintf(typeAlarm, sizeof(typeAlarm), "%s", "DOSE");
            else snprintf(typeAlarm, sizeof(typeAlarm), "%s+%s", typeAlarm, "DOSE");
        }
		// Si la dose a d�pass� la limite de pr�-dose et qu'elle n'est pas d�clench�e
        else if((userDose > (limitDose * 1.05)) && !alarmPreDose)
        {
            alarmPreDose = 1; // Flag de l'alarme pre dose mis � 1
            PlaySound(20000); // D�clenchement de l'alarme pendant 20 secondes (20000 ms)
            numAlarm++;
			// Formatage des valeurs pour typeAlarm 
            if(strlen(typeAlarm) == 0) snprintf(typeAlarm, sizeof(typeAlarm), "%s", "PREDOSE");
            else snprintf(typeAlarm, sizeof(typeAlarm), "%s+%s", typeAlarm, "PREDOSE");
        }
		// Si le d�bit a d�pass� la limite de d�bit
        else if(limitDed * (1 + impuls * 0.05) > (1.20 * limitDed))
        {
			// Si l'alarme n'est pas d�clench�
            if(!IsAlarmRing())
            {
				// Si c'est la premi�re fois que l'alarme de preded 
                if(!alarmPreDed)
                {
                    if(strlen(typeAlarm) == 0) snprintf(typeAlarm, sizeof(typeAlarm), "%s", "DED");
                    else snprintf(typeAlarm, sizeof(typeAlarm), "%s+%s", typeAlarm, "DED");
                    alarmPreDed = 1;
                    numAlarm++;
                }
				/*
					G�n�ration d'un BIP pendant 200 ms tous les 500 ms pour un son discontinu
				*/
                if(!timeDed) timeDed = time;
                
                if( (timeDed + 500) > time)
                    PlaySound(200),
                    timeDed=0;
            }
        }
		// Calcul de la dose
        userDose += limitDed * (1 + impuls * 0.05) * coeffExpo * (time/3600);
        
        // Affichage de la dose
        LcdGotoXY(0,2);
        char string[32] = {0};
        snprintf(string, sizeof(string), "DOSE= %.3fmSv", userDose);
        LcdString(string);
        Delay(500);
    }
 
}
