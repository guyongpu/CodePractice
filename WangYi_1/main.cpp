#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct PointStr {
    int a;
    int b;

    PointStr(int num1, int num2) {
        a = num1;
        b = num2;
    }
};

int main() {
    int N = 0;
    cin >> N;
    vector<PointStr> vec_point;
    for (int i = 0; i < N; i++) {
        int a, b;
        cin >> a >> b;
        PointStr point(a, b);
        vec_point.push_back(point);
    }

    PointStr point1(123, 456);
    //vec_point.push_back(point1);
    PointStr point2(15, 20);
    //vec_point.push_back(point2);
    PointStr point3(20, 15);
    //vec_point.push_back(point3);

    vector<vector<int>> Count;
    for (int i = 0; i < vec_point.size(); i++) {
        int a = vec_point[i].a;
        int b = vec_point[i].b;
        int result = a * b;

        vector<int> one_record(10, 0);
        //1.记录a,b,res
        string str1 = to_string(a);
        for (int i = 0; i < str1.length(); i++) {
            int x = str1[i] - '0';
            one_record[x]++;
        }
        string str2 = to_string(b);
        for (int i = 0; i < str2.length(); i++) {
            int x = str2[i] - '0';
            one_record[x]++;
        }
        string str3 = to_string(result);
        for (int i = 0; i < str3.length(); i++) {
            int x = str3[i] - '0';
            one_record[x]++;
        }

        //2.计算
        string strb = to_string(b);
        int lenb = strb.length();
        for (int i = lenb - 1; i >= 0; i--) {
            int num_b = strb[i] - '0';
            int res = a * num_b;
            //cout << res << endl;

            string str_res = to_string(res);
            for (int i = 0; i < str_res.length(); i++) {
                int x = str_res[i] - '0';
                one_record[x]++;
            }
        }

        Count.push_back(one_record);
    }
    //开始计算幸运数字
    vector<int> lucky(10,0);
    for (int i = 0; i < Count.size(); i++) {
        for(int j =0; j < Count[i].size();j++){
            int num = Count[i][j];
            lucky[j] = lucky[j]+num;
        }
    }
    int MaxNum = -1;
    int MaxCnt = -1;
    for(int i = 1;i<lucky.size();i++){
        if(lucky[i]>MaxCnt)
        {
            MaxCnt = lucky[i];
            MaxNum = i;
        }
    }
    //开始输出
    for(int i = 0;i<Count.size();i++){
        for(int j = 1;j<Count[i].size();j++){
            cout << Count[i][j] << " ";
        }
        cout << endl;
    }
    cout<< MaxNum<< endl;



    return 0;
}