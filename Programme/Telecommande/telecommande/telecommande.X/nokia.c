#include <p33Fxxxx.h>
#include "defines.h"
/*------------------------------------------------------------------------------------
LCD Nokia 3310
--------------------------------------------------------------------------------------*/
#define PosParam        32
// Résolution
#define Lcd_X_res       84
#define Lcd_Y_res       48
// Commandes controleur PCD8544
#define Function_Set    0x20    //Bit 2 = PD, Bit 1 = V, Bit 1 = H
#define Lcd_ON_Norm_H   0x20    //Lcd actif, adressage H, mode normal
#define Lcd_ON_Norm_V   0x22    //Lcd actif, adressage V, mode normal
#define Lcd_ON_Etendu_H 0x21    //Lcd actif, adressage H, mode étendu (H=1)
#define Lcd_OFF_Norm_H  0x00    //Lcd "power down", adressage H, mode normal
// Commandes normales (bit H="0")
#define Set_Config      0x08    //Bit 2 = D, bit 0 = E
#define Set_Adr_Y       0x40    //Adresse Y dans 3 bits LSB (0 à 5)
#define Set_Adr_X       0x80    //Adresse X dans 7 bits LSB (0 à 83)
// Commandes étendues (bit H="1")
#define Set_Bias        0x10    //Bias Mode dans bits 2 à 0
#define Set_Vop         0x80    //Vop dans bits 6 à 0
#define Set_TC          0x04    //Coef de température dans bits 1 à 0

/*------------------------------------------------------------------------------
Variables spécifiques
------------------------------------------------------------------------------*/
char Adr_X ;          // Copie logicielle de l'adresse X du PCD8544 (0 à 83)
char Adr_Y ;          // Copie logicielle de l'adresse Y du PCD8544 (0 à 5)

typedef enum
{
    Lcd_Cmd  = 0,
    Lcd_Data = 1
} LcdCmdData; // Type de transfert : commande ou données

typedef enum
{
    Blanc = 0,
    Noir  = 1
} Couleur; // Type de couleur


/*------------------------------------------------------------------------------------
Constantes en mémoire flash
-------------------------------------------------------------------------------------*/
/*--------------------------------------
Générateur de caractère en mémoire flash
Code Ascii, format 5x7 pixels.
Format : Y    X   1   2   3   4   5
1       D10 D20 D30 D40 D50
2       D11 D21 D31 D41 D51
3       D12 D22 D32 D42 D52
4       D13 D23 D33 D43 D53
5       D14 D24 D34 D44 D54
6       D15 D25 D35 D45 D55
7       D16 D26 D36 D46 D56
1° octet : D16-D15-D14-D13-D12-D11-D10, bit 7 toujours à "0"
2° octet : D26-D25-D24-D23-D22-D21-D20, bit 7 toujours à "0"
3° octet : D36-D35-D34-D33-D32-D31-D30, bit 7 toujours à "0"
4° octet : D46-D45-D44-D43-D42-D41-D40, bit 7 toujours à "0"
5° octet : D56-D55-D54-D53-D52-D51-D50, bit 7 toujours à "0"
--------------------------------------------------------------------------------------*/
const char FontLookup[][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 }    // z
};

/*--------------------------------------
Générateur de caractères
Nombre de caractères : 10
Résolution X : 10
Résolution Y : 14 */
const char MyFontTab[][10] =
{
    { 252, 254,   3,   3,   3,   3,   3,   3, 254, 252  }, // 0 octet LSB
    {   0,  16,  24,  12,   6, 255, 255,   0,   0,   0  }, // 1 octet LSB
    {   2,   3, 131, 131, 131, 131, 131, 195, 254, 124  }, // 2 octet LSB
    {   2,   3,   3, 195, 195, 195, 195, 199, 254,  60  }, // 3 octet LSB
    { 192, 224,  48,  24,  12, 198, 195,   1,   0,   0  }, // 4 octet LSB
    {  31,  63,  99,  99,  99,  99,  99,  99, 195, 131  }, // 5 octet LSB
    { 252, 254, 195, 195, 195, 195, 195, 195, 131,   0  }, // 6 octet LSB
    {   2,   3,   3,   3, 131, 195,  99,  51,  31,  15  }, // 7 octet LSB
    { 124, 254, 195, 195, 195, 195, 195, 195, 254,  60  }, // 8 octet LSB
    { 124, 254, 195, 195, 195, 195, 195, 195, 254, 252  }, // 9 octet LSB
    {  15,  31,  48,  48,  48,  48,  48,  48,  31,  15  }, // 0 octet MSB
    {   0,   0,   0,  48,  48,  63,  63,  48,  48,   0  }, // 1 octet MSB
    {  62,  63,  51,  49,  49,  49,  49,  49,  48,  48  }, // 2 octet MSB
    {  16,  48,  48,  48,  48,  48,  48,  56,  31,  15  }, // 3 octet MSB
    {   3,   3,   3,   3,   3,  63,  63,   3,   3,   3  }, // 4 octet MSB
    {  16,  48,  48,  48,  48,  48,  48,  48,  31,  15  }, // 5 octet MSB
    {  15,  31,  48,  48,  48,  48,  48,  48,  31,  15  }, // 6 octet MSB
    {  56,  60,   6,   3,   1,   0,   0,   0,   0,   0  }, // 7 octet MSB
    {  15,  31,  48,  48,  48,  48,  48,  48,  31,  15  }, // 8 octet MSB
    {   0,  48,  48,  48,  48,  48,  48,  48,  31,  15  }  // 9 octet MSB
};

