; void DisplayChar13( c, x, y, fgd, bkgd)
; requires: video mode 13
; ensures:  char c written to x,y (all ints)
; Note:Chars are 8x8
ARGc	EQU	word ptr [bp+4]
ARGx	EQU	word ptr [bp+6]
ARGy	EQU	word ptr [bp+8]
ARGfgd  	EQU	byte ptr [bp+10]
ARGbkgd	EQU	byte ptr [bp+12]

BytesPerLine	EQU	320

_TEXT	SEGMENT	byte public 'CODE'
	ASSUME	cs:_TEXT

	EXTRN	PixelAddr13:near

	PUBLIC	_DisplayChar13
_DisplayChar13	PROC	near

	push	bp
	mov	bp,sp
	push	di
	push	si
	push	ds

;calc first pixel
	mov	ax,ARGy
	mov	bx,ARGx
	call	PixelAddr13
	mov	di,bx

;set up char def table addressing
	mov	ax,40h
	mov	ds,ax

	mov	cx,ds:[85h]

	xor	ax,ax
	mov	ds,ax

	mov	ax,ARGc
	mov	bx,43h*4
	lds	si,ds:[bx]
	mul	cl

	add	si,ax

;store char in video buffer
	mov	bl,ARGfgd
	mov	bh,ARGbkgd


L10:	push	cx
	mov	cx,8
	lodsb
	mov	ah,al

L11:	mov	al,bl
	shl	ah,1
	jc	L12

	mov	al,bh

L12:	stosb
	loop	L11

	add	di,BytesPerLine-8

	pop	cx
	loop	L10

	pop	ds
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret

_DisplayChar13	ENDP
_TEXT	ENDS
	END