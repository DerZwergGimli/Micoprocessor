// Einheitliche Bibliotheks Funktionen für den XC886
// Enthält Bibliotheksfunktionen für:
// LCD-Display - Analog-Digital Wandler - RS232 - I2C Bus 

// ------------------------------------------------------------------------------------------------------------------
// Bibliotheks Funktionen für den ADC des XC866:
// adcinit() muss einmal aufgerufen werden
// mit wert = ADCIN (kanal)  kann ein Analogwert in 10 Bit 
// Auflösung eingelesen werden . Kanal = 4 .. 7 möglich !!

#include <xc886clm.h>

// Prototypen:
void adc_init(void);
unsigned int adc_in (unsigned char kanal);
unsigned char ADC_Busy(void);
														   
//*******************************************
void adc_init(void)
{
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv 
	ADC_GLOBCTR = 0x70;		// ADC Global Control register
	ADC_PRAR	= 0x90;		// Prio und Arbitrationsreg
	SFR_PAGE(_ad1,noSST);	// Page 1 aktiv
	ADC_CHCTR0 	= 0x00;		// Channel Control Register
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_INPCR0	= 00;		// Sample Time Control
	SFR_PAGE(_ad4,noSST);	// Page 4 aktiv
	ADC_RCR0	= 0xc0;		// Result Control Reg. 0
	SFR_PAGE(_ad5,noSST);	// Page 5 aktiv
	ADC_CHINPR	= 0x00;		// Channel Interrupt Node Pointer
	ADC_EVINPR	= 0x00;		// Event Interrupt Reg
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_LCBR	= 0xb7	;	// Check boundery
	ADC_ETRCR	= 0x00	;	// Ext. Trigger Control
	SFR_PAGE(_ad6,noSST);	// Page 6 aktiv
	ADC_QMR0	= 0x00;		//Queue Mode
	ADC_CRMR1	= 0x01;		// Conv request
	SFR_PAGE(_ad0,noSST);	// Page 0 aktiv
	ADC_GLOBCTR  |= 0x80;	// On ADC
}

unsigned int adc_in(unsigned char kanal)
{	// Nur Kanal 4.. 7 ist möglich !
	 
	ADC_PAGE = 0x06;
	ADC_CRPR1= 1 << kanal ;	// ANx gewählt 
	while(ADC_Busy());
	ADC_PAGE = 0x02;
	return ((ADC_RESR0H<<2)+ (ADC_RESR0L>>6) );
}

unsigned char ADC_Busy(void)
{	unsigned char bsyresult;

	SFR_PAGE(_ad0,SST1);	// Page 0 aktiv	
	bsyresult = (ADC_GLOBSTR & 0x01);
	SFR_PAGE(_ad0,RST1);	// Page 0 aktiv
	return(bsyresult);
}

// ------------------------------------------------------------------------------------------------------------
//******************************************************************
// LCD-Funktionen für XC886 Board
// Ansteuerung LCD-Display seriell (4 * 20 Zeichen LCD) 
//******************************************************************

void lcd_init(void);		// Init LCD
void lcd_clr(void);	 		// LCD Löschen 
void lcd_byte(char);  		// Ausgabe unsigned char  => 3 stellig
void lcd_str(char *ptr);	// Ausgabe String
void lcd_curs(char);		// Cursor setzen
void wait_bsy1(void);  		// kurz warten
void wait_bsy2(void); 		// lang warten
void asc_out(char);	   		// Ausgabe eines ASCII Zeichens
void lcd_int(unsigned int);	// Ausgabe Int Wert => 4 stellig
void send_byte(unsigned char,unsigned char);

// Anschluss des Displays an:
sbit sclk = P1_DATA^6;	// Clock Anschluss LCD
sbit sid = P1_DATA^5;	// seriell Input LCD

