#include "psort.h"
#include <ctime>

/**
 * psort - public wrapper for the parallel sort implementation
 * @param array - the structure to sort
 */
template <class T>
void ParallelSort::psort(vector<T>* array)
{
    int size = array->size();
    int threads;
	//If size is one, already sorted.
    if(size < 2)
        return;

	//If size is less than 5000, use serial quicksort
	//to avoid overhead.
	if(size <= 5000)
    {
        quickSortSerial(array, 0, size - 1);
    }
    else
    {
        #pragma omp parallel default(shared) num_threads(16)
        {
            #pragma omp single nowait
            quickSort(array, 0, size - 1);
        }
    }
}

/**
 * quickSort - implementation of parallel sort using quicksort
 * @param array - the structure to sort
 * @param start - index of start of array section to sort
 * @param end - index of end of array section to sort
 */
template <class T>
void ParallelSort::quickSort(vector<T> * array, int start, int end)
{
    int i = start, j = end, middle = (start + end)/2;
    T temp;
	
	//Select median of three for pivot to avoid 
	//worst case pivot.
    if((*array)[start] > (*array)[middle])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[middle];
        (*array)[middle] = temp;
    }
    if((*array)[start] > (*array)[end])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[end];
        (*array)[end] = temp;
    }
    if((*array)[middle] > (*array)[end])
    {
        temp = (*array)[middle];
        (*array)[middle] = (*array)[end];
        (*array)[end] = temp;
    }

    T pivot = (*array)[middle];
    
	//Organize sections
    while(i <= j)
    {
        while((*array)[i] < pivot)
            i++; 
        while((*array)[j] > pivot)
            j--;

        if(i <= j)
        {
            temp = (*array)[i];
            (*array)[i] = (*array)[j];
            (*array)[j] = temp;
            i++;
            j--;
        }
    }
	
	//Parallelization of first section using omp task
    #pragma omp task
    if(start < j)
    {	
		//If remaining sections are less than 10000, use serial
		//quicksort to avoid overhead.
		if(j - start < 10000)
            quickSortSerial(array, start, j);
        else
            quickSort(array, start, j);
    }
	
	//Parallelization of second section using omp task
    #pragma omp task
    if(i < end)
    {
		//If remaining sections are less than 10000, use serial
		//quicksort to avoid overhead.
		if(end - i < 10000)
            quickSortSerial(array, i, end);
        else
            quickSort(array, i, end);
    }
}

/**
 * quickSortSections - unused implementation of quicksort 
 * parallelization using omp sections instead of tasks
 * @param array - the structure to sort
 * @param start - index of start of array section to sort
 * @param end - index of end of array section to sort
 */
template <class T>
void ParallelSort::quickSortSection(vector<T> * array, int start, int end)
{
    int i = start, j = end, middle = (start + end)/2;
    T temp;
    
	//Select median of three for pivot to avoid 
	//worst case pivot.
    if((*array)[start] > (*array)[middle])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[middle];
        (*array)[middle] = temp;
    }
    if((*array)[start] > (*array)[end])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[end];
        (*array)[end] = temp;
    }
    if((*array)[middle] > (*array)[end])
    {
        temp = (*array)[middle];
        (*array)[middle] = (*array)[end];
        (*array)[end] = temp;
    }

    T pivot = (*array)[middle];
    
	//Organize sections
    while(i <= j)
    {
        while((*array)[i] < pivot)
            i++;
        
        while((*array)[j] > pivot)
            j--;

        if(i <= j)
        {
            temp = (*array)[i];
            (*array)[i] = (*array)[j];
            (*array)[j] = temp;
            i++;
            j--;
        }
    }
	
	//Parallelization of two section using omp sections
    int size;
    #pragma omp parallel sections if(end - start > sortSize) num_threads(2)
    {
        #pragma omp section
        if(start < j)
        {
            size = j - start;
            if(size < 10000)
                quickSortSerial(array, start, j);
            else
                quickSortSection(array, start, j);
        }

        #pragma omp section
        if(i < end)
        {
            size = end - i;
           if(size < 10000)
                quickSortSerial(array, i, end);
            else
                quickSortSection(array, i, end);
        }
    }
}

/**
 * quickSortSerial - serial quicksort
 * @param array - the structure to sort
 * @param start - index of start of array section to sort
 * @param end - index of end of array section to sort
 */
template <class T>
void ParallelSort::quickSortSerial(vector<T> * array, int start, int end)
{
    int i = start, j = end, middle = (start + end)/2;
    T temp;
	
	//Select median of three for pivot to avoid 
	//worst case pivot.
    if((*array)[start] > (*array)[middle])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[middle];
        (*array)[middle] = temp;
    }
    if((*array)[start] > (*array)[end])
    {
        temp = (*array)[start];
        (*array)[start] = (*array)[end];
        (*array)[end] = temp;
    }
    if((*array)[middle] > (*array)[end])
    {
        temp = (*array)[middle];
        (*array)[middle] = (*array)[end];
        (*array)[end] = temp;
    }
    T pivot = (*array)[middle];
    
	//Organize sections
    while(i <= j)
    {
        while((*array)[i] < pivot)
            i++;
        
        while((*array)[j] > pivot)
            j--;

        if(i <= j)
        {
            temp = (*array)[i];
            (*array)[i] = (*array)[j];
            (*array)[j] = temp;
            i++;
            j--;
        }
    }
	
	//Sort first section.
    if(start < j)
    {    
        quickSortSerial(array, start, j);
    }
	//Sort second section.
    if(i < end)
    {
        quickSortSerial(array, i, end);
    }
}

/**
 * insertionSort - Unused insertion sort that could be used to
 * optimize sorts of small arrays of items, as insertion sort is
 * faster on smaller sized arrays, however testing indicated that
 * sort did not come out faster, so it was not used.
 * @param array - the structure to sort
 * @param start - index of start of array section to sort
 * @param end - index of end of array section to sort
 */
template <class T>
void ParallelSort::insertionSort(vector<T> * array, int start, int end)
{
    T item;
    int iPlace;
    for(int i = start; i <= end; i++)
    {
        item = (*array)[i];
        iPlace = i;
        
        while(iPlace > 0 && (*array)[iPlace - 1] > item)
        {
            (*array)[iPlace] = (*array)[--iPlace];
        }
        
        (*array)[iPlace] = item;
    }

    return;
}