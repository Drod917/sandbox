import java.util.*;

// Daniel Rodriguez
// Returns the index of key if it exists in the array, otherwise
// returns -1
public class binarySearch
{
	public static int binarySearch(int [] arr, int key)
	{
		return binarySearch(arr, key, 0, arr.length - 1);
	}
	private static int binarySearch(int [] arr, int key, int lo,
	                                 int hi)
	{
		// Base case
		if (lo > hi)
			return -1;

		int mid = lo + (hi - lo) / 2;

		if (key == arr[mid])
			return mid;
		else if (key < arr[mid])
			return binarySearch(arr, key, lo, mid - 1);
		else 
			return binarySearch(arr, key, mid + 1, hi);

	}

	// Driver code
	public static void main(String [] args)
	{
		int [] arr = {2,3,7,0,8};

		System.out.println(binarySearch(arr, 3));

		return;
	}
}
