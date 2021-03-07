import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class IslandFinder {
	private IslandStack mainStack = new IslandStack();
	private int[][] mapArr;
	private StringBuilder map;
	private String line;
	private BufferedReader bufferedReader;
	private int counter = 0;
	private int row, column;
	
	public IslandFinder(String filePath) {
		counter = 0;
		row = 0;
		column = 0;
		map = new StringBuilder();
		try {
			bufferedReader = new BufferedReader(new FileReader(filePath));
		}catch(Exception e) {
			e.printStackTrace();
		}		
		
	}
	
	public void findIslandCount() {
		try {
			line = bufferedReader.readLine();
			column = line.length();
			while(line != null) {
				++row;
				map.append(line);
				map.append("\n");
				line = bufferedReader.readLine();
			}
			
			mapArr = new int [row][column];
			for(int i = 0 , j = 0, z = 0; i < map.length() ; i++) {
				if(map.charAt(i) == '0' || map.charAt(i) == '1')
					mapArr[z][j++] = Character.getNumericValue(map.charAt(i));
				else {
					j = 0;
					++z;
				}	
			}
			for(int i = 0 ; i < row ; i++) {
				for(int j = 0 ; j < column ; j++){
					if(mapArr[i][j] != 0) {
						mapArr[i][j] = 0 ;
						mainStack.push(i,j);
						counter++;
						while(!mainStack.empty()) {
							if(checkRight(mapArr, i, j)) {
								mapArr[i][j+1] = 0 ;
								mainStack.push(i,j+1);
							}
							if(checkLeft(mapArr, i, j)) {
								mapArr[i][j-1] = 0 ;
								mainStack.push(i, j-1);
							}
							if(checkTop(mapArr, i, j)) {
								mapArr[i-1][j] = 0 ;
								mainStack.push(i-1, j);
							}
							if(checkBottom(mapArr, i, j)) {
								mapArr[i+1][j] = 0 ;
								mainStack.push(i+1, j);
							}	
							i = mainStack.peek().i;
							j = mainStack.peek().j;		
							mainStack.pop();
						}					
					}
				}
			}
			System.out.println(counter);
		}catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	private boolean checkRight(int [][] arr, int i, int j) {
		if(j + 1 < column && arr[i][j+1] == 1)
			return true;
		return false;
	}
	
	private boolean checkLeft(int [][] arr, int i, int j) {
		if(j > 0 && arr[i][j-1] == 1) 
			return true;
		return false;
		
	}

	private boolean checkTop(int [][] arr, int i, int j) {
		if(i > 0 && arr[i-1][j] == 1) 
			return true;
		return false;
	}
	
	private boolean checkBottom(int [][] arr, int i, int j) {
		if(i+1 < row && arr[i+1][j] == 1) 
			return true;
		return false;
	}
	
}
