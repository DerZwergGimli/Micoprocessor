$nomod51
$include(hska_include_.inc)

	jmp ini
;This is my CompulsoryTaskProgramm
;-----------------------------
;Definition of variable
;-----------------------------
meml EQU 1900h

;-----------------------------
;Main Program
;-----------------------------
;-----------------------------
;Define Ports:
;	1. Input or Output
;	2. OD or no OD
;	3. Enable/disable Up/Down
;	4. Select if UP or DOWN
;-----------------------------

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ALL FINE DO NOT MODIFY THIS CODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

;PORT_PAGE=0;

; do all only with Port3

ORG 20h




	ini:
		mov P3_DIR, #0
		;define interrupt
		mov P2_DIR, #0xFF
		mov P2_DATA, #0x01
		inc PORT_PAGE
		
		mov P2_PUDEN, #0
		mov P2_PUDSEL, #0
		dec PORT_PAGE
		;mov P2_DIR,
		//for left to Right
		mov r0, #1
		mov b, #2
		mov r1, #8
		
		jmp first
		
	first:
		mov P3_DATA, #1
		mov P2_DATA, #1
		JNB P2_0 , leftToRight
		jmp first
		
	second:
		mov P3_DATA, #2
		mov P2_DATA, #1
		JNB P2_0 , blink
		jmp second
	third:
		mov P3_DATA, #3
		mov P2_DATA, #1
		JNB P2_0 , rightToLeft
		jmp third
	fourth:
		mov P3_DATA, #4
		mov P2_DATA, #1
		JNB P2_0 , blink2
		jmp fourth
		
	
	leftToRight:
		mov P3_DATA, r0
		mov a, r0
		mul ab
		mov r0, a
		mov b, #2
		;mov P3_DATA, r0
		DJNZ r1, leftToRight
		 
		mov P3_DATA, #0			;clear all Data
		mov r0, #0x80
		mov r1, #8
		jmp second
		
	
	blink:
		mov P3_DATA, #0xFF
		mov r2, #100
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #0xFFFF
		LCALL timeDelay
		 
		mov P3_DATA, #0xFF
		mov r2, #0xFFFF
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #0xFFFF
		LCALL timeDelay
		
		mov P3_DATA, #0xFF
		mov r2, #0xFFFF
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #0xFFFF
		LCALL timeDelay
		
		jmp third
		;jmp rightToLeft
		
	rightToLeft:
		mov P3_DATA, r0
		mov a, r0
		div ab
		mov r0, a
		mov b, #2
		;mov P3_DATA, r0
		DJNZ r1, rightToLeft
		mov P3_DATA, #0			;clear all Data
		
		jmp fourth
	
	blink2:
		mov P3_DATA, #0xFF
		mov r2, #5
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #5
		LCALL timeDelay
		 
		mov P3_DATA, #0xFF
		mov r2, #5
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #5
		LCALL timeDelay
		
		mov P3_DATA, #0xFF
		mov r2, #5
		LCALL timeDelay
		
		mov P3_DATA, #0
		mov r2, #5
		LCALL timeDelay
		
		jmp first
	
	
	
	
	timeDelay:
		DJNZ r2, timeDelay
		ret
		
		
END			