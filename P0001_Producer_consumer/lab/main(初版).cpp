#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

struct Fruit {
    string name;
    int num;

    Fruit(string str, int n) {
        name = str;
        num = n;
    }
};

struct BufferFruit {
    string name;
    int num;
    int flag;

    BufferFruit(string str, int n) {
        name = str;
        num = n;
    }
};

void diplay(vector<Fruit> vec_obj) {
    cout << "OutPut the info: " << endl;
    for (int i = 0; i < vec_obj.size(); i++) {
        cout << vec_obj[i].name << "  " << vec_obj[i].num << endl;
    }
}

void InputData(vector<Fruit> &vec_obj) {
    vector<string> vec_str;
    string line;
    line = "a 12 b 3 c 4 l 1";
//    line = "c 9 b 12 b 3";
//    line = "a 12 a 3 b 4 l 5 c 20";
//    getline(cin, line);
    cout << line << endl;
    stringstream ss(line);
    string str_out;
    while (getline(ss, str_out, ' ')) {
        vec_str.push_back(str_out);
    }

    //存入到str_vec中
    for (int i = 0; i < vec_str.size(); i = i + 2) {
        if (vec_str[i] == "a")
            vec_str[i] = "apple";
        else if (vec_str[i] == "b")
            vec_str[i] = "banana";
        else if (vec_str[i] == "c")
            vec_str[i] = "cherry";
        else if (vec_str[i] == "l")
            vec_str[i] = "lemon";
        Fruit a_info(vec_str[i], stoi(vec_str[i + 1])); //每个a_info是一组
        vec_obj.push_back(a_info);
    }

    //输出数据
//    diplay(vec_obj);
}

// FruitBuffer为缓存区
// FruitBuffer[0]表示当前已生产的编号，当FruitBuffer[0]=1时，表示fruit_vec已全部处理，
// FruitBuffer[1]用于存放生产的水果。
int FruitBuffer[2] = {0};
string FruitName = "NULL";

// TotalSumBuffer用于计数处理的总数
// FruitBuffer[1]-apple; FruitBuffer[1]-banana;
// FruitBuffer[3]-cheery; FruitBuffer[4]-lemon;
int TotalSumBuffer[5] = {0};

sem_t p_sem_a, p_sem_b, p_sem_c, p_sem_l;       //生产者锁
sem_t c_sem_a, c_sem_b, c_sem_c, c_sem_l;       //消费者所
sem_t sh_sem;   //共享变量的锁



//添加产品到仓库
int add_to_lib(string name, int num) {
    if (FruitBuffer[1] != 0)
        return 0;
    FruitBuffer[1] = FruitBuffer[1] + num;                  //仓库中的产品增加num
    FruitName = name;
    return FruitBuffer[1];
}

//从仓库中取出产品
int sub_from_lib() {
    int sum = FruitBuffer[1];          //仓库中的产品数量全部消费
    FruitBuffer[1] = 0;
    FruitName = "NULL";
    return sum;
}

//生产者线程
void *productor(void *studata) {
    vector<Fruit> *fruit_vec = (vector<Fruit> *) studata;
    int FruitSize = (*fruit_vec).size();
    for (int i = 0; i < FruitSize;) {
//        cout << (*fruit_vec)[i].name << " " << (*fruit_vec)[i].num  << endl;
        string name = (*fruit_vec)[i].name;
        int num = (*fruit_vec)[i].num;
        if (name == "apple") {
            sem_wait(&p_sem_a);   //生产信号量减一
            sem_wait(&sh_sem);  //这个信号量是用来互斥的

            int total_num = add_to_lib(name, num);
            if (total_num == 0) { ;
            } else {
                i++;
                if (i == FruitSize)
                    FruitBuffer[0] = 1;
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_a);   //消费信号量加一
            } else
                sem_post(&p_sem_a);
        } else if (name == "banana") {
            sem_wait(&p_sem_b);   //生产信号量减一
            sem_wait(&sh_sem);  //这个信号量是用来互斥的

            int total_num = add_to_lib(name, num);
            if (total_num == 0) { ;
            } else {
                i++;
                if (i == FruitSize)
                    FruitBuffer[0] = 1;
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_b);   //消费信号量加一
            } else
                sem_post(&p_sem_b);
        } else if (name == "cherry") {
            sem_wait(&p_sem_c);   //生产信号量减一
            sem_wait(&sh_sem);  //这个信号量是用来互斥的

            int total_num = add_to_lib(name, num);
            if (total_num == 0) { ;
            } else {
                i++;
                if (i == FruitSize)
                    FruitBuffer[0] = 1;
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_c);   //消费信号量加一
            } else
                sem_post(&p_sem_c);
        } else if (name == "lemon") {
            sem_wait(&p_sem_l);   //生产信号量减一
            sem_wait(&sh_sem);  //这个信号量是用来互斥的

            int total_num = add_to_lib(name, num);
            if (total_num == 0) { ;
            } else {
                i++;
                if (i == FruitSize)
                    FruitBuffer[0] = 1;
                cout << "New " << name << " " << num << endl;
            }
            sem_post(&sh_sem);

            if (total_num != 0) {
                sem_post(&c_sem_l);   //消费信号量加一
            } else
                sem_post(&p_sem_l);

        }
    }
}
//apple 消费者线程
void *consumer_apple(void *arg) {

    while (1) {
        sem_wait(&sh_sem);  //减1
        if (FruitBuffer[0] == 1 && FruitBuffer[1] == 0) {
            sem_post(&sh_sem);  //加1
            break;
        } else {
            if (FruitName != "apple") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_a);   //消费者信号量减一
        sem_wait(&sh_sem);  //减1

        int total_num = sub_from_lib();
        TotalSumBuffer[1] += total_num;
        cout << "Thread 1 consume " << total_num << endl;
        cout << "Delete apple" << endl << endl;

        sem_post(&sh_sem);  //加1
        sem_post(&p_sem_a);   //生产者信号量加一
    }
//    cout << "consumer_apple finished!" << endl;
}

