import java.util.Comparator;

public class BMXComparator implements Comparator<Pixel> {

	@Override
	public int compare(Pixel pixel1, Pixel pixel2) {
		
		String pix1Binary="",pix2Binary="";
		char [] pix1Red,pix1Green,pix1Blue,pix2Red,pix2Green,pix2Blue = new char[8];
		pix1Red = binaryForm(pixel1.getRed(),8).toCharArray();
		pix1Green = binaryForm(pixel1.getGreen(),8).toCharArray();
		pix1Blue = binaryForm(pixel1.getBlue(),8).toCharArray();
		pix2Red = binaryForm(pixel2.getRed(),8).toCharArray();
		pix2Green = binaryForm(pixel2.getGreen(),8).toCharArray();
		pix2Blue = binaryForm(pixel2.getBlue(),8).toCharArray();
		
		for(int i = 0 ; i < 8 ; i++) {
			pix1Binary += pix1Red[i] + pix1Green[i] + pix1Blue[i];
			pix2Binary += pix2Red[i] + pix2Green[i] + pix2Blue[i];
		}
		
		return pix1Binary.toString().compareTo(pix2Binary.toString());	
	}
	
	public String binaryForm (int n, int digit) {
		String binary = "";
		for(int i = 0; i < digit; ++i, n/=2) {
		   if(n % 2 == 0) 
		         binary = "0" + binary;
		   else if(n % 2 == 1) 
		         binary = "1" + binary;
		}
		return binary;
	}

}
