typedef union 
{
    char charval[4];
    unsigned long result;
}   BCDval;

char LCDtext[6];

/* Function prototypes */

unsigned long hex2bin (char *);
unsigned long bin2BCD_conv (unsigned long );
char BCD_adj(char);
char upper_char(char);
char lower_char(char);

/***************************************************************************/
/* Convert data and display 8-digit result on LCD
   On entry, global array RFID contains the hexadecimal data to be converted.
   Leading zeros are not suppressed.
****************************************************************************/
void disp_data (unsigned long HEXin)
{  
    HEXin=HEXin & 0xFFFF;
    BCDval temp;
    char RFID[6];
    RFID[0]=0; // n'est pas effacé par l'initialisation
    char i, j;
    for (i = 5 , j = 0 ; i > 0 ; i--,j++)
    {
        RFID[i]=HEXin >>(4*j) & 0x0F; //transformation du nombre Hexa en tableau
    }                   //de caractères précédé par 0 et justifié à droite

    temp.result = bin2BCD_conv (hex2bin(RFID));
    for (i = 0, j = 0; i < 3; i++)
    {
        LCDtext[j+1] = upper_char (temp.charval[i]);
        LCDtext[j] = lower_char (temp.charval[i]);
        j += 2;
    }
    LCDtext[6] = 0;  /* Null terminate string data */
}
/***************************************************************************/
/* Hexadecimal -> binary conversion
   The array data referenced by the pointer hexseq must contain only one
   nybble in each char of the array, right justified.
   The binary value returned is a 24-bit value, right justified.
*/
unsigned long hex2bin (char *hexseq)
{
    char i;
    unsigned long temp = 0;

    for (i = 0; i < 6; i++) {
        temp = (temp << 4) | *hexseq;
        hexseq++;
    }
    return (temp);
}

/***************************************************************************/
/* Binary -> BCD conversion
   A 24-bit binary value is converted to eight BCD digits, with two digits
   packed per return char.
*/
unsigned long bin2BCD_conv (unsigned long val)
{
    BCDval res;
    char i;
    res.result = 0;
    for (i = 0; i < 24; i++) {
        res.charval[0] = BCD_adj (res.charval[0]);
        res.charval[1] = BCD_adj (res.charval[1]);
        res.charval[2] = BCD_adj (res.charval[2]);
        res.charval[3] = BCD_adj (res.charval[3]);
        res.result <<= 1;
        if (val & 0x800000)
           res.result += 1;
        val <<= 1;
    }
    return (res.result);
}
/****************************************************************************
*/
char BCD_adj (char val)
{
    if ((val & 0x0F) >= 5)
        val += 3;
    if ((val & 0xF0) >= 0x50)
        val += 0x30;
    return (val);
}

/***************************************************************************/
/* Unpack BCD digit and convert to ASCII value
*/
char upper_char (char value)
{
    return (((value >> 4) & 0x0F) + 0x30);
}
char lower_char (char value)
{
    return ((value & 0x0F) + 0x30);
}
/***************************************************************************/
/* Write ASCII string to LCD

void printf_LCD_4bits(char fila, char columna, char *texto)
{
  char adrs;
  adrs = columna - 1;
  if (fila == 2)
     adrs = adrs | 0x40;
     Ctrl4bits(adrs | 0x80);
     while (*texto)
     Datos4bits(texto++);
}
 */