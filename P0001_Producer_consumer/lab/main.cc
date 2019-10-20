#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

/**************程序中用到的结构体/类**********************/
struct Fruit {              //输入数据的记录
    string name;            //水果名
    int num;                //水果数量

    Fruit(string str, int n) {
        name = str;
        num = n;
    }
};

//BufferFruit类，FruitBuffer为缓存区仓库
class BufferFruit {
private:
    string name;            //当前仓库中水果的名字
    int num;                //当前仓库中水果的数量
    bool finish_flag;       //finish_flag表示fruit_vec已全部处理
    int TotalSumBuffer[5];  //TotalSumBuffer用于记录处理的总数，[1]-apple， [2]-banana，[3]-cheery， [4]-lemon;
public:
    //构造函数
    BufferFruit(string str = "NULL", int n = 0, bool flag = false, int initnum = 0) {
        name = str;
        num = n;
        finish_flag = false;
        memset(TotalSumBuffer, initnum, sizeof(TotalSumBuffer));
    }

    //设置水果名
    void SetBufferName(string str) {
        name = str;
    }

    //获取水果名
    string GetBufferName() {
        return name;
    }

    //设置当前仓库水果数量
    void SetBufferNum(int n) {
        num = n;
    }

    //获取当前仓库水果数量
    int GetBufferNum() {
        return num;
    }

    //设置是否所有输入处理结果，true为全部处理完毕，false表示还有输入没有处理
    void SetBufferFlag(bool flag) {
        finish_flag = flag;
    }

    //获取输入处理结果，true为全部处理完毕，false表示还有输入没有处理
    bool GetBufferFlag() {
        return finish_flag;
    }

    //设置某种水果的处理总数，[1]-apple， [2]-banana，[3]-cheery， [4]-lemon;
    void SetBufferTotal(int index, int num) {
        TotalSumBuffer[index] = num;
    }

    //获取每种水果处理总数
    int GetBufferTotal(int index) {
        return TotalSumBuffer[index];
    }

};

/**************程序中用到的变量**********************/
class BufferFruit buffer;

sem_t p_sem_a, p_sem_b, p_sem_c, p_sem_l;       //生产者锁
sem_t c_sem_a, c_sem_b, c_sem_c, c_sem_l;       //消费者锁
sem_t sh_sem;                                   //共享变量buffer的锁


/**************程序中用到函数**********************/
void diplay(vector<Fruit> vec_obj) {
    cout << "OutPut the info: " << endl;
    for (int i = 0; i < vec_obj.size(); i++) {
        cout << vec_obj[i].name << "  " << vec_obj[i].num << endl;
    }
}

