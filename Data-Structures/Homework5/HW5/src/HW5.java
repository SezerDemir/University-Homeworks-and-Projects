import java.awt.image.*;
import java.io.File;

import javax.imageio.ImageIO;

public class HW5 {
	
	public static void main(String[] args) {
		PriorityQueue q1 = new PriorityQueue(new LEXComparator());
		PriorityQueue q2 = new PriorityQueue(new EUCComparator());
		PriorityQueue q3 = new PriorityQueue(new BMXComparator());
		
		Thread t1 = new Thread(new ImgReading(q1,q2,q3,args));
		
		t1.start();
	    
		
			
	}
	
	
}
