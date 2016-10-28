#include <p33Fxxxx.h>  
#include "wifi.h"
#include "defines.h"
#include <string.h>

Wifi configurationWifi;
extern char espResponse[SIZE_ESPBUFF];

void SetWifiMode(int mode)
{
    if(mode == MODE_CLIENT)
    {
        SendData("AT+CWMODE=1\r\n");
   
    }
    else if(mode == MODE_SERVER)
    {
        SendData("AT+CWMODE=2\r\n");

    }
}

void SetWifiIP(char* IP)
{
    char atIP[128];
    snprintf(atIP, sizeof(atIP), "AT+CIPAP=\"%s\"\r\n", IP);
    SendData(atIP);
}

void SetMultiConnection(int cipmux)
{
    if(!cipmux) SendData("AT+CIPMUX=0\r\n");
    else SendData("AT+CIPMUX=1\r\n");
}

void InitSSID(char* SSID, char* pass, int canal)
{
    char atSSID[128];
    snprintf(atSSID, sizeof(atSSID), "AT+CWSAP=\"%s\",\"%s\",%d,0\r\n", SSID, pass, canal);
    SendData(atSSID);
}

void StartServer()
{
    SendData("AT+CIPSERVER=1,1500\r\n");
}

void StopServer()
{
    SendData("AT+CIPSERVER=0,1500\r\n");
}

void InitWifi(int dosiNum)
{
    /*
        Paramètre du module
     */
    
    char IP[32] = {0};
    snprintf(IP, sizeof(IP), "192.168.1.%d", dosiNum);
    
    configurationWifi.IP = IP;
    configurationWifi.mode = 2;
	configurationWifi.connected = 0;
    
   
    Fill_Lcd(0);  
    LcdGotoXY(0,0); 
    LcdString("Initialisation");
    LcdGotoXY(0,2); 
    LcdString(IP);
    LcdGotoXY(0,3); 
    if(configurationWifi.mode == 1)
        LcdString("Mode : client");
    else if(configurationWifi.mode == 2)
        LcdString("Mode : serveur");
    
    SendData("AT+RST\r\n");
    Delay(5000); // Temporisation pour le reset
  
    
    ClearUartEspBuffer();
            
    /*
        Configuration du module en mode client
     */

    SetWifiMode(configurationWifi.mode);
    Delay(100); // Attente de la réponse

    ClearUartEspBuffer();
    /*
        Configuration de l'ip du module
     */
    
    SetWifiIP(configurationWifi.IP); 
    Delay(250); // Attente d'une réponse
   
    ClearUartEspBuffer();
    
    /*
        Modification du nombre de connexion en simultanés (0 pour un client)
     */
    SetMultiConnection(1);      
    Delay(100);
    ClearUartEspBuffer();
    
    char SSID[8] = {0};
    snprintf(SSID, sizeof(SSID), "DOSI%d!", dosiNum);
    InitSSID(SSID, "", 1);
    Delay(4000);
    
    ClearUartEspBuffer();
    
}
