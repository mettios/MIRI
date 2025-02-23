public class BadCookTwo extends Thread {
	BadPotTwo pot;
	public BadCookTwo(BadPotTwo pot) {
		this.pot = pot;
	}
	public void run() {
		while (true) {
			System.out.println(Thread.currentThread().getName() + " fills the pot");
			try {
				Thread.sleep(200);
				pot.fillpot();

			}
			catch(InterruptedException e) {};
		}
	}
}