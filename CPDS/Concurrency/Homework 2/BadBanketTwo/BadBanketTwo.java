public class BadBanketTwo {
	public static void main (String args[]) throws InterruptedException {
		BadPotTwo pot = new BadPotTwo(5);
		Thread s1 = new BadSavageTwo(pot); s1.setName("alice");
		Thread s2 = new BadSavageTwo(pot); s2.setName("bob");
		Thread s3 = new BadSavageTwo(pot); s3.setName("peter");
		Thread s4 = new BadSavageTwo(pot); s4.setName("xana");
		Thread s5 = new BadSavageTwo(pot); s5.setName("tom");
		Thread s6 = new BadSavageTwo(pot); s6.setName("jerry");
		Thread s7 = new BadSavageTwo(pot); s7.setName("kim");
		Thread s8 = new BadSavageTwo(pot); s8.setName("berta");
		Thread c = new BadCookTwo(pot); c.setName("cook");
		s1.start(); s2.start(); s3.start(); s4.start();
		s5.start(); s6.start(); s7.start(); s8.start();
		c.start();
	}
}