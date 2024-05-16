//Baylasan Al-Badawi
#include<iostream>
#include <algorithm> 
#include <queue> 
#include<iomanip>
#include<climits>
#include<fstream>
using namespace std;
struct PCB
{
    int pid;
    int at;
    int bt;
    int ct, wt, tat, rt, start_time;
    int bt_remaining;
}ps[10];
bool comparatorAT(struct PCB a, struct PCB b)
{
    int x = a.at;
    int y = b.at;
    return x < y;
    //    if(x > y)
    //      return false;  // Apply sorting
    //    return true;   // no sorting
}
void generateGanttChart(PCB ps[], int n) {
    cout << "Gantt Chart:" << endl;
    // Assuming each '-' represents one unit of time
    for (int i = 0; i < n; ++i) {
        cout << "|";
        for (int j = 0; j < ps[i].bt; ++j) cout << "-";
        cout << "| P" << ps[i].pid;
        cout << "|-CS|";

    }
    cout << endl;

    // Displaying the start time and finish time below the Gantt chart
    int total_time = 0;
    cout << "0";
    for (int i = 0; i < n; ++i) {
        total_time += ps[i].bt;
        cout << setw(ps[i].bt * 2) << total_time;
    }
    cout << endl;
}

bool comparatorPID(struct PCB a, struct PCB b)
{
    int x = a.pid;
    int y = b.pid;
    return x < y;
}
void readInputFromFile(int CS, int AT, int BT) {
    ifstream fin("process.txt");
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(1);
    }
    int cs, q;
    fin >> cs >> q;
    int at, bt;
    int i = 0;
    while (fin >> at >> bt) {
        ps[i].pid = i;
        ps[i].at = at;
        ps[i].bt = bt;
        ps[i].bt_remaining = bt;
        i++;
    }
    AT = at; BT = bt; CS = cs; 
    fin.close();

}
void readInputFromFile(int CS, int Q, int AT, int BT) {
    ifstream fin("process.txt");
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(1);
    }
    int cs, q;
    fin >> cs >> q;
    int at, bt;
    int i = 0;
    while (fin >> at >> bt) {
        ps[i].pid = i;
        ps[i].at = at;
        ps[i].bt = bt;
        ps[i].bt_remaining = bt;
        i++; 
        AT = at; BT = bt; CS = cs; Q = q;

    }
    fin.close();

}
void SRT(PCB ps[], int n, int CS, int AT, int BT) {
    //int n;
    float bt_remaining[10];
    bool is_completed[10] = { false }, is_first_process = true;
    int current_time = 0;
    int completed = 0;;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0, total_idle_time = 0, length_cycle, prev = 0;
    float cpu_utilization;
    int max_completion_time, min_arrival_time;

    cout << fixed << setprecision(2);
    readInputFromFile(CS, AT, BT);
    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter Process" << i << "Arrival Time: ";
        AT = ps[i].at;
        ps[i].pid = i;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "\nProcess" << i << "Burst Time: ";
        BT = ps[i].bt;
        bt_remaining[i] = ps[i].bt;
    }

    while (completed != n)
    {
        //find process with min. burst time in ready queue at current time
        int min_index = -1;
        int minimum = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (ps[i].at <= current_time && is_completed[i] == false) {
                if (bt_remaining[i] < minimum) {
                    minimum = bt_remaining[i];;
                    min_index = i;
                }
                if (bt_remaining[i] == minimum) {
                    if (ps[i].at < ps[min_index].at) {
                        minimum = bt_remaining[i];;
                        min_index = i;
                    }
                }
            }
        }


        if (min_index == -1)
        {
            current_time++;
        }
        else
        {
            if (bt_remaining[min_index] == ps[min_index].bt)
            {
                ps[min_index].start_time = current_time;
                total_idle_time += (is_first_process == true) ? 0 : (ps[min_index].start_time - prev);
                is_first_process = false;
            }
            bt_remaining[min_index] -= 1;
            current_time++;
            prev = current_time;
            if (bt_remaining[min_index] == 0)
            {
                ps[min_index].ct = current_time;
                ps[min_index].tat = ps[min_index].ct - ps[min_index].at;
                ps[min_index].wt = ps[min_index].tat - ps[min_index].bt;
                ps[min_index].rt = ps[min_index].start_time - ps[min_index].at;


                sum_tat += ps[min_index].tat;
                sum_wt += ps[min_index].wt;
                sum_rt += ps[min_index].rt;
                completed++;
                is_completed[min_index] = true;


            }
        }
    }
    //Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;
    min_arrival_time = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        max_completion_time = max(max_completion_time, ps[i].ct);
        min_arrival_time = min(min_arrival_time, ps[i].at);
    }
    length_cycle = max_completion_time - min_arrival_time;


    //Output
    cout << "\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t" << ps[i].bt << "\t\t" << ps[i].ct << "\t" << ps[i].tat << "\t" << ps[i].wt << "\t" << ps[i].rt << endl;
    cout << endl;

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;
    generateGanttChart(ps, n);
    cout << "\nAverage Turn Around time= " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time= " << (float)sum_wt / n;
    cout << "\nAverage Response Time= " << (float)sum_rt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100 << endl;
};
void FCFS(PCB ps[], int n, int CS, int AT, int BT) {
    //int n;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;
    int length_cycle, total_idle_time = 0;
    float cpu_utilization;
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++)
    {
        //cout << "\nEnter Process\t" << i << "Arrival Time: ";
          AT= ps[i].at;
        ps[i].pid = i;
    }

    for (int i = 0; i < n; i++)
    {
        //cout << "\nEnter Process" << i << "Burst Time: ";
        BT= ps[i].bt ;
    }

    //sort
    sort(ps, ps + n, comparatorAT);

    //calculation
    for (int i = 0; i < n; i++)
    {
        ps[i].start_time = (i == 0) ? ps[i].at : max(ps[i].at, ps[i - 1].ct);
        ps[i].ct = ps[i].start_time + ps[i].bt;
        ps[i].tat = ps[i].ct - ps[i].at;
        ps[i].wt = ps[i].tat - ps[i].bt;
        ps[i].rt = ps[i].wt;

        sum_tat += ps[i].tat;
        sum_wt += ps[i].wt;
        sum_rt += ps[i].rt;
        total_idle_time += (i == 0) ? 0 : (ps[i].start_time - ps[i - 1].ct);
    }
    length_cycle = ps[n - 1].ct - ps[0].start_time;
    //sort so that process ID in output comes in Original order (just for interactivity)
    sort(ps, ps + n, comparatorPID);

    //Output
    cout << "\nProcess No.\tAT\tCPU Burst Time\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t" << ps[i].bt << "\t\t" << ps[i].ct << "\t" << ps[i].tat << "\t" << ps[i].wt << "\t" << ps[i].rt << endl;
    cout << endl;

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;
    generateGanttChart(ps, n);
    cout << "\nAverage Turn Around time= " << sum_tat / n;
    cout << "\nAverage Waiting Time= " << sum_wt / n;
    cout << "\nAverage Response Time= " << sum_rt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100 << endl;
};
void RR(PCB ps[], int n, int CS,int Q, int AT, int BT) {
    int index;
    int cpu_utilization;
    queue<int> q;
    bool visited[100] = { false }, is_first_process = true;
    int current_time = 0, max_completion_time;
    int completed = 0, tq, total_idle_time = 0, length_cycle;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    cout << fixed << setprecision(2);
    //front
    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter Process " << i << " Arrival Time: ";
        //AT=ps[i].at;
        cin >> ps[i].at;
        ps[i].pid = i;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter Process " << i << " Burst Time: ";
        cin >> ps[i].bt;
        ps[i].bt_remaining = ps[i].bt;
    }

    cout << "\nEnter time quanta: ";
    cin >> tq;

    //sort structure on the basis of Arrival time in increasing order
    sort(ps, ps + n, comparatorAT);

    q.push(0);
    visited[0] = true;

    while (completed != n)
    {
        if (!q.empty()) {
            index = q.front();
            q.pop();
        }
        if (ps[index].bt_remaining == ps[index].bt)
        {
            ps[index].start_time = max(current_time, ps[index].at);
            total_idle_time += (is_first_process == true) ? 0 : ps[index].start_time - current_time;
            current_time = ps[index].start_time;
            is_first_process = false;

        }

        if (ps[index].bt_remaining - tq > 0)
        {
            ps[index].bt_remaining -= tq;
            current_time += tq;
        }
        else
        {
            current_time += ps[index].bt_remaining;
            ps[index].bt_remaining = 0;
            completed++;

            ps[index].ct = current_time;
            ps[index].tat = ps[index].ct - ps[index].at;
            ps[index].wt = ps[index].tat - ps[index].bt;
            ps[index].rt = ps[index].start_time - ps[index].at;

            sum_tat += ps[index].tat;
            sum_wt += ps[index].wt;
            sum_rt += ps[index].rt;
        }


        //check which new Processes needs to be pushed to Ready Queue from Input list
        for (int i = 1; i < n; i++)
        {
            if (ps[i].bt_remaining > 0 && ps[i].at <= current_time && visited[i] == false)
            {
                q.push(i);
                visited[i] = true;
            }
        }
        //check if Process on CPU needs to be pushed to Ready Queue
        if (ps[index].bt_remaining > 0)
            q.push(index);

        //if queue is empty, just add one process from list, whose remaining burst time > 0
        if (q.empty())
        {
            for (int i = 1; i < n; i++)
            {
                if (ps[i].bt_remaining > 0)
                {
                    q.push(i);
                    visited[i] = true;
                    break;
                }
            }
        }
    } //end of while

    //Calculate Length of Process completion cycle
    max_completion_time = INT_MIN;

    for (int i = 0; i < n; i++)
        max_completion_time = max(max_completion_time, ps[i].ct);

    length_cycle = max_completion_time - ps[0].at;  //ps[0].start_time; 

    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    //sort so that process ID in output comes in Original order (just for interactivity- Not needed otherwise)  
    sort(ps, ps + n, comparatorPID);


    //Output
    cout << "\nProcess No.\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n";
    for (int i = 0; i < n; i++)
        cout << i << "\t\t" << ps[i].at << "\t" << ps[i].bt << "\t\t" << ps[i].start_time << "\t\t" << ps[i].ct << "\t" << ps[i].tat << "\t" << ps[i].wt << "\t" << ps[i].rt << endl;
    cout << endl;
    generateGanttChart(ps, n);
    cout << "\nAverage Turn Around time= " << (float)sum_tat / n;
    cout << "\nAverage Waiting Time= " << (float)sum_wt / n;
    cout << "\nAverage Response Time= " << (float)sum_rt / n;
    cout << "\nThroughput= " << n / (float)length_cycle;
    cout << "\nCPU Utilization(Percentage)= " << cpu_utilization * 100 << endl;
};
int main()
{
    int n=0, at=0, bt=0, cs=0, q=0, num = 0;
    readInputFromFile(cs,q,at,bt);

    cout << "Enter total number of processes(1-10 maximum): ";
    cin >> n;
    cout << "Choose The schedualing Metod You want to implement: " << endl << "1.First-Come First-Served (FCFS)" << endl << "2.Shortest Remaining Time (SRT)" << endl << "3.Round-Robin (RR)" << endl << "4.Exit" << "\n-----------------------------------------------------------------" << "\n>>Your Choice:";
    cin >> num;
    switch (num)
    {
    case 1: {
        FCFS(ps, n, cs, at, bt);
        break;
    }
    case 2: {
        SRT(ps, n, cs, at, bt);
        break;
    }
    case 3: {
        RR(ps, n, cs,q, at, bt);
        break;
    }
    case 4: {
        break;
    }
    default: {
        cout << "Invalid Input" << endl;
    }
           break;

    }


    return 0;
}
