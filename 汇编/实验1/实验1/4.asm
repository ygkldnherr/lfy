;从键盘接收一个字符串（假定输入长度大于3），试换行输出该字符串中第二个字符开始的连续2个字符。
DATA SEGMENT
    STR1 DB 'PLEASE ENTER A STRING.$'
    STR2 DB 20,?,20 DUP(?) ; str2[0] = 最大输入字符长度 str[1] = 实际输入字符长度
DATA ENDS
CODE SEGMENT
    ASSUME DS:DATA,CS:CODE
START:
    MOV AX, DATA
    MOV DS, AX

    lea dx, STR1
    mov ah, 09h
    int 21h

    mov dl, 0dh
    mov ah, 02h
    int 21h
    mov dl, 0ah
    int 21h

    lea dx, STR2
    mov ah, 0ah
    int 21h

    mov dl, 0dh
    mov ah, 02h
    int 21h
    mov dl, 0ah
    int 21h

    mov dl, [STR2+3]
    mov ah, 02h
    int 21h

    mov dl, [STR2+4]
    int 21h

	
    ;程序结束
    MOV AX, 4C00H
    INT 21H
CODE ENDS
END START