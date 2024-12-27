//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_PROCESS_H
#define OPERATING_SYSTEM_PROCESS_H

#include "windows.h"
#include <vector>
#include <string>

// ���̽ṹ��
struct Process {
    int pid;           // ����ID
    int arrival_time;  // ����ʱ��
    int burst_time;    // ִ��ʱ��
    int priority;      // ���ȼ�
    int remaining_time; // ʣ��ִ��ʱ�䣨����ʱ��Ƭ��ת��
    int completion_time; // ���ʱ��
};

// ��������
void debugMode1();
void userMode1();
void roundRobinScheduler(std::vector<Process>& processes, int time_slice);
void priorityScheduler(std::vector<Process>& processes);
void displayProcesses(const std::vector<Process>& processes);

#endif // OPERATING_SYSTEM_PROCESS_H

