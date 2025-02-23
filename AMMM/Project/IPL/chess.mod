// PLEASE ONLY CHANGE THIS FILE WHERE INDICATED.

// Number of contestants.
int n = ...;

// Matrix of points.
// Input should satisfy: coefficients >= 0, sum of each row == 100.
int p[1..n][1..n] = ...;

// Range of contestants/slots.
range N = 1..n;


// Define here your decision variables and
// any other auxiliary program variables you need.
dvar boolean x_cs [i in N, j in N, k in N]; 
dvar boolean rest [i in N, k in N];
// You can run an execute block if needed.
// check input conditions maybe

maximize sum(i, k in N) rest[i][k] * p[i][k];// Write here the objective function.


subject to {
  /* Player i cannot be matched against i */
  forall(i, k in N)
    x_cs[i][i][k] == 0;
  /* Player i vs Player j exactly once 
  ** also i vs j == j vs i
  */
  forall(i, j in N: i != j)
    sum(k in N) (x_cs[i][j][k] + x_cs[j][i][k]) == 1;
  /* Exactly (n-1)/2 games at each time slot*/
  forall(k in N)
    sum(i in N) sum(j in N) x_cs[i][j][k] == (n - 1) / 2;
  /* In each slot, Player i can be matched with at most one other player */
  forall(i, k in N) 
//  	sum (j in N) (x_cs[i][j][k] + x_cs[j][i][k]) <= 1;
	// either resting or playing:
	sum(j in N) (x_cs[i][j][k] + x_cs[j][i][k]) == 1 - rest[i][k];
  /* as many black as white */
  forall(i in N) {
    sum(j, k in N) x_cs[i][j][k] == sum (j, k in N) x_cs[j][i][k];
  }
  /* each player rests exactly one day */
  forall(i in N)
    sum(k in N) rest[i][k] == 1;
  /* each day only 1 player rests */
  forall(k in N)
    sum(i in N) rest[i][k] == 1;
	/* 1. player i has to play against everyone 
	*     player i has to play against every player j exactly once
	*/
}

// You can run an execute block if needed.

execute {
  for(var i = 1; i <= n; i++) {
    for (var s = 1; s <= n; s++) {
//      writeln("rest[i,s] = " + rest[i][s]);
      if (rest[i][s] == 1) {
        writeln("Player " + i + " rests on day " + s);
        break;
      }
    }
  }
  for(var k = 1; k <= n; k++) {
    writeln("\nSlot " + k);
    for (var i = 1; i <= n; i++) {
      for (var j = 1; j <= n; j++) {
        if (x_cs[i][j][k] == 1) {
          writeln(i + " white, " + j + " black");
        }
      }
    }
  }
};
