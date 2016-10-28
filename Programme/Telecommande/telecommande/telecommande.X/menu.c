#include <p33Fxxxx.h>  
#include "menu.h"
#include "wifi.h"
#include "defines.h"

extern char seqResponse[SIZE_SEQBUFF];
extern char espResponse[SIZE_ESPBUFF];
extern char sequence[SIZE_SEQBUFF];
extern int dosimetres[MAX_DOSIMETRES];


/*
    Tableau contenant le numéro du dosimètre entré pour le séléctionné
*/
char numberInput[2] = {0};

int inputCount = 1;
int impulsVal = 0;

void ClearDosimetreList()
{
    int i = 0;
    for(i; i < MAX_DOSIMETRES;i++)
        dosimetres[i] = 0;
}

void ShowMenu()
{
     Fill_Lcd(0);
     LcdGotoXY(0,0);
     LcdString("1: Dosimetres");
     
     while(1)
     {
        switch(Lecture_Clav())
        {
            case '1':
            {
                ShowDosiListMenu();
                break;
            }
        }
     }
}


void ShowDosiListMenu()
{
    Delay(500); // Temporisation pour éviter le rebond
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString("Recuperation des dosimetres...");
    
    GetAccessPointsList();
    Fill_Lcd(0);
    
    ClearDosimetreList();
    int i = 0, indexCount = 0;
    for(i;i < MAX_DOSIMETRES;i++)
    {
        char listNum[5] = {0}, listName[20] = {0}, dosiName[10] = {0};
       
        snprintf(dosiName, sizeof(dosiName), "DOSI%d!", i + 1);
        
        if(strstr(espResponse, dosiName))
        {
            snprintf(listNum, sizeof(listNum), "%d", indexCount + 1);
            snprintf(listName, sizeof(listName), "%s : %s", listNum, dosiName);
            LcdGotoXY(0,indexCount);
            LcdString(listName);
            // On affecte le numero du dosimètre
            dosimetres[indexCount] = (i + 1);
            indexCount++;
        }
    }
    if(strlen(dosimetres) == 0)
    {
        Fill_Lcd(0);
        LcdGotoXY(0,0);
        LcdString("Aucun dosimetres.");
        LcdGotoXY(0,3);
        LcdString("Touche * pour retour...");
    }

    while(1)
    {
        /*
         Si des dosimetres ont été trouvés
        */
        if(strlen(dosimetres) > 0)
        {
            char tempInput = Lecture_Clav();
            if(tempInput != 'n' && tempInput != '*' && tempInput != '#')
            {
                numberInput[inputCount] = tempInput;
                if(inputCount) inputCount = 0;
                else inputCount = 1;
                Delay(100);
                
            }
            else if(tempInput == '#')
            {
                int dosiNum = 0, i = 0;
                
                /*
                 Nous convertissons les deux chiffres entré en un int
                 */
                if(!IsCharDigit(numberInput[1]) && IsCharDigit(numberInput[0])) dosiNum = (numberInput[0] - '0');
                
                else if(!IsCharDigit(numberInput[0]) && IsCharDigit(numberInput[1])) dosiNum = (numberInput[1] - '0');
                
                else if(IsCharDigit(numberInput[0]) && IsCharDigit(numberInput[1]))
                {
                    dosiNum = (numberInput[0] - '0') * 10 + (numberInput[1] - '0');
                }
                numberInput[0] = 0, numberInput[1] = 0;
                inputCount = 1;
                
                /*
                 On vérifie si le dosimètre est dans la liste des dosimètres récupérer 
                 */
                
                for(i; i < MAX_DOSIMETRES;i++)
                {
                    if((i + 1) == dosiNum && dosimetres[i] > 0)
                    {
                        ShowDosiMenu(dosimetres[i]);
                        break;
                    }
                }  
            }
        }
        if(Lecture_Clav() == '*')
        {
            ShowMenu();
            break;
        }
    }
}

void ShowDosiMenu(int dosinum)
{
    char dosiName[25] = {0};
    snprintf(dosiName, sizeof(dosiName), "DOSI%d!", dosinum);
    
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString("Connexion...");
    /*
        
     * Connexion au point d'accès 
     
     */
    ClearUartEspBuffer();
    ConnectToSSID(dosiName, "");
    Delay(5000);
    
    Fill_Lcd(0);
    LcdGotoXY(0,0);
    LcdString(dosiName);   

    LcdGotoXY(0,2);
    LcdString("4 : Deboost");
    LcdGotoXY(0,3);
    LcdString("6 : Boost");
    LcdGotoXY(0, 4);
    LcdString("0 : RAZ");
    
    char impulsType = 0;
    while(1) 
    {

        if( (impulsType = Lecture_Clav()) == '6' || (impulsType = Lecture_Clav()) == '4' || (impulsType = Lecture_Clav()) == '0')
        {
            int count = 0;
            while (!configurationWifi.connected && count++ < 5)
            { 
                ClearUartEspBuffer();
                
                char IP[32] = {0};
                
                snprintf(IP, sizeof(IP), "192.168.1.%d", dosinum);
                
                ConnectToServer(IP, "1500");
                Delay(100);
                /*
                 *  Fonction permettant de vérifier n fois si la connexion est établie
                 */
                WaitForConnect(1);
                
            }
            
            /*
                Si la connexion a bien été établie
            */
            if (configurationWifi.connected == 1) 
            {
                ClearUartEspBuffer();
                char atSend[16] = {0}, impuls[10] = {0};
                if(impulsType == '6') snprintf(impuls, sizeof(impuls), "!BOOST"), impulsVal++;
                else if(impulsType == '4' && impulsVal > 0) snprintf(impuls, sizeof(impuls), "!DEBOOST"), impulsVal--;
                else snprintf(impuls, sizeof(impuls), "!RAZ"), impulsVal = 0;
                snprintf(atSend, sizeof(atSend), "AT+CIPSEND=%d\r\n", strlen(impuls) + 4);
                SendData(atSend);
                Delay(100);
                ClearUartEspBuffer();
                SendData(impuls);             
            }
            
            Delay(100);
        }
        else if(Lecture_Clav() == '*')
        {
            DisconnectFromServer();
            Delay(200);
            ShowDosiListMenu();
            break;
        }
    }
}