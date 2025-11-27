#include "Lexer.h"

Lexer::Lexer(int key_offset, int symbol_offset) : key_offset(key_offset), symbol_offset(symbol_offset) {
    string str_key = "begin end if then while do const var call procedure odd", s;
    string str_symbol = "+ - * / = # < > := ( ) , . ;";
    stringstream key_in;
    key_in << str_key;
    while (key_in >> s) {
        key.push_back(s);
        key_map[s] = int(key.size()) - 1;
        letter_map[s] = int(key.size()) - 1 + key_offset;
    }

    stringstream symbol_in;
    symbol_in << str_symbol;
    while (symbol_in >> s) {
        symbol.push_back(s);
        symbol_map[s] = int(symbol.size()) - 1;
        letter_map[s] = int(symbol.size()) - 1 + symbol_offset;
    }
}

void Lexer::read_from_file(string file_name, vector<string> &str_input) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cout << "file open failed!\n";
        assert(false);
    }

    // 检查文件是否为空
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "file is empty!" << endl;
        file.close();
        return;
    }

    // 读取文件内容到 stringstream
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();  // 关闭文件流

    // 在文件数据末尾手动添加 @ 符号
    buffer << "\n@";
    read(buffer, str_input);
    file.close();
}

void Lexer::read(istream& input, vector<string> &str_input) {
    if (not str_input.empty()) {
        str_input.clear();
    }
    string str, res, s; 
    while (getline(input, str)) {
        if (str == "@") {
            break;
        }
        if (str.find("//") != string::npos) {
            str.erase(str.find("//"));
        }
        res += str; 
        res.push_back('\n');
    }
    
    stringstream string_stream;
    string_stream << res; 
    res.clear();
    int flag = 1;
    while (string_stream >> s) {
        if (s == "(*") {
            flag ^= 1;
        }
        if (flag) {
            str_input.push_back(s);
        }
        if (s == "*)") {
            flag ^= 1;
        }
    }
}

void Lexer::LexicalAnalysis(vector<string> &str) {
    if (not tokens.empty()) {
        tokens.clear();
    }
    for (string s: str) {
        if (key_map.count(s)) {
            // 关键字
            tokens.push_back(node{letter_map[s], "-"});
        } else if (symbol_map.count(s)) {
            // 符号
            if (int(s.size()) > 10) {
                cout << "symbol's size is too long!\n";
                assert(false);
            }
            tokens.push_back(node{letter_map[s], "-"});
        } else if (identify_map.count(s)) {
            // 标识符
            tokens.push_back(node{identify_type, to_string(identify_map[s])});
        } else {
            // 识别数字, 标识符, 关键符, 算符和界符
            int ns = s.size();
            for (int i = 0; i < ns; i ++ ) {
                if (isdigit(s[i])) {
                    string t; 
                    t += s[i];
                    int j = i; 
                    while (j + 1 < ns and isdigit(s[j + 1])) {
                        ++ j;
                        t += s[j];
                    }
                    tokens.push_back(node{numbers_type, t});        
                    i = j;
                } else if (isalpha(s[i])) {
                    string t; 
                    t += s[i];
                    int j = i; 
                    while (j + 1 < ns and (isalpha(s[j + 1]) or isdigit(s[j + 1]))) {
                        ++ j;
                        t += s[j];
                    }
                    if (key_map.count(t)) {
                        tokens.push_back( {letter_map[t], "-"});        
                    } else {
                        if (int(t.size()) > 10) {
                            cout << "identify's size is too long!\n";
                            assert(false);
                        }
                        if (!identify_map.count(t)) {
                            identify.push_back(t);
                            int new_val = (int)identify.size() - 1;
                            identify_map[t] = new_val;
                        }
                        tokens.push_back(node{identify_type, to_string(identify_map[t])});
                    }
                    i = j;
                } else {
                    string t; 
                    t += s[i];
                    if (symbol_map.count(t)) {
                        tokens.push_back( {letter_map[t], "-"});
                    } else {
                        if (i + 1 >= (int)s.size()) {
                            cout << "illegal!\n";
                            assert(false);
                        }
                        t += s[++ i];
                        if (symbol_map.count(t)) {
                            tokens.push_back(node{letter_map[t], "-"});
                        } else {
                            assert(false);
                        }
                    }
                }
            }
        }
    }
}

Lexer::node Lexer::GetNextToken() {
    if (current_token_index < tokens.size()) {
        return tokens[current_token_index ++];
    }else {
        return node{0, ""}; // Return an empty token if there are no more tokens
    }
    
}

Lexer::node Lexer::get_current_token(int index) {
    if (index < tokens.size()) {
        return tokens[index];
    } else {
        return node{0, ""}; // Return an empty token if there are no more tokens
    }
}

vector<Lexer::node> Lexer::get_tokens() {
    return this->tokens;
} 

unordered_map<string, int> Lexer::get_identify_map() {
    return this->identify_map;
}

vector<string> Lexer::get_identify_vector() {
    return identify;
}
vector<string> Lexer::get_symbol_vector() {
    return symbol;
}
vector<string> Lexer::get_key_vector() {
    return key;
}

