public class bubbleSort
{
	public static void bubbleSort(int [] arr)
	{
		int temp;
		
		for (int i = arr.length - 1; i > 0; i--)
		{
			for (int j = 0; j < i; j++)
			{
				if (arr[j] > arr[j + 1])
				{
					temp = arr[j + 1];
					arr[j + 1] = arr[j];
					arr[j] = temp;
				}
			}
		}
	}
	public static void printArr(int [] arr)
	{
		for (int i = 0; i < arr.length; i++)
			System.out.print(arr[i] + " ");

		System.out.println();
	}
	public static void main(String [] args)
	{
		int n = Integer.parseInt(args[0]);
		int [] arr = new int[n];

		for (int i = 0; i < n; i++)
			arr[i] = (int)(Math.random() * 100 + 1);

		printArr(arr);
		bubbleSort(arr);
		printArr(arr);
		return;
	}
}
