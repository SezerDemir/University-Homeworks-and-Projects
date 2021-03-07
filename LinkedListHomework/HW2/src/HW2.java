
// TODO: Auto-generated Javadoc
/**
 * The Class HW2.
 */
public class HW2 {
	
	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main (String[] args) {
		ExperimentList list = new ExperimentList();
		
		
		list.addExp(new Experiment("Exp1",1,"1:23:21",false,(float)-1.0));
		list.addExp(new Experiment("Exp2",1,"2:23:21",true,(float)2.11));
		
		list.addExp(new Experiment("Exp3",2,"3:23:21",false,(float)2.11));
		list.addExp(new Experiment("Exp4",2,"4:23:21",true,(float)2.12));
		list.addExp(new Experiment("Exp5",2,"5:23:21",true,(float)5.0));
		list.addExp(new Experiment("Exp6",2,"6:23:21",true,(float)3.12));
		
		list.addExp(new Experiment("Exp7",3,"7:23:21",false,(float)-2.11));
		
		list.addExp(new Experiment("Exp8",5,"8:23:21",true,(float)2.11));
		
		list.addExp(new Experiment("Exp9",4,"9:23:21",true,(float)2.11));
		list.addExp(new Experiment("Exp10",4,"10:23:21",true,(float)2.09));
		list.addExp(new Experiment("Exp11",4,"11:23:02",false,(float)-300));
		
		System.out.println("-------All Elements of the List-------");
		MyIterator iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		try{
			System.out.println("\nTest of getExp method for day2 index 1:"+list.getExp(2,1) );
		}catch(Exception e){
			e.getStackTrace();
		}
		try{
			list.setExp(4, 2, new Experiment("Exp12",4,"12:23:21",false,(float)-21));
		}catch(Exception e){
			e.getStackTrace();
		}
		System.out.println("\n-------Test of setExp Method for Day 4 Index 2-------");
		iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		
		System.out.println("\n-------Test of removeExp Method for Day 4 Index 1-------");
		try {
			list.removeExp(4, 1);
		}catch(Exception e){
			e.getStackTrace();
		}
		iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		
		System.out.println("\n-------Test of listExp Method for Day 2-------");
		list.listExp(2);
		
		System.out.println("\n-------Test of removeDay Method for Day 2-------");
		try{
			list.removeDay(2);
		}catch(Exception e){
			e.getStackTrace();
		}
		iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		
		list.addExp(new Experiment("Exp13",4,"11:23:02",true,(float)4.12));
		list.addExp(new Experiment("Exp14",4,"11:23:02",false,(float)-1));
		
		System.out.println("\n-------List of Elements Before orderDay Mehdod Calling-------");
		iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		
		System.out.println("\n-------Test of orderDay Method for Day 4-------");
		try{
			list.orderDay(4);
		}catch(Exception e) {
			e.getStackTrace();
		}
		iter = list.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
		
		System.out.println("\n-------Test of orderExp Method -------");
		System.out.println(list.orderExp());
	}
	
}
