// TODO: Auto-generated Javadoc
/**
 * The Class ExperimentList.
 */
public class ExperimentList implements Iterable<Object>{
	
	/**  To hold nodes of experiments. */
	private Experiment [] experiments = new Experiment[5];
	
	/** To hold capacity of array. */
	private int capacityExp;
	
	/** To hold how many elements is there in array. */
	private int sizeExp;
	
	/**
	 * Instantiates a new experiment list.
	 */
	public ExperimentList() {
		capacityExp = 5;
		sizeExp = 0;
	}
	
	/**
	 * insert experiment to the end of the day.
	 *
	 * @param ob another experiment object to insert
	 */
	public void addExp(Experiment ob) {
		if(ob == null) {
			throw new NullPointerException();
		}
		if(sizeExp == capacityExp) {
			Experiment [] tempExperiment = new Experiment[capacityExp];
			for(int i = 0; i < sizeExp ; i++) {
				tempExperiment[i] = experiments[i];
			}
			capacityExp*=2;
			experiments = new Experiment[capacityExp];
			for(int i = 0; i < sizeExp ; i++) {
				experiments[i] = tempExperiment[i];
			}
		}
		if(sizeExp == 0) {
			experiments[sizeExp++] = ob;
		}else {
			int counter1 = 0, counter2 = 0; 
			int numberOfExp = 0;
			for(int i = 0; i < sizeExp ; i++) {
				if(ob.getDay() == experiments[i].getDay())
					++numberOfExp;
			}
			if(numberOfExp == 0) {
				int val = 1000;
				for(int i = 0 ; i < sizeExp ; i++) {
					if(experiments[i].getDay() < ob.getDay()) {
						if(val > ob.getDay()-experiments[i].getDay())
							val = ob.getDay()-experiments[i].getDay();
					}
				}
				for(int i = 0 ; i < sizeExp ; i++) {
					if(ob.getDay() - experiments[i].getDay() == val) {
						if(experiments[i].getNextDay() != null)
							ob.setNextDay(experiments[i].getNextDay());
						experiments[i].setNextDay(ob);
					}
				}
			}
			for(int i = 0 ; i < sizeExp ; i++) {
				if(ob.getDay() >= experiments[i].getDay())
					++counter1;
			}
			for(int i = 0 ; i < sizeExp ; i++) {
				if(ob.getDay() >= experiments[i].getDay())
					++counter2;
				if(counter1 == counter2) {
					if(experiments[i].getNextExp() != null)
						ob.setNextExp(experiments[i].getNextExp());
					experiments[i].setNextExp(ob);
				}		
			}
			experiments[sizeExp++] = ob;
		}		
	}
	
	/**
	 * Gets the experiment with the given day and position.
	 *
	 * @param day the day of experiment that is wanted to be returned
	 * @param index the indexof experiment that is wanted to be returned
	 * @return the returned experiment 
	 */
	public Experiment getExp(int day, int index) {
		int numberOfExperiment = 0;
		int counter = -1, counterPos = -1;
		for(int i = 0 ; i < sizeExp ; i++) {
			if(day == experiments[i].getDay())
				++numberOfExperiment;
		}
		if(index >= numberOfExperiment)
			throw new ArrayIndexOutOfBoundsException("For Day " + day + " index must be lower then " + numberOfExperiment);
		for(int i = 0 ; i < sizeExp ; i++) {
			++counter;
			if(day == experiments[i].getDay()) {
				++counterPos;
			}
			if(counterPos == index)
				return experiments[counter];
		}
		return null;
			
	}
	
