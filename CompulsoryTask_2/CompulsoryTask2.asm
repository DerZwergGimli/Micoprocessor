$nomod51
$include(hska_include_.inc)
	jmp main
;This is my CompulsoryTaskProgramm
;-----------------------------
;Definition of variable
;-----------------------------
meml EQU 1900h

;-----------------------------
;Main Program
;-----------------------------


ORG 20h

	main:
		mov P0, #00h
		mov P1, #01h
		mov P2, #02h
		mov P3, #04h		
END