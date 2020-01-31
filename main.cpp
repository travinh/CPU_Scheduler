#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include "LinkList.h"

using namespace std;

pcbList readFile(string inputFile, bool print);
void displayFile(string outputFile, pcbList tempList);
void FCFS(pcbList tempList);
void STRF(pcbList tempList);
void preemPriority(pcbList tempList);
void RR(pcbList tempList, int quantum);

int main() {
    int quantumNum;
    int option = 1;
    bool doPrint = true;
    string outputFile;
    string inputFile;
    pcbList tempList;
    while (tempList.getHead() == nullptr) {
        cout << "Please type an input file name: ";
        cin >> inputFile;
        tempList = readFile(inputFile, doPrint);
    }

    cout << "Please type an output file name: ";
    cin >> outputFile;
    while (option != 0) {
        doPrint = false;
        cout << "\t\t------ CPU Scheduler-------\n\n" << "1. First Come First Serve (FCFS)\n" << "2. Shortest Remaining Task First(SRTF)\n"
             << "3. Round Robin(RR)\n" << "4. Preemptive Priority(PP)\n5. Change input file name\n"
             << "0. Exit!\n";
        cin >> option;

        if (option == 1) {
            tempList = readFile(inputFile, doPrint);
            FCFS(tempList);
            displayFile(outputFile, tempList);
        } else if (option == 2) {
            tempList = readFile(inputFile, doPrint);
            STRF(tempList);
            displayFile(outputFile, tempList);
        } else if (option == 3) {
            tempList = readFile(inputFile, doPrint);
            cout << "Enter Number of Quantum: ";
            cin >> quantumNum;
            RR(tempList, quantumNum);
            displayFile(outputFile, tempList);
        } else if (option == 4) {
            tempList = readFile(inputFile, doPrint);
            preemPriority(tempList);
            displayFile(outputFile, tempList);
        } else if (option == 5) {
            tempList.deleteList();
            doPrint = true;
            while (tempList.getHead() == nullptr) {
                cout << "Please input file name: ";
                cin >> inputFile;
                tempList = readFile(inputFile, doPrint);
            }
        }
    }
    return 0;
}


pcbList readFile(string inputFile, bool print) {
    ifstream fileIn;
    string file;
    fileIn.open(inputFile);    //opening the file
    if (fileIn.fail()) {
        cout << "Can not open file.\n";
        pcbList nullList;
        return nullList;   //exit if file not found
    }

    pcbList processList;

    while (!fileIn.eof()) {
        pcb process;
        fileIn >> process.PID;
        fileIn >> process.arrivalTime;
        fileIn >> process.burstTime;
        fileIn >> process.Priority;
        process.remainingBurst = process.burstTime;
        process. newFinish = 0;
        process.newNumberofContext = 0;
        process.newResponeTime = 0;
        process.responded = false;
        processList.addProcess(process);
    }

    fileIn.close();

    if (print) {
        cout << setw(10) << left << "PID" << setw(10) << left << "Burst" << setw(10) << left << "Arrival" << setw(10)
             << left << "Priority";
        cout << endl;
        pcb *printProcess = processList.getHead();

        while (printProcess->next != nullptr) {
            cout << setw(10) << left << printProcess->PID << setw(10) << printProcess->burstTime << setw(10)
                 << left << printProcess->arrivalTime << setw(10) << left << printProcess->Priority << endl;
            printProcess = printProcess->next;
        }

    }
    return processList;
}

void displayFile(string outputFile, pcbList tempList) {
    ofstream FileOut;
    FileOut.open(outputFile);
    int totalProcess = tempList.count();
    pcb outputProcess = *tempList.getHead();
    for (int i = 0; i < totalProcess; i++) {
        double wait = outputProcess.newFinish - outputProcess.burstTime - outputProcess.arrivalTime;
        double turnaround = outputProcess.newFinish - outputProcess.arrivalTime;
        int burst = outputProcess.burstTime;
        FileOut << setw(5) << left << outputProcess.PID << setw(10) << left << outputProcess.arrivalTime << setw(10)
                << left
                << burst << setw(10) << left << outputProcess.Priority << setw(10) << left << outputProcess.newFinish
                << setw(10) << left << wait << setw(12) << left << turnaround << setw(14) << left
                << outputProcess.newResponeTime << setw(14) << left << 0 << "\n";
        outputProcess = *outputProcess.next;
    }

    FileOut.close();
}


