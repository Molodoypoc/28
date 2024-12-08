#pragma once
#include <iostream>

extern bool make_thread;

namespace MergeSort
{
    void merge_sort(int array[], int l, int r);
    void merge (int array[], int l, int m, int r);
}