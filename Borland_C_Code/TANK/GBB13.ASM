; int GetBitBlock13(x0, y0, x1, y1, buf)
;	x0,y0: upper left
;	x1,y1: lower right
;	char far *buf buffer
; returns:	size of bit block in system RAM

ARGx0	EQU	word ptr [bp+4]
ARGy0	EQU	word ptr [bp+6]
ARGx1	EQU	word ptr [bp+8]
ARGy1	EQU	word ptr [bp+10]
ADDRbuf	EQU	         [bp+12]

VARPixelRows	EQU	word ptr [bp-2]
VARPixelRowLen	EQU	word ptr [bp-4]
VARincr	EQU	320

ByteOffsetShift EQU	1		;# Pixels/byte

_TEXT	SEGMENT	byte public 'CODE'
	ASSUME	cs:_TEXT

	EXTRN	PixelAddr13:near

	PUBLIC	_GetBitBlock13
_GetBitBlock13	PROC	near

	push	bp
	mov	bp,sp
	sub	sp,6
	push	ds
	push	si
	push	di

;compute dimensions

	mov	ax,ARGx1
	sub	ax,ARGx0
	mov	cx,0FF07h

	and	cl,al

	xor	cl,7
	shl	ch,cl
	mov	cl,ch
	push	cx

	mov	cl,ByteOffsetShift
	shr	ax,cl
	inc	ax
	push	ax

	mov	ax,ARGy1
	sub	ax,ARGy0
	inc	ax
	push	ax

;extablish addressing

	mov	ax,ARGy0
	mov	bx,ARGx0
	call	PixelAddr13
	xor 	cl,7
	push	es
	pop	ds
	mov	si,bx


	les	di,ADDRbuf;

;build five byte header

	pop	ax
	mov	VARPixelRows,ax
	stosw
	pop	ax
	mov	VARPixelRowLen,ax
	stosw
	pop	ax
	mov	ch,al
	stosb

;copy from video buffer to ram
L02:	mov	bx,VARPixelRowLen
	push	si

L03:	lodsw

	dec	si
	rol	ax,cl
	stosb	
	dec	bx
	jnz	L03

	and	es:[di-1],ch
	pop	si
	add	si,VARincr

	dec	VARPixelRows
	JNZ	L02

	mov	ax,di
	sub	ax,ADDRbuf

	pop	di
	pop	si
	pop	ds
	mov	sp,bp
	pop	bp
	ret

_GetBitBlock13	ENDP
_TEXT	ENDS
	END