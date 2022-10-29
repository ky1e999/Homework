data segment
intro   db 0ah,0dh,"******student system********"
        db 0ah,0dh,"**********A. input**********"
                db 0ah,0dh,"**********B. output*********"
                db 0ah,0dh,"**********C. check**********"
				db 0ah,0dh,"**********D. sort **********"
                db 0ah,0dh,"**********E. quit***********",0ah,0dh,'$'
choice  db 0ah,0dh,"choice:$ ",'$'
stu_name    db 0ah,0dh,"name:",'$'
stu_num    db 0ah,0dh,"number:",'$'
stu_grade    db 0ah,0dh,"grade:",'$'
high_grade db 0ah,0dh,"highest:",'$'
low_grade db 0ah,0dh,"lowest:",'$'
aver_grade db 0ah,0dh,"average:",'$'
numorname db 0ah,0dh,"id(A) or name(B) : $",'$'
section60    db 0ah,0dh,"below 60:$",'$'
section70    db 0ah,0dh,"60-79:$",'$'
section90    db 0ah,0dh,"80-89:$",'$'
section100    db 0ah,0dh,"90-99:$",'$'
sectionbuffer db 20 dup(0)
rankbuf1 db 20 dup(0)
rankbuf2 db 20 dup(0)
rankgrade1 db 20 dup(0)
rankgrade2 db 20 dup(0)
namem    db 400 dup(0)
grade    db 400 dup(0)
num    db 400 dup(0)
bufferout  db 200 dup(0)
buffer db 20,?,20 dup(0)
errorr    db 0ah,0dh,"WRONG SORRY!",'$'
test_data   db 1 dup('***************************************',0ah,0dh,'$')
data ends

stack segment stack
    db 100 dup(?)
stack ends

mem equ 10

print_ macro buffer
    mov ah,9
    lea dx,buffer
    int 21h
endm

keep macro
    push ax
        push bx
        push cx
        push si
        push di
endm
release macro
    pop di
        pop si
        pop cx
        pop bx
        pop ax
endm
output MACRO x
    mov ah,2
        mov dl,x
        int 21h
ENDM
outputs MACRO x
    lea dx,x
        mov ah,9
        int 21h
ENDM
input MACRO
    mov ah,1
        int 21h
ENDM
inputs MACRO
    lea dx,buffer
        mov ah,0ah
        int 21h
ENDM
outputerror MACRO
    lea dx,errorr
        mov ah,9
        int 21h
ENDM
stack segment
    db 128 dup(?)
stack ends
code segment
assume cs:code,ds:data
start:
    mov ax,stack
        mov ss,ax
    mov ax,data
        mov ds,ax
        mov ax,3
        int 10h
    call show
exit:
        mov ax,4c00h
        int 21h
show PROC NEAR
        outputs intro
        outputs choice
        input
        cmp al,'A'
        je callA
        cmp al,'B'
        je callB
        cmp al,'C'
        je callC
        cmp al,'D'
        je callD
		cmp al,'E'
        je exit
        ret
show ENDP
callA PROC NEAR
    call A
        jmp show
callA ENDP
callB proc near
    call B
        jmp show
callB endp
callC proc near
        outputs numorname
        input
        cmp al,'A'
        je Ci_number_call
        cmp al,'B'
        je Ci_name_call
        jmp show
callC endp
callD proc near
    call D
        jmp show
callD endp
Ci_number_call proc near
    nop
    call Ci_number
        jmp show
Ci_number_call endp
Ci_name_call proc near
    nop
    call Ci_name
        jmp show
Ci_name_call endp
A PROC NEAR
    keep
    mov cx,mem
        mov di,0
info:
        push cx
input_name:
    outputs stu_name
    inputs
        mov cl,buffer[1]
        mov si,2
        push ax
        push bx
        mov ax,di
        mov bl,10
        mul bl
namechar:
        mov bl,byte ptr buffer[si]
        inc si
        push ax
        push di
        mov di,ax
        mov byte ptr namem[di],bl
        pop di
        pop ax
        inc ax
        loop namechar
        call clearbuffer
        pop bx
        pop ax
input_number:
    outputs stu_num
        inputs
        mov cl,buffer[1]
        mov si,2
        push ax
        push bx
        mov ax,di
        mov bl,10
        mul bl
numchar:
        mov bl,byte ptr buffer[si]
        inc si
        call storenumber
        loop numchar
        call clearbuffer
        pop bx
        pop ax
    call input_grade
