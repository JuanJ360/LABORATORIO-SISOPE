#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;

struct Result {
    long long total_distance;
    vector<int> sequence;
};

Result calculate_fcfs(int head, const vector<int>& requests) {
    long long distance = 0;
    int current = head;
    vector<int> seq;

    for (int r : requests) {
        distance += abs(r - current);
        current = r;
        seq.push_back(current);
    }
    return {distance, seq};
}

Result calculate_scan(int head, vector<int> requests, int disk_size = 5000) {
    long long distance = 0;
    int current = head;
    vector<int> seq;

    vector<int> left, right;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());


    for (int r : right) {
        distance += abs(r - current);
        current = r;
        seq.push_back(current);
    }

    distance += abs((disk_size - 1) - current);
    current = disk_size - 1;
    seq.push_back(current);


    reverse(left.begin(), left.end());
    for (int r : left) {
        distance += abs(r - current);
        current = r;
        seq.push_back(current);
    }

    return {distance, seq};
}

Result calculate_cscan(int head, vector<int> requests, int disk_size = 5000) {
    long long distance = 0;
    int current = head;
    vector<int> seq;

    vector<int> left, right;
    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());


    for (int r : right) {
        distance += abs(r - current);
        current = r;
        seq.push_back(current);
    }


    distance += abs((disk_size - 1) - current);
    current = disk_size - 1;
    seq.push_back(current);


    distance += abs(0 - current);
    current = 0;
    seq.push_back(current);


    for (int r : left) {
        distance += abs(r - current);
        current = r;
        seq.push_back(current);
    }

    return {distance, seq};
}

void save_to_csv(string filename, const vector<int>& seq) {
    ofstream file(filename);
    for (size_t i = 0; i < seq.size(); ++i) {
        file << i << "," << seq[i] << "\n";
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <initial_head_position>" << endl;
        return 1;
    }

    int head = stoi(argv[1]);
    if (head < 0 || head >= 5000) {
        cout << "Error: Head position must be between 0 and 4999." << endl;
        return 1;
    }


    vector<int> requests;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 4999);

    for (int i = 0; i < 1000; ++i) {
        requests.push_back(dis(gen));
    }

    Result fcfs_res = calculate_fcfs(head, requests);
    Result scan_res = calculate_scan(head, requests);
    Result cscan_res = calculate_cscan(head, requests);

    cout << "Initial Head Position: " << head << endl;
    cout << string(30, '-') << endl;
    cout << "FCFS Total Movement:  " << fcfs_res.total_distance << endl;
    cout << "SCAN Total Movement:  " << scan_res.total_distance << endl;
    cout << "C-SCAN Total Movement: " << cscan_res.total_distance << endl;
    cout << string(30, '-') << endl;

    save_to_csv("fcfs_seq.csv", fcfs_res.sequence);
    save_to_csv("scan_seq.csv", scan_res.sequence);
    save_to_csv("cscan_seq.csv", cscan_res.sequence);

    cout << "\nSequences saved to CSV files (fcfs_seq.csv, scan_seq.csv, cscan_seq.csv) for visualization." << endl;

    return 0;
}
