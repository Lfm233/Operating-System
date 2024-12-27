//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_MEMORY_H
#define OPERATING_SYSTEM_MEMORY_H

#include <vector>
#include <string>

// 分区结构体
struct Partition {
    int start_address;    // 起始地址
    int size;             // 分区大小（KB）
    bool is_allocated;    // 是否已分配
    int process_id;       // 绑定的进程ID（-1表示未分配）

    // 重载 operator== 用来使用 std::remove（
    bool operator==(const Partition& other) const {
        return start_address == other.start_address &&
               size == other.size &&
               is_allocated == other.is_allocated &&
               process_id == other.process_id;
    }
};

// 全局变量
extern std::vector<Partition> allocatedPartitions;  // 已分配分区列表
extern std::vector<Partition> freePartitions;       // 未分配分区列表

// 功能函数
void initializeFixedPartition();
void initializeVariablePartition();
bool allocateFixedPartition(int process_size);
bool allocateVariablePartition(int process_size);
void releasePartition(int start_address);
void mergeFreePartitions();
void displayFixedPartition();
void displayVariablePartition();

void debugMode2();
void userMode2();

#endif //OPERATING_SYSTEM_MEMORY_H
