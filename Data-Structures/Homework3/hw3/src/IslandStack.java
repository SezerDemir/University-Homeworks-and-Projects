import java.util.Arrays;

public class IslandStack {
	 private int size = 0;
	 private int capacity;
	 private Coor elements[];
	 
	 public class Coor{
		public int i ;
		public int j ;
		
		public Coor(int i, int j) {
			this.i = i;
			this.j = j;
		}
		public int getI() {
			return i;
		}
		
		public int getJ() {
			return j;
		}

	 }
	 public IslandStack() {
		 capacity = 1000;
		 elements = new Coor[capacity];
	 }
	 
	 private void reallocate() {
		 capacity *= 2;
		 elements = Arrays.copyOf(elements, capacity);
	 }
	 
	 public void push (int i, int j) {
		 if(size == capacity)
			 reallocate();
		 elements[size++] = new Coor(i,j);
	 }
	 
	 @SuppressWarnings("unchecked")
	public Coor pop () {
		 if(size == 0)
			 throw new IndexOutOfBoundsException();
		 Coor e = elements[--size];
		 elements[size] = null;
		 return e;
	 }
	 
	 @SuppressWarnings("unchecked")
	public Coor peek () {
		 if(size == 0)
			 throw new IndexOutOfBoundsException();
		 return elements[size-1];
	}
	 
	public boolean empty() {
		if(size == 0)
			return true;
		return false;
	}
}
