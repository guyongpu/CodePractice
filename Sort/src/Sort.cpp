//
// Created by yongpu on 2019/9/22.
//

#include "Sort.h"


//-----------------------------------------------
int Sort::test() {
    vector<int> array_test = {3, 5, 9, 1, 7, 6, 8, 2, 0, 4};
    vector<int> array = array_test;

    test_MergeSort(array);      //归并排序
    test_BubbleSort(array);     //冒泡排序
    test_StlSort(array);        //STL快速排序
    test_SelectionSort(array);  //选择排序
    test_QuickSort(array);      //快速排序
    test_InsertSort(array);     //插入排序
    test_HeapSort(array);       //堆排序
    return 0;
}

/*-------------------------------------------------*/
//堆排序
void Sort::test_HeapSort(vector<int> array) {
    int len = array.size();
    HeapSort(array);
    cout << "HeapSort      : ";
    for (int i = 0; i < len; i++)
        cout << array[i] << " ";
}

void Sort::HeapSort(vector<int> &array) {
    swap(array[0], array[1]);             //将a[0]放到后面
    int last_node = array.size() - 1;     //最后一个节点
    int parent = (last_node - 1) / 2;     //parent为最后一个父节点,从[0, parent]都是父节点，
    for (int i = parent; i >= 0; i--) {   //倒序遍历所有父节点
        adjust(array, i, last_node);      //调整父节点array[i]
    }

    int len = array.size();
    int swapidx = len - 1;
    for (int i = 0; i < len - 1; i++) {
        swap(array[0], array[swapidx - i]); //将a[0]放到后面
        adjust(array, 0, swapidx - i - 1);  //从array[0]到array[swapidx-i-1]建堆
    }
}

void Sort::adjust(vector<int> &array, int start, int end) {
    int left = 2 * start + 1;
    int right = 2 * start + 2;
    while (left <= end) {       //存在孩子节点

        int Child_max = left;
        if (right <= end && array[right] > array[left])
            Child_max = right;

        if (array[start] < array[Child_max]) {
            swap(array[start], array[Child_max]);
            start = Child_max;
            left = 2 * start + 1;
            right = 2 * start + 2;
        } else                //不需要调整
            break;
    }
}

void Sort::swap(int &a, int &b) {
    int temp = b;
    b = a;
    a = temp;
}

/*-------------------------------------------------*/
//插入排序
void Sort::test_InsertSort(vector<int> array) {
    int len = array.size();
    InsertSort(array);
    cout << "InsertSort    : ";
    for (int i = 0; i < len; i++)
        cout << array[i] << " ";
    cout << endl;
}

void Sort::InsertSort(vector<int> &array) {
    for (int i = 1; i < array.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (array[i] < array[j]) {
                array.insert(array.begin() + j, array[i]);  //插在array[j]前面
                array.erase(array.begin() + i + 1);         //删除原来的array[i],注意，由于先插入，所以要+1
                break;
            }
        }
    }
}

/*-------------------------------------------------*/
//快速排序
void Sort::test_QuickSort(vector<int> array) {
    int len = array.size();
    QuickSort(array, 0, len - 1);
    cout << "QuickSort     : ";
    for (int i = 0; i < len; i++)
        cout << array[i] << " ";
    cout << endl;
}

void Sort::QuickSort(vector<int> &array, int left, int right) {
    if (left < right) {
        int pos = Partition(array, left, right);
        QuickSort(array, left, pos - 1);         //对[left, pos-1]进行快排
        QuickSort(array, pos + 1, right);        //对[pos + 1, right]进行快排
    }
}

int Sort::Partition(vector<int> &array, int left, int right) {
    int temp = array[left];   //将array[left]存放至temp中
    while (left < right) {
        //必须先从right开始
        while (left < right) {
            if (array[right] > temp)
                right--;
            else {
                array[left] = array[right];
                break;
            }
        }
        //然后再从左边开始
        while (left < right) {
            if (array[left] < temp)
                left++;
            else {
                array[right] = array[left];
                break;
            }
        }
    }
    array[left] = temp;
    return left;
}

/*-------------------------------------------------*/
//选择排序
//以升序为例，每次将最小值放到前面
void Sort::test_SelectionSort(vector<int> array) {
    int len = array.size();
    SelectionSort(array);
    cout << "SelectionSort : ";
    for (int i = 0; i < len; i++)
        cout << array[i] << " ";
    cout << endl;
}

void Sort::SelectionSort(vector<int> &array) {
    int len = array.size();
    for (int i = 0; i < len; i++) {
        int Minpos = i;
        for (int j = i + 1; j < len; j++) {
            if (array[j] < array[Minpos]) {
                Minpos = j;
            }
        }
        swap(array[i], array[Minpos]);
    }
}

/*-------------------------------------------------*/
//STL内置快速排序
bool cmp_stl(int a, int b) {
    return a < b;
}

void Sort::test_StlSort(vector<int> array) {
    int len = array.size();
    vector<int> result = array;
    sort(result.begin(), result.end(), cmp_stl);
    cout << "StlSort       : ";
    for (int i = 0; i < len; i++)
        cout << result[i] << " ";
    cout << endl;
}

/*-------------------------------------------------*/
//归并排序
void Sort::test_MergeSort(vector<int> array) {
    int len = array.size();
    vector<int> result = MergeSort(array, 0, len - 1);
    cout << "MergeSort     : ";
    for (int i = 0; i < len; i++)
        cout << result[i] << " ";
    cout << endl;
}

//功能：将array进行归并排序，并返回排序结果MergeArray
vector<int> Sort::MergeSort(vector<int> array, int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;                          //计算中，将array分为两部分
        vector<int> left = MergeSort(array, start, mid);      //左边部分归并排序,[start, mid]
        vector<int> right = MergeSort(array, mid + 1, end);   //右边部分归并排序,[mid+1, end]

        vector<int> MergeArray = Merge(left, right);          //合并left和right -> MergeArray
        //返回MergeArray
        return MergeArray;
    } else {
        vector<int> MergeArray = {array[start]};
        return MergeArray;
    }
}

//功能：将left和right两个有序序列合并成一个有序序列result
vector<int> Sort::Merge(vector<int> left, vector<int> right) {
    vector<int> result;
    int pos_l = 0;
    int pos_r = 0;
    //逐个合并
    while ((pos_l < left.size()) && (pos_r < right.size())) {
        if (left[pos_l] < right[pos_r]) {
            result.push_back(left[pos_l]);
            pos_l++;
        } else {
            result.push_back(right[pos_r]);
            pos_r++;
        }
    }
    //right中还有元素未合并
    while (pos_r < right.size()) {
        result.push_back(right[pos_r]);
        pos_r++;
    }
    //left中还有元素未合并
    while (pos_l < left.size()) {
        result.push_back(left[pos_l]);
        pos_l++;
    }
    return result;
}

/*-------------------------------------------------*/
//冒泡排序
//以升序为例，每轮循环会把最小的值放到前面
void Sort::test_BubbleSort(vector<int> array) {
    int len = array.size();
    BubbleSort(array);
    cout << "BubbleSort    : ";
    for (int i = 0; i < len; i++)
        cout << array[i] << " ";
    cout << endl;
}

void Sort::BubbleSort(vector<int> &array) {
    int len = array.size();
    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            if (array[i] > array[j]) {
                swap(array[i], array[j]);
            }
        }
    }
}

/*-------------------------------------------------*/