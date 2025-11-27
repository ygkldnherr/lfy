#include <iostream>
#include <vector>
using namespace std;

int sum(const vector<int>& v);
int avg(const vector<int>& v);
int max(const vector<int>& v);
int min(const vector<int>& v);

int main() {
    vector<int> v;
    int num;
    
    while(cin >> num && num != -1) {
        v.push_back(num);
    }

    cout << "总和 = " << sum(v) << "; 平均值 = " << avg(v) << "; 最大值 = " << max(v) << "; 最小值 = " << min(v) << endl;
    return 0;
}

int sum(const vector<int>& v) {
    int sum = 0;
    for(int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    return sum;
}

int avg(const vector<int>& v) {
    return sum(v) / v.size();
}

int max(const vector<int>& v) {
    int max = v[0];
    for(int i = 1; i < v.size(); i++) {
        if(v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

int min(const vector<int>& v) {
    int min = v[0];
    for(int i = 1; i < v.size(); i++) {
        if(v[i] < min) {
            min = v[i];
        }
    }
    return min;
}