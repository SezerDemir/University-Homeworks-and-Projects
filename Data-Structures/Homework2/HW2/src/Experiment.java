
// TODO: Auto-generated Javadoc
/**
 * The Class Experiment.
 */
public class Experiment {
	
	/**  explains the experimental setup. */
	private String setup;
	
	/**  represents the day of start. */
	private int day;
	
	/**  represents the time of start. */
	private String time;
	
	/**  indicates whether it is completed or not. */
	private boolean completed;
	
	/**  represents the output. */
	private float accurancy;
	
	/**  To hold adresses of next experiment and next day's first experiment. */
	private Experiment nextExp,nextDay;
	
	/**
	 * Instantiates a new experiment.
	 */
	public Experiment() {
		setup = null;
		day = 0;
		time = null;
		completed = false;
		accurancy = (float)0.0;
		nextExp = null;
		nextDay = null;
	}
	
	/**
	 * Instantiates a new experiment.
	 *
	 * @param setup the setup
	 * @param day the day
	 * @param time the time
	 * @param completed the completed
	 * @param accurancy the accurancy
	 */
	public Experiment(String setup, int day, String time, boolean completed, float accurancy) {
		this.setup = setup;
		this.day = day;
		this.time = time;
		this.completed = completed;
		this.accurancy = accurancy;
	}
	
	/**
	 * Instantiates a new experiment.
	 *
	 * @param ob The object of another experiment
	 */
	public Experiment(Experiment ob) {
		if(ob == null) {
			throw new NullPointerException("Object is null");
		}
		setup = ob.getSetup().toString();
		day = ob.day;
		time = ob.getTime().toString();
		completed = ob.getCompleted();
		accurancy = ob.getAccurancy();
		nextExp = null;
		nextDay = null;
	}
	
	
	/**
	 * Sets the next experiment.
	 *
	 * @param ob the new next experiment
	 */
	public void setNextExp(Experiment ob) {
		nextExp = ob;
	}
	
	/**
	 * Gets the next experiment.
	 *
	 * @return the next experiment
	 */
	public Experiment getNextExp() {
		if(nextExp == null)
			return null;
		else
			return nextExp;
	}
	
	/**
	 * Sets the next day.
	 *
	 * @param ob the new next day
	 */
	public void setNextDay(Experiment ob) {
		nextDay = ob;
	}
	
	/**
	 * Gets the next day.
	 *
	 * @return the next day
	 */
	public Experiment getNextDay() {
		return nextDay;
	}
	
	/**
	 * Sets the setup.
	 *
	 * @param val the new setup
	 */
	public void setSetup(String val) {
		setup = val;
	}
	
	/**
	 * Gets the setup.
	 *
	 * @return the setup
	 */
	public String getSetup() {
		return setup;
	}
	
	/**
	 * Sets the day.
	 *
	 * @param val the new day
	 */
	public void setDay(int val) {
		day = val;
	}
	
	/**
	 * Gets the day.
	 *
	 * @return the day
	 */
	public int getDay() {
		return day;
	}
	
	/**
	 * Sets the time.
	 *
	 * @param val the new time
	 */
	public void setTime(String val) {
		time = val;
	}
	
	/**
	 * Gets the time.
	 *
	 * @return the time
	 */
	public String getTime() {
		return time;
	}
	
	/**
	 * Sets the completed.
	 *
	 * @param val the new completed
	 */
	public void setCompleted (boolean val) {
		completed = val;
	}
	
	/**
	 * Gets the completed.
	 *
	 * @return the completed
	 */
	public boolean getCompleted () {
		return completed;
	}
	
	/**
	 * Sets the accurancy.
	 *
	 * @param val the new accurancy
	 */
	public void setAccurancy(float val) {
		accurancy = val;
	}
	
	/**
	 * Gets the accurancy.
	 *
	 * @return the accurancy
	 */
	public float getAccurancy() {
		return accurancy;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
	    return "Experiment{" +
	            "setup='" + setup + '\'' +
	            ", day=" + day +
	            ", time='" + time + '\'' +
	            ", accuracy=" + accurancy +
	            ", completed=" + completed +
	            '}';
	}
}
