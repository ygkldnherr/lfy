;试将字节数据 B1 拆分成两个半字节数据，分别存放于其后两个单元 X 和 Y 中
DATA SEGMENT
    B1 DB 1AH
    X  DB ?
    Y  DB ?
DATA ENDS
CODE SEGMENT
ASSUME DS:DATA, CS:CODE
START:
    ; 设置数据段
    MOV AX, DATA
    MOV DS, AX

	
    ; 程序结束
    MOV AX,4C00H
    INT 21H
CODE ENDS
END START


