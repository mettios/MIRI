import java.util.Random;

public class Person extends Thread{
	Random rand = new Random();
	Account account;

	public Person(Account account) {
		this.account = account;
	}
	public void run() {
		while (true) {
			int decision = rand.nextInt(100);
			int amount = rand.nextInt(20) + 1;
		
			if (decision % 3 == 0) { // withdraw
				System.out.println(Thread.currentThread().getName() + " wants to withdraw " + amount);
				try {
					Thread.sleep(200);
					account.withdraw(amount);
				}
				catch(InterruptedException e) {};
			} else { // deposit
				System.out.println(Thread.currentThread().getName() + " wants to deposit " + amount);
				try {
					Thread.sleep(200);
					account.deposit(amount);
				}
				catch(InterruptedException e) {};
			}
		}
	}
}