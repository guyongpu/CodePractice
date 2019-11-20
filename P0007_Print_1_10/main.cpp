#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

/**
 * 使用多线程交替打印1-10
 */

int number;
const int MAXNUM = 10;

/****************************************************************************/
// 使用信号量

mutex mutex_number;
// 打印奇数
void add_1_1() {
    while (1) {
        mutex_number.lock();
        if (number >= MAXNUM) {
            mutex_number.unlock();
            break;
        }
        if (number % 2 == 0) {
            number++;
            cout << "mythread_1: " << number << endl;    // 输出
        }

        mutex_number.unlock();
    }
    cout << endl << "mythread_1 finish" << endl;     // mythread_1完成
}

// 打印偶数
void add_1_2() {
    while (1) {
        mutex_number.lock();

        if (number >= MAXNUM) {
            mutex_number.unlock();
            break;
        }
        if (number % 2 == 1) {
            number++;
            cout << "mythread_2: " << number << endl;    // 输出
        }

        mutex_number.unlock();
    }
    cout << endl << "mythread_2 finish" << endl;     // mythread_2完成
}

/****************************************************************************/
// 无信号量
void add_2_1() {
    while (1) {
        if (number % 2 == 0) {
            int temp_number = number;       // 读出 number

            temp_number = temp_number + 1;  // 加1

            cout << "mythread_1: " << temp_number << endl;    // 输出

            number = temp_number;           // 写回 number
            if (temp_number == MAXNUM - 1) {
                break;
            }
        }
    }
    cout  << "mythread_1 finish" << endl;     // mythread_1完成
}

void add_2_2() {
    while (1) {
        if (number % 2 == 1) {
            int temp_number = number;       // 读出 number

            temp_number = temp_number + 1;  // 加1

            cout << "mythread_2: " << temp_number << endl;    // 输出

            number = temp_number;           // 写回 number
            if (temp_number == MAXNUM) {
                break;
            }
        }
    }
    cout  << "mythread_2 finish" << endl;     // mythread_2完成
}

/****************************************************************************/

int main() {

    number = 0;

    cout << endl << "Create and Start!" << endl;


    if(1){  // 信号量
        thread mythread_1(add_1_1);
        thread mythread_2(add_1_2);

        mythread_1.join();
        mythread_2.join();
    }else{  // 无信号量
        thread mythread_1(add_2_1);
        thread mythread_2(add_2_2);

        mythread_1.join();
        mythread_2.join();
    }


    cout << endl << "Finish and Exit!" << endl;
    return 0;
}
