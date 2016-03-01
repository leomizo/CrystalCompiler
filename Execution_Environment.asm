; ---------------------------------------------
; 
;   CrystalCompiler
;   MVN Execution environment
; 
;   Authors: Gabriela Marques and Leonardo Mizoguti
;   Last updated: 2015-11-27
;
; ---------------------------------------------

; ---------------------------------------------
;   EXECUTION ENVIRONMENT INTERFACE
; ---------------------------------------------

; Import main address for code execution
main    <

; Import stack start and stack end
stacks  <
stacke  <

; Import string buffer counter and start address
strbct  <
strbs   <

; Export environment variables and sub-routines
evaddr  >
evval   >
evoffs  >
evsign  >
evtemp  >
ercad   >
erwrt   >
errd    >
ercpb   >

; Export stack variables and sub-routines
svbptr  >
svsptr  >
svsize  >
srpsa   >
srppa   >
srptv   >
srwfra  >
srrfra  >

; Export I/O sub-routines
scani   >
scans   >
puti    >
puts    >
putb    >
pbrkl   >

; ---------------------------------------------
;   CODE STARTS HERE
; ---------------------------------------------

        &  /0000

; Jumps to the main, start code execution
start   JP main
 
; ---------------------------------------------
;   NUMERICAL CONSTANTS (nk)
; ---------------------------------------------
nk0     K  /0000
nk1     K  /0001
nk2     K  /0002
nk4     K  /0004
nk10    K  /000A
nk16    K  /0010
nk48    K  /0030
nk256   K  /0100
nk10e5  K  /2710
nkneg   K  /FFFF

; ---------------------------------------------
;   STRING CONSTANTS (sk)
; ---------------------------------------------
skbrkl  K  /0D0A
skneg   K  /2D40
skmin   K  '-
skTR    K  'tr
skUE    K  'ue
skFA    K  'fa
skLS    K  'ls
skE     K  'e

; ---------------------------------------------
;   ENVIRONMENT CONSTANTS (ek)
; ---------------------------------------------
ekldcd  LD /0        ; Load address command constant
ekwrcd  MM /0        ; Write into address command constant

; ---------------------------------------------
;   ENVIRONMENT VARIABLES (ev)
;   Usage depends on each sub-routine, whose 
;   documentation is to be verified.   
; ---------------------------------------------
evaddr  K  /0
evval   K  /0
evoffs  K  /0
evsign  K  /0
evtemp  K  /0

; ---------------------------------------------
;   ENVIRONMENT SUB-ROUTINES (er)
; ---------------------------------------------

; ENVIRONMENT SUB-ROUTINE: cad
;
; Calculate a reference address given a base reference (evaddr),
; an offset (evoffs) and an indication whether the offset is positive
; or negative (evsign).
;
;   Parameters:
;       evaddr: Base address
;       evoffs: Offset (in absolute value)
;       evsign: 0 if offset is positive, anything else if negative
;
;   Result:
;       Calculated address in the accumulator
;
ercad   K  /0
        LD evsign    ; Load offset sign
        JZ ercad1    ; Verify if it is zero (positive offset)
        LD evaddr    ; Load base memory address
        -  evoffs    ; Subtract offset
        -  evoffs
        RS ercad     ; Return
ercad1  LD evaddr    ; Load memory address
        +  evoffs    ; Sum offset
        +  evoffs
        RS ercad     ; Return

; ENVIRONMENT SUB-ROUTINE: wrt
;
; Write a given value into a given memory address (evaddr +/- offset).
;
;   Parameters:
;       evval: Value to be written
;       evaddr: Base reference address
;       evoffs: Offset related to the base address (in absolute value)
;       evsign: 0 if offset is positive, anything else if negative
;
erwrt   K  /0
        SC ercad     ; Calculate address, which is returned in the accumulator
        +  ekwrcd    ; Create writing command
        MM erwrt1    ; Write writing command after instruction of loading evval
        LD evval     ; Load value to be written in evval
erwrt1  K  /0        ; Write value into the specified memory address
        RS erwrt     ; Return 


; ENVIRONMENT SUB-ROUTINE: rd
;
; Retrieve the value of a given memory address (evaddr +/- offset).
;
;   Parameters:
;       evaddr: Base address
;       evoffs: Offset related to the base address (in absolute value)
;       evsign: 0 if offset is positive, anything else if negative
;
;   Result:
;       Value in evval
;
errd    K  /0
        SC ercad     ; Calculate address, which is returned in the accumulator
        +  ekldcd    ; Creating reading command
        MM errd1     ; Write reading command after this instruction
