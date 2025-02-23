public class PotNoWait {
	private int capacity;
	private int servings = 0;

	public PotNoWait(int capacity) {
		this.capacity = capacity;
	}
	public synchronized void fillpot() throws InterruptedException {
		if (servings > 0) {
			System.out.println(Thread.currentThread().getName() + " go walk");
		}
		else {
			servings = capacity;
			System.out.println(Thread.currentThread().getName() + " fills the pot");
			print_servings();
		}
	}
	public synchronized void getservings() throws InterruptedException {
		if (servings == 0) {
			System.out.println(Thread.currentThread().getName() + " go walk");
		} else {
			Thread.sleep(200);
			--servings;
			System.out.println(Thread.currentThread().getName() + " is served");
			print_servings();
		}
	}
	public synchronized void print_servings() {
		System.out.println("servings in the pot: " + servings);
	}
}