gon1:
    inc di
        pop cx
    loop info
        release
        ret
A ENDP
clearbuffer proc near
    mov cx,200
        mov si,2
        mov byte ptr buffer[1],'?'
gone:
    mov byte ptr buffer[si],0
        inc si
        loop gone
        ret
clearbuffer endp
storenumber proc near
    push ax
        push di
        mov di,ax
        mov byte ptr num[di],bl
        pop di
        pop ax
        inc ax
        ret
storenumber endp
input_grade proc near
    keep
    mov cx,16
        push bx
        push ax
        mov ax,di
        mov bx,20
        mul bx
        mov si,ax
        pop ax
        pop bx
    outputs stu_grade
        mov bl,'A'
        push di
        mov di,0
gradechar:
    output ' '
        output ' '
        output bl
        output ':'
        inc bl
        push bx
        input
        sub al,30h
        mov ah,0
        mov bx,10
        mul bx
        push ax
        input
        sub al,30h
        mov ah,0
    pop bx
        add al,bl
        mov byte ptr grade[si],al
        add di,ax
        inc si
        pop bx
        loop gradechar
        mov ax,di
        pop di
        push bx
        mov bx,4
        mul bx
        pop bx
        push ax
        output ' '
        output 'B'
        output 'I'
        output 'G'
        output ':'
        input
        sub al,30h
        mov ah,0
        mov bx,10
        mul bx
        push ax
        input
        sub al,30h
        mov ah,0
    pop bx
        add al,bl
        mov byte ptr grade[si],al
        push bx
        mov bx,96
        mul bx
        pop bx
        pop bx
        add ax,bx
        mov bx,160
        div bx
        inc si
        mov byte ptr grade[si],al
        release
        ret
input_grade endp
ranki proc near
    keep
    mov cx,mem
        mov di,0
remove_rank:
        push di
        push ax
        push bx
        mov ax,di
        mov bx,20
        mul bx
        mov di,ax
        pop bx
        pop ax
        mov al,grade[di+17]
        pop di
        mov rankgrade1[di],al
        mov rankgrade2[di],al
        inc di
        loop remove_rank
    mov cx,mem
        sub cx,1
        mov di,0
outer:
    push cx
    mov di,0
inner:
    mov al,rankgrade1[di]
        cmp al,rankgrade1[di+1]
        jb nochange
        xchg al,rankgrade1[di+1]
        mov rankgrade1[di],al
nochange:
    inc di
        loop inner
        pop cx
        loop outer
        mov cx,mem
        mov di,0
ranki_queue1:
        push cx
        mov cx,mem
        mov si,0
ranki_queue2:
        mov al,rankgrade1[di]
        cmp al,rankgrade2[si]
        jne rankigon
		push ax
		push di
		mov ax,di
		mov byte ptr rankbuf1[si],al
		pop di
		pop ax
		push ax
		push si
		mov ax,si
		mov bx,20
		mul bx
		mov si,ax
		push ax
		push di
		mov ax,di
		mov bx,mem
		sub bx,ax
		mov ax,bx
        mov byte ptr grade[si+18],al
		pop di
		pop ax
		pop si
		pop ax
rankigon:
    inc si
        loop ranki_queue2
        inc di
        pop cx
        loop ranki_queue1
		mov cx,mem
		mov di,0
rankbuf_loop1:
        mov si,0
		push cx
		mov cx,mem
rankbuf_loop2:
		push ax
		push di
		mov ax,di
        cmp al,rankbuf1[si]
		pop di
		pop ax
		je rankbuf_jump
		inc si
		loop rankbuf_loop2
rankbuf_jump:
		push ax
		push si
		mov ax,si
        mov rankbuf2[di],al
		pop si
		pop ax
        inc di
		pop cx
		loop rankbuf_loop1
        release
        ret
ranki endp
B proc near
    keep
        call ranki
        mov di,0
        mov cx,mem
outputbegin:
    nop
outputname:
    outputs stu_name
    mov ax,di
        mov bl,10
        mul bl
        push cx
        mov cx,10
        push di
        mov di,ax
namecharoutput:
        mov bl,namem[di]
        output bl
        inc di
        loop namecharoutput
        pop di
        pop cx
outputnumber:
    outputs stu_num
        mov ax,di
        mov bl,10
        mul bl
        push cx
        mov cx,10
        push di
        mov di,ax
numcharoutput:
        mov bl,num[di]
        output bl
        inc di
        loop numcharoutput
        pop di
        inc di
        pop cx
