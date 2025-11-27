;试编写程序，完成下面公式的计算。
; A ←（X-Y+24）/ Z 的商，B ←（X-Y+24）/ Z 的余数
; 其中，变量 X 和 Y 是 32 位有符号数，变量 A、B、Z 是 16 位有符号数
DATA SEGMENT
    X DD 140000
    Y DD 8888
    Z DW 512
    A DW ?
    B DW ?
DATA ENDS
CODE SEGMENT
ASSUME DS:DATA,CS:CODE
START:
    ; 设置数据段
    MOV AX, DATA
    MOV DS, AX

	
    MOV AX,4C00H
    INT 21H
CODE ENDS
END START


