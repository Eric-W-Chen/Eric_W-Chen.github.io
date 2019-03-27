import java.util.Random;

public class HybridSort{

    Random r = new Random();
    private static int max_length = 47; //based off JDK 1.8, if less than size 47 use insertion sort over quick sort

    public void sort (double[] arr) {
        if (arr.length <= max_length){
            hybridsort(arr, 0, arr.length-1);
        }
        else{
            quicksort(arr, 0, arr.length-1);
        }
    }

    public void hybridsort (double [] arr, int left, int right) {

        int subarraySize = right -  left;
        if(subarraySize <= max_length && subarraySize > 0) {
            quadraticsort(arr, left, right);
        }
        else if (left < right) {
            int p = partition(arr, left, right);
            if (p-left < max_length && p-left > 0){
                quadraticsort(arr, left, right);
            }
            else{
                hybridsort(arr, left, p-1);
            }
            if (right-p < max_length && right-p > 0){
                quadraticsort(arr, p, right);
            }
            else{
                hybridsort(arr, p, right);
            }
        }
    }

    public int quicksort (double [] arr, int left, int right){
        if (left < right) {
            int p = partition(arr, left, right);
            quicksort(arr, left, p-1);
            quicksort(arr, p+1, right);
            return p;
        }
        return -1;
    }

    int partition (double [] arr, int left, int right) {
        int pivotIndex = r.nextInt(right - left) + left;
        double pivot = arr[pivotIndex];
        swap(arr, pivotIndex, right);
        int i = (left - 1);  // Index of smaller element

        for (int j = left; j <= right- 1; j++)
        {
            // If current element is smaller than or
            // equal to pivot
            if (arr[j] <= pivot)
            {
                i++;    // increment index of smaller element
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, right);
        return (i + 1);
    }

    void swap(double [] arr, int left, int right){
        double temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
    }

    public void quadraticsort (double [] arr, int left, int right){ //insertion sort
        for(int i=left+1; i<=right; i++){
            double temp = arr[i];
            int j = i-1;
            while(j>=left&&arr[j]>temp){
                arr[j+1] = arr[j];
                --j;
            }
            arr[j+1] = temp;
        }
    }

    public static void main(String[] args){
        double [] arr = new double [] {5, 2, 9, 12, 6, 8, 3, 7, 10, 15, 1, 13, 25, 73, 24, 123, 42, 65};
        HybridSort hybridsort = new HybridSort();
        hybridsort.sort(arr);
        hybridsort.displayArray(arr);
    }

    public void displayArray(double [] arr){
        for(int i=0; i<arr.length;i++){
            System.out.print(arr[i] + " ");
        }
        System.out.println();
    }
}
