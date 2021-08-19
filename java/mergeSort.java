// Daniel Rodriguez
// April 3, 2019
// A simple mergeSort function for reference during practice / katas.

public class MergeSort
{
	// Public wrapper function
	public static void mergeSort(int [] arr)
	{
		mergeSort(arr, 0, arr.length - 1);
	}

	// Actual recursive sorting function
	private static void mergeSort(int [] arr, int lo, int hi)
	{
		// Base Case
		if (lo >= hi)
			return;
		
		int mid = lo + (hi - lo) / 2;
		// Recursive calls
		mergeSort(arr, lo, mid);
		mergeSort(arr, mid + 1, hi);

		// The Merge operation (2 parts)
		// A: Merge 2 subarrays into an auxiliary array
		int [] aux = new int[hi - lo + 1];

		int i = lo;
		int j = mid + 1;
		int k = 0;

		// Walk both subarrays, merging back into the
		// auxiliary 
		while (i <= mid || j <= hi)
		{
			// Can be simplified with cleverness
			if (j > hi)
				aux[k++] = arr[i++];
			else if (i > mid)
				aux[k++] = arr[j++];
			else if (arr[i] < arr[j])
				aux[k++] = arr[i++];
			else
				aux[k++] = arr[j++];
		}

		// B: Merge the auxiliary array back into main array
		for (i = lo; i <= hi; i++)
			arr[i] = aux[i - lo];
	}

	public static void printArr(int [] arr)
	{
		for (int i = 0; i < arr.length; i++)
			System.out.print(arr[i] + " ");
		System.out.println();
	}

	// Driver code
	public static void main(String [] args)
	{
		int n = Integer.parseInt(args[0]);
		if (n < 1)
		{
			System.out.println("Use an integer > 1");
			return;
		}

		int [] arr = new int[n];

		for (int i = 0; i < n; i++)
			arr[i] = (int)(Math.random() * 100 + 1);

		printArr(arr);
	
		mergeSort(arr);
	
		printArr(arr);

		return;
	}
}
