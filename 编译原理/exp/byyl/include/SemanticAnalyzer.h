#ifndef SA_H
#define SA_H
#include "Lexer.h"
#include <unordered_map>
#include <string>
#include <vector>

class SemanticAnalyzer {
public:
    struct Treenode {
        int type;
        string val; 
        vector<Treenode*> children;
        string tempVarName = "-"; // 用于存储临时变量名
        Treenode () {}
        Treenode (Lexer::node nd) { type = nd.type; val = nd.val; children = {};}
        Treenode (Lexer::node nd, string name) { type = nd.type; val = nd.val; children = {}; tempVarName = name;}
        int get_children_count() { 
            return children.size(); 
        }
        void add(Treenode* node) {
            children.push_back(node);
        }
    };
    SemanticAnalyzer(const std::vector<Lexer::Lexer::node>& tokens,
                     const std::unordered_map<std::string, int>& identify_map,
                     const std::vector<std::string>& key,
                     const std::vector<std::string>& symbol,
                     const std::vector<std::string>& identify,
                     int key_offset,
                     int symbol_offset);
    bool isDigit(string s);
    string get_val(Lexer::node nd);
    void exception_print(int type, string expected = "");
    void match(string expected);
    void match(string expected, bool majority);
    

    int find(int x);
    void get_ans();
    
    Treenode* opr(Lexer::node op, int num, vector<Treenode*> allnode);
    Treenode* opr(Lexer::node op, int num, vector<Treenode*> allnode, string name);
    Treenode* id(Lexer::node entry);
    Treenode* id(Lexer::node entry, string name);
    Treenode* num(Lexer::node entry);

    Treenode* getASTRoot() { return root; }

    Treenode* sa_program();
    vector<Treenode*> sa_block();
    Treenode* sa_constdecl();
    Treenode* sa_constitem();
    Treenode* sa_vardecl();
    Treenode* sa_procdecl();
    Treenode* sa_statement();
    Treenode* sa_assignstmt();
    Treenode* sa_callstmt();
    Treenode* sa_compstmt();
    Treenode* sa_ifstmt();
    Treenode* sa_whilestmt();
    Treenode* sa_condition();
    Treenode* sa_expression();
    Treenode* sa_term();
    Treenode* sa_factor();
    Treenode* sa_ident();
    Treenode* sa_number();
    bool ga_sa();
    void print_AST(Treenode *rt, int Tab);
    // bool analyze();
    // void get_limit();
private:
    vector<Lexer::node> tokens; //存储输入的字符串
    unordered_map<string, int> identify_map;
    vector<string> key, identify, symbol; //存储关键字、标识符和符号
    int key_offset;
    int symbol_offset;
    Lexer::node currentToken;
    bool success = true;
    int index = 0;
    string symbolarray[4] = { "=", "#", "<", ">" };
    int symbollength = 4;

    Lexer::node nEndD;
    Treenode* root;
    int limit, f1, f2, t, n;
    bool haveLimit, haveAns;
    map<int, int> mp;
};

#endif // SA_H