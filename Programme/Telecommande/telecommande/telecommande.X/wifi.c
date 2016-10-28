#include <p33Fxxxx.h>  
#include "wifi.h"
#include "defines.h"
#include <string.h>

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
    snprintf(atIP, sizeof(atIP), "AT+CIPSTA=\"%s\"\r\n", IP);
    SendData(atIP);

}

void SetMultiConnection(int cipmux)
{
    if(!cipmux) SendData("AT+CIPMUX=0\r\n");
    else SendData("AT+CIPMUX=1\r\n");
 
}

void ConnectToSSID(char* SSID, char* password)
{
    char atSSID[128];
    snprintf(atSSID, sizeof(atSSID), "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, password);
    SendData(atSSID);
}

void ConnectToServer(char* IP, char* port)
{
    char atServ[128];
    snprintf(atServ, sizeof(atServ), "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", IP, port);
    SendData(atServ);
}

void WaitForConnect(int essai)
{
	while(configurationWifi.connected == 0)
	{
        if(essai == 0) break;
        essai--;
		int i = 0;
		char *p=espResponse;
		for (i = 0;i < strlen(espResponse);i++)
		{ 
			p++;  
			if ((*p=='N') && (*(p+1)=='E') && (*(p+2)=='C') && (*(p+3)=='T') && (*(p+4)=='\r') && (*(p+5)=='\n'))
			{
				configurationWifi.connected = 1; 
			}           
		}  
	}
}


void DisconnectFromServer()
{
    
	if(configurationWifi.connected)  
    {
		SendData("AT+CIPCLOSE'\r\n");
        configurationWifi.connected = 0; 
    }
}

void InitWifi()
{
    /*
        Paramètre du module
     */
    char IP[] = "192.168.1.254";
    configurationWifi.IP = IP;
    configurationWifi.mode = 1;
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
    SetMultiConnection(0);      
    Delay(100);
    ClearUartEspBuffer();
}

void GetAccessPointsList()
{
    ClearUartEspBuffer();
    SendData("AT+CWLAP\r\n");
    Delay(5000);
}