#include <iostream>
#include <vector>
#include <string>
// #include <sstream>
using namespace std;
vector<int> IntersectionSet(const vector<int>& A, const vector<int>& B);
vector<int> UnionSet(const vector<int>& A, const vector<int>& B);
vector<int> DifferenceSet(const vector<int>& A, const vector<int>& B);
int main(){
    vector<int> A;
    vector<int> B;
    // string line;
    int num;
    // getline(cin, line);
    // istringstream StreamA(line);
    // while(StreamA >> num){
    //     A.push_back(num);
    // }

    // getline(cin, line);
    // istringstream StreamB(line);
    // while(StreamB >> num){
    //     B.push_back(num);
    // }

    while(cin >> num && num != -1) {
        A.push_back(num);
    }

    while(cin >> num && num != -1) {
        B.push_back(num);
    }

    vector<int> I = IntersectionSet(A, B);
    vector<int> U = UnionSet(A, B);
    vector<int> D = DifferenceSet(A, B);

    cout << "A交B={";
    for(size_t i = 0; i < I.size(); i++){
        if(i != I.size() - 1){
            cout << I[i] << ",";
        }
        else{
            cout << I[i] << "} ";
        }
        
    }

    cout << "A并B={";
    for(size_t i = 0; i < U.size(); i++){
        if(i != U.size() - 1){
            cout << U[i] << ",";
        }
        else{
            cout << U[i] << "} ";
        }
        
    }

    cout << "A-B={";
    for(size_t i = 0; i < D.size(); i++){
        if(i != D.size() - 1){
            cout << D[i] << ",";
        }
        else{
            cout << D[i] << "} ";
        }
    }
    cout << endl;


    return 0;
}

vector<int> IntersectionSet(const vector<int>& A, const vector<int>& B){
    vector<int> C;
    for(size_t i = 0; i < A.size(); i++){
        for(size_t j = 0; j < B.size(); j++){
            if(A[i] == B[j]){
                C.push_back(A[i]);
            }
        }
    }
    return C;
}

vector<int> UnionSet(const vector<int>& A, const vector<int>& B){
    vector<int> C;
    vector<int> D = IntersectionSet(A, B);
    bool flag;
    for(size_t i = 0; i < A.size(); i++){
        C.push_back(A[i]);
    }

    for(size_t i = 0; i < B.size(); i++){
        flag = true;
        for(size_t j = 0; j < D.size(); j++){
            if(B[i] == D[j]){
                flag = false;
                break;
            }
        }
        if(flag){
            C.push_back(B[i]);
        }
    }
    return C;
}

vector<int> DifferenceSet(const vector<int>& A, const vector<int>& B){
    vector<int> C;
    vector<int> D = IntersectionSet(A, B);
    bool flag;
    for(size_t i = 0; i < A.size(); i++){
        flag = true;
        for(size_t j = 0; j < D.size(); j++){
            if(A[i] == D[j]){
                flag = false;
                break;
            }
        }
        if(flag){
            C.push_back(A[i]);
        }
    }
    return C;
}