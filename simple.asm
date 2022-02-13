                ORG 16393

; System variables.

VERSN:          DEFB 0
E_PPC:          DEFW 2
D_FILE:         DEFW Display
DF_CC:          DEFW Display+1                  ; First character of display
VARS:           DEFW Variables
DEST:           DEFW 0
E_LINE:         DEFW BasicEnd 
CH_ADD:         DEFW BasicEnd+4                 ; Simulate SAVE "X"
X_PTR:          DEFW 0
STKBOT:         DEFW BasicEnd+5
STKEND:         DEFW BasicEnd+5                 ; Empty stack
BREG:           DEFB 0
MEM:            DEFW MEMBOT
UNUSED1:        DEFB 0
DF_SZ:          DEFB 2
S_TOP:          DEFW $0002                      ; Top program line number
LAST_K:         DEFW $FDBF
DEBOUN:         DEFB 15
MARGIN:         DEFB 55
NXTLIN:         DEFW Line2                      ; Next line address
OLDPPC:         DEFW 0
FLAGX:          DEFB 0
STRLEN:         DEFW 0
T_ADDR:         DEFW $0C8D
SEED:           DEFW 0
FRAMES:         DEFW $F5A3
COORDS:         DEFW 0
PR_CC:          DEFB $BC
S_POSN:         DEFW $1821
CDFLAG:         DEFB $40
PRBUFF:         DEFB 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,$76 ; 32 Spaces + Newline
MEMBOT:         DEFB 0,0,0,0,0,0,0,0,0,0,$84,$20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ; 30 zeros
UNUNSED2:       DEFW 0

; End of system variables

Program:        
Line1:          DEFB $00,$01                    ; Line 1
                DEFW Line1End-Line1Text         ; Line 1 length
Line1Text:      DEFB $EA                        ; REM             
   
; ===========================================================
; Insert your program here....
; ===========================================================

			LD 		A,1 			
loop:			INC		A 		
			JP 		NZ,loop		
			RET 					

; ===========================================================
; That's it.  End of user program
; ===========================================================

                DEFB $76                        ; Newline
Line1End:       
Line2:          DEFB $00,$02                    ; Line 2
                DEFW Line2End-Line2Text
Line2Text:      DEFB $F9,$D4                    ; RAND USR
                DEFB $1D,$22,$21,$1D,$20        ; 16514
                DEFB $7E                        ; Number
                DEFB $8F,$01,$04,$00,$00        ; Numeric encoding
                DEFB $76                        ; Newline
Line2End: 

Display:        DEFB $76                        ; Newline
                DEFB $76 ; Line 0
                DEFB $76 ; Line 1
                DEFB $76 ; Line 2
                DEFB $76 ; Line 3
                DEFB $76 ; Line 4
                DEFB $76 ; Line 5
                DEFB $76 ; Line 6
                DEFB $76 ; Line 7
                DEFB $76 ; Line 8
                DEFB $76 ; Line 9 
                DEFB $76 ; Line 10
                DEFB $76 ; Line 11
                DEFB $76 ; Line 12
                DEFB $76 ; Line 13
                DEFB $76 ; Line 14
                DEFB $76 ; Line 15
                DEFB $76 ; Line 16
                DEFB $76 ; Line 17
                DEFB $76 ; Line 18
                DEFB $76 ; Line 19
                DEFB $76 ; Line 20
                DEFB $76 ; Line 21
                DEFB $76 ; Line 22
                DEFB $76 ; Line 23

; Variables area (empty)
Variables:      
VariablesEnd:   DEFB $80
BasicEnd:                              
