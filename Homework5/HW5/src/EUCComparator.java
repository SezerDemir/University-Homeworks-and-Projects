import java.util.Comparator;

public class EUCComparator implements Comparator<Pixel>{

	@Override
	public int compare(Pixel pixel1, Pixel pixel2) {
		double pixel1L2,pixel2L2;
		pixel1L2 = Math.sqrt(Math.pow(pixel1.getRed(), 2)+Math.pow(pixel1.getGreen(), 2)+Math.pow(pixel1.getBlue(), 2));
		pixel2L2 = Math.sqrt(Math.pow(pixel2.getRed(), 2)+Math.pow(pixel2.getGreen(), 2)+Math.pow(pixel2.getBlue(), 2));
		
		if(pixel1L2 == pixel2L2)
			return 0;
		else if(pixel1L2 > pixel2L2)
			return 1;
		else
			return -1;
	}

}
