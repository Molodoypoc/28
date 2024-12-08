#include <condition_variable>
#include <future>
#include "Merge_sort.h"

bool make_thread = true;

int main(int argc, char* argv[]) {
    srand(0);
    long arr_size = 100000000;
    int* array = new int[arr_size];
    for(long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }
    time_t start, end;
    // многопоточный запуск
    time(&start);
    MergeSort::merge_sort(array, 0, arr_size-1);
    time(&end);

    double seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    for(long i = 0; i < arr_size - 1; i++) { 
        if(array[i] > array[i + 1]) {
            std::cout << "Unsorted" << std::endl;
            break;
        }
    }

    for(long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

    make_thread = false;
    time(&start);
    MergeSort::merge_sort(array, 0, arr_size-1);
    time(&end);
    seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);
    delete[] array;
    return 0;
}

void MergeSort::merge_sort(int array[], int l, int r) {
    if(l >= r) {
        return;
    }
    int m = (l + r) / 2;

    if(make_thread == true && (m - l > 10000)) {
        std::future<void> left_future =
            std::async(std::launch::async, [&] { merge_sort(array, l, m); });
        
        merge_sort(array, m + 1, r);
        try {
            left_future.get();
        } catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        merge(array, l, m, r);
    } else {
        merge_sort(array, l, m);
        merge_sort(array, m + 1, r);

        merge(array, l, m, r);
    }
}

void MergeSort::merge(int array[], int l, int m, int r) {
    int nl = m - l + 1;
    int nr = r - m;

    int* left = new int[nl];
    int* right = new int[nr];

    for(int i = 0; i < nl; i++)
        left[i] = array[l + i];
    for(int j = 0; j < nr; j++)
        right[j] = array[m + 1 + j];

    int i = 0, j = 0;
    int k = l;

    while(i < nl && j < nr) {

        if(left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while(i < nl) {
        array[k] = left[i];
        i++;
        k++;
    }
    while(j < nr) {
        array[k] = right[j];
        j++;
        k++;
    }
    delete[] left;
    delete[] right;
}