#include <iostream>
#include <vector>

using namespace std;

/**
 * 题目：KMP字符串匹配算法
 * 描述：使用KMP算法实现字符串匹配
 * 思路：1.计算next；2,.右移next；3.匹配字符串
 * 备注：掌握KMP
 */

//方法1：计算next数组，采用递推式的方法，效率更高
vector<int> cal_next_1(string needle) {
    int len = needle.length();
    vector<int> next(len, 0);
    next[0] = 0;
    int val = 0;
    int i = 1;
    while (i < len) {
        if (needle[i] == needle[val]) {
            val = val + 1;
            next[i] = val;
            i++;
        } else {
            if (val > 0) {
                val = next[val - 1];      //偏移映射
            } else {
                next[i] = 0;
                i++;
            }
        }
    }
    for (int i = 0; i < next.size(); i++) {
        cout << next[i] << " ";
    }
    cout << endl;

    return next;
}

//方法2：计算next数组，采用普通方法求解，效率较低，但易懂
vector<int> cal_next_2(string needle) {
    int len = needle.length();
    vector<int> next(len, 0);
    for (int i = 0; i < len; i++) {
        string substr = needle.substr(0, i + 1);
        int len_sub = substr.length();
        for (int j = len_sub - 1; j > 0; j--) {
            string sub_front = substr.substr(0, j);
            string sub_last = substr.substr(len_sub - j, j);
//            cout << sub_front << " " << sub_last << endl;
            if (sub_front == sub_last) {
                next[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < next.size(); i++) {
        cout << next[i] << " ";
    }
    cout << endl;
    return next;
}

//next数组向右移动一位
vector<int> move_next(vector<int> next) {
    int len = next.size();
    for (int i = len - 1; i > 0; i--) {
        next[i] = next[i - 1];
    }
    next[0] = -1;
    return next;
}

//KMP字符串匹配函数
void kmp_search(string haystack, string needle) {
    int len_1 = haystack.length();
    int len_2 = needle.length();

    if(len_2 > len_1){
        cout << "Not Find" << endl;
        return;
    }
    if (len_2 == 0){
        cout << "Find at 0" << endl;
        return;
    }

    vector<int> next1 = cal_next_1(needle);     //求next数组（增强版）
    vector<int> next2 = cal_next_2(needle);     //求next数组（简单版）

    vector<int> next = move_next(next1);        //next数组向右移动一位，next[0] = -1;

    //开始匹配
    int i = 0;  //指向 haystack 的下标
    int j = 0;  //指向 needle 的下标
    while (i < len_1) {
        if (j == len_2 - 1 && haystack[i] == needle[j]) {
            cout << "Find at " << i - j << endl;
            j = next[j];                        //继续往后匹配
        }
        if (haystack[i] == needle[j]) {         //匹配正确，j和i同时右移
            i++;
            j++;
        } else {
            j = next[j];                        //匹配错误，j回退到next[j]
            if (j == -1) {                      //如果j=-1,则j和i同时右移
                i++;
                j++;
            }
        }
    }
}

int main() {
    string haystack = "ABABABABCABAABABABCABAAD";
    string needle = "ABABA";
    kmp_search(haystack, needle);
    return 0;
}