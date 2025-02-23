public class Company extends Thread {
	int deposit_amount = 10;
	Account account;

	public Company(Account account, int amount) {
		this.deposit_amount = amount;
		this.account = account;
	}
	public void run() {
		while(true) {
			System.out.println(Thread.currentThread().getName() + " wants to deposit " + deposit_amount);
			try {
				sleep(200);
				account.deposit(deposit_amount);
				sleep(5000);
			}
			catch(InterruptedException e) {}
		}
	}
}