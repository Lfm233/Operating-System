//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_PROCESS_H
#define OPERATING_SYSTEM_PROCESS_H

#include "windows.h"
#include <vector>
#include <string>

// 进程结构体
struct Process {
    int pid;           // 进程ID
    int arrival_time;  // 到达时间
    int burst_time;    // 执行时间
    int priority;      // 优先级
    int remaining_time; // 剩余执行时间（用于时间片轮转）
    int completion_time; // 完成时间
};

// 声明函数
void debugMode1();
void userMode1();
void roundRobinScheduler(std::vector<Process>& processes, int time_slice);
void priorityScheduler(std::vector<Process>& processes);
void displayProcesses(const std::vector<Process>& processes);

#endif // OPERATING_SYSTEM_PROCESS_H

