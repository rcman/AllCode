%TITLE "moduleShell"
;---------------------------------------------------------------------
; CAUTION!
;      This module is written in Ideal model, meaning you need
;      Borland Turbo Assembler to compile it.
;------------------------------------------------------------------------

       IDEAL
       P286
       include "model.inc"

      DATASEG


PAL_SIZE EQU 768


       CODESEG


       PUBLIC   _setpal,_fade_out_once,_bright_out_once
       PUBLIC   _sub_palette,_fade_between_once,_fade_in_once
       PUBLIC   _fill_pal,_copyPal,_rotate_pal



PROC _setpal
;-----------------------------------------------------------------
; void setpal(void far *pal)
; This function simply updates hardware palette using the data
; stored in char pal[PAL_SIZE]
;  (PAL_SIZE=768)
;-----------------------------------------------------------------
ARG pal:dword

     push      bp
     mov       bp,sp
     push      ds
     push      di
     push      si
     lds      si,[pal]
     call      SETPAL
     pop       si
     pop       di
     pop       ds
     pop       bp
     ret
ENDP _setpal


PROC _fade_out_once
;-----------------------------------------------------------------------
; void  fade_out_once(void far *pal)
;
; *pal points to an array of PAL_SIZE (768) bytes (chars)
; This routine changes the content of palette array. So if you need to
; preserve the content of the palette array, you would have to copy
; the information to another array before calling this routine.
;
; Call this routine 32 times to fade out the screen completely.
;------------------------------------------------------------------------
ARG pal:dword
      push      bp
      mov       bp,sp
      push      ds
      push      di
      push      si

       lds      si,[pal]            ; ds:si points to the palette array

      mov       cx,PAL_SIZE

@@fl1:cmp       [byte ptr si],0     ; if [ds:si]=0 goto @@fade2
      je        @@fade2             ;
      cmp       [byte ptr si],1     ; if [ds:si]=1 goto @@fade1
      je        @@fade1
      dec       [byte ptr si]       ; [ds:si]=[ds:si]-1
@@fade1:dec       [byte ptr si]
@@fade2:inc       si                ; si=si+1
      loop      @@fl1

      sub       si,PAL_SIZE         ; restore si

      call       SETPAL
      pop       si
      pop       di
      pop       ds
      pop       bp
      ret

ENDP _fade_out_once


PROC _bright_out_once
;-------------------------------------------------------------
; bright_out_once(char far *pal)
;
; It works very much like fade_once() function. But it brightens
; the screen, instead of darkening the screen.
; Call this function 32 times to whiten the screen completely.
;-------------------------------------------------------------
ARG pal:dword
      push      bp
      mov       bp,sp
      push      ds                ; let's preserve some registers
      push      di
      push      si

      lds      si,[pal]            ;ds:si points to the palette array

      mov       cx,PAL_SIZE

@@b1:cmp       [byte ptr si],62   ; if pal[si]<62
      jge        @@b2
      inc       [byte ptr si]      ; pal[si]=pal[si]+2
      inc       [byte ptr si]
@@b2:
      cmp       [byte ptr si],63   ;else if pal[si]<63
      je        @@b3
      inc       [byte ptr si]      ; pal[si]=pal[si]+1
@@b3:
      inc       si                 ; si=si+1;
      loop      @@b1

;-----update palette
      sub       si,PAL_SIZE              ; restore the value of si
				    ; now ds:si again points to the
				    ; beginning of the palette array

      call      SETPAL

      pop       si
      pop       di
      pop       ds
      pop       bp
      ret

ENDP _bright_out_once



PROC _sub_palette
;----------------------------------------------------------------------
; sub_palette(void *pal,void far *pal_dest)
; This routine prepares the palettes for fade_between_once()
; function.
; WARNING: pal points to char array of 768*2 bytes, not 768 bytes
;----------------------------------------------------------------------
ARG pal:dword,pal_dest:dword
      push   bp
      mov    bp,sp
      push   ds
      push   di
      push   si

      lds    si,[pal]          ;ds:si points to pal[]
      les    di,[pal_dest]     ;es:di points to pal_dest[]

      mov    cx,PAL_SIZE       ; the following loop is basically