	/**
	 * sets the experiment with the given day and position.
	 *
	 * @param day the day of experiment that is wanted to be added
	 * @param index the index of experiment that is wanted to be added
	 * @param ob the object of another experiment
	 */
	public void setExp(int day, int index, Experiment ob) {
		int numberOfExperiment = 0;
		int counter = -1;
		if(index == sizeExp+1)
			throw new ArrayIndexOutOfBoundsException();
		for(int i = 0; i < sizeExp ; i++) {
			if(day == experiments[i].getDay())
				++numberOfExperiment;
		}
		if(numberOfExperiment == 0 || (index > numberOfExperiment) )
			throw new IllegalArgumentException("There is no such a element on that index in this day");
		for(int i = 0 ; i < sizeExp ; i++) {
			if(experiments[i].getDay() == day) 
				++counter;
			if(counter == index) {
				Experiment tempExp = null, tempDay = null;
				if(experiments[i].getNextDay() != null) {
					tempDay = experiments[i].getNextDay();
				}
				if(experiments[i].getNextExp() != null) {
					tempExp = experiments[i].getNextExp();
				}
				experiments[i].setSetup(ob.getSetup());
				experiments[i].setTime(ob.getTime());
				experiments[i].setCompleted(ob.getCompleted());
				experiments[i].setAccurancy(ob.getAccurancy());
				experiments[i].setNextDay(tempDay);
				experiments[i].setNextExp(tempExp);
				break;
			}			
		}		
	}
	
	
	/**
	 * remove the experiment specified as index from given day
	 *
	 * @param day the day of experiment
	 * @param index the index of experiment
	 */
	public void removeExp(int day, int index) {
		int numberOfExperiment = 0;
		int pos = -1;
		int counter = -1;
		if(index == sizeExp+1)
			throw new ArrayIndexOutOfBoundsException();
		for(int i = 0; i < sizeExp ; i++) {
			if(day == experiments[i].getDay())
				++numberOfExperiment;
		}
		if(numberOfExperiment == 0 || (index > numberOfExperiment) )
			throw new IllegalArgumentException("There is no such a element on that index in this day");
		for(int i = 0 ; i < sizeExp ; i++) {
			++pos;
			if(experiments[i].getDay() == day)
				++counter;
			if(counter == index)
				break;
		}
		int val = 1000;
		for(int i = 0 ; i < sizeExp ; i++) {
			if(experiments[i].getDay() < experiments[pos].getDay() ) {
				if(val > experiments[pos].getDay()-experiments[i].getDay())
					val = experiments[pos].getDay()-experiments[i].getDay();
			}
		}
		if(val != 1000) {
			for(int i = 0 ; i < sizeExp ; i++) {
				if(experiments[pos].getDay() - experiments[i].getDay() == val) {
					if(experiments[pos].getNextDay() != null)
						experiments[i].setNextDay(experiments[pos].getNextDay());
					else
						experiments[i].setNextDay(null);
				}
			}
		}		
		experiments[pos].setNextDay(null);
		if(experiments[pos].getNextExp() != null) {
			for(int i = 0 ; i < sizeExp ; i++) {
				if(experiments[i].getNextExp() == experiments[pos])
					experiments[i].setNextExp(experiments[pos].getNextExp());
			}
			experiments[pos].setNextExp(null);
		}
		Experiment [] tempArr = new Experiment[capacityExp];
		for(int i = 0; i < sizeExp ; i++)
			tempArr[i] = experiments[i];
		for(int i = sizeExp-1 ; i > pos ; i--) {
			experiments[i-1] = tempArr[i]; 
		}
		experiments[sizeExp-1] = null;
		--sizeExp;
	}
	
	/**
	 * list all completed experiments in a given day.
	 *
	 * @param day the day of experiments
	 */
	public void listExp(int day) {
		boolean flag = true;
		System.out.println("List of completed experiments in day "+ day +": ");
		for(int i = 0 ; i < sizeExp ; i++) {
			if(experiments[i].getDay() == day && experiments[i].getCompleted() == true) {
				System.out.println(experiments[i]);
				flag = false;
			}
		}
		System.out.println("");
		if(flag) 
			throw new IllegalArgumentException("There is no such a completed experiment in this day");					
	}
	
	/**
	 * removes all experiments in a given day.
	 *
	 * @param day the day of experiment
	 */
	public void removeDay(int day) {
		int indexDay = -1;
		int counter = 0;
		Experiment [] tempArr = new Experiment[capacityExp];
		for(int i = 0 ; i < sizeExp ; i++) {
			++indexDay;
			if(experiments[i].getDay() == day)
				break;
		}		
		if(indexDay == -1)
			throw new IllegalArgumentException("There is no such exception in this day");		
		int val = 1000;
		for(int i = 0 ; i < sizeExp ; i++) {
			if(experiments[i].getDay() < experiments[indexDay].getDay() ) {
				if(val > experiments[indexDay].getDay()-experiments[i].getDay())
					val = experiments[indexDay].getDay()-experiments[i].getDay();
			}
		}
		if(val != 1000) {
			if(experiments[indexDay].getNextDay() != null) {
				for(int i = 0 ; i < sizeExp ; i++) {
					if((experiments[indexDay].getDay() - experiments[i].getDay()) == val) {
						experiments[i].setNextDay(experiments[indexDay].getNextDay());	
						for(int j = i ; j < sizeExp; j++) {
							if(experiments[j].getNextExp().getDay() != experiments[j].getDay() ) {
								experiments[j].setNextExp(experiments[indexDay].getNextDay());
								break;							
							}
						}
						break;
					}									
				}
			}
			else {
				for(int i = 0 ; i < sizeExp ; i++) {
					if( (experiments[indexDay].getDay() - experiments[i].getDay() == val ) && (experiments[i].getDay() != experiments[i].getNextExp().getDay()) ) {
						experiments[i].setNextDay(null);	
					}									
				}
			}
		}
		for(int i = 0 ; i < sizeExp ; i++)
			tempArr[i] = experiments[i];
		if(experiments[indexDay].getNextDay() == null) {
			for(int i = indexDay ; i < sizeExp ; i++) {
				experiments[i].setNextExp(null);
				experiments[i] = null;
			}
		}
		else {
			for(int i = 0 ; i < sizeExp ; i++) {
				if(tempArr[i].getDay() == day) {
					tempArr[i] = null;
					++counter;
				}
			}
		}
		int y = 0 ;
		experiments = new Experiment[capacityExp];
		for(int i = 0;  i < sizeExp ; i++) {
			if(tempArr[i] != null) {
				experiments[y++] = tempArr[i];
			}				
		}
		sizeExp-=counter;
	}
	
