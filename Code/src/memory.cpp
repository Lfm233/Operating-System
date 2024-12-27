// memory.cpp
#include "../include/memory.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// 全局变量
std::vector<Partition> allocatedPartitions; // 已分配分区列表
std::vector<Partition> freePartitions;      // 未分配分区列表
int next_process_id = 1;                    // 进程ID自动分配

// 初始化固定分区
void initializeFixedPartition() {
    allocatedPartitions.clear();
    freePartitions.clear();
    int total_size = 128; // 总内存大小（单位：KB）
    int os_reserved = 4;  // OS占用的内存
    int partition_size = 12; // 固定分区大小
    int available_size = total_size - os_reserved;

    // 初始化固定分区
    for (int i = os_reserved; i < total_size; i += partition_size) {
        freePartitions.push_back({i, partition_size, false, -1});
    }
    std::cout << "固定分区内存已初始化。\n";
}

// 初始化可变分区
void initializeVariablePartition() {
    allocatedPartitions.clear();
    freePartitions.clear();
    freePartitions.push_back({4, 124, false, -1}); // 可变分区初始为124KB空闲
    std::cout << "可变分区内存已初始化。\n";
}

// 分配固定分区（自动分配 process_id）
bool allocateFixedPartition(int process_size) {
    for (auto it = freePartitions.begin(); it != freePartitions.end(); ++it) {
        if (!it->is_allocated && it->size >= process_size) {
            int start_address = it->start_address;
            int process_id = next_process_id++;

            // 分配分区
            it->is_allocated = true;
            it->process_id = process_id;

            // 将分区添加到已分配列表
            allocatedPartitions.push_back(*it);

            // 从空闲分区列表中移除该分区
            freePartitions.erase(it);

            std::cout << "进程 " << process_id << " 分配到固定分区，起始地址: "
                      << start_address << "。\n";

            return true; // 分配成功
        }
    }
    std::cout << "分配失败：未找到合适的固定分区，进程大小: " << process_size << "KB。\n";
    return false; // 分配失败
}

// 分配可变分区（自动分配 process_id）
bool allocateVariablePartition(int process_size) {
    for (auto it = freePartitions.begin(); it != freePartitions.end(); ++it) {
        if (it->size >= process_size) {
            int remaining_size = it->size - process_size;
            int start_address = it->start_address;
            int process_id = next_process_id++;

            // 修改空闲分区
            it->start_address += process_size;
            it->size = remaining_size;

            // 添加到已分配分区
            Partition allocated = {start_address, process_size, true, process_id};
            allocatedPartitions.push_back(allocated);

            // 如果剩余空间为0，移除空闲分区
            if (remaining_size == 0) {
                freePartitions.erase(it);
            }

            std::cout << "进程 " << process_id << " 分配到可变分区，起始地址: "
                      << start_address << "。\n";
            return true; // 分配成功
        }
    }
    std::cout << "分配失败：未找到足够大的空闲分区，进程大小: " << process_size << "KB。\n";
    return false; // 分配失败
}

// 显示固定分区状态
void displayFixedPartition() {
    std::cout << "\n固定分区内存状态:\n";
    std::cout << std::setw(15) << "起始地址" << std::setw(10) << "大小(KB)"
              << std::setw(15) << "状态" << std::setw(15) << "进程ID\n";
    std::cout << "------------------------------------------------------------\n";

    // 显示已分配的固定分区
    for (const auto& partition : allocatedPartitions) {
        if (partition.size == 12) { // 仅显示固定分区（假设固定分区大小为12KB）
            std::cout << std::setw(15) << partition.start_address
                      << std::setw(10) << partition.size
                      << std::setw(15) << "已分配"
                      << std::setw(15) << partition.process_id << "\n";
        }
    }

    // 显示空闲的固定分区
    for (const auto& partition : freePartitions) {
        if (partition.size == 12) { // 仅显示固定分区
            std::cout << std::setw(15) << partition.start_address
                      << std::setw(10) << partition.size
                      << std::setw(15) << "空闲"
                      << std::setw(15) << "-" << "\n";
        }
    }

    std::cout << "============================================================\n";
}

// 显示可变分区状态
void displayVariablePartition() {
    std::cout << "\n可变分区内存状态:\n";
    std::cout << "已分配分区:\n";
    if (allocatedPartitions.empty()) {
        std::cout << "无\n";
    } else {
        std::cout << std::setw(15) << "起始地址" << std::setw(10) << "大小(KB)" << std::setw(15) << "进程ID\n";
        for (const auto& partition : allocatedPartitions) {
            if (partition.size != 12) { // 排除固定分区
                std::cout << std::setw(15) << partition.start_address
                          << std::setw(10) << partition.size
                          << std::setw(15) << partition.process_id << "\n";
            }
        }
    }

    std::cout << "空闲分区:\n";
    if (freePartitions.empty()) {
        std::cout << "无\n";
    } else {
        std::cout << std::setw(15) << "起始地址" << std::setw(10) << "大小(KB)\n";
        for (const auto& partition : freePartitions) {
            if (partition.size != 12) { // 排除固定分区
                std::cout << std::setw(15) << partition.start_address
                          << std::setw(10) << partition.size << "\n";
            }
        }
    }

    std::cout << "============================================================\n";
}