@@l:  lodsb                    ; for(n=0;n<768;++n)
      sub    [es:di],al        ;  pal_dest[n]=pal_dest[n]-pal[n];
      inc    di                ;
      loop   @@l

      mov    cx,PAL_SIZE/2     ; the following loop is equivalent to
      xor    ax,ax
@@l2: mov    [ds:si],ax        ; for(n=0;n<768;++n)
      inc    si                ;  pal[n+PAL_SIZE]=0;
      inc    si
      loop   @@l2

      pop     si
      pop     di
      pop     ds
      pop     bp
      ret
ENDP _sub_palette




PROC _fade_between_once
;-------------------------------------------------------------------
; fade_between_once(void far *pal,void far *pal_dest)
; call sub_palette first
; Which will destroy original values of pal_dest,
;    but it's a necessary step.
; After calling it 63 times,the content of pal[768] will become
; identical to the original values of pal_dest anyway :)
;-------------------------------------------------------------------
ARG  pal:dword,pal_dest:dword
     push   bp
     mov    bp,sp
     push   ds
     push   di
     push   si
     lds    si,[pal]
     les    di,[pal_dest]

     cld
     push   si

      mov     cx,PAL_SIZE
fl2:                         ; increase the value of pallettes
      mov     bl,[es:di]     ; es:di = dest_pal
      cmp     bl,0
      jl      fl4            ; if bl>0 then goto fl4

      add     [PAL_SIZE+si],bl
      jmp     fl3

fl4:  neg     bl
      add     [PAL_SIZE+si],bl
fl5:  cmp     [byte ptr PAL_SIZE+si],63
      jb      fl7
      dec     [byte ptr si]
      sub     [byte ptr PAL_SIZE+si],63
      jmp     fl7

fl3:  cmp     [byte ptr PAL_SIZE+si],63
      jb      fl7      ;
      inc     [byte ptr si] ; increase by one
      sub     [byte ptr PAL_SIZE+si],63
fl7:  inc     di
      inc     si
      loop    fl2


     pop     si
     call    Setpal

     pop     si
     pop     di
     pop     ds
     pop     bp
     ret
ENDP _fade_between_once


PROC _fade_in_once
;------------------------------------------------------------------------
; fadein_once(void far *pal,void far *pal_dest)
; warning: pal should point to an array of 768*2 (NOT 768) BLANK bytes
; call this function 32 times to fade in the screen completely.
;-------------------------------------------------------------------------
     ARG pal:dword,pal_dest:dword
      push    bp
      mov     bp,sp
      push    ds                  ; let's save some registers as usual
      push    di
      push    si

      lds     si,[pal]            ;ds:si points to pal
      les     di,[pal_dest]       ;es:di points to pal_dest

      cld
      push    si             ;ds:si=pal
      mov     cx,PAL_SIZE
bl2:                         ; increase the value of pallettes
      mov     bl,[es:di]     ; es:di = dest_pal
      add     [PAL_SIZE+si],bl
bl3:  cmp     [byte ptr PAL_SIZE+si],32
      jb      @@bl7      ;
      inc     [byte ptr si] ; increase by one
      sub     [byte ptr PAL_SIZE+si],32
      jmp     bl3
@@bl7:inc     di
      inc     si
      loop    bl2

      pop     si             ; restore si

      call    SETPAL
      pop     si
      pop     di
      pop     ds
      pop     bp
      ret
;      jmp     SETPAL         ; I probably don't need this but just to
			     ; make sure...

ENDP _fade_in_once


