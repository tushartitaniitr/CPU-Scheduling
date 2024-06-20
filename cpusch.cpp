#include<bits/stdc++.h>
#define int long long
using namespace std;
struct Process {
    int process_id;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};
bool st(Process p1,Process p2){
    return p1.priority<=p2.priority;
}
int fcfs_scheduling(vector<Process>& processes,vector<pair<int,pair<int,int>>> & es) {
    int n=processes.size();
    int current_time = 0;
    int avt=0;
    for (auto& process : processes) {
         int cs= current_time;
        if (current_time < process.arrival_time) {
            current_time = process.arrival_time;
        }
       
        process.completion_time = current_time + process.burst_time;
        process.turnaround_time = process.completion_time - process.arrival_time;
        process.waiting_time = process.turnaround_time - process.burst_time;
        current_time = process.completion_time;
        avt+=(process.waiting_time);
        es.push_back({process.process_id,{cs,current_time}});
    }
    avt+=(n-1);
    avt/=n;
    return avt;
}
bool compareByArrival(const Process& a, const Process& b) {
    return a.arrival_time < b.arrival_time;
}

bool compareByBurst(const Process& a, const Process& b) {
    return a.burst_time < b.burst_time;
}

int sjf_scheduling(vector<Process>& processes, vector<pair<int,pair<int,int>>>& execution_sequence) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    vector<bool> is_completed(n, false);
    int avw=0;

    while (completed != n) {
        int min_burst_index = -1;
        int min_burst_time = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst_time) {
                    min_burst_time = processes[i].burst_time;
                    min_burst_index = i;
                }
            }
        }

        if (min_burst_index != -1) {
            Process& p = processes[min_burst_index];
            int c =current_time;

            p.completion_time = current_time + p.burst_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - p.burst_time;
            avw+=(p.waiting_time);
            current_time = p.completion_time;
            is_completed[min_burst_index] = true;
            execution_sequence.push_back({p.process_id,{c,current_time}});
            completed++;
        }
        
         else {
        
            current_time++;
        }
        
        
    }
    avw+=(n-1);
    avw/=n;
    return avw;
}

int preemptive_sjf_scheduling(vector<Process>& processes, vector<Process>& execution_sequence,vector<pair<int,pair<int,int>>> & es) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    int min_burst_index = -1;
    int min_burst_time = INT_MAX;

    for (auto& process : processes) {
        process.remaining_time = process.burst_time;
    }
    int awt=0;

    while (completed != n) {
        
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_burst_time) {
                    min_burst_time = processes[i].remaining_time;
                    min_burst_index = i;
                }
            }
        }

        if (min_burst_index != -1) {
            execution_sequence.push_back(processes[min_burst_index]);
            processes[min_burst_index].remaining_time--;
            current_time++;
            min_burst_time = processes[min_burst_index].remaining_time;

            
            if (processes[min_burst_index].remaining_time == 0) {
                processes[min_burst_index].completion_time = current_time;
                processes[min_burst_index].turnaround_time = processes[min_burst_index].completion_time - processes[min_burst_index].arrival_time;
                processes[min_burst_index].waiting_time = processes[min_burst_index].turnaround_time - processes[min_burst_index].burst_time;
                awt+=processes[min_burst_index].waiting_time;
                completed++;
                min_burst_index = -1;
                min_burst_time = INT_MAX;
            }
        } else {
            current_time++;
        }
    }
    awt+=n-1;
    awt/=n;
    es.push_back({execution_sequence[0].process_id,{0,0}});
    for(int i=1;i<execution_sequence.size();i++){
        if(execution_sequence[i].process_id!=execution_sequence[i-1].process_id){
            es[es.size()-1].second.second=i;
            es.push_back({execution_sequence[i].process_id,{i,0}});

        }

    }
    es[es.size()-1].second.second=current_time;
    return awt;
}
int round_robin_scheduling(vector<Process>& processes, int time_quantum, vector<pair<int,pair<int,int>>>& execution_sequence) {
    int n = processes.size();
    int current_time = 0;
    queue<int> ready_queue;
    vector<bool> is_in_queue(n, false);
    int awt=0;

    for (auto& process : processes) {
        process.remaining_time = process.burst_time;
    }
    ready_queue.push(0);
    is_in_queue[0] = true;

    while (!ready_queue.empty()) {
        int current_process_index = ready_queue.front();
        ready_queue.pop();
        Process& current_process = processes[current_process_index];
        int cr=current_time;
        

        if (current_process.remaining_time <= time_quantum) {
             
            current_time += current_process.remaining_time;
            current_process.remaining_time = 0;
            current_process.completion_time = current_time;
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;
        } else {
                
            current_time += time_quantum;
            current_process.remaining_time -= time_quantum;
        }
        awt+=current_process.waiting_time;
        execution_sequence.push_back( {processes[current_process_index].process_id,{cr,current_time}});
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival_time <= current_time && !is_in_queue[i] && processes[i].remaining_time > 0) {
                ready_queue.push(i);
                is_in_queue[i] = true;
            }
        }
        if (current_process.remaining_time > 0) {
            ready_queue.push(current_process_index);
        }

    
        if (ready_queue.empty()) {
            for (int i = 0; i < n; ++i) {
                if (processes[i].remaining_time > 0) {
                    ready_queue.push(i);
                    is_in_queue[i] = true;
                    break;
                }
            }
        }
    }
    awt+=(n-1);
    awt/=n;
    return awt;
}