void FCFS(pcbList tempList) {
    int clock = 0;
    int totalWork = tempList.count();
    int currentWork = 0;
    pcb *currProcess = nullptr;

    pcbList newQueue;  //create newQueue

    while (currentWork < totalWork) {
        pcb *getQueueProcess = tempList.getHead();
        for (int i = 0; i < tempList.count(); i++) {
            if (getQueueProcess->arrivalTime == clock)
                newQueue.addProcess(*getQueueProcess);
            getQueueProcess = getQueueProcess->next;
        }
        if (newQueue.getHead() != nullptr) {
            currProcess = newQueue.getHead();

            if (currProcess->remainingBurst == 0) {
                newQueue.setHead(currProcess->next);  //Remove
                currProcess->newFinish = clock;  //save finish time
                pcb *setFinishProcess = tempList.getHead();

                for (int i = 0; i < tempList.count(); i++) {
                    if (setFinishProcess->PID == currProcess->PID) {
                        setFinishProcess->newFinish = currProcess->newFinish;
                        setFinishProcess->newResponeTime =
                                currProcess->newFinish - currProcess->burstTime - currProcess->arrivalTime;
                    }
                    setFinishProcess = setFinishProcess->next;
                }
                delete (currProcess);

                currProcess = newQueue.getHead();

                currentWork++;
            }
        }
        if (currProcess != nullptr)
            currProcess->remainingBurst--;
        clock++;
    }

    double avgBurst = 0;
    double avgWait = 0;
    double avgTurnArround = 0;
    double avgResponse = 0;
    int totalContext = 0;
    cout << "**********************************************************************************************\n";
    cout << "********************************* Scheduling algorithm: FCFS *********************************\n";
    cout << "**********************************************************************************************\n";
    cout<<setw(5)<<"|PID|"<<setw(10)<<"|Arrival|"<<setw(10)<<"|CPU-Burst|"<<setw(10)<<"|Priority|"<<setw(10)
        <<"|Finish Time|"<<setw(15)<< "|Waiting Time|" <<setw(15)<<"|Turn Around|"<<setw(15)<<"|Response Time|"
        <<setw(15)<<"|No. of Context|\n";
    pcb *display = tempList.getHead();
    for (int i = 0; i < totalWork; i++) {
        double wait = display->newFinish - display->burstTime - display->arrivalTime;
        double turnaround = display->newFinish - display->arrivalTime;
        double response = display->newFinish - display->burstTime - display->arrivalTime;
        int burst = display->burstTime;
        avgBurst += burst;
        avgWait += wait;
        avgTurnArround += turnaround;
        avgResponse += response;

        cout <<"|"<<setw(4)<<display->PID<<"|"<<setw(9)<<display->arrivalTime<<"|"<<setw(10)<<display->burstTime
             <<"|" <<setw(9)<<display->Priority<<"|" <<setw(12)<< display->newFinish<<"|"<<setw(14)<< wait
             <<"|"<<setw(14)<< turnaround<<"|"<<setw(14) <<display->newResponeTime
             <<"|"<<setw(14)<<display->newNumberofContext<<"\n";
            display = display->next;
    }
    avgBurst = avgBurst / totalWork;
    avgWait = avgWait / totalWork;
    avgTurnArround = avgTurnArround / totalWork;
    avgResponse = avgResponse / totalWork;
    cout << "\nAverage CPU burst time = " << avgBurst << "ms,\t\tAverage waiting time = " << avgWait << "ms\n";
    cout << "Average turn around time = " << avgTurnArround << "ms,\t\tAverage response time = " << avgResponse
         << "ms\n";
    cout << "Total No. of Context Switching Performed = " << totalContext << endl << endl;
    cout << "**********************************************************************************************\n";
    newQueue.deleteList();
}

