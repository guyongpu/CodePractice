//
// Created by yongpu on 2019/9/22.
//

#ifndef CODINGPRACTICE_SORT_H
#define CODINGPRACTICE_SORT_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Sort {
public:
    int test();

private:
    vector<int> array;

    //归并排序----------------------------------------
    void test_MergeSort(vector<int> array);
    vector<int> MergeSort(vector<int> array, int start, int end);
    vector<int> Merge(vector<int> left, vector<int> right);

//冒泡排序----------------------------------------
    void test_BubbleSort(vector<int> array);
    void BubbleSort(vector<int> &array);

//STL排序----------------------------------------
    void test_StlSort(vector<int> array);

//选择排序----------------------------------------
    void test_SelectionSort(vector<int> array);
    void SelectionSort(vector<int> &array);

//快速排序----------------------------------------
    void test_QuickSort(vector<int> array);
    void QuickSort(vector<int> &array, int left, int right);
    int Partition(vector<int> &array, int left, int right);

//插入排序----------------------------------------
    void test_InsertSort(vector<int> array);
    void InsertSort(vector<int> &array);

//堆排序------------------------------------------
    void test_HeapSort(vector<int> array);
    void HeapSort(vector<int> &array);
    void adjust(vector<int> &array, int start, int end);
    void swap(int &a, int &b);
};


#endif //CODINGPRACTICE_SORT_H
