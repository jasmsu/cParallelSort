#ifndef _PSORT_H_
#define _PSORT_H_

#include <iostream>
#include <vector>
#include <omp.h>

using std::vector;

namespace ParallelSort
{
    /**
     * psort - public wrapper for the parallel sort implementation
     * @param array - the structure to sort
     */
    template <class T>
    void psort(vector<T>* array);

    /**
    * quickSort - implementation of parallel sort using quicksort
    * @param array - the structure to sort
    * @param start - index of start of array section to sort
    * @param end - index of end of array section to sort
    */
    template <class T>
    void quickSort(vector<T> * array, int start, int end);
	
    /**
     * quickSortSections - unused implementation of quicksort 
     * parallelization using omp sections instead of tasks
     * @param array - the structure to sort
     * @param start - index of start of array section to sort
     * @param end - index of end of array section to sort
     */
    template<class T>
    void quickSortSection(vector<T> * array, int start, int end);

    /**
     * quickSortSerial - serial quicksort
     * @param array - the structure to sort
     * @param start - index of start of array section to sort
     * @param end - index of end of array section to sort
     */
    template<class T>
    void quickSortSerial(vector<T> * array, int start, int end);

    /**
     * insertionSort - Unused insertion sort that could be used to
     * optimize sorts of small arrays of items, as insertion sort is
     * faster on smaller sized arrays, however testing indicated that
     * sort did not come out faster, so it was not used.
     * @param array - the structure to sorts
     * @param start - index of start of array section to sort
     * @param end - index of end of array section to sort
     */
    template <class T>
    void insertionSort(vector<T> * array, int start, int end);
	
} // end namespace ParallelSort

#include "psort.cpp"
#endif
