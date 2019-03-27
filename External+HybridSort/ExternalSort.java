import java.nio.file.*;
import java.io.File;
import java.util.*;
import java.lang.*;
import java.io.IOException;
import java.io.FileWriter;

public class ExternalSort{
    int n;
    int k;
    int totalchunks;
    int remainder = 0;
    int sortingAlgo = 0;
    int INPUT_FILE_LENGTH = 10000;
    HybridSort sorter = new HybridSort();


    public static void main(String[] args) throws IOException{
        ExternalSort esorter = new ExternalSort();
        esorter.externalSort("myInputFile.txt", "myOutputFile.txt", 80, 50);
        //runSortingExperiments(esorter);
    }

    /*  public static void runSortingExperiments(ExternalSort esorter) throws IOException {
        long start, end;
        long qsortTotal = 0, qdsortTotal = 0, hsortTotal = 0;
        int totalLoops = 0;

        esorter.sortingAlgo = 0;
        for (int k = 3; k < esorter.INPUT_FILE_LENGTH; k*=2) {
            start = System.nanoTime();
            esorter.createChunks("myInputFile.txt", esorter.INPUT_FILE_LENGTH, k);
            end = System.nanoTime() - start;
            qsortTotal += end;
            totalLoops += 1;
            System.out.println("quick sort total time: " + qsortTotal);
            System.out.println("k =" + k);
        }

        esorter.sortingAlgo = 1;
        for (int k = 3; k < esorter.INPUT_FILE_LENGTH; k*=2) {
            start = System.nanoTime();
            esorter.createChunks("myInputFile.txt", esorter.INPUT_FILE_LENGTH, k);
            end = System.nanoTime() - start;
            qdsortTotal += end;
            System.out.println("insertion sort total time: " + qdsortTotal);
            System.out.println("k =" + k);
        }

        esorter.sortingAlgo = 2;
        for (int k = 3; k < esorter.INPUT_FILE_LENGTH; k*=2) {
            start = System.nanoTime();
            esorter.createChunks("myInputFile.txt", esorter.INPUT_FILE_LENGTH, k);
            end = System.nanoTime() - start;
            hsortTotal += end;
            System.out.println("hybrid sort total time: " + hsortTotal);
            System.out.println("k =" + k);
        }
        final double NANO_TO_MILLI = 1000000.;

        System.out.println("Average createChunks times for 10000 element list for k = 1 to 10000 in powers of 2");
        System.out.println("Algorithm      :  Total(ms)  : Average(ms) |");
        System.out.println("Quick Sort     : " + qsortTotal +    " : " + (qsortTotal/NANO_TO_MILLI)/totalLoops);
        System.out.println("Insertion Sort : " + qdsortTotal +   " : " + (qdsortTotal/NANO_TO_MILLI)/totalLoops);
        System.out.println("Hybrid Sort    : " + hsortTotal +    " : " + (hsortTotal/NANO_TO_MILLI)/totalLoops);

        // Seems hybrid sort performs the best overall.
    }
        @param  inputFile - input file name
        @param  outputFile - output file name
        @param  n - number of elements to sort in input file
        @param  k - chunk size
     */
    public void externalSort(String inputFile, String outputFile, int n, int k) {
        FileWriter out = null;
        try {
            out = new FileWriter(outputFile);
            out.write("");
            out.close();
            out = new FileWriter(outputFile, true);
            createChunks(inputFile, n, k);
            merge(out);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (out != null) {
                try {
                    out.close();
                } catch(IOException e) {
                     e.printStackTrace();
                }
            }
        }
    }

    public Scanner[] createTempFileScanners() throws IOException {
        Scanner [] scanners = new Scanner [this.totalchunks];
        for(int i = 0; i < scanners.length; i++){
            scanners[i] = new Scanner(new File("temp" + i));
            scanners[i].useDelimiter(", ");
        }
        return scanners;
    }

    public void setScannersToIndex(Scanner[] scanners, int[] indices) {
        for (int i = 0; i < scanners.length; i++) {
            int index = indices[i];
            while(scanners[i].hasNextDouble() && index > 0) {
                scanners[i].nextDouble();
                index--;
            }
        }
    }