errd1   K  /0        ; Read value of memory address
        MM evval     ; Place it in evval
        RS errd      ; Return

; ENVIRONMENT SUB-ROUTINE: cpb
;
; Copy a block of memory from a place to another.
;
;   Parameters:
;       evaddr: Address of the first element of the block to be copied
;       evval: Address of the first element of the block in the destination
;       evoffs: Size of the block to be copied (in number of addresses)
;
ercpbi  K  /0        ; Internal index variable
ercpbt  K  /0        ; Internal temporary value holder

ercpb   K  /0
ercpb1  LD evoffs    ; Load block size
        -  ercpbi    ; Subtract index variable
        JZ ercpb2    ; Test if it has finished (size == index variable)
        LD evaddr    ; Load first address of the block to be copied
        +  ercpbi    ; Add index variable
        +  ercpbi
        +  ekldcd    ; Create reading command
        MM ercpb3    ; Write reading command after this instruction
ercpb3  K  /0        ; Read memory address (first address + index variable)
        MM ercpbt    ; Store memory content in temporary value holder
        LD evval     ; Load first address of the destination block
        +  ercpbi    ; Add index variable
        +  ercpbi
        +  ekwrcd    ; Create writing command
        MM ercpb4    ; Write writing command after value load
        LD ercpbt    ; Load value in temporary holder
ercpb4  K  /0        ; Write value to the new address
        LD nk1      ; Increment index variable
        +  ercpbi
        MM ercpbi
        JP ercpb1    ; Loop
ercpb2  RS ercpb     ; Return


; ---------------------------------------------
;    STACK VARIABLES (sv)
; ---------------------------------------------
svbptr  K  /0        ; Base pointer (first address of activation record on the top of the stack)
svsptr  K  /0        ; Stack pointer (last address of activation record on the top of the stack)
svsize  K  /0        ; Size of the activation record

; ---------------------------------------------
;    STACK SUB-ROUTINES (sr)
; ---------------------------------------------

; STACK SUB-ROUTINE: psa
;
; Push a new activation record to the stack.
;
;   Parameters:
;       svsize: Size of the activation record to be pushed
;
;   Result:
;       svbptr and svsptr values updated
;
srpsa   K   /0
        LD  svbptr    ; Load base pointer value
        MM  evval     
        LD  svsptr    ; Use stack pointer as reference address
        MM  evaddr    
        LD  nk1       ; Define offset as 1 (first address of the activation rec ord to be pushed)
        MM  evoffs    
        LD  nk0       ; Define positive offset
        MM  evsign
        SC  erwrt     ; Write current base pointer to the next activation rec ord (offset 0)
        LD  svsptr    ; Update base pointer
        +   nk2
        MM  svbptr
        LD  svsptr    ; Update stack pointer
        +   nk4
        +   svsize
        MM  svsptr
        RS  srpsa     ; Return

; STACK SUB-ROUTINE: psa
;
; Pop an activation record from the stack.
;
;   Result:
;       svbptr and svsptr values updated
;
srppa   K  /0
        LD svbptr    ; Load base pointer value
        -  nk2
        MM svsptr
        LD svbptr
        MM evaddr
        LD nk0
        MM evsign
        MM evoffs
        SC errd
        LD evval
        MM svbptr
        RS srppa     ; Return

; STACK SUB-ROUTINE: ptv
;
; Push temporary variable to the activation record on the top of the stack.
;
;   Result:
;       svsptr value updated
;
srptv   K  /0
        LD svsptr
        +  nk2
        MM svsptr
        RS srptv

; STACK SUB-ROUTINE: wfra
;
; Write function return address to the activation record on the top of the stack.
;
;   Parameters:
;       evval: Function return address
;
;   Result:
;       Return function address written to offset 1 of the current activation record.
;
srwfra  K   /0
        LD  svbptr
        MM  evaddr
        LD  nk1
        MM  evoffs
        LD  nk0
        MM  evsign
        SC  erwrt
        RS  srwfra