PROC _rotate_pal
;--------------------------------------------------------------------
; rotate_pal(void far *pal,char index, char numCOl,unsigned char displacement)
;
; note this carefully.
; this routine DOES preserve the content of pal[]
; Let me give you an example how this routine works.
;
; rotate_pal(&pal,63,32,0);
;  this routine will update palette number from 63-95.
;  but no rotation will take place
; rotate_pal(&pal,63,32,1);
;  this routine will also update palette number from 63-95.
;  but this time, the color #64 will replace the content of color #63
;  and color #65 will replace #66 etc.
;
; keep it in mind that this routine does NOT change the content of pal[]
;
;---------------------------------------------------------------------
ARG pal:dword,index:byte,numCol:byte,displacement:byte

      push    bp
      mov     bp,sp
      push    ds
      push    di
      push    si

      lds     si,[pal]

      mov     ax,3
      mul     [index]         ; ax=index*3
      add     si,ax              ; si=si+ax

      push    si                 ; preserve si

      mov     ax,3
      mul     [byte ptr displacement]
      add     si,ax

      mov     bh,[index]
      mov     bl,1
      xor     ch,ch
      mov     cl,[numCol]         ;cx=(numCol-displacement)*3
      sub     cl,[byte ptr displacement]
      mov     ax,cx
      add     cx,cx
      add     cx,ax


      call    SETPAL2

      pop     si
      mov     bh,[index]
      add     bh,[numCol]
      sub     bh,[displacement]
      mov     bl,1
      mov     ax,3
      mul     [displacement]    ; cx=displacement*3
      mov     cx,ax
      call    SETPAL2


      pop     si
      pop     di
      pop     ds
      pop     bp
      ret
ENDP _rotate_pal



;-------------------------------------------------------------------------
; update the hardware palette
; input: ds:si points to palette array
;-------------------------------------------------------------------------


PROC SETPAL
      mov       bh,0               ; bh=# of the first palette color to
				   ; update.
      mov       bl,2               ;This is the loop index.
				   ;In the following codes, we are updating
				   ;128 color at a time. Therefore, we need
				   ;a loop that runs twice in order to update
				   ;all 256 colors.

	mov     cx,128*3         ; di=128(the number of colors to update)*3

PROC SETPAL2
s:
	mov dx, 03DAh               ; CRT controller input status 1 register
v1:
	in    al, dx
	test   al,08h
	jnz    v1                     ; wait until vertical retrace starts
v2:
	in     al, dx
	test   al,08h                 ; wait until vertical retrace ends
		jz     v2

 ;--------- We have done waiting. Now let's update the palette
	mov     al,bh            ; get first color # to process into al
	mov     dx, 03c8h        ; DAC palette index register

	push    cx
	out     dx, al           ; Write the register number to the dac
	inc     dx
	rep     outsb
	pop     cx

	add     bh,128           ; color index=color index+128
	dec     bl
	jnz     s

      ret
;----------------------------------------

ENDP SETPAL2




PROC _fill_pal
;-------------------------------------------------------------
; fill_pal(void far *pal, char red, char green, char blue);
; fills the palette array with the color data given
; does not update hardware palette
;-------------------------------------------------------------
ARG pal:dword, red:byte, green:byte,blue:byte
      push   bp
      mov    bp,sp
      push   di
      les    di,[pal]
      cld
      mov    cx,PAL_SIZE/3
      mov    al,[red]
      mov    ah,[green]
      mov    bl,[blue]
@@fpl:mov    [es:di],al
      mov    [es:di+1],ah
      mov    [es:di+2],bl
      add    di,3
      loop   @@fpl
      pop    di
      pop    bp
      ret
ENDP _fill_pal


PROC _copyPal
;----------------------------------------------------------------
; copyPal(void far *source,void far *dest:dword)
;
; both *source and *dest points to arrays of 768 bytes (chars)
; I know there are C functions out there to copy strings. But
; I wrote this anyway. What the heck, it only took me a minute.
;----------------------------------------------------------------
ARG source:dword,dest:dword
      push   bp
      mov    bp,sp
      push   ds
      push   di
      push   si

      cld
      les    di,[dest]
      lds    si,[source]
      mov    cx,PAL_SIZE
      rep    movsb
      pop    si
      pop    di
      pop    ds
      pop    bp
      ret
ENDP _copyPal

      END                     ; End of module