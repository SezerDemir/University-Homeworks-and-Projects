
public class Iterator <E> {
	E[] temp;
	int current = 0 ;
	int capacity;
	@SuppressWarnings("unchecked")
	Iterator(E[][] array,int row,int column){
		temp = (E[])new Object[row*column];
		capacity = row*column;
		createTemp(array,row-1,column-1,0,0,0,capacity,0,1);
	}

	private void createTemp(E[][] array, int row, int column, int rowCurrent, int columnCurrent, int vis, int total, int index, int dir) {
		int counter = 0;
		if(capacity == 1) {
			temp[index] = array[index][index];
			return ;
		}
		if(vis != total){
			if(dir == 1) {
				for(int i = columnCurrent ; i <= column ; i++) {
					temp[index++] = array[rowCurrent][i];
					++counter;
				}
				createTemp(array,row,column,rowCurrent+1,columnCurrent,vis+counter,total,index,2);
			}
			else if(dir == 2) {
				for(int i = rowCurrent ; i <= row ; i++) {
					temp[index++] = array[i][column];
					++counter;
				}
				createTemp(array,row,column-1,rowCurrent,columnCurrent,vis+counter,total,index,3);
			}
			else if(dir == 3) {
				for(int i = column ; i >= columnCurrent ; i--) {
					temp[index++] = array[row][i];
					++counter;
				}
				createTemp(array,row-1,column,rowCurrent,columnCurrent,vis+counter,total,index,4);
			}
			else if(dir == 4) {
				for(int i = row ; i >= rowCurrent ; i--) {
					temp[index++] = array[i][columnCurrent];
					++counter;
				}
				createTemp(array,row,column,rowCurrent,columnCurrent+1,vis+counter,total,index,1);
			}
		}
	}
	
	public boolean hasNext() {
		if(current < capacity)
			return true;
		return false;
	}
	
	public E next() {
		return temp[current++];
	}
}