gradeoutput:
    outputs stu_grade
        mov bl,'A'
        push di
        push cx
        call gradecharoutput
        pop cx
        pop di
        loop outputbegin
    release
        call sectiongrade
        ret
B endp
gradecharoutput proc near
    mov cx,19
        push ax
        push bx
        sub di,1
        mov ax,di
        mov bx,20
        mul bx
        mov si,ax
        pop bx
        pop ax
gradecharoutput_loop:
    output ' '
        output ' '
        cmp bl,'Q'
        je big_output
        cmp bl,'R'
        je fin_output
        cmp bl,'S'
        je rank_output
        output bl
outputgradegon:
        output ':'
        inc bx
        push bx
        mov al,byte ptr grade[si]
        mov ah,0
        call tenstotwo
        inc si
        pop bx
        loop gradecharoutput_loop
        ret
big_output:
    output 'B'
        output 'I'
        output 'G'
        jmp outputgradegon
fin_output:
    output 'F'
        output 'I'
        output 'N'
        output 'A'
        output 'L'
        jmp outputgradegon
rank_output:
    output 'R'
        output 'A'
        output 'N'
        output 'K'
        jmp outputgradegon
gradecharoutput endp
sectiongrade proc near
    keep
        mov cx,mem
        mov di,17
comparegrade_loop:
        mov al,grade[di]
        add di,20
        cmp al,60
        jb grade60
        cmp al,80
        jb grade80
        cmp al,90
        jb grade90
        cmp al,100
        jb grade100
comparegrade_loopgon:
        loop comparegrade_loop
        outputs section60
        mov al,byte ptr sectionbuffer[0]
        mov ah,0
        call tenstotwo
        outputs section70
        mov al,byte ptr sectionbuffer[1]
        mov ah,0
        call tenstotwo
        outputs section90
        mov al,byte ptr sectionbuffer[2]
        mov ah,0
        call tenstotwo
        outputs section100
        mov al,byte ptr sectionbuffer[3]
        mov ah,0
        call tenstotwo
        release
        ret
grade60:
    add sectionbuffer[0],1
        jmp comparegrade_loopgon
grade80:
    add sectionbuffer[1],1
        jmp comparegrade_loopgon
grade90:
    add sectionbuffer[2],1
        jmp comparegrade_loopgon
grade100:
    add sectionbuffer[3],1
        jmp comparegrade_loopgon
sectiongrade endp
tenstotwo proc near
        push bx
    mov bl,10
        div bl
        add al,30h
        add ah,30h
        push bx
        mov bx,ax
        output bl
        output bh
        pop bx
        pop bx
        ret
tenstotwo endp
Ci_name proc near
    keep
    outputs stu_name
        inputs
    mov cx,mem
        mov di,0
        mov dl,buffer[1]
        add dl,2
checkloop3:
    push cx
    push di
        mov cl,dl
        sub cl,2
        mov si,2
        push ax
        push bx
        push dx
        mov ax,di
        mov bx,10
        mul bx
        mov di,ax
        pop dx
        pop bx
        pop ax
checkloop4:
    mov al,namem[di]
        mov ah,buffer[si]
        cmp al,ah
        jne findnext3
        inc di
        inc si
        mov ch,0
        mov dh,0
        cmp si,dx
        jz gotofindnext4
        loop checkloop4
findnext3:
    pop di
    inc di
        pop cx
    loop checkloop3
        call findnext2
        ret
gotofindnext4:
    pop di
    pop cx
        call findnext2
        release 
        ret
Ci_name endp
Ci_number proc near
    keep
    outputs stu_num
        inputs
    mov cx,mem
        mov di,0
        mov dl,buffer[1]
        add dl,2
checkloop1:
    push cx
    push di
        mov cl,dl
        sub cl,2
        mov si,2
        push ax
        push bx
        push dx
        mov ax,di
        mov bx,10
        mul bx
        mov di,ax
        pop dx
        pop bx
        pop ax
checkloop2:
    mov al,num[di]
        mov ah,buffer[si]
        cmp al,ah
        jne findnext1
        inc di
        inc si
        mov ch,0
        mov dh,0
        cmp si,dx
        jz gotofindnext2
        loop checkloop2
findnext1:
    pop di
    inc di
        pop cx
    loop checkloop1
        call findnext2
        ret
gotofindnext2:
    pop di
    pop cx
        call findnext2
        release 
        ret
