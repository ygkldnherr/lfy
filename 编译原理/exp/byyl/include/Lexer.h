#ifndef LEXER_H
#define LEXER_H
#include <bits/stdc++.h>

using namespace std;
class Lexer {
public:
    struct node {
        int type;
        string val;
    };
    Lexer(int key_offset, int symbol_offset);
    vector<node> get_tokens();
    void read_from_file(string file_name, vector<string> &str_input);
    void read(istream& input, vector<string> &str_input);
    void LexicalAnalysis(vector<string> &str);
    node GetNextToken();
    unordered_map<string, int> get_identify_map();
    vector<string> get_symbol_vector();
    vector<string> get_identify_vector();
    vector<string> get_key_vector();
    node get_current_token(int index);

private:
    vector<string> key, identify, symbol;
    vector<node> tokens; //词法分析结果
    unordered_map<string, int> key_map, identify_map, symbol_map, letter_map;
    /*
        关键字：1-11
        算符和界符：12-25
        标识符：26
        数字：27
    */
    const int identify_type = 26;
    const int numbers_type = 27;
    unsigned int current_token_index = 0;
    int key_offset;
    int symbol_offset;
};

#endif // LEXER_H