// Sende ein Byte zum Display, senddate ist Bytewert
// rs => Command = 0 und Data = 1
void send_byte(unsigned char senddata, unsigned char rs)
{	 
	unsigned char i;
	unsigned long sendwert, zw;
	zw = (unsigned long)senddata;   // Typconvertierung
	sendwert = 0x001f + ((zw & 0xf0)<<12) + ((zw&0x0f)<<8);
	if (rs == 1) sendwert = sendwert + 0x40;
	for (i = 0; i<24;i++)	// serielle Ausgabe von 24Bit
	{  	sclk = 0;
		if ((sendwert & 0x00000001) == 0)sid = 0;
		else sid = 1;
		sclk = 1;
		sendwert = sendwert >> 1;
	}
}

void wait_bsy1(void)	// ca. 50 us warten
{      
	unsigned int zaehler ;
	for (zaehler = 0; zaehler < 0xaf; zaehler++);
}

void wait_bsy2(void)	  // ca. 1.6 ms warten
{      
	unsigned int zaehler ;
	for (zaehler = 0; zaehler < 0xaff; zaehler++);
}

void lcd_init (void)
{ 	// P1 auf Ausgabe
	SFR_PAGE(_pp1, noSST);     	// switch to page 1
  	P1_PUDSEL = 0xFF;         	// load pullup/pulldown select register
  	P1_PUDEN = 0x00;        	// load pullup/pulldown enable register
	SFR_PAGE(_pp3, noSST);     	// switch to page 3
	P1_OD = P1_OD | 0x60;		// Open Drain P1.5 , P1.6 
  	SFR_PAGE(_pp0, noSST);      // switch to page 0
  	P1_DIR = P1_DIR | 0x60;		// load direction register	=> P1.6 und P1.5 => out
	wait_bsy2();
	wait_bsy2();
	// LCD Init
	send_byte(0x34,0); 			// 8 Bit Mode
	wait_bsy2();
	send_byte(0x09,0); 			// 4 Zeilen Mode
	wait_bsy2();
	send_byte(0x30,0); 			// 8 Bit Datenläge
	wait_bsy2();
	send_byte(0x0f,0); 			// display ein , Cursor ein
	wait_bsy2();
	send_byte(0x01,0); 			// clear Display
	wait_bsy2();
	send_byte(0x07,0); 			// Cursor autoincremet
	wait_bsy2();
}

void lcd_clr(void)
{   send_byte( 01 ,0);  // CLR Befehl
	wait_bsy2();
	send_byte( 0x0e ,0); // Cursor on
	wait_bsy1();
}

void asc_out(unsigned char zeichen)
{
	send_byte(zeichen,1);
	wait_bsy1();
	wait_bsy1();
}

void lcd_str(char *ptr)
{ 
	unsigned char i=0;
	while (*ptr != 0 )
	{ 
		asc_out(*ptr);
	  	ptr++;
	  	i++;
	  	if (i == 20) lcd_curs(20);
	  	if (i == 40) lcd_curs(40);
	  	if (i == 60) lcd_curs(60);
	}
}

void lcd_byte(unsigned char wert)
{
	unsigned char i;
	i = wert / 100;
	wert %= 100;
	i += 0x30;
	asc_out(i);
	i = wert / 10;
	wert %= 10 ;
	i += 0x30;
	asc_out(i);
	wert += 0x30;
	asc_out(wert);
}

void lcd_int(unsigned int wert_16)
{	// Ausgabe von 0 bis 9999 !!!!
	unsigned char i;
	
	i = wert_16 / 1000; // Anzahl 1000er
	wert_16 %= 1000;	// Rest nach 1000er Division
	asc_out(i+0x30);    // Ausgabe als ASCII Zeichen
    i= wert_16 / 100;
	wert_16 %= 100;
	asc_out(i+0x30);
	i = wert_16 / 10;
	wert_16 %= 10;
    asc_out(i+0x30);
	wert_16 += 0x30;
	asc_out((char)wert_16);
}

