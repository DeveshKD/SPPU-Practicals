#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class FCFS_Scheduler {
public:
    struct Process {
        int id;
        int arrivalTime;
        int burstTime;
        int priority;
        int waitingTime;
        int turnaroundTime;
        int remainingTime; // For round robin
    };

    FCFS_Scheduler(int numProcesses) : numProcesses(numProcesses) {
        processes.resize(numProcesses);
    }

    void generateProcesses() {
        // Fixed values for demonstration purposes
        processes = {
            {1 , 0 , 3 , 1, 0, 0, 3},  // Process 1
            {2, 2, 6, 2, 0, 0, 6},      // Process 2
            {3, 4, 4, 3, 0, 0, 4},      // Process 3
            {4, 5, 5, 4, 0, 0, 5},      // Process 4
            {5, 7, 2, 5, 0, 0, 2}       // Process 5
        };
    }

    void displayProcesses() const {
        cout << left << setw(10) << "Process ID"
             << setw(15) << "Arrival Time"
             << setw(15) << "Burst Time"
             << setw(10) << "Priority" << endl;

        for (const auto& process : processes) {
            cout << left << setw(10) << process.id
                 << setw(15) << process.arrivalTime
                 << setw(15) << process.burstTime
                 << setw(10) << process.priority << endl;
        }
    }

    void calculateTimes() {
        int totalWaitingTime = 0;
        int totalTurnaroundTime = 0;

        for (const auto& process : processes) {
            totalWaitingTime += process.waitingTime;
            totalTurnaroundTime += process.turnaroundTime;
        }

        averageWaitingTime = static_cast<float>(totalWaitingTime) / processes.size();
        averageTurnaroundTime = static_cast<float>(totalTurnaroundTime) / processes.size();
    }

    void printGanttChart(const string& title) const {
        cout << title << endl;
        cout << "Gantt Chart:" << endl;
        cout << "|";
        for (const auto& process : processes) {
            cout << " P" << process.id << " |";
        }
        cout << endl;

        int currentTime = 0;
        cout << "0";
        for (const auto& process : processes) {
            currentTime += process.burstTime;
            cout << setw(3) << currentTime;
        }
        cout << endl;
    }

    void fcfs() {
        // Sort processes by arrival time
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        int currentTime = 0;
        for (auto& process : processes) {
            if (currentTime < process.arrivalTime) {
                currentTime = process.arrivalTime; // Wait for the process to arrive
            }
            process.waitingTime = currentTime - process.arrivalTime;
            process.turnaroundTime = process.waitingTime + process.burstTime;

            currentTime += process.burstTime;
        }

        calculateTimes();
    }

    void sjf() {
        // Sort processes by arrival time
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        vector<Process> sjfProcesses;
        int currentTime = 0;
        int index = 0;

        while (index < processes.size() || !sjfProcesses.empty()) {
            while (index < processes.size() && processes[index].arrivalTime <= currentTime) {
                sjfProcesses.push_back(processes[index]);
                index++;
            }

            if (sjfProcesses.empty()) {
                currentTime = processes[index].arrivalTime; // Jump to the next process arrival
                continue;
            }

            // Select the process with the shortest burst time
            auto minProcessIt = min_element(sjfProcesses.begin(), sjfProcesses.end(), [](const Process& a, const Process& b) {
                return a.burstTime < b.burstTime;
            });

            Process process = *minProcessIt;
            sjfProcesses.erase(minProcessIt);

            currentTime += process.burstTime;
            process.waitingTime = currentTime - process.arrivalTime - process.burstTime;
            process.turnaroundTime = process.waitingTime + process.burstTime;
            processes[process.id - 1] = process; // Update the original process vector
        }

        calculateTimes();
    }

    void priorityScheduling() {
        // Sort processes by arrival time
        sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        vector<Process> priorityProcesses;
        int currentTime = 0;
        int index = 0;

        while (index < processes.size() || !priorityProcesses.empty()) {
            while (index < processes.size() && processes[index].arrivalTime <= currentTime) {
                priorityProcesses.push_back(processes[index]);
                index++;
            }

            if (priorityProcesses.empty()) {
                currentTime = processes[index].arrivalTime; // Jump to the next process arrival
                continue;
            }

            // Select the process with the highest priority (lower number means higher priority)
            auto minProcessIt = min_element(priorityProcesses.begin(), priorityProcesses.end(), [](const Process& a, const Process& b) {
                return a.priority < b.priority;
            });

            Process process = *minProcessIt;
            priorityProcesses.erase(minProcessIt);

            currentTime += process.burstTime;
            process.waitingTime = currentTime - process.arrivalTime - process.burstTime;
            process.turnaroundTime = process.waitingTime + process.burstTime;
            processes[process.id - 1] = process; // Update the original process vector
        }

        calculateTimes();
    }

    void roundRobin(int quantum) {
        queue<Process> rrQueue;
        vector<Process> rrProcesses = processes;
        int currentTime = 0;
        int index = 0;

        while (index < rrProcesses.size() || !rrQueue.empty()) {
            while (index < rrProcesses.size() && rrProcesses[index].arrivalTime <= currentTime) {
                rrQueue.push(rrProcesses[index]);
                index++;
            }

            if (rrQueue.empty()) {
                currentTime = rrProcesses[index].arrivalTime; // Jump to the next process arrival
                continue;
            }

            Process process = rrQueue.front();
            rrQueue.pop();

            int timeSlice = min(process.remainingTime, quantum);
            currentTime += timeSlice;
            process.remainingTime -= timeSlice;

            if (process.remainingTime == 0) {
                process.turnaroundTime = currentTime - process.arrivalTime;
                process.waitingTime = process.turnaroundTime - process.burstTime;
                processes[process.id - 1] = process; // Update the original process vector
            } else {
                rrQueue.push(process); // Re-queue if not finished
            }
        }
    calculateTimes();
    }
    void scheduleAll() {
        // Display the initial process table
        displayProcesses();
        cout << endl; // Leave a line
       // FCFS Scheduling
        fcfs();
        cout << "CPU Scheduling by FCFS" << endl << endl;
        printGanttChart("Gantt Chart by FCFS");
        cout << "Average Turnaround Time by FCFS: " << averageTurnaroundTime << endl;
        cout << "Average Waiting Time by FCFS: " << averageWaitingTime << endl;
        cout << endl; // Leave a line
      // SJF Scheduling
        sjf();
        cout << "CPU Scheduling by SJF" << endl << endl;
        printGanttChart("Gantt Chart by SJF");
        cout << "Average Turnaround Time by SJF: " << averageTurnaroundTime << endl;
        cout << "Average Waiting Time by SJF: " << averageWaitingTime << endl;
        cout << endl; // Leave a line
      // Priority Scheduling
        priorityScheduling();
        cout << "CPU Scheduling by Priority Scheduling" << endl << endl;
        printGanttChart("Gantt Chart by Priority Scheduling");
        cout << "Average Turnaround Time by Priority: " << averageTurnaroundTime << endl;
        cout << "Average Waiting Time by Priority: " << averageWaitingTime << endl;
        cout << endl; // Leave a line
     // Round Robin Scheduling
        for (auto& proc : processes) {
            proc.remainingTime = proc.burstTime; // Reset remaining time for Round Robin
        }
        roundRobin(3); // Quantum time for Round Robin
        cout << "CPU Scheduling by Round Robin" << endl << endl;
        printGanttChart("Gantt Chart by Round Robin");
        cout << "Average Turnaround Time by Round Robin: " << averageTurnaroundTime << endl;
        cout << "Average Waiting Time by Round Robin: " << averageWaitingTime << endl;
        cout << endl; // Leave a line
    }
private:
    int numProcesses;
    vector<Process> processes;
    float averageWaitingTime;
    float averageTurnaroundTime;
};
int main() {
    cout << endl;
    cout << "Name: Devesh Danthara" << endl;
    cout << "Roll No: A-28" << endl;
    cout << endl;
    const int numProcesses = 5; // Fixed number of processes
    FCFS_Scheduler scheduler(numProcesses);
    scheduler.generateProcesses();
    scheduler.scheduleAll(); // Execute all scheduling methods
    return 0;
}