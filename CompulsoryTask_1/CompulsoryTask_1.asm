$nomod51
$include(hska_include_.inc)
	jmp main															;cycles 4*1=4
;This is my CompulsoryTaskProgramm
;-----------------------------
;Definition of variable
;-----------------------------
meml EQU 1900h
X EQU 250 ;250    --> r1
Y EQU 50 ;50	  --> r0
;-----------------------------
;Main Program
;-----------------------------


ORG 20h

	main:
		mov r1,#X															
		
	loop:
		lcall U1
		djnz r1,loop
		lcall ende
		
	ende:
		nop

;--------- U1 -----------------
	U1: 
		mov r0, #Y															;cycles 6*250=1500
		
	rep: 	
		inc dptr														;cycles 4*50*250=50000
		djnz r0 ,rep													;cycles 8*50*250=100000
		ret
;-------------------------------	
	
END																	
																		;total cycles = 157024
;8+6+(250*(10+8+6+4+(50*12)=157014		;clock is at 24MHz
;+10RET =157024																		;one clock takes 1/24Mhz = 4.1*10^-6 secs
;157024*(1/24000000)=0.0065sec								;hole program to finish 25351cycles *4.1*10^-6secs = 0.0010562916666667secs = 1.0562916666667ms