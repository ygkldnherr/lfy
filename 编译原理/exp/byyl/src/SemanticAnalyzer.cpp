#include "SemanticAnalyzer.h"
/*
program → block 
		【program.nptr=block.nptr;】 "."
block → constdecl vardecl procdecl statement
		【block.nptr=opr(".", 4, 
			constdel.nptr, vardecl.nptr, 
			procdecl.nptr, statement.nptr);】
constdecl → ["const" constitem 
		【tmpP=constitem.nptr;】 
		{"," constitem 
		【tmpP=opr(",", 2, tmpP, constitem.nptr);】 
		} ";" 
		【constdecl.nptr=opr("const", 1, tmpP);】] 
constitem → ident 
		【tmpP=id(entry);】"=" number
		【constitem.nptr=op('=', 2, tmpP, num(value));】
vardecl → ["var" ident
		【tmpP=id(entry);】{"," ident
		【tmpP=op(',', 2, tmpP, id(entry));】} ";"
		【vardecl.nptr=opr("var", 1, tmpP);】] 
procdecl → 【tmpP=NULL;】{"procedure" ident
		【tmpPN=id(entry);】 ";" block
		【tmpPB=block.nptr;】 ";"
		【if(tmpP==NULL) 
			tmpP=op("procedure", 2, tmpPN, tmpPB);
		else
			tmpP=op(";", 2, tmpP, op("procedure", 2, tmpPN, tmpPB));】}
		【procdecl.nptr=tmpP;】
statement → assignstmt
		 【tmpP=assignment.nptr;
			if(tmpP!=NULL) statement.nptr=tmpP;】
			| callstmt 
		 【tmpP=callstmt.nptr;
			if(tmpP!=NULL) statement.nptr=tmpP;】
			| compstmt 
		 【tmpP=compstmt.nptr;
			if(tmpP!=NULL) statement.nptr=tmpP;】
			| ifstmt 
		 【tmpP=ifstmt.nptr;
			if(tmpP!=NULL) statement.nptr=tmpP;】
			| whilestmt
		 【tmpP=whilestmt.nptr;
			if(tmpP!=NULL) statement.nptr=tmpP;】
assignstmt → [ident
		【tmpP=id(entry);】 ":=" expression
		【assignment.nptr=opr(":=", 2, tmpP, expression.nptr);】]
callstmt → ["call" ident
		【callstmt.nptr=opr("call", 1, id(entry));】 ]
compstmt → ["begin" statement
		【tmpP=statement.nptr;】 {";" statement
		【tmpP=opr(";", 2, tmpP, statement.nptr;】} "end"
		【compstmt.nptr=opr("degin", 1, tmpP);】 ]
ifstmt → ["if" condition
		【tmpP=condition.nptr;】 "then" statement
		【ifstmt.nptr=opr("if", 2, tmpP, statement.nptr);】 ]
whilestmt → ["while" condition
		【tmpP=condition.nptr;】 "do" statement
		【whilestmt.nptr=opr("while", 2, tmpP, statement.nptr);】 ]
condition = "odd" expression 
		【condition.nptr=opr("odd", 1, expression.nptr);】
			| expression
		【tmpP=expression.nptr;】 ("="|"#"|"<"|">") expression
		【condition.nptr=opr("="|"#"|"<"|">", 2, tmpP, expression.nptr);】
expression → term
		【tmpP=term.nptr;】 { ("+"|"-") term
		【tmpP=opr("+"|"-", 2, tmpP, term.nptr);】 }
		【expression.nptr=tmpP;】
term → factor
		【tmpP=factor.nptr;】 {("*"|"/") factor
		【tmpP=opr("*"|"/", 2, tmpP, factor.nptr);】 }
		【term.nptr=tmpP;】
factor → [ "+"|"-"
		【sign=UPLUS|UMINUS;】] (ident
		【tmpP=id(entry);】 | number
		【tmpP=num(value);】 | "(" expression
		【tmpP=expression.nptr;】 ")")
		【 if(sign) factor.nptr=opr(sign, 1, tmpP)
			else factor.nptr=tmpP;】

*/

