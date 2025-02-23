public class Neighbor extends Thread {
	private Flags flags;
	private Turn turn;

	public Neighbor(Flags flags, Turn turn) {
		this.flags = flags;
		this.turn = turn;
	}
	public void run () {
		while(true) {
			try {
				String name = Thread.currentThread().getName();
				System.out.println("try again, my name is: " + name);
				// greedy
				flags.set_true(name);
				turn.pass_turn(name);
				Thread.sleep((int)(200*Math.random()));
				// System.out.println("name: " + name + " turn: " + turn.get());
				while (flags.query_flag(name) && !turn.query_turn(name)) {
					// if other neighbor's flag is up and its their turn, wait
					System.out.println(name + " has to wait");
					Thread.sleep(400);
				}
				System.out.println(name + " enters");
				Thread.sleep(400);
				System.out.println(name + " exits");
				
				Thread.sleep((int)(200 * Math.random()));
				flags.set_false(name);
			}
			catch (InterruptedException e) {};
		}
	}
}