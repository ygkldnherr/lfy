;从键盘接收两个不大于5的十进制数字，并以数据形式显示其和。 
DATA SEGMENT
    X DB ?
    Y DB ?
    STR1 DB 'PLEASE ENTER A NUMBER (0-4):$'
DATA ENDS
CODE SEGMENT
ASSUME DS:DATA,CS:CODE
START:
    ; 设置数据段
    MOV AX, DATA
    MOV DS, AX

    lea dx, STR1
    mov ah, 09H
    int 21h

    mov ah, 01h
    int 21h
    sub al, 30h
    mov X, al

    mov dl, 0DH
    mov ah, 02h
    int 21h
    mov dl, 0AH
    int 21h


    lea dx, STR1
    mov ah, 09H
    int 21h

    mov ah, 01h
    int 21h
    sub al, 30h
    mov Y, al

    mov al, X
    add al, Y
    mov X, al

    mov dl, 0DH
    mov ah, 02h
    int 21h
    mov dl, 0AH
    int 21h

    mov ah, 02h
    mov dl, X
    add dl, 30h
    int 21h
	
    MOV AX,4C00H
    INT 21H
CODE ENDS
END START


