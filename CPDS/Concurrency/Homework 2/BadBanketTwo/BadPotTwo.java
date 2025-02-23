public class BadPotTwo {
	private int capacity;
	private int servings = 0;

	public BadPotTwo(int capacity) {
		this.capacity = capacity;
	}
	public void fillpot() throws InterruptedException {
		if (servings > 0) {
			System.out.println(Thread.currentThread().getName() + " would like to fill the pot");
		}
		servings = capacity;
		System.out.println(servings);
		print_servings();
		notifyAll();
	}
	public void getservings() throws InterruptedException {
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