void STRF(pcbList tempList) {
    double clock = 0;
    double lastClock = -1;
    int totalWork = tempList.count();
    int currentWork = 0;
    int last = 0;
    double switchCount = 0;
    pcb *currProcess = nullptr;

    pcbList newQueue;  //create newQueue

    //    1. add to newQueue
    while (currentWork < totalWork) {
        pcb *getQueueProcess = tempList.getHead();

        for (int i = 0; i < tempList.count(); i++) {
            //add to newQueue
            if (getQueueProcess->arrivalTime > lastClock && getQueueProcess->arrivalTime <= clock)
                newQueue.addProcess(*getQueueProcess);
            getQueueProcess = getQueueProcess->next;
        }
        lastClock = clock;

        //    2. find shortest process in newQueue
        if (newQueue.getHead() != nullptr) {
            currProcess = newQueue.getHead();
            pcb *getShortestProcess = newQueue.getHead()->next;
            for (int i = 0; i < newQueue.count(); i++) {
                if (currProcess->remainingBurst > getShortestProcess->remainingBurst) {
                    currProcess = getShortestProcess;
                } else if ((currProcess->remainingBurst == getShortestProcess->remainingBurst) &&
                           (currProcess->arrivalTime > getShortestProcess->arrivalTime)) {
                    currProcess = getShortestProcess;    //if tie, use FCFS
                }
                getShortestProcess = getShortestProcess->next;
            }

            //3. Check context switch
            if (last != 0)
                if (last != currProcess->PID) {
                    pcb *getContextSwitch = newQueue.getHead();
                    while (getContextSwitch != nullptr) {
                        if (getContextSwitch->PID == last) {
                            if (getContextSwitch->remainingBurst !=
                                0) {  //last process was interrupted before completed
                                getContextSwitch->newNumberofContext++;
                                switchCount++;
                                clock += 0.5;
                            }
                        }
                        getContextSwitch = getContextSwitch->next;
                    }
                }
            last = currProcess->PID;

            //    4. decrement remaining burst time
            currProcess->remainingBurst--;

            if (!currProcess->responded) {
                currProcess->newResponeTime = (clock - currProcess->arrivalTime);
                currProcess->responded = true;
            }
            clock++;

            //if process has completed
            if (currProcess->remainingBurst == 0) {
                //Record finish time of currProcess
                currProcess->newFinish = clock;
                pcb *setFinishProcess = tempList.getHead();
                //loop to save process finish time in original list
                for (int i = 0; i < tempList.count(); i++) {
                    if (setFinishProcess->PID == currProcess->PID) {
                        setFinishProcess->newFinish = currProcess->newFinish;
                        setFinishProcess->newNumberofContext = currProcess->newNumberofContext;
                        setFinishProcess->newResponeTime = currProcess->newResponeTime;
                    }
                    setFinishProcess = setFinishProcess->next;
                }

                //    5. check if process is complete, if it is remove from newQueue
                if (currProcess == newQueue.getHead())  //current process @ head
                    newQueue.setHead(currProcess->next);
                else if (currProcess == newQueue.getTail()) { //current process @tail
                    pcb *lastProcess = newQueue.getHead();
                    for (int i = 0; i < newQueue.count() - 1; i++) {
                        lastProcess = lastProcess->next;
                    }
                    lastProcess->next = nullptr;
                    newQueue.setTail(lastProcess);
                }  else {
                    pcb *lastProcess = newQueue.getHead();         //@ mid
                    for (int i = 0; i < newQueue.count(); i++) {
                        if (lastProcess->next == currProcess)
                            lastProcess->next = lastProcess->next->next;
                        lastProcess = lastProcess->next;
                    }
                }
                delete (currProcess);
                currentWork++;
            }
        }
    }

    double avgBurst = 0;
    double avgWait = 0;
    double avgTurnAround = 0;
    double avgResponse = 0;
    int totalContext = 0;
    cout << "**********************************************************************************************\n";
    cout << "****************************** Scheduling algorithm: SRTF ************************************\n";
    cout << "**********************************************************************************************\n";

    cout<<setw(5)<<"|PID|"<<setw(10)<<"|Arrival|"<<setw(10)<<"|CPU-Burst|"<<setw(10)<<"|Priority|"<<setw(10)
        <<"|Finish Time|"<<setw(15)<< "|Waiting Time|" <<setw(15)<<"|Turn Around|"<<setw(15)<<"|Response Time|"
        <<setw(15)<<"|No. of Context|\n";
    pcb display = *tempList.getHead();
    for (int i = 0; i < totalWork; i++) {
        double wait = display.newFinish - display.burstTime - display.arrivalTime;
        double turnaround = display.newFinish - display.arrivalTime;
        int burst = display.burstTime;
        avgBurst += burst;
        avgWait += wait;
        avgTurnAround += turnaround;
        avgResponse += display.newResponeTime;
        totalContext += display.newNumberofContext;
        cout <<"|"<<setw(4)<<display.PID<<"|"<<setw(9)<<display.arrivalTime<<"|"<<setw(10)<<display.burstTime
             <<"|" <<setw(9)<<display.Priority<<"|" <<setw(12)<< display.newFinish<<"|"<<setw(14)<< wait
             <<"|"<<setw(14)<< turnaround<<"|"<<setw(14) <<display.newResponeTime
             <<"|"<<setw(14)<<display.newNumberofContext<<"\n";

        if(display.next!= nullptr)
            display = *display.next;

    }
    avgBurst = avgBurst / totalWork;
    avgWait = avgWait / totalWork;
    avgTurnAround = avgTurnAround / totalWork;
    avgResponse = avgResponse / totalWork;
    cout << "\nAverage CPU burst time = " << avgBurst << "ms,   Average waiting time = " << avgWait << "ms"
         << endl;
    cout << "Average turn around time = " << avgTurnAround << "ms,   Average response time = " << avgResponse
         << "ms" << endl;
    cout << "Total No. of Context Switching Performed = " << totalContext << endl << endl;
    cout << "**********************************************************************************************\n\n";
    newQueue.deleteList();
}

