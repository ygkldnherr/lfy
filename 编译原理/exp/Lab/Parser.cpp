#include "Parser.h"


//对实验文档中的消除左递归和回溯的PL/0的EBNF文法分析
/*
    program：程序，由 block 和 "." 组成。
    block：代码块，包含 constdecl、vardecl、procdecl 和 statement 四部分。
    constdecl：常量声明，由可选的 "const" 关键字，多个 constitem，以及 ";" 组成。
    constitem：常量条目，包括标识符 ident 和数字 number 之间的 "=" 号连接。
    vardecl：变量声明，由可选的 "var" 关键字，多个标识符 ident，以及 ";" 组成。
    procdecl：过程声明，由多个 "procedure" 关键字，标识符 ident，代码块 block 和 ";" 组成。
    statement：语句，包括赋值语句 assignstmt、过程调用语句 callstmt、复合语句 compstmt、条件语句 ifstmt 和循环语句 whilestmt。
    assignstmt：赋值语句，包括标识符 ident，":=" 连接后面的 expression。
    callstmt：过程调用语句，包括 "call" 关键字和标识符 ident。
    compstmt：复合语句，由 "begin" 关键字和多个语句 statement，以及 "end" 关键字组成。
    ifstmt：条件语句，由 "if" 关键字、condition、"then" 关键字和语句 statement 组成。
    whilestmt：循环语句，由 "while" 关键字、condition、"do" 关键字和语句 statement 组成。
    condition：条件判断，包括 "odd" 和 expression 之间的关系运算符（"="、"#"、"<"、">"）。
    expression：表达式，由 term 和多个加减法运算符组成。
    term：项，由 factor 和多个乘除法运算符组成。
    factor：因子，包括可选的正负号、标识符 ident、数字 number 和括号中的 expression。
*/

Lexer lexer; //全局变量，Lexer类的实例化对象

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename = argv[1];
    Parser parser;
    vector<string> str;
    lexer.read_from_file(filename, str);
    lexer.LexicalAnalysis(str);
    parser.GrammaticalAnalysis();
    return 0;
}

#pragma region Parser
bool Parser::isDigit(string s) {
    if (s[0] == '-') {
      reverse(s.begin(), s.end()); 
      s.pop_back();
      reverse(s.begin(), s.end());
      if (s == "0") return false;
    }
    if (s.empty()) return false;
    if (s[0] == '0') {
      return int(s.size()) == 1;
    } else {
      for (auto c: s) if (c < '0' or c > '9') return false;
    }
    return true;
}


string Parser::get_str() {
    auto [type, val] = lexer.get_current_token(index);
    vector<string> key = lexer.get_key_vector();
    vector<string> symbol = lexer.get_symbol_vector();
    vector<string> identify = lexer.get_identify_vector();
    if (type >= 1 and type <= 11) {
      return key[type - key_offset];
    } else if (type >= 12 and type <= 25) {
      return symbol[type - symbol_offset];
    } else if (type == 26) {
      return identify[stoi(val)];
    } else if (type == 27) {
      return val;
    } else {
      assert(false);
    }
    return "error";
  }


void Parser::exception_print(int type, string expected) {
    string ret;
    if (type == 0) { //语法错误
        ret = "语法错误：预期为 " + expected + "，但输入的是 " + get_str();
    } else if (type == 1) {
        ret = "标识符 " + get_str() + " 格式错误";
    } else if (type == 2) {
        ret = "整数 " + get_str() + "格式错误";
    }
    cout << ret << "\n";
    success = false;
    exit(100);
}

void Parser::match(string expected) {
    if (get_str() != expected) {
        exception_print(0, expected);
    }else {
        index++; // Advance to the next token if the current one matches the expected token
    }
}
  
void Parser::match(string expected, bool majority) {
    bool iseuqal = false;
    for (int i = 0; i < symbollength; i++) {
        if (get_str() == symbolarray[i]) {
            index++;
            iseuqal = true; 
            break;
        }
        expected += symbolarray[i];
        if (i != symbollength - 1) {
        expected += " ";
        }
    }
    if (!iseuqal) {
        exception_print(0, expected);
    }
}

void Parser::parse_program() {
    parse_block();
    match(".");
}

void Parser::parse_block() {
    parse_constdecl();
    parse_vardecl();
    parse_procdecl();
    parse_statement();
}

void Parser::parse_constdecl() {
    if (get_str() == "const") {
        match("const");
        parse_constitem();
        while (get_str() == ",") {
            match(",");
            parse_constitem();
        }
        match(";");
    }
}

void Parser::parse_constitem() {
    parse_ident();
    match("=");
    parse_number();
}

void Parser::parse_vardecl() {
    if (get_str() == "var") {
        match("var");
        parse_ident();
        while (get_str() == ",") {
            match(",");
            parse_ident();
        }
        match(";");
    }
}

void Parser::parse_procdecl() {
    while (get_str() == "procedure") {
        match("procedure");
        parse_ident();
        match(";");
        parse_block();
        match(";");
    }
}

void Parser::parse_statement() {
    if (lexer.get_identify_map().count(get_str())) {
        parse_assignstmt();
    } else if (get_str() == "call") {
        parse_callstmt();
    } else if (get_str() == "begin") {
        parse_compstmt();
    } else if (get_str() == "if") {
        parse_ifstmt();
    } else if (get_str() == "while") {
        parse_whilestmt();
    } 
}

void Parser::parse_assignstmt() {
    parse_ident();
    match(":=");
    parse_expression();
}

void Parser::parse_callstmt() {
    match("call");
    parse_ident();
}

void Parser::parse_compstmt() {
    match("begin");
    parse_statement();
    while (get_str() == ";") {
        match(";");
        parse_statement();
    }
    match("end");
}

void Parser::parse_ifstmt() {
    match("if");
    parse_condition();
    match("then");
    parse_statement();
}

void Parser::parse_whilestmt() {
    match("while");
    parse_condition();
    match("do");
    parse_statement();
}

void Parser::parse_condition() {
    if (get_str() == "odd") {
        match("odd");
        parse_expression();
    } else {
        parse_expression();
        match("", true);
        parse_expression();
    }
}

void Parser::parse_expression() {
    parse_term();
    while (get_str() == "+" || get_str() == "-") {
        match(get_str());
        parse_term();
    }
}

void Parser::parse_term() {
    parse_factor();
    while (get_str() == "*" || get_str() == "/") {
        match(get_str());
        parse_factor();
    }
}

void Parser::parse_factor() {
    if (get_str() == "+" || get_str() == "-") {
        match(get_str());
    } 
    if (isDigit(get_str())) {
        parse_number();
    } else if (lexer.get_identify_map().count(get_str())) {
        parse_ident();
    } else {
        match("(");
        parse_expression();
        match(")");
    }
}

void Parser::parse_ident() {
    if (lexer.get_identify_map().count(get_str())) {
        match(get_str());
    } else {
        exception_print(1);
    }
}

void Parser::parse_number() {
    if (isDigit(get_str())) {
        match(get_str());
    } else {
        exception_print(2);
    }
}


bool Parser::GrammaticalAnalysis() {
    parse_program();
    if (success) {
        cout << "语法正确" << endl;
    }
    return success;
}
#pragma endregion Parser