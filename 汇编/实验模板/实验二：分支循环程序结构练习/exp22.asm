;实验二-2 分支循环结构程序练习
DATA SEGMENT
     DB 'Please input a string (length<9): $'
     DB 'Please input the index of the char to display: $'
     DB 'Please input a char: $'
     DB  10, 13, 'The char is: $'
     DB 'The count of ''',?,''' is: $'
     DB 'The string is too long!', 10, 13, '$'
     DB  10, 13, 'The index is invalid!', 10, 13, '$'
     DB  10 DUP(?)
DATA ENDS

CODE SEGMENT 
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX


    ; 显示  提示输入字符串 的内容

    ; 连续输入单个字符，组成字符串

    ; 输入字符串长度是否大于 9

    ; 显示 提示输入下标 的内容

    ; 输入一个数字
    
    ; 比较该数字与输入字符串长度
    
    ; 显示 下标对应的字符 内容

    ; 换行

    ; 显示 提示输入要统计的字符 内容

    ; 输入一个字符

    ; 循环统计字符个数
    
    ; 换行

    ; 显示 提示信息和统计值 内容

EXIT:
    MOV AX,4C00H
    INT 21H
CODE ENDS
  END START