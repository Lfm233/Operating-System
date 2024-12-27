//
// Created by mu yu on 2024/12/23.
//


#include "../include/process.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip> // 用于格式化输出
#include <unordered_set>


// 调试模式
void debugMode1() {
    std::cout << "\n=== 进程管理 - 调试模式 ===\n";
    std::vector<Process> processes = {
            {1, 0, 5, 2, 5, 0},
            {2, 0, 5, 1, 5, 0},
            {3, 0, 1, 3, 1, 0}
    };

    int scheduler_choice;
    std::cout << "请选择调度算法：\n1. 时间片轮转调度\n2. 优先级调度\n";
    std::cin >> scheduler_choice;

    if (scheduler_choice == 1) {
        int time_slice = 4;
        roundRobinScheduler(processes, time_slice);
    } else if (scheduler_choice == 2) {
        priorityScheduler(processes);
    }
}

// 用户模式
void userMode1() {
    std::cout << "\n=== 进程管理 - 用户模式 ===\n";
    int n;
    std::cout << "请输入进程数量：";
    std::cin >> n;

    std::vector<Process> processes;
    for (int i = 0; i < n; ++i) {
        Process p;
        p.pid = i + 1;
        std::cout << "请输入进程 " << p.pid << " 的到达时间：";
        std::cin >> p.arrival_time;
        std::cout << "请输入进程 " << p.pid << " 的运行时间：";
        std::cin >> p.burst_time;
        std::cout << "请输入进程 " << p.pid << " 的优先级：";
        std::cin >> p.priority;
        p.remaining_time = p.burst_time;
        p.completion_time = 0;
        processes.push_back(p);
    }

    int scheduler_choice;
    std::cout << "请选择调度算法：\n1. 时间片轮转调度\n2. 优先级调度\n";
    std::cin >> scheduler_choice;

    if (scheduler_choice == 1) {
        int time_slice;
        std::cout << "请输入时间片大小：";
        std::cin >> time_slice;
        roundRobinScheduler(processes, time_slice);
    } else if (scheduler_choice == 2) {
        priorityScheduler(processes);
    }
}
// 显示进程信息
void displayProcesses(const std::vector<Process>& processes) {
    std::cout << "\n========================= 进程表 =========================\n";
    std::cout << std::setw(5) << "标识号"
              << std::setw(10) << "到达时间"
              << std::setw(10) << "运行时间"
              << std::setw(10) << "优先级"
              << std::setw(15) << "完成时间"
              << std::setw(15) << "剩余时间\n";
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
    std::queue<Process*> ready_queue; // 就绪队列
    std::unordered_set<int> added_processes; // 记录已加入队列的进程
    int current_time = 0; // 当前时间
    int completed = 0; // 完成进程数
    int n = processes.size();

    // 按到达时间排序进程
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });
    // 主循环，直到所有进程完成
    while (completed < n) {
        // 将到达的进程加入队列
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0 && added_processes.find(p.pid) == added_processes.end()) {
                ready_queue.push(&p); // 加入就绪队列
                added_processes.insert(p.pid); // 标记为已加入
                std::cout << "时间 " << current_time << ": 进程 " << p.pid << " 到达，加入就绪队列。\n";
            }
        }
        // 如果就绪队列不为空
        if (!ready_queue.empty()) {
            Process* current_process = ready_queue.front(); // 取队首进程
            ready_queue.pop(); // 从队列中移除
            added_processes.erase(current_process->pid); // 从记录中移除

            // 计算执行时间（实际执行时间为时间片或剩余时间的较小值）
            int execution_time = std::min(time_slice, current_process->remaining_time);
            std::cout << "时间 " << current_time << ": 进程 " << current_process->pid
                      << " 开始执行，预计执行 " << execution_time << " 个时间单位。\n";

            current_process->remaining_time -= execution_time; // 更新剩余时间
            current_time += execution_time; // 更新当前时间
            // 将在执行期间到达的进程加入队列
            for (auto& p : processes) {
                if (p.arrival_time > (current_time - execution_time) && p.arrival_time <= current_time && p.remaining_time > 0 && added_processes.find(p.pid) == added_processes.end()) {
                    ready_queue.push(&p);
                    added_processes.insert(p.pid);
                    std::cout << "时间 " << current_time << ": 进程 " << p.pid << " 到达，加入就绪队列。\n";
                }
            }
            // 如果进程完成
            if (current_process->remaining_time == 0) {
                current_process->completion_time = current_time; // 记录完成时间
                completed++; // 增加完成进程计数
                std::cout << "时间 " << current_time << ": 进程 " << current_process->pid
                          << " 已完成。\n";
            } else {
                // 如果未完成，将进程重新加入队尾
                std::cout << "时间 " << current_time << ": 进程 " << current_process->pid
                          << " 暂停，剩余时间 " << current_process->remaining_time << "，重新加入队尾。\n";
                ready_queue.push(current_process); // 重新加入队尾
                added_processes.insert(current_process->pid); // 重新标记为已加入
            }
        } else {
            // 如果队列为空，则当前时间加1，等待新进程到达
            std::cout << "时间 " << current_time << ": 没有进程在就绪队列中，等待...\n";
            current_time++;
        }
    }

    // 使用 displayProcesses 函数统一输出
    displayProcesses(processes);

}


// 优先级调度
void priorityScheduler(std::vector<Process>& processes) {
    // 按优先级排序，优先级相同则按到达时间排序
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.priority == b.priority) {
            return a.arrival_time < b.arrival_time;
        }
        return a.priority < b.priority; // 优先级数字越小，优先级越高
    });

    int current_time = 0; // 当前时间
    int n = processes.size(); // 总进程数
    int completed = 0; // 完成进程数

    std::cout << "\n=== 优先级调度 ===\n";

    while (completed < n) {
        // 找到所有已到达且未完成的进程
        std::vector<Process*> available_processes;
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                available_processes.push_back(&p);
            }
        }

        if (!available_processes.empty()) {
            // 选择优先级最高的进程（优先级数字越小越高）
            std::sort(available_processes.begin(), available_processes.end(),
                      [](const Process* a, const Process* b) {
                          if (a->priority == b->priority) {
                              return a->arrival_time < b->arrival_time;
                          }
                          return a->priority < b->priority;
                      });

            Process* current_process = available_processes.front();

            // 如果当前时间小于进程到达时间，CPU处于空闲状态
            if (current_time < current_process->arrival_time) {
                std::cout << "时间 " << current_time << ": CPU 空闲，等待进程 " << current_process->pid << " 到达。\n";
                current_time = current_process->arrival_time;
            }

            // 开始执行进程
            std::cout << "时间 " << current_time << ": 进程 " << current_process->pid
                      << " 开始执行 (运行时间: " << current_process->burst_time
                      << ", 优先级: " << current_process->priority << ")。\n";

            // 记录进程的开始执行时间（可选，用于计算响应时间等）
            // current_process->start_time = current_time;

            // 执行进程（非抢占式，直到完成）
            current_time += current_process->burst_time;
            current_process->completion_time = current_time;
            current_process->remaining_time = 0;
            completed++;

            std::cout << "时间 " << current_time << ": 进程 " << current_process->pid << " 已完成。\n\n";
        } else {
            // 如果没有可执行的进程，CPU处于空闲状态
            std::cout << "时间 " << current_time << ": 没有可执行的进程，CPU 空闲。\n";
            current_time++;
        }
    }

    // 输出所有进程信息
    displayProcesses(processes);



}












