import java.util.Comparator;

public class PriorityQueue {
	private Pixel[] arr; 
	private int size, capacity;
	private Comparator<Pixel> comparator;
	
	public PriorityQueue(Comparator<Pixel> c)
    {    
		comparator = c;
        this.capacity = 10000000;
        arr = new Pixel[this.capacity];
        size = 0;
    }

	 
	public boolean isEmpty()
	{
	       return size == 0;
	}
	
	public void show() {
		int counter = 0;
		for (int i = 1 ; i <= size ; i++) {
			System.out.println(arr[i].getRed() + " " + arr[i].getGreen() + " " + arr[i].getBlue());
			counter++;
			
		} 
		System.out.print(counter);
	}
	
	public synchronized void insert(Pixel pixel)
	{	
			arr[++size] = pixel;       
	        int index = size;
	        while (index != 1 && comparator.compare(pixel, arr[index/2]) == 1)
	        {
	        	arr[index] = arr[index/2];
	            index /=2;
	        }
	        arr[index] = pixel;
	        this.notifyAll();
    }
	
	public synchronized Pixel remove()
	{
		Pixel pixel, tempPixel;
	    int parent, child; 
	    while (isEmpty())
	    {
	        try {
				this.wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    }
	    pixel = arr[1];
	    tempPixel = arr[size--]; 
	    parent = 1;
	    child = 2;
	    while (child <= size)
	    {
	        if (child < size && comparator.compare(arr[child + 1], arr[child]) == 1)
	            child++;
	        if (comparator.compare(tempPixel, arr[child]) == 1 || comparator.compare(tempPixel, arr[child]) == 0 )
	            break;
	        arr[parent] = arr[child];
	        parent = child;
	        child *= 2;
	    }
	    arr[parent] = tempPixel;	 
	    return pixel;
	}
}
