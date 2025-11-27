#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

/*
    关键字：
    begin       1
    end         2
    if          3
    then        4
    while       5
    do          6
    const       7
    var         8
    call        9
    procedure   10
    odd         11

    运算符：
    +           12
    -           13
    *           14
    /           15
    =           16
    #           17
    <           18
    >           19
    =           20
    (           21
    )           22
    ,           23
    .           24
    ;           25

    标识符：    26
    常数：      27
*/

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    int key_offset = 1;
    int symbol_offset = 12;
    string filename = argv[1];
    Lexer lexer(key_offset, symbol_offset);
    vector<string> str;
    lexer.read_from_file(filename, str);
    lexer.LexicalAnalysis(str);

    for (auto [type, val] : lexer.get_tokens()) {
        cout << "(" << type << "," << val << ")";
        if (type == 26) {
            cout << " //" << lexer.get_identify_vector()[stoi(val)] << endl;
        } else {
            cout << endl;
        }
    }

    Parser parser(lexer.get_tokens(), lexer.get_identify_map(),
                  lexer.get_key_vector(), lexer.get_symbol_vector(),
                  lexer.get_identify_vector(), key_offset, symbol_offset);

    if (parser.GrammaticalAnalysis()) {
        cout << "Grammar correct" << endl;
    } else {
        cout << "Grammar error" << endl;
        return 1;
    }

    SemanticAnalyzer sa(lexer.get_tokens(), lexer.get_identify_map(),
                                  lexer.get_key_vector(), lexer.get_symbol_vector(),
                                  lexer.get_identify_vector(), key_offset, symbol_offset);

    // cout << "Semantic analysis passed.\nAST:\n";
    sa.ga_sa();
    return 0;
}
