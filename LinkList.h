#ifndef LAB2_PROCESSLIST_H
#define LAB2_PROCESSLIST_H

struct pcb {
public:
    int PID;//
    int burstTime;//
    float remainingBurst;//
    float arrivalTime;//
    int Priority;//
    pcb *next;//
    float newFinish;//
    float newResponeTime;//
    int newNumberofContext;//
    bool responded;//

};

class pcbList {
private:
    pcb *head, *tail;
public:
    pcb *getHead() const {
        return head;
    }

    void setHead(pcb *head) {
        pcbList::head = head;
    }

    pcb *getTail() const {
        return tail;
    }

    void setTail(pcb *tail) {
        pcbList::tail = tail;

    }

    pcbList() {
        head = nullptr;
        tail = nullptr;
    }

    void addProcess(pcb process) {
        auto *temp = new pcb;
        temp->PID = process.PID;
        temp->burstTime = process.burstTime;
        temp->remainingBurst = process.remainingBurst;
        temp->arrivalTime = process.arrivalTime;
        temp->Priority = process.Priority;
        temp->newFinish = process.newFinish;
        temp->newNumberofContext = process.newNumberofContext;
        temp->newResponeTime = process.newResponeTime;
        temp->responded = process.responded;
        temp->next = nullptr;

        if (head == nullptr) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }
    }

    int count() {
        pcb process = *head;
        int total = 0;

        while (process.next != nullptr) {
            total++;
            process = *process.next;
        }

        return total;
    }

    void deleteList() {
        pcb *freeProcess = this->getHead();
        if (this->getHead() != nullptr)
            while (freeProcess->next != nullptr) {
                this->setHead(freeProcess->next);
                delete(freeProcess);
                freeProcess = this->getHead();
            }
        this->setHead(nullptr);
        this->setTail(nullptr);
    }
};

#endif //LAB2_PROCESSLIST_H