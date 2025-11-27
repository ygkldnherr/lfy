#include "Parser.h"

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

#pragma region Parser

Parser::Parser(vector<Lexer::node> tokens, const unordered_map<string,int>& identify_map, 
               const vector<string>& key, const vector<string>& symbol, const vector<string>& identify, int key_offset, int symbol_offset) {
    this->tokens = tokens;
    this->identify_map = identify_map;
    this->key = key;
    this->symbol = symbol;
    this->identify = identify;
    this->index = 0;
    this->success = true;
    this->key_offset = key_offset;
    this->symbol_offset = symbol_offset;
}

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

string Parser::get_val(Lexer::node &nd) {
    if (index >= tokens.size()) {
        return ""; // 或抛错，防止越界
    }
    auto [type, val] = nd;
    if (type >= 1 && type <= 11) {
        return key[type - key_offset];
    } else if (type >= 12 && type <= 25) {
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
        ret = "语法错误：预期为 " + expected + "，但输入的是 " + get_val(tokens[index]);
    } else if (type == 1) {
        ret = "标识符 " + get_val(tokens[index]) + " 格式错误";
    } else if (type == 2) {
        ret = "整数 " + get_val(tokens[index]) + "格式错误";
    }
    cout << ret << "\n";
    success = false;
    exit(100);
}

void Parser::match(string expected) {
    if (get_val(tokens[index]) != expected) {
        exception_print(0, expected);
    }else {
        index++;
    }
}
  
void Parser::match(string expected, bool majority) {
    bool iseuqal = false;
    for (int i = 0; i < symbollength; i++) {
        if (get_val(tokens[index]) == symbolarray[i]) {
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
    if (get_val(tokens[index]) == "const") {
        match("const");
        parse_constitem();
        while (get_val(tokens[index]) == ",") {
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
    if (get_val(tokens[index]) == "var") {
        match("var");
        parse_ident();
        while (get_val(tokens[index]) == ",") {
            match(",");
            parse_ident();
        }
        match(";");
    }
}

void Parser::parse_procdecl() {
    while (get_val(tokens[index]) == "procedure") {
        match("procedure");
        parse_ident();
        match(";");
        parse_block();
        match(";");
    }
}

void Parser::parse_statement() {
    string current = get_val(tokens[index]);
    if (identify_map.count(current)) {
        parse_assignstmt();
    } else if (current == "call") {
        parse_callstmt();
    } else if (current == "begin") {
        parse_compstmt();
    } else if (current == "if") {
        parse_ifstmt();
    } else if (current == "while") {
        parse_whilestmt();
    } else {
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
    while (get_val(tokens[index]) == ";") {
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
    if (get_val(tokens[index]) == "odd") {
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
    while (get_val(tokens[index]) == "+" || get_val(tokens[index]) == "-") {
        match(get_val(tokens[index]));
        parse_term();
    }
}

void Parser::parse_term() {
    parse_factor();
    while (get_val(tokens[index]) == "*" || get_val(tokens[index]) == "/") {
        match(get_val(tokens[index]));
        parse_factor();
    }
}

void Parser::parse_factor() {
    string current = get_val(tokens[index]);
    if (current == "+" || current == "-") {
        match(current);
        current = get_val(tokens[index]); // 重新获取下一个token
    }
    if (isDigit(current)) {
        parse_number();
    } else if (identify_map.count(current)) {
        parse_ident();
    } else if (current == "(") {
        match("(");
        parse_expression();
        match(")");
    } else {
        exception_print(0, "因子");
    }
}

void Parser::parse_ident() {
    string current = get_val(tokens[index]);
    if (identify_map.count(current)) {
        match(current);
    } else {
        exception_print(1, "标识符");
    }
}

void Parser::parse_number() {
    if (isDigit(get_val(tokens[index]))) {
        match(get_val(tokens[index]));
    } else {
        exception_print(2);
    }
}

bool Parser::GrammaticalAnalysis() {
    parse_program();
    return success;
}

#pragma endregion Parser