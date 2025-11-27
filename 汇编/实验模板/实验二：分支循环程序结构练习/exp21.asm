;实验二-1 分支循环结构程序练习
DATA SEGMENT
    A DB 'Please input a string (length<9): $'
     DB 'Please input the index of the char to display: $'
     DB 10, ?, 10 DUP(?)
     DB 'The char is: $'
     DB 'The index is invalid!$'
DATA ENDS

CODE SEGMENT 
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    
    ; 显示  提示输入字符串 的内容
    mov ah, 9
    lea dx, A
    int 21H
    ; 输入字符串

    ; 换行
    
    ; 显示 提示输入下标 的内容

    ; 输入一个数字
    
    ; 比较该数字与输入字符串长度

    ; 使 BX 指向要输出的字符地址

    ; 换行

    ; 显示 要显示的字符串 内容
    
    ; 显示错误信息
    
EXIT:
    MOV AX,4C00H
    INT 21H
CODE ENDS
  END START