	/**
	 * sorts the experiments in a given day according to the accuracy, the changes will be done on the list.
	 *
	 * @param day the day of experiments
	 */
	public void orderDay(int day) {
		int indexDay = -1;
		int numberOfExperiment = 0;
		boolean flag2 = true;
		Experiment tempOb;
		for(int i = 0 ; i < sizeExp ; i++) {
			++indexDay;
			if(experiments[i].getDay() == day)
				break;
		}	
		if(indexDay == -1) 
			throw new IllegalArgumentException();
		for(Experiment ob : experiments) {
			if(ob != null && ob.getDay() == day)
				++numberOfExperiment;
		}
		if(numberOfExperiment == 0 || numberOfExperiment == 1)
			throw new IllegalArgumentException();
		int val = 1000;
		for(int i = 0 ; i < sizeExp ; i++) {
			if(experiments[i].getDay() < experiments[indexDay].getDay() ) {
				if(val > experiments[indexDay].getDay()-experiments[i].getDay())
					val = experiments[indexDay].getDay()-experiments[i].getDay();
			}
		}
		for(int i = 0 ; i < sizeExp-1 ; i++) {
			for(int j = i+1 ; j < sizeExp ; j++) {
				if(experiments[i].getDay() > experiments[j].getDay()) {
					tempOb = experiments[j];
					experiments[j] = experiments[i];
					experiments[i] = tempOb;
				}
			}
		}
		indexDay = -1;
		for(int i = 0 ; i < sizeExp ; i++) {
			++indexDay;
			if(experiments[i].getDay() == day)
				break;
		}	
		for(int i = indexDay ; i < indexDay+numberOfExperiment-1 ; i++) {
			for(int j = i+1 ; j < indexDay+numberOfExperiment ; j++ ) {
				if(experiments[i].getAccurancy() > experiments[j].getAccurancy()) {
					tempOb = experiments[j];
					experiments[j] = experiments[i];
					experiments[i] = tempOb;
				}			
			}
		}
		if(val != 1000) {
			for(int i = 0  ; i < sizeExp ; i++) {
				if(flag2 && day - experiments[i].getDay() == val ) {
					experiments[i].setNextDay(experiments[indexDay]);
					flag2 = false;
				}
				if(day - experiments[i].getDay() == val && experiments[i].getNextExp().getDay() == day) {
					experiments[i].setNextExp(experiments[indexDay]);
				}				
			}
		}
		for(int i = indexDay ; i < indexDay+numberOfExperiment ; i++) {
			if(i == capacityExp-1) {
				experiments[i].setNextExp(null);
			}
			else{
				experiments[i].setNextExp(experiments[i+1]);
				if(experiments[i+1] != null && experiments[i+1].getDay() != day)
					experiments[indexDay].setNextDay(experiments[i+1]);
			}
		}
	}
	
	/**
	 * sorts all the experiments in the list according to the accuracy.
	 *
	 * @return the experiment of the head of the list
	 */
	public Experiment orderExp() {
		Experiment [] tempArr = new Experiment[capacityExp];
		Experiment tempOb ;
		if(sizeExp == 0)
			throw new IndexOutOfBoundsException();
		for(int i = 0 ; i < sizeExp; i++)
			tempArr[i] = experiments[i];
		for(int i = 0 ; i < sizeExp-1 ; i++) {
			for(int j = i+1 ; j < sizeExp ; j++) {
				if(tempArr[i].getAccurancy() > tempArr[j].getAccurancy()) {
					tempOb = tempArr[j];
					tempArr[j] = tempArr[i];
					tempArr[i] = tempOb;
				}
			}
		}
		return tempArr[0];		
	}
	
	/**
	 * Gets the array of ExperimentList class
	 *
	 * @return the array of ExperimentList class
	 */
	public Experiment[] getArr() {
		return experiments;
	}
	
	/**
	 * Gets the size.
	 *
	 * @return the size
	 */
	public int getSize() {
		return sizeExp;
	}

	/* (non-Javadoc)
	 * @see java.lang.Iterable#iterator()
	 */
	public MyIterator iterator() {
		MyIterator iter = new MyIterator(this);
		return iter;
	}
	
	
}
