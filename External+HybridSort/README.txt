HybridSort: I ended up using insertion sort for the quadratic sorting algorithm as insertion sort is generally faster than even nlogn sorts for smaller arrays. The other quadratic sorts will always be O(n^2). It's obvious that insertion sort is the fastest when you have an array like: [1, 2, 3, 4, 6, 5, 8] where it'll skip 1-4 and swap 6,5 and skip 6 and 8. Quicksort would be pretty inefficient in that case, let alone any other quadratic sort as it would either utilize a divide and conquer algorithm or go through each number one by one.

For the choice of the size of array, I decided on 47. Even though the right number is reliant on your computer and the data you are sorting, according to JDK 1.8, the file DualPivotQuicksort.java defines:
"if the length of an array to be sorted is less than this constant, insertion sort is used in preference to Quicksort"
private static final int INSERTION_SORT_THRESHOLD = 47;


ExternalSort: I ended up using the HybridSort for the algorithm as I made a method called "runSortingExperiments" with essentially tests out the speed of quick sort, insertion sort, and hybrid sort from a 100000 element list. The results are as follows: 
Algorithm      :  Total(ms)  : Average(ms) |
Quick Sort     : 1660591920 : 138.38266000000002
Insertion Sort : 1410942131 : 117.57851091666667
Hybrid Sort    : 1106900107 : 92.24167558333333

To test out the speed of each sorting algorithm for the list, simply uncomment the "runSortingExperiments" method and line 22 to call the method. The file IO can make the times vary a lot, so it’s best to run it a few times; however, the majority of times hybrid sort is the fastest.

Clearly, hybrid sort outperformed the other sorting algorithms. For the input file, just be sure to name it "myInputFile.txt" and put it in the same folder as ExternalSort.java. Also, be sure to put HybridSort in the same folder as well as we use HybridSort in this program as it is the fastest sorting algorithm in this case. In the input file, be sure to separate the numbers by a ", ". Also, be sure to modify the parameters of line 21 accordingly to the number of elements to sort in the input file (n) and the size of each chunk (k). 
The “myInputFile.txt” that I was testing this on had around 14,000 elements in it.

The format of “myInputFile.txt” will be as follows:
20.0, 13.2, 34.2, 12.4, 32.1, 54.2, 12.3, 45.6, 78.5, 12.4, 12.5, 63.2, 95.6, 32.2, 43.2, 54.3, 43.5, 65.3, 43.6, 56.8