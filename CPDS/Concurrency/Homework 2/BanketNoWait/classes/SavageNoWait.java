public class SavageNoWait extends Thread {
	PotNoWait pot;
	public SavageNoWait(PotNoWait pot) {	
		this.pot = pot;
	}
	public void run() {
		while (true) {
			System.out.println(Thread.currentThread().getName() + " is hungry and would like to eat");
			try {
				Thread.sleep(200);
				pot.getservings();
			}
			catch(InterruptedException e) {};
		}
	}
}