void InputData(vector<Fruit> &vec_obj) {
    vector<string> vec_str;
    string line;
//    line = "a 12 b 3 c 4 l 1";
//    line = "c 9 b 12 b 3";
//    line = "a 12 a 3 b 4 l 5 c 20";
    getline(cin, line);
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


//添加产品到仓库
int add_to_lib(string name, int num) {
    if (buffer.GetBufferNum() != 0)
        return 0;
    int temp = buffer.GetBufferNum() + num;
    buffer.SetBufferNum(temp);                  //仓库中的产品增加num
    buffer.SetBufferName(name);
    return buffer.GetBufferNum();
}

//从仓库中取出产品
int sub_from_lib() {
    int sum = buffer.GetBufferNum();          //仓库中的产品数量全部消费
    buffer.SetBufferNum(0);
    buffer.SetBufferName("NULL");
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
        if (name == "apple") {          //生产apple
            sem_wait(&p_sem_a);         //生产信号量减一
            sem_wait(&sh_sem);          //这个信号量是用来互斥的

            //开始生产 apple
            int total_num = add_to_lib(name, num);
            if (total_num != 0) {
                i++;
                if (i == FruitSize) {
                    buffer.SetBufferFlag(true);
                }
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_a);     //消费信号量加一
            } else
                sem_post(&p_sem_a);
        } else if (name == "banana") {  //生产banana
            sem_wait(&p_sem_b);         //生产信号量减一
            sem_wait(&sh_sem);          //这个信号量是用来互斥的

            //开始生产 banana
            int total_num = add_to_lib(name, num);
            if (total_num != 0) {
                i++;
                if (i == FruitSize) {
                    buffer.SetBufferFlag(true);
                }
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_b);     //消费信号量加一
            } else
                sem_post(&p_sem_b);
        } else if (name == "cherry") {  //生产cherry
            sem_wait(&p_sem_c);         //生产信号量减一
            sem_wait(&sh_sem);          //这个信号量是用来互斥的

            //开始生产 cherry
            int total_num = add_to_lib(name, num);
            if (total_num != 0) {
                i++;
                if (i == FruitSize) {
                    buffer.SetBufferFlag(true);
                }
                cout << "New " << name << " " << num << endl;
            }

            sem_post(&sh_sem);
            if (total_num != 0) {
                sem_post(&c_sem_c);     //消费信号量加一
            } else
                sem_post(&p_sem_c);
        } else if (name == "lemon") {   //生产lemon
            sem_wait(&p_sem_l);         //生产信号量减一
            sem_wait(&sh_sem);          //这个信号量是用来互斥的

            //开始生产 lemon
            int total_num = add_to_lib(name, num);
            if (total_num != 0) {
                i++;
                if (i == FruitSize) {
                    buffer.SetBufferFlag(true);
                }
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
        sem_wait(&sh_sem);          //减1
        if (buffer.GetBufferFlag() == true && buffer.GetBufferNum() == 0) {
            sem_post(&sh_sem);      //加1
            break;
        } else {
            if (buffer.GetBufferName() != "apple") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_a);         //消费者信号量减一
        sem_wait(&sh_sem);          //减1

        int total_num = sub_from_lib();
        int temp = buffer.GetBufferTotal(1) + total_num;
        buffer.SetBufferTotal(1, temp);
        cout << "Thread 1 consume " << total_num << endl;
        cout << "Delete apple" << endl;

        sem_post(&sh_sem);          //加1
        sem_post(&p_sem_a);         //生产者信号量加一
    }
//    cout << "consumer_apple finished!" << endl;
}

//banana 消费者线程
void *consumer_banana(void *arg) {

    while (1) {
        sem_wait(&sh_sem);  //减1
        if (buffer.GetBufferFlag() == true && buffer.GetBufferNum() == 0) {
            sem_post(&sh_sem);      //加1
            break;
        } else {
            if (buffer.GetBufferName() != "banana") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_b);         //消费者信号量减一
        sem_wait(&sh_sem);          //减1

        int total_num = sub_from_lib();
        int temp = buffer.GetBufferTotal(2) + total_num;
        buffer.SetBufferTotal(2, temp);
        cout << "Thread 2 consume " << total_num << endl;
        cout << "Delete banana" << endl;

        sem_post(&sh_sem);          //加1
        sem_post(&p_sem_b);         //生产者信号量加一
    }
//    cout << "consumer_banana finished!" << endl;
}

//cherry 消费者线程
void *consumer_cheery(void *arg) {
    while (1) {
        sem_wait(&sh_sem);          //减1
        if (buffer.GetBufferFlag() == true && buffer.GetBufferNum() == 0) {
            sem_post(&sh_sem);      //加1
            break;
        } else {
            if (buffer.GetBufferName() != "cherry") {
                sem_post(&sh_sem);  //加1
                continue;
            } else {
                sem_post(&sh_sem);  //加1
            }
        }

        sem_wait(&c_sem_c);         //消费者信号量减一
        sem_wait(&sh_sem);          //减1

        int total_num = sub_from_lib();
        int temp = buffer.GetBufferTotal(3) + total_num;
        buffer.SetBufferTotal(3, temp);
        cout << "Thread 3 consume " << total_num << endl;
        cout << "Delete cheery" << endl;

        sem_post(&sh_sem);          //加1
        sem_post(&p_sem_c);         //生产者信号量加一
    }
//    cout << "consumer_cheery finished!" << endl;
}

//lemon 消费者线程
void *consumer_lemon(void *arg) {

    while (1) {
        sem_wait(&sh_sem);          //减1
        if (buffer.GetBufferFlag() == true && buffer.GetBufferNum() == 0) {
            sem_post(&sh_sem);      //加1
            break;
        } else {
            if (buffer.GetBufferName() != "lemon") {
                sem_post(&sh_sem);  //加1
                continue;
            } else
                sem_post(&sh_sem);  //加1
        }

        sem_wait(&c_sem_l);         //消费者信号量减一
        sem_wait(&sh_sem);          //减1

        int total_num = sub_from_lib();
        int temp = buffer.GetBufferTotal(4) + total_num;
        buffer.SetBufferTotal(4, temp);
        cout << "Thread 4 consume " << total_num << endl;
        cout << "Delete lemon" << endl;

        sem_post(&sh_sem);          //加1
        sem_post(&p_sem_l);         //生产者信号量加一
    }
//    cout << "consumer_lemon finished!" << endl;
}