//banana 消费者线程
void *consumer_banana(void *arg) {

    while (1) {
        sem_wait(&sh_sem);  //减1
        if (FruitBuffer[0] == 1 && FruitBuffer[1] == 0) {
            sem_post(&sh_sem);  //加1
            break;
        } else {
            if (FruitName != "banana") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_b);   //消费者信号量减一
        sem_wait(&sh_sem);  //减1

        int total_num = sub_from_lib();
        TotalSumBuffer[2] += total_num;
        cout << "Thread 2 consume " << total_num << endl;
        cout << "Delete banana" << endl << endl;

        sem_post(&sh_sem);  //加1
        sem_post(&p_sem_b);   //生产者信号量加一
    }
//    cout << "consumer_banana finished!" << endl;
}

//cheery 消费者线程
void *consumer_cheery(void *arg) {
    while (1) {
        sem_wait(&sh_sem);  //减1
        if (FruitBuffer[0] == 1 && FruitBuffer[1] == 0) {
            sem_post(&sh_sem);  //加1
            break;
        } else {
            cout << FruitName << endl;
            if (FruitName != "cherry") {
                sem_post(&sh_sem);  //加1
                continue;
            } else{
                sem_post(&sh_sem);  //加1
            }
        }

        sem_wait(&c_sem_c);   //消费者信号量减一
        sem_wait(&sh_sem);  //减1

        int total_num = sub_from_lib();
        TotalSumBuffer[3] += total_num;
        cout << "Thread 3 consume " << total_num << endl;
        cout << "Delete cheery" << endl << endl;

        sem_post(&sh_sem);  //加1
        sem_post(&p_sem_c);   //生产者信号量加一
    }
//    cout << "consumer_cheery finished!" << endl;
}

//lemon 消费者线程
void *consumer_lemon(void *arg) {

    while (1) {
        sem_wait(&sh_sem);  //减1
        if (FruitBuffer[0] == 1 && FruitBuffer[1] == 0) {
            sem_post(&sh_sem);  //加1
            break;
        } else {
            if (FruitName != "lemon") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_l);   //消费者信号量减一
        sem_wait(&sh_sem);  //减1

        int total_num = sub_from_lib();
        TotalSumBuffer[4] += total_num;
        cout << "Thread 4 consume " << total_num << endl;
        cout << "Delete lemon" << endl << endl;

        sem_post(&sh_sem);  //加1
        sem_post(&p_sem_l);   //生产者信号量加一
    }
//    cout << "consumer_lemon finished!" << endl;
}

void init_sem() {
    sem_init(&p_sem_a, 0, 1);   //生产者信号量为1
    sem_init(&c_sem_a, 0, 0);   //消费者信号量为0

    sem_init(&p_sem_b, 0, 1);   //生产者信号量为1
    sem_init(&c_sem_b, 0, 0);   //消费者信号量为0

    sem_init(&p_sem_c, 0, 1);   //生产者信号量为1
    sem_init(&c_sem_c, 0, 0);   //消费者信号量为0

    sem_init(&p_sem_l, 0, 1);   //生产者信号量为1
    sem_init(&c_sem_l, 0, 0);   //消费者信号量为0

    sem_init(&sh_sem, 0, 1);
}


int main() {

    vector<Fruit> fruit_vec;

    InputData(fruit_vec);
    pthread_t tid_p;
    pthread_t tid_c_a, tid_c_b, tid_c_c, tid_c_l;

    init_sem();

    pthread_create(&tid_c_a, NULL, consumer_apple, NULL);
    cout << "New Thread 1" << endl;
    pthread_create(&tid_c_b, NULL, consumer_banana, NULL);
    cout << "New Thread 2" << endl;
    pthread_create(&tid_c_c, NULL, consumer_cheery, NULL);
    cout << "New Thread 3" << endl;
    pthread_create(&tid_c_l, NULL, consumer_lemon, NULL);
    cout << "New Thread 4" << endl << endl;

    pthread_create(&tid_p, NULL, productor, (void *) &fruit_vec);


    while (1) {
        sem_wait(&sh_sem);      //信号量减1，访问FruitBuffer，查询数据是否全部处理
        if ((FruitBuffer[0] == 1) && (FruitBuffer[1] != 0)) {    //所有数据生产结束且全部被消费
            sem_post(&sh_sem);  //释放信号量，结束探测
            break;              //已全部处理完，准备结束所有线程
        } else {
            sem_post(&sh_sem);  //释放信号量，继续探测，等待处理结束
        }
    }

    pthread_join(tid_p, NULL);
    pthread_join(tid_c_a, NULL);    //线程1结束
    cout << "Delete Thread 1 consume " << TotalSumBuffer[1] << endl;

    pthread_join(tid_c_b, NULL);    //线程2结束
    cout << "Delete Thread 2 consume " << TotalSumBuffer[2] << endl;

    pthread_join(tid_c_c, NULL);    //线程3结束
    cout << "Delete Thread 3 consume " << TotalSumBuffer[3] << endl;
    pthread_join(tid_c_l, NULL);    //线程4结束
    cout << "Delete Thread 4 consume " << TotalSumBuffer[4] << endl;

    return 0;
}
