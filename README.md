# CPU Scheduling Simulator in Java

This project simulates four classic **CPU scheduling algorithms**:

- **FCFS** (First-Come, First-Served)
- **SJF** (Shortest Job First - Non-preemptive)
- **Priority Scheduling**
- **Round Robin**

It is implemented using **Java**, with a simple CLI-based input/output system. The user can enter process details and select the algorithm to simulate.

**Sample Output**

Enter number of processes: 3
Process 1
Arrival Time: 0
Burst Time: 5
Priority: 2
Process 2
Arrival Time: 1
Burst Time: 3
Priority: 1
Process 3
Arrival Time: 2
Burst Time: 8
Priority: 3

Choose Scheduling Algorithm:
1. FCFS
2. SJF
3. Priority
4. Round Robin
3

=== Priority Scheduling ===
PID   Arrival      Burst      Waiting    Turnaround 
1     0            5          3          8         
2     1            3          0          3         
3     2            8          6          14        

Average Waiting Time: 3.00
Average Turnaround Time: 8.33

