# 生产者-消费者程序

# 文件说明
main.cc ---------- 程序源代码  
main-------------- 执行程序  
makefile.sh--------编译脚本  
readme.md----------说明文件

# 使用说明
编译命令：sh makefile.sh   或   g++ main.cc -o main -pthread  
运行命令：./main a 12 a 3 b 4 l 5 c 20

# 测试样例
## 样例1
样例说明：只有四种水果  
输入：  ./main a 12 b 3 c 4 l 1  
输出：  
New Thread 1  
New Thread 2  
New Thread 3  
New Thread 4  
New apple 12  
Thread 1 consume 12  
Delete apple  
New banana 3  
Thread 2 consume 3  
Delete banana  
New cherry 4  
Thread 3 consume 4  
Delete cheery  
New lemon 1  
Thread 4 consume 1  
Delete lemon  
Delete Thread 1 consume 12  
Delete Thread 2 consume 3  
Delete Thread 3 consume 4  
Delete Thread 4 consume 1  


## 样例2
样例说明：当输入数量不足4种水果  
输入： ./main a 6 b 4 c 7  
输出：  
New Thread 1  
New Thread 2  
New Thread 3  
New Thread 4  
New apple 6  
Thread 1 consume 6  
Delete apple  
New banana 4  
Thread 2 consume 4  
Delete banana  
New cherry 7  
Thread 3 consume 7  
Delete cheery  
Delete Thread 1 consume 6  
Delete Thread 2 consume 4  
Delete Thread 3 consume 7  
Delete Thread 4 consume 0  

## 样例3
样例说明：当输入水果有重复  
输入： ./main a 12 b 3 c 4 l 5 c 20
输出：
New Thread 1  
New Thread 2  
New Thread 3  
New Thread 4  
New apple 12  
Thread 1 consume 12  
Delete apple  
New banana 3  
Thread 2 consume 3  
Delete banana  
New cherry 4  
Thread 3 consume 4  
Delete cheery  
New lemon 5  
Thread 4 consume 5  
Delete lemon  
New cherry 20  
Thread 3 consume 20  
Delete cheery  
Delete Thread 1 consume 12  
Delete Thread 2 consume 3  
Delete Thread 3 consume 24  
Delete Thread 4 consume 5  

## 样例4
样例说明：当输入数据有错误  
输入： ./main  
输出：Error\!

输入： ./main a     
输出：Error\!  

输入： ./main a 12 b c  
输出：Error\!  

输入： ./main a 12 b 5 c  
输出：Error\!  
