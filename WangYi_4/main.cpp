
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct song {
    int px;
    int py;

    song(int x, int y) {

        this->px = x;
        this->py = y;
    }
    bool operator==(const song& rhs) { return (*this).px == rhs.px && (*this).py == rhs.py;}

};

bool cmp(song &s1, song &s2){
    if(s1.px != s2.px)
        return s1.px < s2.px;
    else
        return s1.py < s2.py;

}

int main() {
    int time = 0;
    cin >> time;
    while (time--) {
        int Num = 0;
        cin >> Num;
        vector<song> vec;
        for (int i = 0; i < Num; i++) {
            int a, b;
            cin >> a >> b;
            song str(a, b);
            vec.push_back(str);
        }
        vector<song> mySet1;
        for (int i = 0; i < vec.size(); i++) {
            int ax = vec[i].px;
            int ay = vec[i].py;
            for (int j = 0; j < vec.size(); j++) {
                if (vec[j].px >= ax && vec[j].py <= ay) {
                    song temp(ax, vec[j].py);
                    //cout << "t:" <<temp.px << " " << temp.py << endl;
                    mySet1.push_back(temp);
                }
            }
        }
        sort(mySet1.begin(),mySet1.end(),cmp);
        auto iter = unique(mySet1.begin(),mySet1.end());
        mySet1.erase(iter, mySet1.end());

        cout << mySet1.size() << endl;

        /*
        for (auto it:mySet1) {
            cout << "<" << it.px << "," << it.py << ">" << endl;
        }*/
    }
};