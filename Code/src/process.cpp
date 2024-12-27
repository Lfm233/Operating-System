//
// Created by mu yu on 2024/12/23.
//


#include "../include/process.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip> // ���ڸ�ʽ�����
#include <unordered_set>


// ����ģʽ
void debugMode1() {
    std::cout << "\n=== ���̹��� - ����ģʽ ===\n";
    std::vector<Process> processes = {
            {1, 0, 5, 2, 5, 0},
            {2, 0, 5, 1, 5, 0},
            {3, 0, 1, 3, 1, 0}
    };

    int scheduler_choice;
    std::cout << "��ѡ������㷨��\n1. ʱ��Ƭ��ת����\n2. ���ȼ�����\n";
    std::cin >> scheduler_choice;

    if (scheduler_choice == 1) {
        int time_slice = 4;
        roundRobinScheduler(processes, time_slice);
    } else if (scheduler_choice == 2) {
        priorityScheduler(processes);
    }
}

// �û�ģʽ
void userMode1() {
    std::cout << "\n=== ���̹��� - �û�ģʽ ===\n";
    int n;
    std::cout << "���������������";
    std::cin >> n;

    std::vector<Process> processes;
    for (int i = 0; i < n; ++i) {
        Process p;
        p.pid = i + 1;
        std::cout << "��������� " << p.pid << " �ĵ���ʱ�䣺";
        std::cin >> p.arrival_time;
        std::cout << "��������� " << p.pid << " ������ʱ�䣺";
        std::cin >> p.burst_time;
        std::cout << "��������� " << p.pid << " �����ȼ���";
        std::cin >> p.priority;
        p.remaining_time = p.burst_time;
        p.completion_time = 0;
        processes.push_back(p);
    }

    int scheduler_choice;
    std::cout << "��ѡ������㷨��\n1. ʱ��Ƭ��ת����\n2. ���ȼ�����\n";
    std::cin >> scheduler_choice;

    if (scheduler_choice == 1) {
        int time_slice;
        std::cout << "������ʱ��Ƭ��С��";
        std::cin >> time_slice;
        roundRobinScheduler(processes, time_slice);
    } else if (scheduler_choice == 2) {
        priorityScheduler(processes);
    }
}
// ��ʾ������Ϣ
void displayProcesses(const std::vector<Process>& processes) {
    std::cout << "\n========================= ���̱� =========================\n";
    std::cout << std::setw(5) << "��ʶ��"
              << std::setw(10) << "����ʱ��"
              << std::setw(10) << "����ʱ��"
              << std::setw(10) << "���ȼ�"
              << std::setw(15) << "���ʱ��"
              << std::setw(15) << "ʣ��ʱ��\n";
    std::cout << "------------------------------------------------------------\n";

    for (const auto& p : processes) {
        std::cout << std::setw(5) << p.pid
                  << std::setw(10) << p.arrival_time
                  << std::setw(10) << p.burst_time
                  << std::setw(10) << p.priority
                  << std::setw(15) << p.completion_time
                  << std::setw(15) << p.remaining_time << "\n";
    }

    std::cout << "============================================================\n";
}

