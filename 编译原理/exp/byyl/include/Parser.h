#ifndef Parser_H
#define Parser_H
#include <bits/stdc++.h>
#include "Lexer.h"
using namespace std;
extern Lexer lexer;
class Parser {
public:
    Parser(vector<Lexer::node> tokens, const unordered_map<string,int>& identify_map, const vector<string>& key, 
        const vector<string>& symbol, const vector<string>& identify, int key_offset, int symbol_offset);
    bool isDigit(string s); //判断是否是数字
    string get_val (Lexer::node &nd); //获取当前token的字符串表示
    void exception_print(int type, string expected = "");
    void match(string expected);           //单输入匹配函数
    void match(string expected, bool majority); //多输入匹配函数  E -> aB | B
    void parse_program();   //program -> block "."
    void parse_block();     //block -> constdecl vardecl procdecl statement
    void parse_constdecl(); //constdecl ->["const" constitem{ "," constitem } ";"]
    void parse_constitem(); //constitem -> ident "=" number
    void parse_vardecl();   //vardecl -> ["var" ident {"," ident} ";"]
    void parse_procdecl();  //procdecl -> {"procedure" ident ";" block ";"}
    void parse_statement(); //statement -> assignstmt | callstmt | compstmt | ifstmt | whilestmt
    void parse_assignstmt();//assignstmt ->[ident ":=" expression]
    void parse_callstmt();  //callstmt -> ["call" ident]
    void parse_compstmt();  //compstmt -> ["begin" statement {";" statement} "end"]
    void parse_ifstmt();    //ifstmt -> ["if" condition "then" statement]
    void parse_whilestmt(); //whilestmt -> ["while" condition "do" statement]
    void parse_condition(); //condition = "odd" expression | expression ("="|"#"|"<"|">") expression
    void parse_expression();//expression -> term { ("+"|"-") term}
    void parse_term();      //term -> factor {("*"|"/") factor}
    void parse_factor();    //factor -> [ "+"|"-"] (ident | number | "(" expression ")")
    void parse_ident();     //ident -> letter {letter | digit}
    void parse_number();    //number -> digit {digit} 
    bool GrammaticalAnalysis();
    

private:
    vector<Lexer::node> tokens; //存储输入的字符串
    unordered_map<string, int> identify_map;
    vector<string> key, identify, symbol; //存储关键字、标识符和符号
    int key_offset;
    int symbol_offset;
    Lexer::node currentToken;
    bool success = true;
    string symbolarray[4] = { "=", "#", "<", ">" };
    int symbollength = 4;
    int index = 0; //当前token的下标
};

#endif // Parser_H