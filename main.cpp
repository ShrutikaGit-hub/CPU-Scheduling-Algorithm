#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Process {
    int id, arrivalTime, burstTime, priority;
    int startTime = 0, completionTime = 0, waitingTime = 0, turnaroundTime = 0;
    int remainingTime = 0, executedTime = 0;
};

// Helper: Reset process state
void resetProcesses(vector<Process>& processes) {
    for (auto& p : processes) {
        p.startTime = p.completionTime = p.waitingTime = p.turnaroundTime = p.executedTime = 0;
        p.remainingTime = p.burstTime;
    }
}

// FCFS
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
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// SPN (SJF non-preemptive)
void spn(vector<Process> processes) {
    cout << "\n--- SPN Scheduling ---\n";
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
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// SRT (Shortest Remaining Time)
void srt(vector<Process> processes) {
    cout << "\n--- SRT Scheduling ---\n";
    int n = processes.size(), time = 0, completed = 0;
    vector<bool> done(n, false);
    vector<Process> result(n);

    while (completed < n) {
        int idx = -1, minTime = 1e9;
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrivalTime <= time && processes[i].remainingTime < minTime && processes[i].remainingTime > 0) {
                idx = i;
                minTime = processes[i].remainingTime;
            }
        }

        if (idx != -1) {
            if (processes[idx].executedTime == 0) processes[idx].startTime = time;
            processes[idx].remainingTime--;
            processes[idx].executedTime++;
            time++;

            if (processes[idx].remainingTime == 0) {
                processes[idx].completionTime = time;
                processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result[idx] = processes[idx];
                done[idx] = true;
                completed++;
            }
        } else {
            time++;
        }
    }

    for (auto& p : result) {
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// HRRN
void hrrn(vector<Process> processes) {
    cout << "\n--- HRRN Scheduling ---\n";
    int time = 0;
    vector<Process> completed;
    while (!processes.empty()) {
        auto it = max_element(processes.begin(), processes.end(), [&](Process a, Process b) {
            if (a.arrivalTime > time && b.arrivalTime > time) return a.arrivalTime < b.arrivalTime;
            if (a.arrivalTime > time) return true;
            if (b.arrivalTime > time) return false;
            float r1 = (float)(time - a.arrivalTime + a.burstTime) / a.burstTime;
            float r2 = (float)(time - b.arrivalTime + b.burstTime) / b.burstTime;
            return r1 < r2;
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
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// Round Robin
void roundRobin(vector<Process> processes, int quantum) {
    cout << "\n--- Round Robin Scheduling (Quantum = " << quantum << ") ---\n";
    int time = 0, completed = 0, n = processes.size();
    vector<Process> result(n);
    vector<int> remainingTime(n);
    vector<bool> added(n, false);
    queue<int> q;

    for (int i = 0; i < n; i++) remainingTime[i] = processes[i].burstTime;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!added[i] && processes[i].arrivalTime <= time) {
                q.push(i);
                added[i] = true;
            }
        }

        if (!q.empty()) {
            int idx = q.front(); q.pop();
            if (processes[idx].executedTime == 0) processes[idx].startTime = time;
            int exec = min(quantum, remainingTime[idx]);
            time += exec;
            processes[idx].executedTime += exec;
            remainingTime[idx] -= exec;

            for (int i = 0; i < n; i++) {
                if (!added[i] && processes[i].arrivalTime <= time) {
                    q.push(i);
                    added[i] = true;
                }
            }

            if (remainingTime[idx] > 0) {
                q.push(idx);
            } else {
                processes[idx].completionTime = time;
                processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result[idx] = processes[idx];
                completed++;
            }
        } else {
            time++;
        }
    }

    for (auto& p : result) {
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
    }
}

// Feedback Queue
void feedbackQueue(vector<Process> processes, int levels, int quantumBase) {
    cout << "\n--- Feedback Queue Scheduling (" << levels << " levels, base quantum = " << quantumBase << ") ---\n";
    vector<queue<Process>> queues(levels);
    int time = 0, completed = 0, n = processes.size();
    vector<bool> added(n, false);
    vector<Process> result(n);

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!added[i] && processes[i].arrivalTime <= time) {
                queues[0].push(processes[i]);
                added[i] = true;
            }
        }

        bool idle = true;
        for (int lvl = 0; lvl < levels; lvl++) {
            if (!queues[lvl].empty()) {
                Process current = queues[lvl].front();
                queues[lvl].pop();
                int idx = current.id - 1;
                int q = quantumBase << lvl;
                int execTime = min(q, processes[idx].remainingTime);
                if (processes[idx].executedTime == 0) processes[idx].startTime = time;
                processes[idx].remainingTime -= execTime;
                processes[idx].executedTime += execTime;
                time += execTime;

                for (int i = 0; i < n; i++) {
                    if (!added[i] && processes[i].arrivalTime <= time) {
                        queues[0].push(processes[i]);
                        added[i] = true;
                    }
                }

                if (processes[idx].remainingTime > 0) {
                    if (lvl < levels - 1) queues[lvl + 1].push(processes[idx]);
                    else queues[lvl].push(processes[idx]);
                } else {
                    processes[idx].completionTime = time;
                    processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
                    processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                    result[idx] = processes[idx];
                    completed++;
                }
                idle = false;
                break;
            }
        }
        if (idle) time++;
    }

    for (const auto& p : result) {
        cout << "P" << p.id << ": WT=" << p.waitingTime << ", TAT=" << p.turnaroundTime << "\n";
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
    resetProcesses(processes);
    spn(processes);
    resetProcesses(processes);
    srt(processes);
    resetProcesses(processes);
    hrrn(processes);
    resetProcesses(processes);
    roundRobin(processes, 3);
    resetProcesses(processes);
    feedbackQueue(processes, 1, 1);  // FB-1
    resetProcesses(processes);
    feedbackQueue(processes, 3, 1);  // FB-2i

    return 0;
}
