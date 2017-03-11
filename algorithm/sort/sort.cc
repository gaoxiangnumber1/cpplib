#include <stdio.h>
#include <utility>
using namespace std;

// Assume n is the number of elements to be sorted.
// TC: Best = O(n), Average = O(n^2), Worst = O(n^2)
// SC: O(1)
void BubbleSort(int *data, int first, int last) // [first, last)
{
	int unsorted = last - first; // The number of unsorted elements.
	for(; unsorted > 0;) // Repeat only if there are unsorted elements.
	{
		int last_swap_index = -1;
		for(int index = first + 1; index < first + unsorted; ++index)
		{
			if(data[index - 1] > data[index])
			{
				swap(data[index - 1], data[index]);
				last_swap_index = index;
			}
		}
		// Up to now, we divide the array into two parts:
		// 1. [first, last_swap_index): unsorted. Need traverse.
		// 2. [last_swap_index, last): sorted. No need traverse.
		unsorted = last_swap_index - first;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TC: Best = O(n^2), Average = O(n^2), Worst = O(n^2)
// SC: O(1)
void SelectionSort(int *data, int first, int last)
{
	// [first, first_unsorted) is the sorted part.
	// [first_unsorted, last) is the unsorted part.
	for(int first_unsorted = first; first_unsorted < last - 1; ++first_unsorted)
	{
		int min_index = first_unsorted;
		for(int unsorted_index = first_unsorted + 1; unsorted_index < last; ++unsorted_index)
		{
			if(data[min_index] > data[unsorted_index])
			{
				min_index = unsorted_index;
			}
		}
		if(min_index != first_unsorted)
		{
			swap(data[min_index], data[first_unsorted]);
		}
		// [first, first_unsorted] is sorted.
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helper temporary stores the sub-range sorted elements of data in merge process,
// and later copy all sorted elements to data after each merge.
void Merge(int *data, int first, int middle, int last, int *helper) // O(n)
{
	int left = first; // index in left-subrange [first, middle).
	int right = middle; // index in right-subrange [middle, last).
	int helper_index = first; // index in helper.
	while(left < middle || right < last)  // Either subrange is not empty
	{
		// Copy left element into helper when:
		// 1.	Left subrange is not empty and right subrange is empty:
		//		`left < middle && right >= last`
		// 2.	Both are not empty and left element is lesser or equal than right element:
		//		`left < middle && right < last && data[left] <= data[right]`
		// NOTE:
		// 1. `||` is short-circuit.
		// 2. `<=` keep the relative order of equal elements, thus guarantee sorting stability.
		if(right >= last || (left < middle && data[left] <= data[right]))
		{
			helper[helper_index++] = data[left++];
		}
		else
		{
			helper[helper_index++] = data[right++];
		}
	}
	// Copy sorted elements into data.
	for(int index = first; index < last; ++index)
	{
		data[index] = helper[index];
	}
}
void MergeSortMain(int *data, int first, int last, int *helper)
{
	// When `last - first == 1`: only 1 element is automatically sorted.
	if(last - first >= 2)
	{
		int middle = first + (last - first) / 2;
		// 1. Sort two subranges: [first, middle), [middle, last).
		MergeSortMain(data, first, middle, helper);
		MergeSortMain(data, middle, last, helper);
		// 2. Merge two sorted subranges into data.
		Merge(data, first, middle, last, helper);
	}
}
// TC: Best = O(nlogn), Average = O(nlogn), Worst = O(nlogn)
// SC: O(n)
void MergeSort(int *data, int first, int last) // [first, last)
{
	int helper[last - first];
	MergeSortMain(data, first, last, helper);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Partition(int *data, int first, int last) // O(n)
{
	int pivot = data[last - 1];
	int divide = first;
	// divide is the index of pivot at last(by swapping with[last - 1]), our aim is:
	// elements in [first, divide) <= [divide](i.e., pivot)
	// [divide] < elements in [divide + 1, last)
	for(int index = first; index < last - 1; ++index)
	{
		// NOTE: `<=` not `<` Guarantee the Stability.
		if(data[index] <= pivot)
		{
			if(index != divide) // No need swap when both points to the same element.
			{
				// Condition: data[divide] > pivot >= data[index]
				swap(data[index], data[divide]);
			}
			++divide;
		}
	}
	swap(data[divide], data[last - 1]);
	return divide;
}
// TC: Best = O(nlogn), Average = O(nlogn), Worst = O(n^2)
// SC: Best = O(logn), Worst = O(n)
void QuickSort(int *data, int first, int last) // [first, last)
{
	if(last - first >= 2) // At least 2 elements
	{
		int divide = Partition(data, first, last);
		// Elements in [first, divide) are all <= [divide].
		// [divide] is < all elements in [divide + 1, last)
		QuickSort(data, first, divide);
		QuickSort(data, divide + 1, last);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RadixSort(int *data, int total_digit, int length, int max_number)
// we assume all numbers are in [0, 10^10 - 1], so total_digit = 10 and
// because all numbers are in decimal, so max_number = 9.
{
	int divisor = 1;  // help to get each digit number from least to most significant digits
	// for each digit_number: use counting sort to sort.
	for(int digit = 1; digit <= total_digit; digit++)
	{
		// first store corresponding index element's digit_number of data
		int digit_number[length];
		for(int index = 0; index < length; index++)
		{
			digit_number[index] = (data[index] / divisor) % 10;
		}
		// second use counting sort to sort data base on each digit_number
		int temp[length], count[max_number + 1];
		memset(count, 0, sizeof(count));
		for(int index = 0; index < length; index++)
		{
			count[digit_number[index]]++;
		}
		for(int index = 1; index <= max_number; index++)
		{
			count[index] += count[index - 1];
		}
		for(int index = length - 1; index >= 0; index--)
		{
			temp[count[digit_number[index]] - 1] = data[index];
			count[digit_number[index]]--;
		}
		for(int index = 0; index < length; index++)
		{
			data[index] = temp[index];
		}
		// third, update divisor to get the next high digit_number.
		divisor *= 10;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintData(int *data, int first, int last)
{
	for(int index = 0; index < last; ++index)
	{
		printf("%d ", data[index]);
	}
	printf("\n");
}
using SortFunction = void(*)(int*, int, int);
void Test(const char *name, SortFunction Sort)
{
	printf("----------%s----------\n", name);
	const int data_length = 10;
	int data[][data_length] =
	{
		{0,1,2,3,4,5,6,7,8,9},
		{9,8,7,6,5,4,3,2,1,0},
		{0,2,4,6,8,9,7,5,3,1}
	};
	int data_number = static_cast<int>(sizeof(data) / sizeof(data[0]));
	for(int data_index = 0; data_index < data_number; ++data_index)
	{
		Sort(data[data_index], 0, data_length);
		PrintData(data[data_index], 0, data_length);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	Test("BubbleSort", BubbleSort);
	Test("SelectionSort", SelectionSort);
	Test("MergeSort", MergeSort);
	Test("QuickSort", QuickSort);
	Test("RadixSort", RadixSort);
}
