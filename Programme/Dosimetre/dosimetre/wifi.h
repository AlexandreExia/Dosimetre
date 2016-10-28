/* 
 * File:   wifi.h
 * Author: ariehl
 *
 * Created on 16 mars 2016, 16:41
 */

#ifndef WIFI_H
#define	WIFI_H

typedef struct{
    
    int mode; // Mode client ou serveur
    char* IP; // IP du serveur ou du client
    char* SSID; // SSID du serveur (si configuré en serveur)
    char* ssidPass; // Mot de passe du SSID serveur (si configuré en serveur)
    char* port; // Port du serveur (si configuré en serveur)
	int connected;
    
}Wifi;

#define MODE_CLIENT 1
#define MODE_SERVER 2

#endif	/* WIFI_H */

