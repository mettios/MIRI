/* Homework 4: Peterson 
**
** Name: Mehdi
** Surname: Hassanpour
*/
public class Turn {
	private String turn;

	public Turn() {
		this.turn = "";
	}

	public synchronized boolean query_turn(String s) {
		// returns true if its caller's turn
		return s.equals(turn);
	}

	public synchronized void pass_turn(String s) {
		if (s.equals("alice"))
			turn = "bob";
		else 
			turn = "alice";
		// System.out.println("turn : " + turn);
	}
}