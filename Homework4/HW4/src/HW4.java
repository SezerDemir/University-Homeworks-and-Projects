
public class HW4 {

	public static void main(String[] args) {
		Integer[][] matrix = new Integer[][] { { 1, 2, 3, 12 },
				                               { 4, 5, 6, 11 },
				                               { 7, 8, 9, 10} };
		Iterator<Integer> iterator = new Iterator<Integer>(matrix,3,4);
		while(iterator.hasNext()) {
			System.out.print(iterator.next() + " ");
		}
	}

}
