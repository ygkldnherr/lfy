;实验三，第2小题，输入若干个10进制的数，<10，相加，显示10进制和
DATA SEGMENT 
    STR1 DB 'Please input a number(<10): $'
    STR2 DB 'The sum is: $'
DATA ENDS
CODE SEGMENT
  ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    
    ; 循环初始化（条件控制的循环程序）

    ; 转化为数值后相加

    
    ; 每次加法运算之后都进行 BCD 码调整

    
    ; 调用子程序输入单字符

    ; 判断是否为回车，若不是，继续循环。

    
    ; 显示提示信息

    
    ; 将BCD码转换为ASCII码

    
    ; 分别单字符显示高位和低位结果


    MOV AX, 4C00H
    INT 21H

INPUT PROC
    ; 显示提示信息

    
    ; 单字符输入
    
    ; 保护出口参数并显示换行
    
    RET
INPUT ENDP

CODE ENDS
  END START