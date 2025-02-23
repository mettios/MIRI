public class Account {
	private int balance = 0;

	public Account(int balance) {
		this.balance = balance;
		print_balance();
	}
	public synchronized void withdraw(int amount) throws InterruptedException {
		while(balance < amount) {
			System.out.println(Thread.currentThread().getName() + " has to wait for withdrawal (" + amount + "), not enough balance");
			print_balance();
			wait();
		}
		balance = balance - amount;
		System.out.println(Thread.currentThread().getName() + " withdrew " + amount);
		print_balance();
		//if ()

	}
	public synchronized void deposit(int amount) throws InterruptedException {
		System.out.println(Thread.currentThread().getName() + " deposits " + amount);
		this.balance = balance + amount;
		print_balance();
		notifyAll(); // inform other other threads that balance has changed
	}
	public synchronized void print_balance() {
		System.out.println("Account balance is: " + this.balance);
	}
}