void lcd_curs(unsigned char pos)
{
	pos %=80;
	if ( pos < 20)  send_byte( pos +0x80 ,0);	 // 1. Zeile 
	if (( pos >= 20) && (pos < 40))send_byte( pos -20 + 0x20 + 0x80 ,0);
	if (( pos >= 40) && (pos < 60))send_byte( pos -40 + 0x40 + 0x80 ,0);
	if (( pos >= 60) && (pos < 80))send_byte( pos -60 + 0x60 + 0x80 ,0);	
	
	wait_bsy1();
	wait_bsy1();		
}

// ------------------------------------------------------------------------------------------------------------------
//************************************************************************
// I2C Basis Funktionen XC886
// Stand: 1/2011
// A. Schilling
// Timing: Setup und Hold Time immer min. 4,7us 
// SDA: P3.0, SCL: P3.1
//************************************************************************
#define WARTE 10		// Schleife für XC886 von 5 auf 10 erhöht(ca. 7us)
				
// Funktionsprototypen
void nak_out(void);
void ack_out(void);
unsigned char byte_in(void);
bit byte_out(unsigned char wert);
void stop_i2c(void);
bit start_i2c(void);
void repstart_i2c(void);

// Variable 
sbit SDA = P3_DATA^0;
sbit SCL = P3_DATA^1;

unsigned char k,j;

//************************************************************************
// NAK => Negativ Acknowledge Bit (high) senden
//************************************************************************
void nak_out(void)
{
	P3_DIR |= 3;				// SCL und SDA Ausgänge 
	SDA = 1;
	SDA = 1;
	SDA = 1;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 1;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 0;
}

//************************************************************************
// ACK => Acknowledge Bit (low) senden
//************************************************************************
void ack_out(void)
{
	P3_DIR |= 3;				// SCL und SDA Ausgänge 
	SDA = 0;
	SDA = 0;
	SDA = 0;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 1;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 0;
}

//************************************************************************
// byte_in => einlesen Byte, Ende mit SCL = 0, ohne Start Kennung 
//************************************************************************
unsigned char byte_in(void)
{  
    unsigned char wert;
	wert = 0;
	P3_DIR |= 3;				// SCL und SDA Ausgänge 
	SDA = 1;
	for (j=0;j<8;j++) 			// Anzahl Bit's = 8
	{
	  SCL = 1;	  				// Daten müssen stabil sein 
	  SCL = 1;				   	
	  SCL = 1;					
      for (k=0;k<WARTE;k++);	// kurz warten
	  wert = wert << 1;			// eine Stelle links schieben
	  P3_DIR &= 0xFE;			// SDA Eingang 
	  if (SDA == 1) wert +=1;	// wenn SDA = 1, dann 1 addieren
	  SCL = 0;				 	// Daten dürfen wechseln
	  SCL = 0;
	  SCL = 0;
	  for (k=0;k<WARTE;k++);	// kurz warten
	}
	return (wert);
}

//************************************************************************
// byte_out => Ausgabe eines Bytes 
// SCL low time > 4,7us; SCL high time > 4us
//************************************************************************
bit byte_out(unsigned char wert)
{
	bit errorbit;

	P3_DIR |= 3;				// SCL und SDA Ausgänge 
    for (j=0;j<8;j++) 			// 8 Bit
	{
	  if (wert > 127)  SDA = 1;	// MSB prüfen > 127 dann Bit gesetzt
	  else SDA = 0;
	  wert = wert << 1;			
	  SCL = 1;					// SCL=1, dann Daten gültig
	  for (k=0;k<WARTE;k++);	// kurz warten
	  SCL = 0;					// Daten dürfen wechseln
	  for (k=0;k<WARTE;k++);	// kurz warten
	}
	// Anforderung ACK-Bit:
	SDA = 1;
	SDA = 1;
	SCL = 1;
    P3_DIR &= 0xFE;				// Eingang 
	for (k=0;k<WARTE;k++);		// kurz warten
	errorbit = SDA;				// wenn errorbit=1, dann Fehler	
	SCL = 0;

	return(errorbit);
}

