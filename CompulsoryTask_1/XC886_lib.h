// Einheitliche Funktionsprototypen

//******************************************************************
// Prototypen für den ADC des XC866:
//******************************************************************

extern void adc_init(void);
extern unsigned int adc_in (unsigned char kanal);
extern unsigned char ADC_Busy(void);

//******************************************************************
// Prototypen für die LCD-Funktionen
//******************************************************************

extern void lcd_init(void);		// Init LCD
extern void lcd_clr(void);	 		// LCD Löschen 
extern void lcd_byte(char);  		// Ausgabe unsigned char  => 3 stellige Zahl
extern void lcd_str(char *ptr);	// Ausgabe String
extern void lcd_curs(char);		// Cursor setzen
extern void wait_bsy1(void);  		// kurz warten
extern void wait_bsy2(void); 		// lang warten
extern void asc_out(char);	   		// Ausgabe eines ASCII Zeichens
extern void lcd_int(unsigned int);	// Ausgabe Int Wert => 4 stellig
extern void send_byte(unsigned char,unsigned char);

//************************************************************************
// Prototypen für die I2C Funktionen des XC886
//************************************************************************
extern void nak_out(void);
extern void ack_out(void);
extern unsigned char byte_in(void);
extern bit byte_out(unsigned char wert);
extern void stop_i2c(void);
extern bit start_i2c(void);
extern void repstart_i2c(void);

//************************************************************************
// Prototypen für die RS232 (Serielle Schnittstelle) des XC886
//************************************************************************

extern void rs232_init(void);

//************************************************************************
// Prototypen für T13 (Pulsweitenmodulation) des XC886
//************************************************************************

extern void t13_init(void);
extern void t13_comp_val(unsigned int);