void preemPriority(pcbList tempList) {
    double clock = 0;
    double lastClock = -1;
    int totalProcesses = tempList.count();
    int completeProcesses = 0;
    int last = 0;
    double switchCount = 0;
    pcb *currentProcess = nullptr;

    pcbList queue;  //Initialize linked list of processes for queue
    //The following while loop simulates the clock and queue
    while (completeProcesses < totalProcesses) {
        //Add process to queue if the process has arrived
        pcb *getQueueProcess = tempList.getHead();
        //Must step through whole list to get arrived processes
        for (int i = 0; i < tempList.count(); i++) {
            //If between this clock and last clock process arrived, add to queue
            if (getQueueProcess->arrivalTime > lastClock && getQueueProcess->arrivalTime <= clock)
                queue.addProcess(*getQueueProcess);
            getQueueProcess = getQueueProcess->next;
        }
        lastClock = clock;
        if (queue.getHead() != nullptr) {
            currentProcess = queue.getHead();
            pcb *getPriorityProcess = queue.getHead()->next;            //Loop to find shortest remaining burst time in queue
            for (int i = 0; i < queue.count(); i++) {
                if (currentProcess->Priority > getPriorityProcess->Priority) {
                    currentProcess = getPriorityProcess;
                } else if ((currentProcess->Priority == getPriorityProcess->Priority) &&
                           (currentProcess->arrivalTime > getPriorityProcess->arrivalTime)) {
                    currentProcess = getPriorityProcess;    //if tie, use FCFS
                }
                getPriorityProcess = getPriorityProcess->next;
            }

            //Check for context switches
            if (last != 0)
                if (last != currentProcess->PID) {
                    pcb *getContextSwitch = queue.getHead();
                    while (getContextSwitch != nullptr) {
                        if (getContextSwitch->PID == last) {
                            if (getContextSwitch->remainingBurst !=
                                0) {  //last process was interrupted before completed
                                getContextSwitch->newNumberofContext++;
                                switchCount += 0.5;
                                if (switchCount == 1.5)
                                    switchCount = 0.5;
                                clock += 0.5;
                            }
                        }
                        getContextSwitch = getContextSwitch->next;
                    }
                }
            last = currentProcess->PID;

            currentProcess->remainingBurst--;

            if (!currentProcess->responded) {
                currentProcess->newResponeTime = (clock - currentProcess->arrivalTime);
                currentProcess->responded = true;
            }
            clock++;

            //if process has completed
            if (currentProcess->remainingBurst == 0) {
                //Record finish time of currentProcess
                currentProcess->newFinish = clock;
                pcb *setFinishProcess = tempList.getHead();
                //loop to save process finish time in original list
                for (int i = 0; i < tempList.count(); i++) {
                    if (setFinishProcess->PID == currentProcess->PID) {
                        setFinishProcess->newFinish = currentProcess->newFinish;
                        setFinishProcess->newNumberofContext = currentProcess->newNumberofContext;
                        setFinishProcess->newResponeTime = currentProcess->newResponeTime;
                    }
                    setFinishProcess = setFinishProcess->next;
                }

                //step through queue and remove current process from queue
                if (currentProcess == queue.getHead())  //current process is head of queue
                    queue.setHead(currentProcess->next);
                else if (currentProcess == queue.getTail()) { //current process is tail of queue
                    pcb *lastProcess = queue.getHead();
                    //get element right before tail
                    for (int i = 0; i < queue.count() - 1; i++) {
                        lastProcess = lastProcess->next;
                    }
                    //remove reference to tail
                    lastProcess->next = nullptr;
                    queue.setTail(lastProcess);
                } else {  //else current process is in middle of queue, must remove references
                    pcb *lastProcess = queue.getHead();
                    for (int i = 0; i < queue.count(); i++) {
                        if (lastProcess->next == currentProcess)
                            lastProcess->next = lastProcess->next->next; //remove current process from linked list
                        lastProcess = lastProcess->next;
                    }
                }
                //if (queue.getHead() == nullptr && queue.getTail() != nullptr)
                //    queue.setHead(queue.getTail());

                delete (currentProcess);
                completeProcesses++;
            }
        }
    }

    double averageBurst = 0;
    double averageWait = 0;
    double averageTurnaround = 0;
    double averageResponse = 0;
    int totalContext = 0;
    cout << "********************** Scheduling algorithm: Preemptive Priority *****************************\n\n";
    cout << setw(5) << left << "PID" << setw(9) << left << "Arrival" << setw(10) << left << "CPU Burst" << setw(10)
         << left << "Priority" << setw(8) << left << "Finish" << setw(10) << left << "Wait time" << setw(12)
         << left << "Turnaround" << setw(14) << left << "Response time" << setw(14) << left << "Context Switches"
         << "\n";
    pcb printProcess = *tempList.getHead();
    for (int i = 0; i < totalProcesses; i++) {
        double wait = printProcess.newFinish - printProcess.burstTime - printProcess.arrivalTime;
        double turnaround = printProcess.newFinish - printProcess.arrivalTime;
        int burst = printProcess.burstTime;
        averageBurst += burst;
        averageWait += wait;
        averageTurnaround += turnaround;
        averageResponse += printProcess.newResponeTime;
        totalContext += printProcess.newNumberofContext;
        cout << setw(5) << left << printProcess.PID << setw(9) << left << printProcess.arrivalTime << setw(10) << left
             << burst << setw(10) << left << printProcess.Priority << setw(8) << left << printProcess.newFinish
             << setw(10) << left << wait << setw(12) << left << turnaround << setw(14) << left
             << printProcess.newResponeTime
             << setw(14) << left << printProcess.newNumberofContext << "\n";
        printProcess = *printProcess.next;
    }
    averageBurst = averageBurst / totalProcesses;
    averageWait = averageWait / totalProcesses;
    averageTurnaround = averageTurnaround / totalProcesses;
    averageResponse = averageResponse / totalProcesses;
    cout << "\nAverage CPU burst time = " << averageBurst << "ms,   Average waiting time = " << averageWait << "ms"
         << endl;
    cout << "Average turn around time = " << averageTurnaround << "ms,   Average response time = " << averageResponse
         << "ms" << endl;
    cout << "Total No. of Context Switching Performed = " << totalContext << endl << endl;
    cout << "**********************************************************************************************\n\n";
    queue.deleteList();
}