    public void merge(FileWriter out) throws IOException{
        double [] cache = new double [this.k]; // cache to keep at most k elements of the input in memeory
        Scanner[] scanners = createTempFileScanners();
        int[] indices = new int[this.totalchunks];

        while(canMerge(scanners)){
            // Find index of chunk with the minimum element
            int minIndex = -1;
            int start = 0;
            double min = Double.MAX_VALUE;

            //  Doing this while loop so we can keep at most k elements from the chunks in memory
            while(start < totalchunks) {
                // reset scanners to appropriate element
                scanners = createTempFileScanners();
                setScannersToIndex(scanners, indices);
                // Sometimes totalchunks will be less than k, so get the minimum.
                int end = Math.min(totalchunks - start, this.k);

                // Reading at most k elements to from at most k chunks into cache
                readChunksToCache(start, end, scanners, cache);

                // Get index of chunk with the minimum element
                int cacheMinIndex = getMinIndex(cache, start, end);
                if (cacheMinIndex != -1 && cache[cacheMinIndex] < min) {
                    minIndex = cacheMinIndex + start;
                    min = cache[cacheMinIndex];
                }
                start += this.k;
            }
            out.write(min + ", ");

            indices[minIndex]++;
            if (minIndex == -1) {
                break;
            }
        }

        setScannersToIndex(scanners, indices);
        int lastScanner = getLastScanner(scanners);
        while(lastScanner != - 1 && scanners[lastScanner].hasNextDouble()){
            String next = "" + scanners[lastScanner].nextDouble();
            if (scanners[lastScanner].hasNextDouble()) {
                next += ", ";
            }
            out.write(next);
        }
    }

    public int getLastScanner(Scanner [] scanners){
        for (int i = 0; i < scanners.length-1; i++){
            if(scanners[i].hasNextDouble()){
                return i;
            }
        }
        return -1;
    }

    public boolean canMerge(Scanner[] scanners){
        //iterate through each pointer and check if less than k
        int numValidPointers = 0;

        for (int i = 0; i < scanners.length; i++){
            if(scanners[i].hasNextDouble()){
                numValidPointers++;
                if(numValidPointers > 1){
                    return true;
                }
            }
        }
        return false;
    }

    public void readChunksToCache(int start, int end, Scanner[] scanners, double[] cache) throws IOException{
        //go from 'temp<start>' to at most 'temp<start+k>' overwriting the current cache
        for(int i = start; i < start + end; i++){
            if(scanners[i].hasNextDouble()){
                cache[i - start] = scanners[i].nextDouble();
            }
            else{
                cache[i - start] = Double.MAX_VALUE;
            }
        }
    }

    public int getMinIndex(double[] cache, int start, int end){
        //getting index of minimum element in cache
        int index = -1;
        double min = Double.MAX_VALUE;
        for(int i = 0; i < end; i++){
            if(cache[i] < min){
                index = i;
                min = cache[i];
            }
        }
        return index;
    }

    public void sort(double[] array) {
        int start = 0;
        int end = array.length - 1;
        if (this.sortingAlgo == 0) {
            sorter.quicksort(array, start, end);
        }
        else if (this.sortingAlgo == 1) {
            sorter.quadraticsort(array, start, end);
        } else {
            sorter.hybridsort(array, start, end);
        }
    }

    public void createChunks(String inputFile, int n, int k) throws IOException{
        this.n = n;
        this.k = k;
        this.totalchunks = (int) Math.ceil((1.0*n)/(k*1.0)); //rounding method when dividing n/k
        this.remainder = n%k;

        File input = new File(inputFile);
        Scanner scanner = new Scanner(input);
        scanner.useDelimiter(", "); //split
        double [] chunk = new double [k];

        for(int i = 0; i < totalchunks; i++){
            if(i == totalchunks - 1 && remainder > 0){
                break;
            }
            for(int j = 0; j < k; j++){  //making sure we don't go past our memory
                chunk[j] = scanner.nextDouble(); //parse for doubles
            }
            sort(chunk);
            String tempFile = "temp" + i; //creating temp chunk file name
            Path file = Paths.get(tempFile); //creates a pointer to the location of tempFile
            String chunkString = "";

            for(int l = 0; l < k - 1; l++){
                chunkString += chunk[l] + ", ";
            }
            chunkString += chunk[k - 1];

            Files.write(file, chunkString.getBytes()); //creates chunks
        }
        if (remainder != 0){ //create last remainder chunk depending on what remainder equals
            for(int j = 0; j < remainder; j++){
                chunk[j] = scanner.nextDouble();
            }
            sort(chunk); //sort remainder

            String tempFile = "temp" + (totalchunks-1);
            Path file = Paths.get(tempFile);
            String chunkString = "";
            for(int p = 0; p < remainder - 1; p++){
                chunkString += chunk[p] + ", ";
            }
            chunkString += chunk[remainder - 1];
            Files.write(file, chunkString.getBytes()); //create chunk for remainder
        }
        //System.out.println("Created chunks");
    }
}