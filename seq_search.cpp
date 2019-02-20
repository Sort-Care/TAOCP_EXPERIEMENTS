/********
 * Experiment of TAOCP vol.3 p398, Quicker sequential search.
 *
 * It is supposed that a loop within each pass, testing more
 * than 1 elements can speed up the running time, because of
 * the reduction of number of i+=1 instructions. However,
 * doing so will introduce more bounds checks in the loop,
 * (maybe there is a way to reduce number or bound checks),
 * which can have negative effect when the number of elements
 * tested for one iteration gets big. So here comes the
 * experiment to see the relationship between the speed up
 * , the size of array, and the number of elements tested
 * per iteration.
 *
 * Tried with array size of 2^20, Skip-2 achieved around 35%
 * running time reduction when the key is not present in the
 * array. Skip-3 and skip-5 both achieved around 50% reduction.
 * Skip-4 seemed to be the optimal here.
 *
 * Results: time unit(us), array size 2^20
 * |----------+---------+-----------+-----------|
 * | Skip Num | Skip RT | Normal RT | Reduction |
 * |----------+---------+-----------+-----------|
 * |        2 |    1430 |      2310 |     38.1% |
 * |        3 |    1173 |      2257 |     48.0% |
 * |        2 |    1459 |      2282 |     36.1% |
 * |        3 |    1227 |      2243 |     45.3% |
 * |        4 |    1055 |      2240 |     52.9% |
 * |        4 |    1092 |      2298 |     52.5% |
 * |        4 |    1072 |      2216 |     51.6% |
 * |        5 |    1440 |      2201 |     34.6% |
 * |        5 |    1436 |      2224 |     35.4% |
 * |----------+---------+-----------+-----------|
 *
 * TODO:
 * Plot a chart where the x-axis is array size, and the y-axis
 * is number of skips per loop range from [2, 6]
 *
 *
 ************/


#include <iostream>
#include <chrono>
#include <cmath>

#define LOG(X) std::cout << X << std::endl
const int ARR_SIZE = std::pow(2, 20);

int seq_find_S(int *arr, int key);
int seq_find_Q_2(int *arr, int key);
int seq_find_Q_5(int *arr, int key);
int skip_find(int *arr, int arr_size, int key, int skip);

int main(int argc, char const *argv[])
{
    // initialize the array for searching
    int arr[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE ; i++){
        arr[i] = i;
    }
    int key = std::pow(2, 21);

    LOG("Doing Sequential Search Runnint Time test:");
    // ===== PROGRAM S ========
    LOG("First: program S");
    auto s_start = std::chrono::high_resolution_clock::now();
    seq_find_S(arr, key);
    auto s_stop = std::chrono::high_resolution_clock::now();
    auto s_dura = std::chrono::duration_cast<std::chrono::microseconds>(s_stop - s_start);
    LOG(s_dura.count());

    // ===== PROGRAM Q ========
    LOG("Second: maybe faster program Q");
    auto q_start = std::chrono::high_resolution_clock::now();
    seq_find_Q_5(arr, key);
    auto q_stop = std::chrono::high_resolution_clock::now();
    auto q_dura = std::chrono::duration_cast<std::chrono::microseconds>(q_stop - q_start);
    LOG(q_dura.count());

    return 0;
}

int seq_find_S(int *arr, int key){
    for (int i = 0; i < ARR_SIZE; i++){
        if (arr[i] == key) return i;
    }
    return -1;
}

int seq_find_Q_2(int *arr, int key){
    for(int i = 0; i < ARR_SIZE; i += 2){
        if (arr[i] == key) return i;
        if (i+1 < ARR_SIZE && arr[i+1] == key) return i+1;
    }
    return -1;
}

int seq_find_Q_5(int *arr, int key){
    for(int i = 0; i < ARR_SIZE; i += 4){
        if (arr[i] == key) return i;
        if (i+1 < ARR_SIZE && arr[i+1] == key) return i+1;
        if (i+2 < ARR_SIZE && arr[i+2] == key) return i+2;
        if (i+3 < ARR_SIZE && arr[i+2] == key) return i+3;
        //if (i+4 < ARR_SIZE && arr[i+2] == key) return i+4;
    }
    return -1;
}


// turns out this is slower then program S because the inner loop j++ instruction!!
int skip_find(int *arr,
              int arr_size,
              int key,
              int skip){
  for (int i = 0; i < arr_size; i += skip){
    // test arr[i] to arr[i+skip-1] but need to test range
    for (int j = 0; j < skip ; j++){
      if (i+j < arr_size && arr[i+j] == key) return i+j;
    }
  }
  return -1;
}
