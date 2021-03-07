
public class DeletionThread implements Runnable {
	private PriorityQueue q;
	private String name;
	private int size , counter = 0;
	
	public DeletionThread(PriorityQueue q , String name , int size) {
		this.q = q;
		this.name = name;
		this.size = size;
	}
	@Override
	public void run() {
		while(counter != size) {
			counter++;
			Pixel p = q.remove();
			ImgReading.incereasePrintCount();
			System.out.println("(" + name + ") " + "Red: " + p.getRed() + "Green: " + p.getGreen() + " Blue: " + p.getBlue());	
		}	
	}
}