// 合并相邻的空闲分区
void mergeFreePartitions() {
    // 按起始地址排序空闲分区
    std::sort(freePartitions.begin(), freePartitions.end(), [](const Partition& a, const Partition& b) {
        return a.start_address < b.start_address;
    });

    // 合并相邻的分区
    for (size_t i = 0; i < freePartitions.size() - 1; ) {
        if (freePartitions[i].start_address + freePartitions[i].size == freePartitions[i + 1].start_address) {
            // 合并分区
            freePartitions[i].size += freePartitions[i + 1].size;
            freePartitions.erase(freePartitions.begin() + i + 1);
        } else {
            ++i;
        }
    }
}

// 释放分区
void releasePartition(int start_address) {
    // 遍历已分配分区列表，找到匹配的分区
    for (auto it = allocatedPartitions.begin(); it != allocatedPartitions.end(); ++it) {
        if (it->start_address == start_address) {
            // 打印释放信息
            std::cout << "释放分区，起始地址: " << start_address << ", 大小: " << it->size << "KB。\n";

            // 更新分区状态为未分配
            it->is_allocated = false;
            it->process_id = -1;

            // 将分区移动到空闲分区列表
            freePartitions.push_back(*it);
            allocatedPartitions.erase(it);

            // 合并空闲分区
            mergeFreePartitions();

            return;
        }
    }

    // 如果未找到分区，打印错误信息
    std::cout << "错误：未找到起始地址为 " << start_address << " 的已分配分区。\n";
}

// 调试模式
void debugMode2() {
    std::cout << "\n=== 调试模式 ===\n";

    // 初始化固定分区并分配进程
    std::cout << "\n--- 初始化固定分区 ---\n";
    initializeFixedPartition();
    allocateFixedPartition(10); // 进程ID = 1
    allocateFixedPartition(12);  // 进程ID = 2
    allocateFixedPartition(20);  // 进程ID = 3
    displayFixedPartition();

    // 初始化可变分区并分配进程
    std::cout << "\n--- 初始化可变分区 ---\n";
    initializeVariablePartition(); // 初始化可变分区
    allocateVariablePartition(30); // 进程ID = 4 分配30KB
    allocateVariablePartition(50); // 进程ID = 5 分配50KB
    displayVariablePartition();

    // 释放分区
    std::cout << "\n--- 释放分区 ---\n";
    releasePartition(34); // 假设释放起始地址为34的分区
    displayVariablePartition();
}

// 用户模式
void userMode2() {
    int choice;
    while (true) {
        std::cout << "\n=== 用户模式 ===\n";
        std::cout << "1. 初始化固定分区\n";
        std::cout << "2. 初始化可变分区\n";
        std::cout << "3. 分配固定分区\n";
        std::cout << "4. 分配可变分区\n";
        std::cout << "5. 显示固定分区状态\n";
        std::cout << "6. 显示可变分区状态\n";
        std::cout << "7. 释放分区\n";
        std::cout << "8. 退出用户模式\n";
        std::cout << "请输入您的选择: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                initializeFixedPartition();
                break;
            case 2:
                initializeVariablePartition();
                break;
            case 3: { // 分配固定分区
                int process_size;
                std::cout << "请输入进程大小 (KB): ";
                std::cin >> process_size;
                if (!allocateFixedPartition(process_size)) {
                    std::cout << "建议查看固定分区状态或调整进程大小。\n";
                }
                break;
            }
            case 4: { // 分配可变分区
                int process_size;
                std::cout << "请输入进程大小 (KB): ";
                std::cin >> process_size;
                if (!allocateVariablePartition(process_size)) {
                    std::cout << "建议查看可变分区状态或调整进程大小。\n";
                }
                break;
            }
            case 5:
                displayFixedPartition();
                break;
            case 6:
                displayVariablePartition();
                break;
            case 7: { // 释放分区
                int start_address;
                std::cout << "请输入释放分区的起始地址: ";
                std::cin >> start_address;
                releasePartition(start_address);
                break;
            }
            case 8:
                std::cout << "退出用户模式。\n";
                return;
            default:
                std::cout << "无效的选择，请重试。\n";
        }
    }
}
