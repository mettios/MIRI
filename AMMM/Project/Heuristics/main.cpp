#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <cmath>
#include <ctime>
#include <chrono>
#include "config.h"

void print_rest_vector(int* rest_vector, int size) {
	cout << "rest vector: ";
	for (int i = 0; i < size; i++) {
		cout << rest_vector[i] << " ";
	}
	cout << endl;
}
int read_input(vector<vector<int> >& points, string file_name="input.txt") {
	string line;
	ifstream file(file_name);
	int size = -1;
 	if (file.is_open()) {
    	while ( getline (file,line)) {
    		if (size < 0) {
    			stringstream ss;
    			string temp;
    			ss.str(line);
    			ss >> temp >> temp >> size;
    			cout << "size: " << size << endl;
    			continue;
    		}
    		regex e("\\[(.*?)\\]");
    		if(!regex_search(line, e)) {
    			continue;
    		}
      		istringstream ss{regex_replace(line, regex{R"(\[|\])"}, " ")}; 
			vector<int> v{istream_iterator<int>{ss}, istream_iterator<int>{}};
			points.push_back(v);
			istream_iterator<int> isi(ss);
    }
    file.close();
    return size;
  }
}
int get_score(vector<vector<int> > p, int* rest_vector, int size) {
	int score = 0;
	for (int i = 0; i < size; i++) {
		score += p[rest_vector[i]][i];
	}
	return score;
}
void local_search(vector<vector<int> > p, int* solution, int size, int score) {
	ofstream file(output_sol_file);
	cout << "current score: " << get_score(p, solution, size) << endl;
	file << "current score: " << get_score(p, solution, size) << endl;
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			int cost = (p[solution[i]][i] + p[solution[j]][j]) - (p[solution[j]][i] + p[solution[i]][j]);
			if (cost < 0) {
				print_rest_vector(solution, size);
				int temp_i = solution[i];
				solution[i] = solution[j];
				solution[j] = temp_i;
				int new_score = get_score(p, solution, size);
				cout << "improving solution by " << -cost << ", new score: " << new_score << endl;
				file << "improving solution by " << -cost << ", new score: " << new_score << endl;
				// print_rest_vector(solution, size);
			}
		}
	}
	file << "===============================" << endl;
	file.flush();
	file.close();

}

int* get_rest_order_greedy(vector<vector<int> > points, int player_size) { //greedy
	bool* visited_player = new bool[player_size];	
	bool* visited_day = new bool[player_size];
	int* rest_order = new int[player_size]; // day i player <rest_order[i]> rests
	int score = 0;
	
	for (int i = 0; i < player_size; i++) {
		visited_day[i] = false;
		visited_player[i] = false;
	}
	for (int i = 0; i < player_size; i++) {
		int max_value = -1;
		int max_player = -1;
		int max_day = -1;
		for(int p = 0; p < player_size; p++) {
			if (visited_player[p])
				continue;
			for(int d = 0; d < player_size; d++) {
				if (visited_day[d])
					continue;
				if (points[p][d] > max_value) {
					max_value = points[p][d];
					max_player = p;
					max_day = d;
				}

			}
		}		
		score += max_value;
		rest_order[max_day] = max_player;
		visited_player[max_player] = true;
		visited_day[max_day] = true;
	}
	cout << "scheduling score (Greedy): " << score << endl;

	delete[] visited_player;
	delete[] visited_day;

	return rest_order;
}

int get_candidate_grasp(int iteration, vector<vector<int> > points, int size, vector<pair<int, int> >& candidates, bool* visited_player, bool* visited_day) {
	int q_min = 101, q_max = -1;
	for(int i = 0; i < size; i++) {
		if (visited_player[i])
			continue;
		for (int j = 0; j < size; j++) {
			if (visited_day[j])
				continue;
			candidates.push_back(pair<int, int>(i, j));
			if (points[i][j] < q_min) {
				q_min = points[i][j];
			}
			if (points[i][j] > q_max) {
				q_max = points[i][j];
			}
		}
	}
	if (iteration == 0)
		return q_max;
	int th = (int)ceil(q_max - ALPHA * (q_max - q_min));
	return th;
}
int* get_rest_order_grasp(vector<vector<int> > points, int player_size) { // GRASP
	bool* visited_player = new bool[player_size];	
	bool* visited_day = new bool[player_size];
	int* rest_order = new int[player_size]; // day i player <rest_order[i]> rests
	int score = 0;
	int iteration = 0;
	for (int i = 0; i < player_size; i++) {
		visited_day[i] = false;
		visited_player[i] = false;
	}

	for (int i = 0; i < player_size; i++) {
		vector<pair<int, int> > candidates;
		int th = get_candidate_grasp(iteration, points, player_size, candidates, visited_player, visited_day);
		vector<pair<int, int> > rcl;
		for(int j = 0; j < (int)candidates.size(); j++) {
			if (points[candidates[j].first][candidates[j].second] >= th) {
				rcl.push_back(candidates[j]);
			}
		}
		int rcl_idx = rand() % (int)rcl.size();
		rest_order[rcl[rcl_idx].second] = rcl[rcl_idx].first;
		score += points[rcl[rcl_idx].first][rcl[rcl_idx].second];
		visited_player[rcl[rcl_idx].first] = true;
		visited_day[rcl[rcl_idx].second] = true;
		iteration += 1;

	}
	
	delete[] visited_player;
	delete[] visited_day;

	cout << "scheduling score (GRASP): " << score << endl;
	return rest_order;
}

