public class BanketNoWait {
	public static void main (String args[]) throws InterruptedException {
		PotNoWait pot = new PotNoWait(5);
		Thread s1 = new SavageNoWait(pot); s1.setName("alice");
		Thread s2 = new SavageNoWait(pot); s2.setName("bob");
		Thread s3 = new SavageNoWait(pot); s3.setName("peter");
		Thread s4 = new SavageNoWait(pot); s4.setName("xana");
		Thread s5 = new SavageNoWait(pot); s5.setName("tom");
		Thread s6 = new SavageNoWait(pot); s6.setName("jerry");
		Thread s7 = new SavageNoWait(pot); s7.setName("kim");
		Thread s8 = new SavageNoWait(pot); s8.setName("berta");
		Thread c = new CookNoWait(pot); c.setName("cook");
		s1.start(); s2.start(); s3.start(); s4.start();
		s5.start(); s6.start(); s7.start(); s8.start();
		c.start();
	}
}