Ci_number endp
findnext2 proc near
        push di
        mov ax,di
        mov bx,10
        mul bl
        mov di,ax
        push di
        mov cx,10
        outputs stu_name
output_check_name:
    mov al,namem[di]
        output al
        inc di
        loop output_check_name
        pop di
        push di
        mov cx,10
        outputs stu_num
output_check_number:
    mov al,num[di]
        output al
        inc di
        loop output_check_number
        pop di
        pop di
        mov ax,di
        mov bx,20
        mul bx
        mov di,ax
        mov cx,19
        outputs stu_grade
        mov bl,'A'
output_check_grade:
    output ' '
        output ' '
        cmp bl,'Q'
        je big_output_check
        cmp bl,'R'
        je fin_output_check
		cmp bl,'S'
        je rank_output_check
        output bl
outputgradegon_check:
        output ':'
        mov ah,0
        mov al,grade[di]
        call tenstotwo
        inc di
        inc bl
        loop output_check_grade
        ret
big_output_check:
    output 'B'
        output 'I'
        output 'G'
        jmp outputgradegon_check
fin_output_check:
    output 'F'
        output 'I'
        output 'N'
        output 'A'
        output 'L'
        jmp outputgradegon_check
rank_output_check:
    output 'R'
        output 'A'
        output 'N'
        output 'K'
        jmp outputgradegon_check
findnext2 endp
D proc near
    keep
        call ranki
        mov di,0
        mov cx,mem
outputbegin_sort:
    nop
outputname_sort:
    outputs stu_name
	mov bl,byte ptr rankbuf2[di]
		mov bh,0
		mov ah,0
        mov al,bl
        mov bl,10
        mul bl
        push cx
        mov cx,10
        push di
        mov di,ax
namecharoutput_sort:
        mov bl,namem[di]
        output bl
        inc di
        loop namecharoutput_sort
        pop di
        pop cx
outputnumber_sort:
    outputs stu_num
	mov bl,byte ptr rankbuf2[di]
		mov bh,0
		mov ah,0
        mov al,bl
        mov bl,10
        mul bl
        push cx
        mov cx,10
        push di
        mov di,ax
numcharoutput_sort:
        mov bl,num[di]
        output bl
        inc di
        loop numcharoutput_sort
        pop di
        inc di
        pop cx
gradeoutput_sort:
    outputs stu_grade
        mov bl,'A'
        push di
        push cx
        call gradecharoutput_sort
        pop cx
        pop di
        loop outputbegin_sort
high_gradee:
    mov bx,mem
	sub bx,1
	mov di,bx
	mov ah,0
	mov al,rankbuf2[di]
	push ax
	push bx
	mov bx,20
	mul bx
	pop bx
	mov di,ax
	mov bl,grade[di+17]
	pop ax
	outputs high_grade
	mov ah,0
	mov al,bl
	call tenstotwo
low_gradee:
    mov ah,0
	mov al,rankbuf2[0]
	push ax
	push bx
	mov bx,20
	mul bx
	pop bx
	mov di,ax
	mov bl,grade[di+17]
	pop ax
	outputs low_grade
	mov ah,0
	mov al,bl
	call tenstotwo
aver_gradee:
    mov cx,mem
	mov di,17
	mov ax,0
aver_loop:
    mov bh,0
    mov bl,byte ptr grade[di]
	add ax,bx
	add di,20
	loop aver_loop
	mov bx,mem
	div bl
	mov ah,0
	outputs aver_grade
	mov ah,0
	call tenstotwo	
    release
        ret
D endp
gradecharoutput_sort proc near
    mov cx,18
        push ax
        push bx
        sub di,1
		mov bl,byte ptr rankbuf2[di]
		mov bh,0
		mov ah,0
        mov al,bl
        mov bx,20
        mul bx
        mov si,ax
        pop bx
        pop ax
gradecharoutput_loop_sort:
    output ' '
        output ' '
        cmp bl,'Q'
        je big_output_sort
        cmp bl,'R'
        je fin_output_sort
        output bl
outputgradegon_sort:
        output ':'
        inc bx
        push bx
        mov al,byte ptr grade[si]
        mov ah,0
        call tenstotwo
        inc si
        pop bx
        loop gradecharoutput_loop_sort
        ret
big_output_sort:
    output 'B'
        output 'I'
        output 'G'
        jmp outputgradegon_sort
fin_output_sort:
    output 'F'
        output 'I'
        output 'N'
        output 'A'
        output 'L'
        jmp outputgradegon_sort
gradecharoutput_sort endp
code ends
end start