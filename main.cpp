#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Process {
    int id, arrivalTime, burstTime, priority;
    int startTime = 0, completionTime = 0, waitingTime = 0, turnaroundTime = 0;
};

// First Come First Serve (FCFS)
void fcfs(vector<Process> processes) {
    cout << "\n--- FCFS Scheduling ---\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;
    for (auto& p : processes) {
        time = max(time, p.arrivalTime);
        p.startTime = time;
        time += p.burstTime;
        p.completionTime = time;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
    }

    for (auto& p : processes) {
        cout << "P" << p.id << ": WT=" << p.waitingTime
             << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// Shortest Job First (SJF)
void sjf(vector<Process> processes) {
    cout << "\n--- SJF Scheduling ---\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;
    vector<Process> completed;
    while (!processes.empty()) {
        auto it = min_element(processes.begin(), processes.end(), [&](Process a, Process b) {
            if (a.arrivalTime > time && b.arrivalTime > time) return a.arrivalTime < b.arrivalTime;
            if (a.arrivalTime > time) return false;
            if (b.arrivalTime > time) return true;
            return a.burstTime < b.burstTime;
        });

        Process p = *it;
        processes.erase(it);
        time = max(time, p.arrivalTime);
        p.startTime = time;
        time += p.burstTime;
        p.completionTime = time;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        completed.push_back(p);
    }

    for (auto& p : completed) {
        cout << "P" << p.id << ": WT=" << p.waitingTime
             << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// Priority Scheduling
void priorityScheduling(vector<Process> processes) {
    cout << "\n--- Priority Scheduling ---\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int time = 0;
    vector<Process> completed;
    while (!processes.empty()) {
        auto it = min_element(processes.begin(), processes.end(), [&](Process a, Process b) {
            if (a.arrivalTime > time && b.arrivalTime > time) return a.arrivalTime < b.arrivalTime;
            if (a.arrivalTime > time) return false;
            if (b.arrivalTime > time) return true;
            return a.priority < b.priority;
        });

        Process p = *it;
        processes.erase(it);
        time = max(time, p.arrivalTime);
        p.startTime = time;
        time += p.burstTime;
        p.completionTime = time;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        completed.push_back(p);
    }

    for (auto& p : completed) {
        cout << "P" << p.id << ": WT=" << p.waitingTime
             << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// Round Robin
void roundRobin(vector<Process> processes, int quantum) {
    cout << "\n--- Round Robin Scheduling (Quantum = " << quantum << ") ---\n";
    queue<Process> q;
    vector<int> remainingTime(processes.size());
    int time = 0, completed = 0;
    vector<bool> added(processes.size(), false);
    int n = processes.size();
    vector<Process> result(n);

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!added[i] && processes[i].arrivalTime <= time) {
                q.push(processes[i]);
                remainingTime[i] = processes[i].burstTime;
                added[i] = true;
            }
        }

        if (!q.empty()) {
            Process current = q.front();
            q.pop();
            int idx = current.id - 1;

            int execTime = min(quantum, remainingTime[idx]);
            remainingTime[idx] -= execTime;
            time += execTime;

            for (int i = 0; i < n; i++) {
                if (!added[i] && processes[i].arrivalTime <= time) {
                    q.push(processes[i]);
                    remainingTime[i] = processes[i].burstTime;
                    added[i] = true;
                }
            }

            if (remainingTime[idx] > 0) {
                q.push(current);
            } else {
                result[idx] = current;
                result[idx].completionTime = time;
                result[idx].turnaroundTime = time - current.arrivalTime;
                result[idx].waitingTime = result[idx].turnaroundTime - current.burstTime;
                completed++;
            }
        } else {
            time++;
        }
    }

    for (const auto& p : result) {
        cout << "P" << p.id << ": WT=" << p.waitingTime
             << ", TAT=" << p.turnaroundTime << "\n";
    }
}

int main() {
    vector<Process> processes = {
        {1, 0, 5, 2},
        {2, 1, 3, 1},
        {3, 2, 8, 4},
        {4, 3, 6, 3}
    };

    fcfs(processes);
    sjf(processes);
    priorityScheduling(processes);
    roundRobin(processes, 3);

    return 0;
}
