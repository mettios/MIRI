#include <string>

using namespace std;

// Algorithm methods (Do not modify the enum values)
enum Methods { greedy, grasp };

// GRASP method parameter (ignored in greedy mode)
#define ALPHA 0.1

// Enable or disable local search
bool local_search_en = true;

// Input and output file paths
string input_data_file = "instances/instance1_41.txt";
string output_sol_file_name = "41_out.txt";
string output_sol_file = "solutions/" + output_sol_file_name;

// Algorithm selection (set to "greedy" or "grasp")
Methods method = grasp;

/* Instance Generator Settings */
int instance_size = 5;                 // Size of each instance
string instance_file_name = "instance"; // Base instance name
int number_of_instances = 1;            // Number of instances to generate