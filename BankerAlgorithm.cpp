#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// function prints 2D matrix
void print(vector<vector<int>> v) {
	cout << endl;
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[0].size(); j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

// safety algorithm
void perform_safety_algorithm(int &resources,int &processes,int &found_safe,vector<int>& sequence,vector<int>& available, vector<vector<int>> &allocation, vector<vector<int>>& Need){
	int i = 0, j = 0;
	while (sequence.size() < processes) {
		int size = sequence.size();
		for (i = 0; i < processes; i++) {
			for (j = 0; j < resources; j++) {
				if (find(sequence.begin(), sequence.end(), i) != sequence.end() || available[j] < Need[i][j]) {
					break;
				}
			}
			if (j == resources) {
				sequence.push_back(i);
				for (int k = 0; k < resources; k++) {
					available[k] = (available[k] + allocation[i][k]);
				}
				break;
			}
		}
		if (size == sequence.size()) {
			found_safe = 0;
			break;
		}
	}
}


int main()
{
	int processes{}, resources{};
	cout << "Enter the number of Processes:" << endl;
	cin >> processes;
	cout << "Enter the number of Resources:" << endl;
	cin >> resources;

	// Declaring Used Data structures and variables.
	vector <vector<int>> allocation(processes, vector<int>(resources));
	vector <vector<int>> Max(processes, vector<int>(resources));
	vector <vector<int>> Need(processes, vector<int>(resources));
	vector <int> available(resources);
	vector <int> sequence;
	int found_safe = 1;
	int granted = 0;
	int i = 0, j = 0;
	int sample{};

	// Taking the allocation matrix
	cout << "Enter The allocation matrix row by row:" << endl;
	for (i = 0; i < processes; i++) {
		for (j = 0; j < resources; j++) {
			cin >> sample;
			allocation[i][j] = sample;
		}
	}
	// Taking the Max matrix
	cout << "Enter The Max matrix row by row:" << endl;
	for (i = 0; i < processes; i++) {
		for (j = 0; j < resources; j++) {
			cin >> sample;
			Max[i][j] = sample;
		}
	}
	// Taking the available vector
	cout << "Enter The available vector:" << endl;
	for (i = 0; i < resources; i++) {
		cin >> sample;
		available[i] = sample;
	}
	// Calculating Need matrix
	for (i = 0; i < processes; i++) {
		for (j = 0; j < resources; j++) {
			Need[i][j] = Max[i][j] - allocation[i][j];
		}
	}
	cout << endl;
	cout << "Need Matrix: ";
	print(Need);

	char c;
	char x;
	do {
		cout << endl << "if you want to perform regualar safety algorithm PRESS 'S' or if you want to check if a certain process can be granted immediately PRESS 'R'." << endl;
		cin >> c;
		// Safety Algorithm
		if (c == 'S') {
			perform_safety_algorithm(resources, processes, found_safe, sequence, available, allocation, Need);
			cout << endl << "if you want to know whether the system is in safety state or not, PRESS 'Y'. " << endl;
			cin >> x;
			// checking for safety sequence
			if (x == 'Y') {
				if (found_safe) {
					cout << "Yes, Safe state found." << endl;
					cout << "Safe Sequence: " << "< ";
					for (auto const& num : sequence) {
						cout << "P" << num << " ";
					}
					cout << ">" << endl;
				}
				else {
					cout << "No Safe state found." << endl;
				}
			}
		}
		// Resource request Algorithm
		else if (c == 'R') {
			int pr_num{};
			vector<int> res_values(resources);
			cout << "Enter the number of process you want to check it's granting." << endl;
			cin >> pr_num;
			cout << "Enter the process wanted resources values." << endl;
			// Taking the process input resources
			for (i = 0; i < resources; i++) {
				cin >> sample;
				res_values[i] = sample;
			}
			// checking if the inputed process resources are above it's Need or the inputed process resources are above the currently available.
			for (i = 0; i < resources; i++) {
				if (Need[pr_num][i] < res_values[i] || available[i] < res_values[i]) {
					granted = 0;
					goto finish;
				}
			}
			// first we need to modify available,Need and allocation matrices.
			for (i = 0; i < resources; i++) {
				Need[pr_num][i] -= res_values[i];
				available[i] -= res_values[i];
				allocation[pr_num][i] += res_values[i];
			}
			// performing safety algorithm
			perform_safety_algorithm(resources, processes, found_safe, sequence, available, allocation, Need);
			// if safe seq found means this process can be granted.
			if (found_safe == 1)
				granted = 1;
		finish:
			if (granted) {
				cout << "P" << pr_num << " can be granted immedialtely." << endl;
				cout << "Safe Sequence: " << "< ";
				for (auto const& num : sequence) {
					cout << "P" << num << " ";
				}
				cout << ">" << endl;
			}
			else {
				cout << "P" << pr_num << " CAN'T be granted immediately." << endl;
			}
		}
		else {
			cout << "Invalid input, Please Enter valid character ('R' or 'S'). " ;
		}
	} while (c != 'R' && c != 'S');
}