; STACK SUB-ROUTINE: rfra
;
; Retrieve function return address from the activation record on the top of the stack.
;
;   Result:
;       Return function address in evval.
;
srrfra  K   /0
        LD  svbptr
        MM  evaddr
        LD  nk1
        MM  evoffs
        LD  nk0
        MM  evsign
        SC  errd
        RS  srrfra

; ---------------------------------------------
;    I/O SUB-ROUTINES
; ---------------------------------------------

; I/O buffer
iobuff  K  /000

; I/O SUB-ROUTINE: scani
;
; Read next integer from the input file.
;
;   Result:
;       Integer value in evval.
;
scanif  K  /000
scani   K  /000
        LD nk0
        MM evval
        MM scanif
        GD /300
        MM iobuff
        -  skneg
        JN scani0
        JP scani1
scani0  LD nk1
        MM scanif
        JP scani4
scani2  GD /300
        MM iobuff
        -  skbrkl
        JZ scani3
scani1  SC scsum        
scani4  LD iobuff
        *  nk256
        MM iobuff
        SC scsum  
        JP scani2
scani3  LD scanif
        JZ scani5
        LD evval
        *  nkneg
        MM evval
scani5  RS scani

; I/O Internal sub-routine: scsum
; Converts character into integer and sum into evval
scsum   K  /0000
        LD evval
        *  nk10
        MM evval
        LD iobuff
        /  nk256
        -  nk48
        +  evval
        MM evval  
        RS scsum        

; I/O SUB-ROUTINE: scans
;
; Read next string from the input file.
;
;   Result:
;       String address value in evval.
;
scans   K   /0
        LD  strbct
        MM  evval
scans0  LV  /100
        -   strbct
        JZ  scans4
        JN  scans4
        GD  /300
        MM  iobuff
        -   skbrkl
        JZ  scans2
        LV  strbs
        +   strbct
        +   ekwrcd
        MM  scans1
        LD  iobuff
scans1  K   /0000
        LV  /002
        +   strbct
        MM  strbct
        JP  scans0
scans2  LV  strbs
        +   strbct
        +   ekwrcd
        MM  scans3
        LV  /000
scans3  K   /0000
        LV  /002
        +   strbct
        MM  strbct
        RS  scans
scans4  HM  scans   ; Error: buffer overflow

; I/O SUB-ROUTINE: puti
;
; Print integer to the console.
;
;   Parameters:
;       evval: Integer to be printed
;
putii   K  /0
putif   K  /0
puti    K  /0
        LD nk0
        MM putif
        LD nk10e5
        MM putii
        LD evval
        MM iobuff
        JN puti0
        JP puti1
puti0   LD skmin
        PD /100
        LD iobuff
        *  nkneg
        MM iobuff
puti1   LD putii
        -  nk1
        JZ puti2
        LD iobuff
        /  putii
        JZ puti3
        LD nk1
        MM putif
        LD iobuff
        /  putii
        JP puti5
puti3   LD putif
        JZ puti4  
        LD nk0
puti5   +  nk48
        *  nk256
        PD /100
puti4   LD iobuff
        /  putii
        *  putii
        -  iobuff
        *  nkneg
        MM iobuff
        LD putii
        /  nk10
        MM putii
        JP puti1
puti2   LD iobuff
        /  putii
        +  nk48
        *  nk256
        PD /100
        RS puti
        

; I/O SUB-ROUTINE: puts
;
; Print string to the console.
;
;   Parameters:
;       evval: Address in the string buffer of the string to be printed
;
putsi   K   /0
puts    K   /0
        LV  strbs
        +   evval
        MM  putsi
puts1   LD  putsi
        +   ekldcd
        MM  puts2
puts2   K   /0
        JZ  puts3
        PD  /100
        LV  /002
        +   putsi
        MM  putsi
        JP  puts1
puts3   RS  puts

; I/O SUB-ROUTINE: putb
;
; Print boolean value to console ("true"/"false").
;
;   Parameters:
;       evval: Boolean value to be printed
;
putb    K   /0
        LD  evval
        JZ  putb1
        LD  skTR
        PD  /100
        LD  skUE
        PD  /100
        JP  putb2
putb1   LD  skFA
        PD  /100
        LD  skLS
        PD  /100
        LD  skE
        PD  /100
putb2   RS  putb

; I/O SUB-ROUTINE: pbrkl
;
; Print a break line to the console
;
pbrkl   K   /0000
        LD  skbrkl
        PD  /100
        RS  pbrkl

        #  start  