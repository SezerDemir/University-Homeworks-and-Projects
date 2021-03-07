import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

public class ImgReading implements Runnable{
	private int w,h;
	private int x = 0,y = 0;
	private BufferedImage img;
	private PriorityQueue q1,q2,q3;
	private String[] args;
	private int counter = 0 ;
	private static int printCount = 0; 
	private boolean finishFlag1 = false, finishFlag2 = false, finishFlag3 = false, finishFlag4 = false;
	Thread deletionThread1,deletionThread2,deletionThread3;
	public ImgReading(PriorityQueue q1, PriorityQueue q2, PriorityQueue q3, String[] args) {
		this.q1 = q1;
		this.q2 = q2;
		this.q3 = q3;
		this.args = args;
	}
	synchronized public static void incereasePrintCount() {
		++printCount;
	}
	
	public static int getCount() {
		return printCount;
	}
	@Override
	public void run() {
		try 
		{
			img= ImageIO.read(new File(args[0]));
		}
		catch (Exception e)
		{
		    e.printStackTrace();
		}
		w=img.getWidth();
		h=img.getHeight();
		
		deletionThread1 = new Thread(new DeletionThread(q1, "t1", w*h));
		deletionThread2 = new Thread(new DeletionThread(q2, "t2", w*h));
		deletionThread3 = new Thread(new DeletionThread(q3, "t3", w*h));
		
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
			    counter++;
			    int pixel = img.getRGB(j, i);
			    q1.insert(new Pixel(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff)));
		        q2.insert(new Pixel(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff)));
		        q3.insert(new Pixel(((pixel >> 16) & 0xff), ((pixel >> 8) & 0xff), (pixel & 0xff)));
		        if(counter == 100) {
		            try {
		     	        deletionThread1.start();
		    	        deletionThread2.start();
		    		    deletionThread3.start();
		    	    }catch(Exception e) {
		    	        e.printStackTrace();
		    	    }
		    	}
			 }
		}
	}
}

