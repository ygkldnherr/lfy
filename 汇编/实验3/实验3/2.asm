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
  xor ax, ax
  xor bx, bx
  ; 调用子程序输入单字符
READ:
  call INPUT
  ; 判断是否为回车，若不是，继续循环。
  cmp al, 0dh
  jE OUTPUT
  ; 转化为数值后相加
  sub al, 30h
  add bl, al
  ; 每次加法运算之后都进行 BCD 码调整
  mov al, bl
  daa
  mov bl, al
  jmp READ
  ; 显示提示信息
OUTPUT:
  mov ah, 09h
  lea dx, STR2
  int 21H
  ; 将BCD码转换为ASCII码
  ; 分别单字符显示高位和低位结果
  mov al, bl
  push ax
  mov cl, 4
  shr al, cl
  call D2A
  mov ah, 02h
  mov dl, al
  int 21h
  pop ax
  and al, 0fh
  call D2A
  mov ah, 02h
  mov dl, al
  int 21H

  MOV AX, 4C00H
  INT 21H

INPUT PROC
  ; 显示提示信息
  mov ah, 09h
  lea dx, STR1
  int 21h
  ; 单字符输入
  mov ah, 01h
  int 21h
  ; 保护出口参数并显示换行
  push ax
  mov ah, 02h
  mov dl, 0dh
  int 21h
  mov dl, 0ah
  int 21h
  pop ax
  RET
INPUT ENDP

D2A PROC
  ; 将 AL 中的数值转换为 ASCII 码
  ; AL: 待转换的数值
  ; DL: 转换后的 ASCII 码
  add al, 30h
  RET
D2A ENDP

CODE ENDS
  END START