import java.util.*;

// Daniel Rodriguez
// Returns the index of key if it exists in the (already sorted) array, 
// otherwise returns -1.
// 
// There are 2 implementations, one iterative, one recursive.

public class binarySearch
{
	public static int binarySearch(int [] arr, int key)
	{
		int lo = 0, hi = arr.length -1;
		int mid;

		while (lo < hi)
		{
			mid = lo + (hi - lo) / 2;

			if (arr[mid] > key)
				hi = mid - 1;
			else if (arr[mid] < key)
				lo = mid + 1;
			else if (arr[mid] == key)
				return mid;
		}
		return -1;
	}

	public static int binarySearchRecursive(int [] arr, int key)
	{
		return binarySearchRecursive(arr, key, 0, arr.length - 1);
	}
	private static int binarySearchRecursive(int [] arr, int key, int lo,
	                                 int hi)
	{
		// Base case
		if (lo > hi)
			return -1;

		int mid = lo + (hi - lo) / 2;

		if (key == arr[mid])
			return mid;
		else if (key < arr[mid])
			return binarySearchRecursive(arr, key, lo, mid - 1);
		else 
			return binarySearchRecursive(arr, key, mid + 1, hi);

	}

	// Driver code
	public static void main(String [] args)
	{
		int [] arr = new int[] {1,2,3,4,5};

		System.out.println(binarySearch(arr, 3));
		System.out.println(binarySearchRecursive(arr,3));

		return;
	}
}