void print_results( vector<Process>& processes) {
    
    for (int i = 0; i < processes.size(); ++i) {
        cout << "{\"process_id\": " << processes[i].process_id
             << ", \"completion_time\": " << processes[i].completion_time
             << ", \"waiting_time\": " << processes[i].waiting_time
             << ", \"turnaround_time\": " << processes[i].turnaround_time<<"}"
             << endl;
        if (i < processes.size() - 1) {
            cout << ",";
        }
    }

}
void preemptive(vector<Process>& p,int q){
    vector<Process> p1=p,p2=p,p3=p;
    vector<Process> p4;
    
    vector<pair<int,pair<int,int>>>  es1,es2,es3,es4;
    int a1 = fcfs_scheduling(p1,es1);
      for(int i=0;i<es1.size();i++){
            cout<<es1[i].first<<"         "<<es1[i].second.first<<"         "<<es1[i].second.second<<endl;
        }
    cout<<"aja"<<endl;
    int a2 = preemptive_sjf_scheduling(p2,p4,es2);
    
    int a3 = round_robin_scheduling(p3,q,es3);

    cout<<"AVG WAITING TIME OF FCFS is"<<" = "<<a1<<endl;
    cout<<"AVG WAITING TIME OF PREEMPTIVE SJF is"<<" = "<<a2<<endl;
    cout<<"AVG WAITING TIME OF RR SCHEDULING is"<<" = "<<a3<<endl;
    if(a2<=a1&&a2<=a3){
        cout<<"Hence PREMTIVE SJF SCHEDULING IS MOST OPTIMAL CONSIDERING AVG WAITING"<<endl;
        cout<<"THE ORDER IS"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es2.size();i++){
            cout<<es3[i].first<<"           "<<es3[i].second.first<<"            "<<es3[i].second.second<<endl;
        }
        print_results(p2);
    }
    else   if(a3<=a1&&a3<=a2){
        cout<<"Hence  ROUND ROBIN IS MOST OPTIMAL CONSIDERING AVG WAITING"<<endl;
        cout<<"THE ORDER IS"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es3.size();i++){
            cout<<es3[i].first<<"           "<<es3[i].second.first<<"            "<<es3[i].second.second<<endl;
        }
        print_results(p3);
    }
    cout<<"NOW IF WE CONSIDER SCHEDULING ACC TO PRIORITY"<<endl;
    sort(p1.begin(),p1.end(),st);
    sort(p2.begin(),p2.end(),st);
    sort(p3.begin(),p3.end(),st);
    vector<int> v1(p1.size()),v2(p2.size()),v3(p3.size());
    for(int i=0;i<p1.size();i++){
        v1[i]=p1[i].waiting_time;
    }
    for(int i=0;i<p2.size();i++){
        v2[i]=p2[i].waiting_time;
    }
     for(int i=0;i<p3.size();i++){
        v3[i]=p3[i].waiting_time;
    }
    vector<pair<vector<int>,int>> vs;
    vs.push_back({v1,1});
    vs.push_back({v2,2});
    vs.push_back({v3,3});
    sort(vs.begin(),vs.end());
    if(vs[0].second==1){
        cout<<"FCFS IS MOST OPTIMAL"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es1.size();i++){
            cout<<es1[i].first<<"           "<<es1[i].second.first<<"             "<<es1[i].second.second<<endl;
        }
        print_results(p1);
    }
    else if(vs[0].second==2){
         cout<<"PREEMITIVE SJF IS MOST OPTIMAL"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es2.size();i++){
            cout<<es2[i].first<<"           "<<es2[i].second.first<<"             "<<es2[i].second.second<<endl;
        }
        print_results(p2);

    }
    else{
          cout<<"PREEMITIVE RR IS MOST OPTIMAL"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es3.size();i++){
            cout<<es3[i].first<<"           "<<es3[i].second.first<<"            "<<es3[i].second.second<<endl;
        }
        print_results(p3);

    }



    
}
void npreemitive(vector<Process> &p){
    vector<Process> p1=p,p2=p;
    vector<pair<int,pair<int,int>>> es1,es2;
    int a1 = fcfs_scheduling(p1,es1);
    int a2 =sjf_scheduling(p2,es2);


    cout<<"AVG WAITING TIME OF FCFS is"<<" = "<<a1<<endl;
    cout<<"AVG WAITING TIME OF SJF is"<<" = "<<a2<<endl;
    if(a2<a1){
         cout<<"Hence  SJF SCHEDULING IS MOST OPTIMAL CONSIDERING AVG WAITING"<<endl;
        cout<<"THE ORDER IS"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es2.size();i++){
        cout<<es2[i].first<<"           "<<es2[i].second.first<<"            "<<es2[i].second.second<<endl;
        }
        print_results(p2);


    }
    else{
        cout<<"Hence  FCFS SCHEDULING IS MOST OPTIMAL CONSIDERING AVG WAITING"<<endl;
        cout<<"THE ORDER IS"<<endl;
        cout<<"PROCESS ID"<<" "<<"CPU entry time"<<" "<<"CPU exit time"<<endl;
        for(int i=0;i<es1.size();i++){
                  cout<<es1[i].first<<"           "<<es1[i].second.first<<"            "<<es1[i].second.second<<endl;
        }
        print_results(p1);



    }
    cout<<"NOW IF WE CONSIDER SCHEDULING ACC TO PRIORITY"<<endl;
    sort(p1.begin(),p1.end(),st);
    sort(p2.begin(),p2.end(),st);
     vector<int> v1(p1.size()),v2(p2.size());
    for(int i=0;i<p1.size();i++){
        v1[i]=p1[i].waiting_time;
    }
    for(int i=0;i<p2.size();i++){
        v2[i]=p2[i].waiting_time;
    }
    vector<pair<vector<int>,int>> vs;
    vs.push_back({v1,1});
    vs.push_back({v2,2});
    
    sort(vs.begin(),vs.end());
    if(vs[0].second==1){
        cout<<"FCFS IS MOST OPTIMAL"<<endl;
        cout<<"PROCESSID"<<" "<<"CPUentrytime"<<" "<<"CPUexittime"<<endl;
        for(int i=0;i<es1.size();i++){
            cout<<es1[i].first<<"               "<<es1[i].second.first<<"                 "<<es1[i].second.second<<endl;
        }
        print_results(p1);
    }
    else {
         cout<<"SJF IS MOST OPTIMAL"<<endl;
        cout<<"PROCESS ID"<<" "<<"CPU entry time"<<" "<<"CPU exit time"<<endl;
        for(int i=0;i<es2.size();i++){
            cout<<es2[i].first<<"               "<<es2[i].second.first<<"                   "<<es2[i].second.second<<endl;
        }
        print_results(p2);

    }




}
signed main() {
    cout<<"Enter P for preemptive scheduling any other key for non preemptive"<<endl;
    char s;
    cin>>s;
    vector<Process> p;
    int n;
    cout<<"ENTER NO OF PROCESS"<<endl;
    cin>>n;
    cout<<"enter in this order PROCESSID>ARRIVAL TIME>BURST TIME>PRIORITY"<<endl;
    for(int i=0;i<n;i++){
        int a,sk,d,f;
        cin>>a>>sk>>d>>f;
        p.push_back({a,sk,d,f});

    }
    if(s=='P'){
        cout<<"ENTER THE TIME QuANTUM FOR ROUND ROBIN"<<endl;
        int q;
        cin>>q;
        preemptive(p,q);


    }
    else{
        npreemitive(p);

    }
    
}