/*------------------------------------------------------------------------------
Déclarations des fonctions en mémoire flash
------------------------------------------------------------------------------*/

/*---------------------------------------------------------------
Nom          :  LcdSend
Description  :  Envoi d'un octet vers le controleur du LCD
Arguments    :  data -> Donnée ou commande à transmettre
cd   -> Type de transfert : commande (Lcd_Cmd)
ou donnée (Lcd_Data)
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdSend(char Data, LcdCmdData CD)
{
    int i;
    // Sélection du controleur (actif à "0").
    Lcd_CE=0; // CE = "0"
    // positionnement de la patte Data/Commande
    if (CD == Lcd_Data) Lcd_DC=1;
    else Lcd_DC=0;
    // Transmission série de l'octet sur DIN rythmé par CLK
    for (i=0; i<8; i++)
    {
        if ((Data & 0x80)==0) Lcd_DIN=0;
        else Lcd_DIN=1;
        Data <<= 1;
        Lcd_CLK=1; //pour assurer durée mini état haut de 125ns (fmax pour clk =4MHz)
        Lcd_CLK=1; //
        Lcd_CLK=1;
        Lcd_CLK=1;
        Lcd_CLK=0;
    }
    // Dé-sélection du controleur
    Lcd_CE=1;
}
/*---------------------------------------------------------------
Nom          :  Fill_Lcd
Description  :  Remplir le LCD de la même couleur
Arguments    :  Coul_Lcd : Noir ou Blanc
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void Fill_Lcd(Couleur Coul_Lcd)
{
    int x,y;
    char b;
    if (Coul_Lcd==Noir) b=255;
    else b=0;
    for (y=0; y<Lcd_Y_res/8+1; y++)
    {
        LcdSend(Set_Adr_X+0,Lcd_Cmd); //Adresse X = 0
        LcdSend(Set_Adr_Y+y,Lcd_Cmd); //Adresse Y
        for (x=0; x<Lcd_X_res; x++) LcdSend(b,Lcd_Data);
    }
    Adr_X=0; Adr_Y=0;
    LcdSend(Set_Adr_X+0,Lcd_Cmd); //Adresse X = 0
    LcdSend(Set_Adr_Y+0,Lcd_Cmd); //Adresse Y = 0
}
/*---------------------------------------------------------------
Nom          :  LcdChar
Description  :  Dessin d'un caractère à la position actuelle des compteurs X et Y
Arguments    :  Code Ascii du caractère
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdChar(char c)
{
    int i;
    for (i=0; i<5; i++) LcdSend(FontLookup[c-32][i],Lcd_Data);
    Adr_X += 6;
}
/*---------------------------------------------------------------
Nom          :  LcdString
Description  :  Dessin d'une phrase à la position actuelle des compteurs X et Y
Arguments    :  Pointeur de la chaîne de caractères
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdString(char *Phrase)
{
    while (*Phrase)
    {
        LcdChar(*Phrase++);
        LcdSend(0,Lcd_Data); // Espace inter-caractères
        Adr_X++;
    }
}
/*---------------------------------------------------------------
Nom          :  LcdBigDigit
Description  :  Dessin d'un chiffre 14x10 pixels à la position actuelle des
compteurs X et Y
Arguments    :  Chiffre codé BCD
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdBigDigit(char b)
{
    int i;
    for (i=0; i<10; i++) LcdSend(MyFontTab[b][i],Lcd_Data); // D'abord les octets LSB
    LcdSend(Set_Adr_X+Adr_X,Lcd_Cmd);   // Retour au début du dessin
    LcdSend(Set_Adr_Y+Adr_Y+1,Lcd_Cmd); // Octets MSB à la ligne de 8 pixels inférieure
    for (i=0; i<10; i++) LcdSend(MyFontTab[b+10][i],Lcd_Data); // Puis les octets MSB
    Adr_X += 12;
    LcdSend(Set_Adr_Y+Adr_Y,Lcd_Cmd); //Prochain chiffre sur la même ligne
    LcdSend(Set_Adr_X+Adr_X,Lcd_Cmd); //et 12 pixels à droite
}
/*---------------------------------------------------------------
Nom          :  LcdBigString
Description  :  Dessin d'une suite de chiffres à la position actuelle des compteurs X et Y
Arguments    :  Pointeur de la chaîne de caractères
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdBigString(char *Chaine)
{
    while (*Chaine) LcdBigDigit(*Chaine++ - '0');
}
/*---------------------------------------------------------------
Nom          :  LcdGotoXY
Description  :  Affecter les adresses X et Y du PCD8544
Arguments    :  X : position H (0 à 83)
Y : position V (0 à 5)
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdGotoXY(char x, char y)
{
    Adr_X=x; Adr_Y=y;
    LcdSend(Set_Adr_X+Adr_X,Lcd_Cmd); // Affectation adresse X
    LcdSend(Set_Adr_Y+Adr_Y,Lcd_Cmd); // Affectation adresse Y
}
/*---------------------------------------------------------------
Nom          :  BarGraphLcd
Description  :  Dessiner une barre noire horizontale sur le LCD
Arguments    :  Y : position verticale (Y:0 à 5)
LngLine : longueur de la barre en pixels
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void BarGraphLcd(char Y, int LngLine)
{
    int i;
    LcdGotoXY(0,Y);
    for (i=0; i<Lcd_X_res; i++)
    {
        if (i<2) LcdSend(0,Lcd_Data);
        else if (i<LngLine) LcdSend(0x3C,Lcd_Data);
        else LcdSend(0,Lcd_Data);
    }
}
/*---------------------------------------------------------------
Nom          :  LcdClrLine
Description  :  Effacer la ligne complète donnée en argument
Arguments    :  Y : numéro de la ligne
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdClrLine(char Y)
{
    int i;
    LcdGotoXY(0,Y);
    for (i=0; i<Lcd_X_res; i++)
    {
        LcdSend(0,Lcd_Data); // Effacer une ligne verticale de 8 pixels
    }
}
/*---------------------------------------------------------------
Nom          :  write
Description  :  Sorties de caractères des fonctions "printf"
Arguments    :
Valeur renvoyée :
-----------------------------------------------------------------*/
int write(int handle, void *buffer, unsigned int len)
{
    int i;
    if (Adr_X >= Lcd_X_res) return(len);
    switch (handle)
    {
        case 0:
        case 1:
        case 2:
        for (i = len; i; --i)
        {
            LcdChar(*(char*)buffer);
            LcdSend(0,Lcd_Data); // Espace inter-caractères
            Adr_X++;
        }
        break;
        default: break;
    }
    return(len);
}

