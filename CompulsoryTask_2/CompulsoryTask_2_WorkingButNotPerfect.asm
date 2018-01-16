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



;PORT_PAGE=0;

; do all only with Port3

ORG 20h

	ini:
		mov P3_DIR, #0
		mov r0, #1
		mov b, #2
		mov r1, #8
		jmp main
		
	main:
		jmp leftToRight
		;mov r1, P1_DATA
		;mov P1_DATA, #10h
		jmp main
	
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
		jmp blink
	
	blink:
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
		
		jmp rightToLeft
	
	rightToLeft:
		mov P3_DATA, r0
		mov a, r0
		div ab
		mov r0, a
		mov b, #2
		;mov P3_DATA, r0
		DJNZ r1, rightToLeft
		mov P3_DATA, #0			;clear all Data
		jmp blink
	
	
	timeDelay:
		DJNZ r2, timeDelay
		ret
		
		
END			