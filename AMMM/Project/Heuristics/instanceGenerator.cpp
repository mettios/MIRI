#include "config.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Generates a random sequence that sums to 100
vector<int> random_algorithm1(int size) {
    int max = 101;
    vector<int> random_nums;
    int sum = 0;

    for (int i = 0; i < (size - 1); i++) {
        int value = (max == 0) ? 0 : rand() % max;
        random_nums.push_back(value);
        max -= value;
        sum += value;
    }
    random_nums.push_back(100 - sum);

    return random_nums;
}

// Alternative random sequence generator
vector<int> random_algorithm2(int size) {
    vector<int> random_nums_t = {0, 100};
    vector<int> random_nums;

    for (int i = 0; i < size - 1; i++) {
        random_nums_t.push_back(rand() % 101);
    }
    sort(random_nums_t.begin(), random_nums_t.end());

    for (int i = 1; i <= size; i++) {
        random_nums.push_back(random_nums_t[i] - random_nums_t[i - 1]);
    }

    if (accumulate(random_nums.begin(), random_nums.end(), 0) != 100) {
        cout << "Error: sum != 100" << endl;
    }

    return random_nums;
}

// Generates instances and writes to file
void generateInstances() {
    for (int i = 0; i < number_of_instances; i++) {
        ofstream file("instances/" + instance_file_name + to_string(i + 1) + "_" + to_string(instance_size) + ".txt");
        file << "n = " << instance_size << ";" << endl;
        file << "\np = \n\t[" << endl;

        int rand_alg = rand() % 2;
        for (int row = 0; row < instance_size; row++) {
            file << "\t\t[ ";
            vector<int> random_nums = rand_alg ? random_algorithm2(instance_size) : random_algorithm1(instance_size);

            for (int j = 0; j < instance_size; j++) {
                file << random_nums[j] << " ";
            }
            file << " ]" << endl;
        }
        file << "\t];";
        file.close();
    }
}

int main() {
    srand(time(NULL));
    generateInstances();
    return 0;
}