void roundRobinScheduler(std::vector<Process>& processes, int time_slice) {
    std::queue<Process*> ready_queue; // ��������
    std::unordered_set<int> added_processes; // ��¼�Ѽ�����еĽ���
    int current_time = 0; // ��ǰʱ��
    int completed = 0; // ��ɽ�����
    int n = processes.size();

    // ������ʱ���������
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });
    // ��ѭ����ֱ�����н������
    while (completed < n) {
        // ������Ľ��̼������
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0 && added_processes.find(p.pid) == added_processes.end()) {
                ready_queue.push(&p); // �����������
                added_processes.insert(p.pid); // ���Ϊ�Ѽ���
                std::cout << "ʱ�� " << current_time << ": ���� " << p.pid << " �������������С�\n";
            }
        }
        // ����������в�Ϊ��
        if (!ready_queue.empty()) {
            Process* current_process = ready_queue.front(); // ȡ���׽���
            ready_queue.pop(); // �Ӷ������Ƴ�
            added_processes.erase(current_process->pid); // �Ӽ�¼���Ƴ�

            // ����ִ��ʱ�䣨ʵ��ִ��ʱ��Ϊʱ��Ƭ��ʣ��ʱ��Ľ�Сֵ��
            int execution_time = std::min(time_slice, current_process->remaining_time);
            std::cout << "ʱ�� " << current_time << ": ���� " << current_process->pid
                      << " ��ʼִ�У�Ԥ��ִ�� " << execution_time << " ��ʱ�䵥λ��\n";

            current_process->remaining_time -= execution_time; // ����ʣ��ʱ��
            current_time += execution_time; // ���µ�ǰʱ��
            // ����ִ���ڼ䵽��Ľ��̼������
            for (auto& p : processes) {
                if (p.arrival_time > (current_time - execution_time) && p.arrival_time <= current_time && p.remaining_time > 0 && added_processes.find(p.pid) == added_processes.end()) {
                    ready_queue.push(&p);
                    added_processes.insert(p.pid);
                    std::cout << "ʱ�� " << current_time << ": ���� " << p.pid << " �������������С�\n";
                }
            }
            // ����������
            if (current_process->remaining_time == 0) {
                current_process->completion_time = current_time; // ��¼���ʱ��
                completed++; // ������ɽ��̼���
                std::cout << "ʱ�� " << current_time << ": ���� " << current_process->pid
                          << " ����ɡ�\n";
            } else {
                // ���δ��ɣ����������¼����β
                std::cout << "ʱ�� " << current_time << ": ���� " << current_process->pid
                          << " ��ͣ��ʣ��ʱ�� " << current_process->remaining_time << "�����¼����β��\n";
                ready_queue.push(current_process); // ���¼����β
                added_processes.insert(current_process->pid); // ���±��Ϊ�Ѽ���
            }
        } else {
            // �������Ϊ�գ���ǰʱ���1���ȴ��½��̵���
            std::cout << "ʱ�� " << current_time << ": û�н����ھ��������У��ȴ�...\n";
            current_time++;
        }
    }

    // ʹ�� displayProcesses ����ͳһ���
    displayProcesses(processes);

}


// ���ȼ�����
void priorityScheduler(std::vector<Process>& processes) {
    // �����ȼ��������ȼ���ͬ�򰴵���ʱ������
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.priority == b.priority) {
            return a.arrival_time < b.arrival_time;
        }
        return a.priority < b.priority; // ���ȼ�����ԽС�����ȼ�Խ��
    });

    int current_time = 0; // ��ǰʱ��
    int n = processes.size(); // �ܽ�����
    int completed = 0; // ��ɽ�����

    std::cout << "\n=== ���ȼ����� ===\n";

    while (completed < n) {
        // �ҵ������ѵ�����δ��ɵĽ���
        std::vector<Process*> available_processes;
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                available_processes.push_back(&p);
            }
        }

        if (!available_processes.empty()) {
            // ѡ�����ȼ���ߵĽ��̣����ȼ�����ԽСԽ�ߣ�
            std::sort(available_processes.begin(), available_processes.end(),
                      [](const Process* a, const Process* b) {
                          if (a->priority == b->priority) {
                              return a->arrival_time < b->arrival_time;
                          }
                          return a->priority < b->priority;
                      });

            Process* current_process = available_processes.front();

            // �����ǰʱ��С�ڽ��̵���ʱ�䣬CPU���ڿ���״̬
            if (current_time < current_process->arrival_time) {
                std::cout << "ʱ�� " << current_time << ": CPU ���У��ȴ����� " << current_process->pid << " ���\n";
                current_time = current_process->arrival_time;
            }

            // ��ʼִ�н���
            std::cout << "ʱ�� " << current_time << ": ���� " << current_process->pid
                      << " ��ʼִ�� (����ʱ��: " << current_process->burst_time
                      << ", ���ȼ�: " << current_process->priority << ")��\n";

            // ��¼���̵Ŀ�ʼִ��ʱ�䣨��ѡ�����ڼ�����Ӧʱ��ȣ�
            // current_process->start_time = current_time;

            // ִ�н��̣�����ռʽ��ֱ����ɣ�
            current_time += current_process->burst_time;
            current_process->completion_time = current_time;
            current_process->remaining_time = 0;
            completed++;

            std::cout << "ʱ�� " << current_time << ": ���� " << current_process->pid << " ����ɡ�\n\n";
        } else {
            // ���û�п�ִ�еĽ��̣�CPU���ڿ���״̬
            std::cout << "ʱ�� " << current_time << ": û�п�ִ�еĽ��̣�CPU ���С�\n";
            current_time++;
        }
    }

    // ������н�����Ϣ
    displayProcesses(processes);



}












