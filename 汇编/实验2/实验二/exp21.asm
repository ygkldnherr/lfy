;实验二-1 分支循环结构程序练习
DATA SEGMENT
    STR1 DB 'Please input a string (length<9): $'
    STR2 DB 'Please input the index of the char to display: $'
    STR_input DB 10, ?, 10 DUP(?)
    STR3 DB 'The char is: $'
    STR_invalid DB 'The index is invalid!$'
DATA ENDS

CODE SEGMENT 
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    
    ; 显示  提示输入字符串 的内容
    mov ah, 9
    lea dx, STR1
    int 21H
    ; 输入字符串
    mov ah, 0ah
    lea dx, STR_input
    int 21H
    ; 换行
    mov ah, 2
    mov dl, 0dh
    int 21H
    mov dl, 0ah
    int 21H
    ; 显示 提示输入下标 的内容
    mov ah, 9
    lea dx, STR2
    int 21H
    ; 输入一个数字
    mov ah, 1
    int 21H
    ; 比较该数字与输入字符串长度
    mov bl, STR_input[1]
    sub al, '0'
    cmp al, bl
    jae INVALID
    ; 使 BX 指向要输出的字符地址
    lea bx, STR_input + 2
    xor ah, ah
    add bx, ax
    ; 换行
    mov ah, 2
    mov dl, 0dh
    int 21H
    mov dl, 0ah
    int 21H
    ; 显示 要显示的字符 内容
    mov ah, 9
    lea dx, STR3
    int 21H
    ; 显示要输出的字符
    mov ah, 2
    mov dl, [bx]
    int 21H
    jmp EXIT
    ; 显示错误信息
INVALID:
    mov ah, 9
    lea dx, STR_invalid
    int 21H
    jmp EXIT
    
EXIT:
    MOV AX,4C00H
    INT 21H
CODE ENDS
  END START