/*---------------------------------------------------------------------------
** Delay utilisant TMR1 en t en ms à vérifier
-----------------------------------------------------------------------------
void Delay( unsigned int t)
{
    while (t--)
    {
        TMR1 = 0;
        while (TMR1<16000);
    }
}
*/
/*---------------------------------------------------------------
Nom          :  LcdInit
Description  :  Initialisation du PCD8544
Arguments    :  aucun
Valeur renvoyée : aucune.
-----------------------------------------------------------------*/
void LcdInit(void)
{
    LCD_PinsOut;  // instruction d'initialisation des ports définie dans les #define
    LCDInitState; // instruction d'initialisation des ports définie dans les #define
    Lcd_CE=1;   // Controleur non sélectionné (CE = "1")
    Lcd_RST=1;   // et pas de reset (RST = "1")
    Delay(500); // Délai de 0,5s
    Lcd_RST=0;   // Reset PCD8544 (RST = "0")
    Delay(500); // Délai de 0,5s
    Lcd_RST=1;   // Arrêt reset PCD8544 (RST = "1")
    Adr_X=0; Adr_Y=0;
    LcdSend(Lcd_ON_Etendu_H, Lcd_Cmd); //Mode commandes étendues 0x21
    LcdSend(Set_Vop+72,Lcd_Cmd); //Set LCD Vop (contrast) 0x80
    LcdSend(Set_TC+2,Lcd_Cmd);   //Set temp. coefficient 0x04
    LcdSend(Set_Bias+3,Lcd_Cmd);    //LCD Bias mode 1:48 0x10
    LcdSend(Lcd_ON_Norm_H,Lcd_Cmd); //LCD standard commands. Horizontal addressing mode. 0x20
    LcdSend(Set_Config+4,Lcd_Cmd);  //LCD in normal mode (DE = 10) 0x08
}

/*********************************************************************
Nom             :  Conv_Bin8_String
Description     :  Conversion d'une valeur codée 8 bits en binaire
-> chaîne de caractères
Arguments       :  x : nombre binaire à convertir
*String : pointeur vers la chaîne de destination
Valeur renvoyée : aucune
********************************************************************/
void Conv_Bin8_String(char *String, unsigned char x)
{
    String[0]=x/100;                              // Centaines
    String[1]=(x-(100*String[0]))/10;             // Dizaines
    String[2]=x-(100*String[0])-10*String[1]+'0'; // Unités en Ascii
    String[0]+='0'; // Conversion BCD -> Ascii des centaines
    String[1]+='0'; // Conversion BCD -> Ascii des dizaines
    String[3]=0;    // Terminateur de chaîne
}