void init_sem() {
    sem_init(&p_sem_a, 0, 1);   //apple 生产者信号量为1
    sem_init(&c_sem_a, 0, 0);   //apple 消费者信号量为0

    sem_init(&p_sem_b, 0, 1);   //banana 生产者信号量为1
    sem_init(&c_sem_b, 0, 0);   //banana 消费者信号量为0

    sem_init(&p_sem_c, 0, 1);   //cherry 生产者信号量为1
    sem_init(&c_sem_c, 0, 0);   //cherry 消费者信号量为0

    sem_init(&p_sem_l, 0, 1);   //lemon 生产者信号量为1
    sem_init(&c_sem_l, 0, 0);   //lemon 消费者信号量为0

    sem_init(&sh_sem, 0, 1);    //buffer共享变量信号量为1
}

/*******************主函数***********************/
//编译命令：g++ main.cc -o main -pthread
//运行命令：./main a 12 a 3 b 4 l 5 c 20

int main(int argc, char* argv[]) {
    vector<Fruit> fruit_vec;    //输入数据记录

    //输入数据
    if(1){                      //采用命令行方式输入数据
      if(argc < 3 || argc%2 ==0){
        cout << "Error!" << endl;
        return 0;
      }
      string name;
      for(int i = 1; i < argc; i = i + 2){
        if (string(argv[i]) == "a")
            name = "apple";
        else if (string(argv[i]) == "b")
            name = "banana";
        else if (string(argv[i]) == "c")
            name = "cherry";
        else if (string(argv[i]) == "l")
            name = "lemon";
        else{
          cout << "Error!" << endl;
          return 0;
        }
        int argv_num;
        try{
          argv_num = stoi(argv[i + 1]);
        }
        catch(...){
          cout << "Error!" << endl;
          return 0;
        }
        Fruit a_info(name, argv_num); //每个a_info是一组
        fruit_vec.push_back(a_info);
      }
    }
    else{
      InputData(fruit_vec);   //采用直接输入方式输入数据
    }


    pthread_t tid_p;
    pthread_t tid_c_a, tid_c_b, tid_c_c, tid_c_l;

    init_sem();                 //初始化信号量

    pthread_create(&tid_c_a, NULL, consumer_apple, NULL);   //创建线程1
    cout << "New Thread 1" << endl;
    pthread_create(&tid_c_b, NULL, consumer_banana, NULL);  //创建线程2
    cout << "New Thread 2" << endl;
    pthread_create(&tid_c_c, NULL, consumer_cheery, NULL);  //创建线程3
    cout << "New Thread 3" << endl;
    pthread_create(&tid_c_l, NULL, consumer_lemon, NULL);   //创建线程4
    cout << "New Thread 4" << endl;

    pthread_create(&tid_p, NULL, productor, (void *) &fruit_vec);   //创建生产者线程


    //当数据全部生产 且 消费者全部消费后，结束各个线程
    while (1) {
        sem_wait(&sh_sem);      //信号量减1，访问FruitBuffer，查询数据是否全部处理
        if (buffer.GetBufferFlag() == true && buffer.GetBufferNum() == 0) {    //所有数据生产结束且全部被消费
            sem_post(&sh_sem);  //释放信号量，结束探测
            break;              //已全部处理完，准备结束所有线程
        } else {
            sem_post(&sh_sem);  //释放信号量，继续探测，等待处理结束
        }
    }

    pthread_join(tid_p, NULL);
    pthread_join(tid_c_a, NULL);    //线程1结束
    cout << "Delete Thread 1 consume " << buffer.GetBufferTotal(1) << endl;

    pthread_join(tid_c_b, NULL);    //线程2结束
    cout << "Delete Thread 2 consume " << buffer.GetBufferTotal(2) << endl;

    pthread_join(tid_c_c, NULL);    //线程3结束
    cout << "Delete Thread 3 consume " << buffer.GetBufferTotal(3) << endl;
    pthread_join(tid_c_l, NULL);    //线程4结束
    cout << "Delete Thread 4 consume " << buffer.GetBufferTotal(4) << endl;

    return 0;
}