void match_players(int** matches, int p1, int p2, int day, int* rest_vector, int player_size, int* white, int* black) {
	int mapped_p1 = rest_vector[p1], mapped_p2 = rest_vector[p2];
	if ((matches[mapped_p1][mapped_p2] != -1) || (matches[mapped_p2][mapped_p1] != -1)) {// i vs j exists
		return;
	}

	if (white[mapped_p1] == (player_size-1)/2) {
		// p1 has to play black
		matches[mapped_p2][mapped_p1] = day;
		white[mapped_p2]++;
		black[mapped_p1]++;
	} else if(black[mapped_p1] == (player_size-1)/2) {
		// p1 has to play white
		white[mapped_p1]++;
		black[mapped_p2]++;
		matches[mapped_p1][mapped_p2] = day;
	} else if(white[mapped_p2] == (player_size-1)/2) {
		// p2 has to play black
		matches[mapped_p1][mapped_p2] = day;
		white[mapped_p1]++;
		black[mapped_p2]++;
	} else if(black[mapped_p2] == (player_size-1)/2) {
		// p2 has to play white
		matches[mapped_p2][mapped_p1] = day;
		white[mapped_p2]++;
		black[mapped_p1]++;
	} else {
		// black or white for p1, p2 doesnt matter
		// assume p1 white p2 black
		matches[mapped_p1][mapped_p2] = day;
		white[mapped_p1]++;
		black[mapped_p2]++;
	}
	return;
}

int** get_schedule(int player_size, int* rest_vector) { 
	int** matches = new int*[player_size];
	for (int i = 0; i < player_size; i++) {
		matches[i] = new int[player_size];
		for (int j = 0; j < player_size; j++)
			matches[i][j] = -1;
	}

	int* w_track = new int[player_size];
	int* b_track = new int[player_size];
	for (int i = 0; i < player_size; i++) {
		w_track[i] = 0;
		b_track[i] = 0;
	}
	for (int i = 0; i < player_size; i++) {
		for (int j = 0; j < player_size; j++) {
			if (i == j) // i vs i is not allowed
				continue;
			int match_day = (i + j) % player_size;
			if ((i == match_day) || (j == match_day))
				match_day = (match_day * 2) % player_size;
			//lets ignore black & white for now 
			match_players(matches, i, j, match_day, rest_vector, player_size, w_track, b_track);
		}
	}

	delete[] w_track;
	delete[] b_track;

	return matches;
}

void print_schedule(int** matches, int size, int* rest_order) {
	ofstream file(output_sol_file, ios::app);
	for(int i = 0; i < size; i++) {
		cout << "Slot " << i << ": Player " << rest_order[i] << " rests" << endl;
		file << "Slot " << i+1 << ":\tPlayer " << rest_order[i]+1 << " rests" << endl;
		for(int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				if (matches[j][k] == i){
					cout << j << "\t(white)\tvs " << k << "\t(black)" << endl;
					file << j+1 << "\t(white)\tvs " << k+1 << "\t(black)" << endl;
				}

			}
		}
		cout << "===============================" << endl;
		file << "===============================" << endl;
	}
	file.flush();
	file.close();
}

void solve(vector<vector<int> > points, int size) {
	auto start_time = chrono::high_resolution_clock::now();
	int* rest_order;

	if (method == greedy) {
		rest_order = get_rest_order_greedy(points, size);
	} else if (method == grasp) {
		rest_order = get_rest_order_grasp(points, size);

	} else {
		cout << "Error: undefined method. Method has to be greedy or grasp" << endl;
		return;
	}

	int score = get_score(points, rest_order, size);
	print_rest_vector(rest_order, size);

	if (local_search_en) {
		local_search(points, rest_order, size, score);
	}

	int** sch = get_schedule(size, rest_order);
	auto stop_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration<float>(stop_time - start_time);
	print_schedule(sch, size, rest_order);
	ofstream file(output_sol_file, ios::app);
	file << "Scheduling Score:\t" << get_score(points, rest_order, size) << endl;
	cout << "Execution time:\t" << duration.count() << " seconds" << endl;
	
	file << "Execution time:\t" << duration.count() << " seconds" << endl;
	file.flush();
	file.close();
	delete[] sch;
	delete[] rest_order;

}

bool check_inputs(vector<vector<int> > points, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sum = 0;
		for (int j = 0; j < size; j++) {
			if (points[i][j] < 0) {
				cout << " error in inputs, matrix points must be non-negative" << endl;
				return false;
			}
			sum += points[i][j];
		}
		if (sum != 100) {
			cout << "Error in matrix p, row " << i << endl << "Sum of each row must be 100" << endl;
			return false;
		}
	}
	return true;
}
int main() {
	srand (time(NULL));
	vector<vector<int> > points;
	int size = read_input(points, input_data_file);
	if(check_inputs(points, size))
		solve(points, size);
	else
		cout << "cannot run this input"

	return 0;
}