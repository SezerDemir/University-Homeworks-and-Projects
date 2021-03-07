import java.util.Comparator;

public class LEXComparator implements Comparator<Pixel>,Runnable{
	
	@Override
	public int compare(Pixel pixel1, Pixel pixel2) {
		String rgb1,rgb2;
		rgb1 = Integer.toString(pixel1.getRed()) + Integer.toString(pixel1.getGreen()) + Integer.toString(pixel1.getBlue());
		rgb2 = Integer.toString(pixel2.getRed()) + Integer.toString(pixel2.getGreen()) + Integer.toString(pixel2.getBlue());
		return rgb1.toString().compareTo( rgb2.toString() );
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
	}

}
