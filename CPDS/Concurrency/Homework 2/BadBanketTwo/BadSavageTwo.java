public class BadSavageTwo extends Thread {
	BadPotTwo pot;
	public BadSavageTwo(BadPotTwo pot) {	
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