//************************************************************************
// SDA muss zuerst auf 0 gehen, frühestens 4us später dann SCL
//************************************************************************
bit start_i2c(void)
{
	bit errorbit;
 
 	P3_DIR |= 3;				// SCL und SDA Ausgänge 
	SDA = 1;  					// Bus freigeben
	SCL = 1;					
	// Bus verfügbar ??
	if (SDA == 0) errorbit = 1;	// Bus belegt
	if (SCL == 0) errorbit = 1;	// Bus belegt
	SDA = 0;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 0;

	return(errorbit);
}

//************************************************************************
// stop => sendet stop I2C 
//************************************************************************
void stop_i2c(void)
{
	P3_DIR |= 3;				// SCL und SDA Ausgänge 
	SDA = 0;
	SDA = 0;
	SDA = 0;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 1;
	for (k=0;k<WARTE;k++);		// kurz warten
	SDA = 1;
}

void repstart_i2c(void)
{
	P3_DIR |= 0x03;				// SCL und SDA Ausgänge 
	SDA = 1;
	SDA = 1;
	SDA = 1;
	SCL = 1;
	for (k=0;k<WARTE;k++);		// kurz warten
	SDA = 0;
	for (k=0;k<WARTE;k++);		// kurz warten
	SCL = 0;
	for (k=0;k<WARTE;k++);		// kurz warten
}


//**************************************************************************
// Bibliotheksfunktionen für den I2C Bus
// *************************************************************************
void rs232_init(void)
{
	PORT_PAGE = 0x02; // Port Page 2  für ALT Select
   	P1_ALTSEL1 = 0x02;   //Alternative Portbelegung 			
   	P1_ALTSEL0 = 0x00;	 //für  TX Out
  	PORT_PAGE = 0;
  	P1_DIR = P1_DIR | 0x02;		// Direction TXD   P1.1
  	SCON = 0x50;	// Mode 1 , Receive enable
	// Baudrate :
  	BCON =  0;		// damit BG Write-enable
  	BG	 =	156;	// Baud Rate Reload Register für 9600 Baud
  	BCON = 0x11; 		// 	   00 0 1 000 1 = 0x11
}


//******************************************************************
// PWM Erzeugung mit T13 (Teil von CCU6) P3.7
// Test von T13 als einfache Alternative bei PWM zu T0/1
// Duty Cycle über AN5 einstellbar
//******************************************************************

void t13_init(void);
void t13_comp_val(unsigned int);


void t13_init(void)
{
	CCU6_PAGE	= 0x01;  	// switch to page 1
	CCU6_TCTR0H	= 0x00;		// fT13 = fCCU/2 - 0x02=fCCU/4
	CCU6_T13PRL	= 65535;	
	CCU6_T13PRH	= 65535>>8;	

	CCU6_PAGE 	= 0x02;    	// switch to page 2
	CCU6_MODCTRH = 0x80;	// COUT63 enable
	//CCU6_PSLR = 0x00;		// Ausgänge im passiven Zustand = 0

	CCU6_PAGE = 0x03;    	// switch to page 3
	//CCU6_CMPSTATH = 0x40;	// Ausgang negieren	Bit 6 = 0

	SFR_PAGE(_pp2, noSST); 	// switch to page ,Port
	P3_ALTSEL0 = 0x80;		// Alt Select0 = 1
	P3_ALTSEL1 = 0;
	SFR_PAGE(_pp0, noSST); 	// switch to page 0
	P3_DIR = 0x80;			// P3.7 = 0ut
	CCU6_PAGE = 0x00;      	// switch to page 0
	//CCU6_TCTR4H |= 0x42;	// Start Timer 13 
							// + Shadow Transfer T13 aktivieren
}
//*****************************************************************
// Aktualisiert die Vergleichswerte	dynamisch
//*****************************************************************
void t13_comp_val(unsigned int compare)
{
	CCU6_PAGE	= 0x00;
	CCU6_CC63SRL = compare;
	CCU6_CC63SRH = compare >> 8; 
	CCU6_TCTR4H |= 0x42; 	// Shadow Transfer zyklisch aktivieren
}










