; 将一个数插入长度一直的有序数组-从前往后比较
DATA SEGMENT 
    LIST DW 5,200H,300H,400H,500H,600H
    X DW 360H
DATA ENDS
CODE SEGMENT
  ASSUME CS:CODE,DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    
    MOV AX, X       ;获取要插入的数据
    LEA BX, LIST    ;获取数据指针
    MOV CX, LIST    ;获取数组长度

    ; 循环比较

    ; 若X小于，则跳转

    ; 若X大于，则继续循环
    
    ; 小于，则与该数进行交换
    
    ; 改变数据指针并继续交换

    ; 修改数组长度

    
    MOV AX,4C00H
    INT 21H
CODE ENDS
END START