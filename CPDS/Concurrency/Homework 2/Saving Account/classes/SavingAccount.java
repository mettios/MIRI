public class SavingAccount {
	public static void main(String args[]) {
		Account account = new Account(10);
		Thread a = new Person(account);
		a.setName("Alice");
		Thread b = new Person(account);
		b.setName("Bob");
		Thread c = new Company(account, 10);
		c.setName("Anonymous Donor");
		a.start();
		b.start();
		c.start();
	}
}