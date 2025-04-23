package CPUSchedulingProject;

import java.util.*;

class Process {
    int pid, arrivalTime, burstTime, priority, waitingTime, turnaroundTime, remainingTime;

    public Process(int pid, int arrivalTime, int burstTime, int priority) {
        this.pid = pid;
        this.arrivalTime = arrivalTime;
        this.burstTime = burstTime;
        this.remainingTime = burstTime;
        this.priority = priority;
    }
}

public class CPUScheduler {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        List<Process> processes = new ArrayList<>();

        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();

        for (int i = 0; i < n; i++) {
            System.out.println("Process " + (i + 1));
            System.out.print("Arrival Time: ");
            int at = sc.nextInt();
            System.out.print("Burst Time: ");
            int bt = sc.nextInt();
            System.out.print("Priority (Lower is higher priority): ");
            int p = sc.nextInt();
            processes.add(new Process(i + 1, at, bt, p));
        }

        System.out.println("\nChoose Scheduling Algorithm:");
        System.out.println("1. FCFS\n2. SJF\n3. Priority\n4. Round Robin");
        int choice = sc.nextInt();

        switch (choice) {
            case 1: fcfs(new ArrayList<>(processes)); break;
            case 2: sjf(new ArrayList<>(processes)); break;
            case 3: priorityScheduling(new ArrayList<>(processes)); break;
            case 4:
                System.out.print("Enter Time Quantum: ");
                int tq = sc.nextInt();
                roundRobin(new ArrayList<>(processes), tq);
                break;
            default: System.out.println("Invalid choice");
        }
    }

    static void fcfs(List<Process> list) {
        list.sort(Comparator.comparingInt(p -> p.arrivalTime));
        int currentTime = 0;
        for (Process p : list) {
            currentTime = Math.max(currentTime, p.arrivalTime);
            p.waitingTime = currentTime - p.arrivalTime;
            currentTime += p.burstTime;
            p.turnaroundTime = p.waitingTime + p.burstTime;
        }
        printTable(list, "FCFS");
    }

    static void sjf(List<Process> list) {
        List<Process> completed = new ArrayList<>();
        int time = 0;

        while (completed.size() < list.size()) {
            Process shortest = list.stream()
                .filter(p -> p.arrivalTime <= time && !completed.contains(p))
                .min(Comparator.comparingInt(p -> p.burstTime))
                .orElse(null);

            if (shortest == null) {
                time++;
                continue;
            }

            shortest.waitingTime = time - shortest.arrivalTime;
            time += shortest.burstTime;
            shortest.turnaroundTime = shortest.waitingTime + shortest.burstTime;
            completed.add(shortest);
        }
        printTable(completed, "SJF");
    }

    static void priorityScheduling(List<Process> list) {
        List<Process> completed = new ArrayList<>();
        int time = 0;

        while (completed.size() < list.size()) {
            Process highest = list.stream()
                .filter(p -> p.arrivalTime <= time && !completed.contains(p))
                .min(Comparator.comparingInt(p -> p.priority))
                .orElse(null);

            if (highest == null) {
                time++;
                continue;
            }

            highest.waitingTime = time - highest.arrivalTime;
            time += highest.burstTime;
            highest.turnaroundTime = highest.waitingTime + highest.burstTime;
            completed.add(highest);
        }
        printTable(completed, "Priority");
    }

    static void roundRobin(List<Process> list, int tq) {
        Queue<Process> queue = new LinkedList<>();
        List<Process> completed = new ArrayList<>();
        int time = 0;
        list.sort(Comparator.comparingInt(p -> p.arrivalTime));
        int index = 0;

        while (completed.size() < list.size()) {
            while (index < list.size() && list.get(index).arrivalTime <= time)
                queue.add(list.get(index++));

            if (queue.isEmpty()) {
                time++;
                continue;
            }

            Process current = queue.poll();
            int execTime = Math.min(tq, current.remainingTime);
            current.remainingTime -= execTime;
            time += execTime;

            while (index < list.size() && list.get(index).arrivalTime <= time)
                queue.add(list.get(index++));

            if (current.remainingTime == 0) {
                current.turnaroundTime = time - current.arrivalTime;
                current.waitingTime = current.turnaroundTime - current.burstTime;
                completed.add(current);
            } else {
                queue.add(current);
            }
        }
        printTable(completed, "Round Robin");
    }

    static void printTable(List<Process> list, String title) {
        System.out.println("\n=== " + title + " Scheduling ===");
        System.out.printf("%-5s %-12s %-10s %-10s %-10s\n", "PID", "Arrival", "Burst", "Waiting", "Turnaround");
        int totalWT = 0, totalTAT = 0;
        for (Process p : list) {
            System.out.printf("%-5d %-12d %-10d %-10d %-10d\n", p.pid, p.arrivalTime, p.burstTime, p.waitingTime, p.turnaroundTime);
            totalWT += p.waitingTime;
            totalTAT += p.turnaroundTime;
        }
        System.out.printf("Average Waiting Time: %.2f\n", totalWT / (float) list.size());
        System.out.printf("Average Turnaround Time: %.2f\n", totalTAT / (float) list.size());
    }
}