void RR(pcbList tempList, int quantum) {
    double clock = 0;
    double lastClock = -1;
    int totalWork = tempList.count();
    int currrentWork = 0;
    int last = 0;
    double switchCount = 0;
    pcb *currProcess = nullptr;
    bool Incomplete = false;
    bool flagContextSwitch = false;

    pcbList newQueue;  //create queue

    while (currrentWork < totalWork) {
        pcb *getQueueProcess = tempList.getHead();
        for (int i = 0; i < tempList.count(); i++) {
            if (getQueueProcess->arrivalTime > lastClock && getQueueProcess->arrivalTime <= clock)
                newQueue.addProcess(*getQueueProcess);
            getQueueProcess = getQueueProcess->next;
        }
        lastClock = clock;

        if (Incomplete) {
            currProcess->newNumberofContext++;
            flagContextSwitch = true;
            newQueue.addProcess(*currProcess);
            newQueue.setHead(currProcess->next);
            delete (currProcess);
            Incomplete = false;
        }

        if (newQueue.getHead() != nullptr) {
            currProcess = newQueue.getHead();
            //Check context switches
            if (flagContextSwitch) {
                clock += 0.5;
                flagContextSwitch = false;
            }

            last = currProcess->PID;

            if (!currProcess->responded) {
                currProcess->newResponeTime = (clock - currProcess->arrivalTime);
                currProcess->responded = true;
            }

            // process runs for quantum || remain =0
            for (int i = 0; i < quantum; i++) {
                if (currProcess->remainingBurst > 0) {
                    currProcess->remainingBurst--;
                    clock++;
                }
            }

            if (currProcess->remainingBurst == 0) {
                currProcess->newFinish = clock;
                pcb *setFinishProcess = tempList.getHead();
                for (int i = 0; i < tempList.count(); i++) {
                    if (setFinishProcess->PID == currProcess->PID) {
                        setFinishProcess->newFinish = currProcess->newFinish;
                        setFinishProcess->newNumberofContext = currProcess->newNumberofContext;
                        setFinishProcess->newResponeTime = currProcess->newResponeTime;
                    }
                    setFinishProcess = setFinishProcess->next;
                }
                newQueue.setHead(currProcess->next);
                delete (currProcess);

                currrentWork++;
            } else {
                Incomplete = true;
            }

        }
    }

    double avgBurst = 0;
    double avgWait = 0;
    double avgTurnAround = 0;
    double avfResponse = 0;
    int totalContext = 0;
    cout << "**********************************************************************************************\n";
    cout << "***************************** Scheduling algorithm: Round Robin ******************************\n";
    cout << "***************************** ( No. of Task = " << totalWork << " Quantum = " << quantum
         << " )  *******************************\n";
    cout << "**********************************************************************************************\n";
    cout<<setw(5)<<"|PID|"<<setw(10)<<"|Arrival|"<<setw(10)<<"|CPU-Burst|"<<setw(10)<<"|Priority|"<<setw(10)
        <<"|Finish Time|"<<setw(15)<< "|Waiting Time|" <<setw(15)<<"|Turn Around|"<<setw(15)<<"|Response Time|"
        <<setw(15)<<"|No. of Context|\n";
    pcb display = *tempList.getHead();
    for (int i = 0; i < totalWork; i++) {
        double wait = display.newFinish - display.burstTime - display.arrivalTime;
        double turnaround = display.newFinish - display.arrivalTime;
        int burst = display.burstTime;
        avgBurst += burst;
        avgWait += wait;
        avgTurnAround += turnaround;
        totalContext += display.newNumberofContext;
        cout <<"|"<<setw(4)<<display.PID<<"|"<<setw(9)<<display.arrivalTime<<"|"<<setw(10)<<display.burstTime
             <<"|" <<setw(9)<<display.Priority<<"|" <<setw(12)<< display.newFinish<<"|"<<setw(14)<< wait
             <<"|"<<setw(14)<< turnaround<<"|"<<setw(14) <<display.newResponeTime
             <<"|"<<setw(14)<<display.newNumberofContext<<"\n";
        display = *display.next;
    }
    avgBurst = avgBurst / totalWork;
    avgWait = avgWait / totalWork;
    avgTurnAround = avgTurnAround / totalWork;
    avfResponse = avfResponse / totalWork;
    cout << "\nAverage CPU burst time = " << avgBurst << "ms,   Average waiting time = " << avgWait << "ms\n";
    cout << "Average turn around time = " << avgTurnAround << "ms,   Average response time = " << avfResponse
         << "ms\n";
    cout << "Total No. of Context Switching Performed = " << totalContext << endl << endl;
    cout << "************************************************************************************************\n\n";
    newQueue.deleteList();
}

