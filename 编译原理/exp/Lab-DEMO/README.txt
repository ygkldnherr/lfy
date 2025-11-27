1、说明
本文件夹下是编译原理课程实验的DEMO程序，目的是为了更好理解实验要求，避免对实验指导书中的内容产生理解歧义。
DEMO程序的输出非常简陋，仅供参考，并非要求实验程序产生一样的输出。

2、使用方法
DEMO.PL0是所有实验要处理的PL/0示例程序，Lab1、Lab2、Lab3三个子目录下分别是实验一、二、三的示例程序，具体使用需要在命令行窗口进行。
（1）实验一：
cd Lab1
pl0lex ..\DEMO.PL0
（2）实验二：
cd Lab2
pl0syn ..\DEMO.PL0
（3）实验三：
cd Lab3
pl0disp ..\DEMO.PL0
pl0comp ..\DEMO.PL0
pl0intp ..\DEMO.PL0
其中，pl0disp显示抽象语法树，pl0comp编译生成四元式中间代码，pl0intp解释执行。