import java.util.Iterator;

// TODO: Auto-generated Javadoc
/**
 * The Class MyIterator.
 */
public class MyIterator implements Iterator<Object>{
	
	/** The object to hold list's objects. */
	private Experiment tempOb;
	
	/** The ExperimentList to hold list's copy. */
	private ExperimentList tempList;
	
	/** The current position of iterator. */
	private int current;
	
	/**
	 * Instantiates a new my iterator.
	 *
	 * @param ob another object of ExperimentList class
	 */
	public MyIterator(ExperimentList ob) {
		int index = 0;
		if(ob.getArr()[0] == null)
			throw new IllegalArgumentException();
		for(int i = 1 ; i < ob.getArr().length-1 ; i++) {
			if(ob.getArr()[i] != null && ob.getArr()[index].getDay() > ob.getArr()[i].getDay())
				index = i;
		}		
		tempOb = ob.getArr()[index];
		tempList = ob;
		current = -1; 
	}
	
	/* (non-Javadoc)
	 * @see java.util.Iterator#hasNext()
	 */
	public boolean hasNext() {
		if(current < tempList.getSize()-1)
			return true;
		else
			return false;
	}
	
	/* (non-Javadoc)
	 * @see java.util.Iterator#next()
	 */
	public Experiment next() {
		Experiment ob = tempOb;
		if(hasNext()) {
			tempOb = tempOb.getNextExp();
			current++;
		}
		else {
			throw new IndexOutOfBoundsException();
		}
		return ob;
	}
	
	
}