SemanticAnalyzer::SemanticAnalyzer(const std::vector<Lexer::Lexer::node>& tokens, 
                                    const std::unordered_map<std::string, int>& identify_map, 
                                    const std::vector<std::string>& key, 
                                    const std::vector<std::string>& symbol, 
                                    const std::vector<std::string>& identify,
                                    int key_offset,
                                    int symbol_offset)
{
    this->tokens = tokens;
    this->identify_map = identify_map;
    this->key = key;
    this->symbol = symbol;
    this->identify = identify;
    this->index = 0;
    this->nEndD = Lexer::node{0, ""}; // Initialize nEndD with an empty node
    this->root = nullptr;
    this->haveLimit = false;
    this->haveAns = false;
    this->limit = 0;
    this->f1 = 0;
    this->f2 = 0;
    this->t = 0;
    this->n = 0;
    this->key_offset = key_offset;
    this->symbol_offset = symbol_offset;
}

// string SemanticAnalyzer::getTempNumber() {
//     string ans = "T" + to_string(tempVarNumber);
//     tempVarNumber += 1;
//     return ans;
// }

bool SemanticAnalyzer::isDigit(string s) {
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

string SemanticAnalyzer::get_val (Lexer::node nd) {
    auto [type, val] = nd;
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

void SemanticAnalyzer::exception_print(int type, string expected) {
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
}

void SemanticAnalyzer::match(string expected) {
    if (get_val(tokens[index]) != expected) {
        exception_print(0, expected);
    }else {
        index++;
    }
}

void SemanticAnalyzer::match(string expected, bool majority) {
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



// + - * / :=  > < >= <= 
SemanticAnalyzer::Treenode* SemanticAnalyzer::opr(Lexer::node op, int num, vector<Treenode*> allNode) {
    Treenode *rt = new Treenode(op, "-");
    for (auto Item: allNode) {
        rt->add(Item);
    }
    return rt;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::opr(Lexer::node op, int num, vector<Treenode*> allNode, string name) {
    Treenode *rt = new Treenode(op, name);
    for (auto Item: allNode) {
        rt->add(Item);
    }
    return rt;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::id(Lexer::node entry) {
    Treenode *rt = new Treenode(entry, "-");
    return rt;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::id(Lexer::node entry, string name) {
    Treenode *rt = new Treenode(entry, name);
    return rt;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::num(Lexer::node entry) {
    Treenode *rt = new Treenode(entry);
    return rt;
}

int SemanticAnalyzer::find(int x) {
    if (x <= 1) return x;
    if (mp.count(x)) return mp[x];
    mp[x] = find(x - 2) + find(x - 1); 
    return mp[x];
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_program() {
    auto tmpP = sa_block();
    Lexer::node nD = tokens[index];
    match(".");
    auto ans = opr(nD, 4, tmpP);
    return ans;
}

vector<SemanticAnalyzer::Treenode*> SemanticAnalyzer::sa_block() {
    return vector<Treenode*>{sa_constdecl(), sa_vardecl(), sa_procdecl(), sa_statement()};
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_constdecl() {
    Treenode* tmpP = NULL;
    
    if (get_val(tokens[index]) == "const") {
        Lexer::node nConst = tokens[index];
        match("const");
        tmpP = sa_constitem();

        while (get_val(tokens[index]) == ",") {
            Lexer::node nD = tokens[index];
            match(",");
            tmpP = opr(nD, 2, vector<Treenode*>{tmpP, sa_constitem()});
        }
        match(";");
        tmpP = opr(nConst, 1, vector<Treenode*>{tmpP});
    }
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_constitem() {
    Treenode* tmpP = id(tokens[index]);
    sa_ident();
    auto Equal = tokens[index];
    match("=");
    //Sec0.push_back(section{"const", get_val(node{tmpP->type, tmpP->val}), get_val(tokens[index]), "-"});
    tmpP = opr(Equal, 2, vector<Treenode*>{tmpP, num(tokens[index])});
    sa_number();
    return tmpP;
}

// void SemanticAnalyzer::get_limit() {
//     for (auto x: Sec0) {
//         if (x.op == "const" and x.arg1 == "limit") {
//             haveLimit = true;
//             limit = stoi(x.arg2);
//             return ; 
//         } 
//     }
// }

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_vardecl() {
    Treenode* tmpP = NULL;
    if (get_val(tokens[index]) == "var") {
        Lexer::node nVar = tokens[index];
        match("var");
        //Sec0.push_back(section{"var", get_val(tokens[index]), "0", "-"});
        tmpP = sa_ident();
        
        while (get_val(tokens[index]) == ",") {
        Lexer::node d = tokens[index];
        match(",");
        //Sec0.push_back(section{"var", get_val(tokens[index]), "0", "-"});
        tmpP = opr(d, 2, vector<Treenode*>{tmpP, sa_ident()});
        }
        tmpP = opr(nVar, 1, vector<Treenode*>{tmpP});
        match(";");
    }
    return tmpP;
}

void SemanticAnalyzer::get_ans() {
    n = limit;
    f2 = find(limit);
    f1 = find(limit - 1);
    t = f1;
    haveAns = true;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_procdecl() {
    Treenode* tmpP = NULL;
    while (get_val(tokens[index]) == "procedure") {
        Lexer::node Procedure = tokens[index];
        match("procedure");
        Treenode* tmpPN = id(tokens[index]);
        sa_ident();
        match(";");
        auto vectmpPB = sa_block();
        Treenode* tmpPB = opr(nEndD, 4, vectmpPB);
        Lexer::node f = tokens[index];
        match(";");
        auto tp = opr(Procedure, 2, vector<Treenode*> {tmpPN, tmpPB}); 
        if (tmpP == NULL) {
        tmpP = tp;
        } else {
        tmpP = opr(f, 2, vector<Treenode*>{tmpP, tp});
        }
    }
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_statement() {
    Treenode* tmpP = NULL;
    if (identify_map.count(get_val(tokens[index]))) {
        tmpP = sa_assignstmt();
    } else if (get_val(tokens[index]) == "call") {
        tmpP = sa_callstmt();
    } else if (get_val(tokens[index]) == "begin") {
        tmpP = sa_compstmt();
    } else if (get_val(tokens[index]) == "if") {
        tmpP = sa_ifstmt();
    } else if (get_val(tokens[index]) == "while") {
        tmpP = sa_whilestmt();
    } 
    // if (!haveLimit) get_limit();
    // if (!haveAns) get_ans();
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_assignstmt() {
    auto nowIdent = get_val(tokens[index]);
    auto tmpP = sa_ident();
    Lexer::node mEqual = tokens[index];
    match(":=");
    auto Rtexpression = sa_expression();
    //string RtName = Rtexpression->tempVarName;
    //if (RtName == "-") RtName = get_val(Lexer::node{Rtexpression->type, Rtexpression->val});
    //Sec.push_back(section{":=", RtName, "-", nowIdent}); // Tag1
    tmpP = opr(mEqual, 2, vector<Treenode*>{tmpP, Rtexpression});
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_callstmt() {
    Treenode* tmpP = NULL;
    Lexer::node Call = tokens[index];
    match("call");
    auto ParseIdent = sa_ident();
    //Sec.push_back(section{"call", "-", "-", "print"});
    tmpP = opr(Call, 1, vector<Treenode*>{ParseIdent});
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_compstmt() {
    Lexer::node nBegin = tokens[index];
    Treenode* tmpP = NULL;
    match("begin");
    tmpP = sa_statement();
    while (get_val(tokens[index]) == ";") {
        auto F = tokens[index];
        match(";");
        tmpP = opr(F, 2, vector<Treenode*>{tmpP, sa_statement()});
    }
    tmpP = opr(nBegin, 1, vector<Treenode*>{tmpP});
    match("end");
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_ifstmt() {
    Treenode* tmpP = NULL;
    auto nIf = tokens[index];
    match("if");
    tmpP = sa_condition();
    match("then");
    tmpP = opr(nIf, 2, vector<Treenode*>{tmpP, sa_statement()});
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_whilestmt() {
    Treenode* tmpP = NULL;
    auto nWhile = tokens[index];
    match("while");
    tmpP = sa_condition();
    match("do");
    tmpP = opr(nWhile, 2, vector<Treenode*>{tmpP, sa_statement()});
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_condition() {
    Treenode* tmpP = nullptr;
    
    // 处理 odd 条件
    if (get_val(tokens[index]) == "odd") {
        auto nOdd = tokens[index];
        match("odd");  // 直接使用你的基础 match 方法
        tmpP = opr(nOdd, 1, vector<Treenode*>{sa_expression()});
    } 
    // 处理普通条件表达式
    else {
        tmpP = sa_expression();
        Lexer::node nSymbol = tokens[index];
        
        // 使用带 majority 参数的 match 来匹配任意运算符
        match("", true);  // 这会匹配任何在 symbolarray 中的符号
        
        // 处理特殊运算符（如 # 表示 !=）
        if (nSymbol.val == "#") {
            auto ntmp = sa_expression();
            string tmpName = tmpP->tempVarName;
            if (tmpName == "-") {
                tmpName = get_val(Lexer::node{tmpP->type, tmpP->val});
            }
            
            string ntmpName = ntmp->tempVarName;
            if (ntmpName == "-") {
                ntmpName = get_val(Lexer::node{ntmp->type, ntmp->val});
            }
            
            // 这里可以添加你的特殊处理逻辑
        }
        // 处理其他比较运算符
        else {
            tmpP = opr(nSymbol, 2, vector<Treenode*>{tmpP, sa_expression()});
        }
    }
    
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_expression() { // Tag2 
    auto tmpP = sa_term();
    while (get_val(tokens[index]) == "+" || get_val(tokens[index]) == "-") {
        Lexer::node nOp = tokens[index];
        match(get_val(tokens[index]));
        auto Rt = sa_term();
        string tmpPname = tmpP->tempVarName; 
        if (tmpPname == "-") tmpPname = get_val(Lexer::node{tmpP->type, tmpP->val});
        string Rtname = Rt->tempVarName; 
        if (Rtname == "-") Rtname = get_val(Lexer::node{Rt->type, Rt->val});
        //Sec.push_back(section{get_val(nOp), tmpPname, Rtname, getTempNumber()});
        // cout << Sec.back().result << " ::\n";
        //tmpP = opr(nOp, 2, vector<Treenode*>{tmpP, Rt}, Sec.back().result);
    }
    // cout << tmpP->tempVarName << "  ppp\n";
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_term() { // Tag3
    auto tmpP = sa_factor();
    while (get_val(tokens[index]) == "*" || get_val(tokens[index]) == "/") {
        //string name = getTempNumber();
        Lexer::node nOp = tokens[index];
        match(get_val(tokens[index]));
        auto Rt = sa_factor();
        string tmpPname = tmpP->tempVarName; 
        if (tmpPname == "-") tmpPname = get_val(Lexer::node{tmpP->type, tmpP->val});
        string Rtname = Rt->tempVarName; 
        if (Rtname == "-") Rtname = get_val(Lexer::node{Rt->type, Rt->val});
        //Sec.push_back(section{get_val(nOp), tmpPname, Rtname, getTempNumber()});
        // cout << Sec.back().result << "  result\n";
        //tmpP = opr(nOp, 2, vector<Treenode*>{tmpP, Rt}, Sec.back().result);
    }
    // cout << tmpP->tempVarName << " :::\n";
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_factor() {
    Lexer::node sign{-1, ""};
    Treenode* tmpP = NULL;
    if (get_val(tokens[index]) == "+" || get_val(tokens[index]) == "-") {
        sign = tokens[index];
        match(get_val(tokens[index]));
    } 
    if (isDigit(get_val(tokens[index])) ) { 
        tmpP = num(tokens[index]);
        sa_number();
    } else if (identify_map.count(get_val(tokens[index]))) {
        tmpP = id(tokens[index]);
        sa_ident();
    } else {
        match("(");
        tmpP = sa_expression();
        match(")");
    }
    if (sign.type != -1) {
        string tmpPname = tmpP->tempVarName; 
        if (tmpPname == "-") tmpPname = get_val(Lexer::node{tmpP->type, tmpP->val});
        //Sec.push_back(section{"neg", tmpPname, "-", getTempNumber()});
        //tmpP = opr(sign, 1, vector<Treenode*>{tmpP}, Sec.back().result);
    } else {
        tmpP->tempVarName = get_val(Lexer::node{tmpP->type, tmpP->val});
    }
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_ident() {
    Treenode* tmpP = NULL;
    if (identify_map.count(get_val(tokens[index]))) {
        tmpP = id(tokens[index]);
        match(get_val(tokens[index]));
    } else {
        exception_print(1);
    }
    return tmpP;
}

SemanticAnalyzer::Treenode* SemanticAnalyzer::sa_number() {
    Treenode* tmpP = NULL;
    if (isDigit(get_val(tokens[index]))) {
        tmpP = num(tokens[index]);
        match(get_val(tokens[index]));
    } else {
        exception_print(2);
    }
    return tmpP;
}

bool SemanticAnalyzer::ga_sa() {
    root = sa_program();
    if (root == NULL) {
        cout << "语法分析失败\n";
        success = false;
        return success;
    }
    print_AST(root, 0);
    return success;
}

void SemanticAnalyzer::print_AST(Treenode *rt, int Tab) { // print abstract grammar tree
    if (rt == NULL) return ;
    for (int i = 0; i < Tab; i ++ ) cout << string(2, ' ');
        cout << get_val(Lexer::node{rt->type, rt->val}) << "\n";
    for (auto p: rt->children) {
        print_AST(p, Tab + 1);
    }
}