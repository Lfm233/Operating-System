//
// Created by muyu on 2024/12/23.
//


#include "../include/virtual_memory.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <list>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <cstdint>

// 构造函数实现
VirtualMemory::VirtualMemory(int frame_count) : frame_count(frame_count) {
    frames.assign(frame_count, -1); // 初始化帧为空
}
// 添加重置方法
void VirtualMemory::resetFrames() {
    std::fill(frames.begin(), frames.end(), -1);
}

// 调试模式
void debugMode3() {
    std::cout << "\n=== 虚拟内存管理 - 调试模式 ===\n";
    VirtualMemory vm(3); // 假设帧数为 3
    vm.loadPageSequence(R"(D:\operating system\Code\data\page_sequence.txt)"); // 假设调试模式使用固定的测试文件
    vm.fifo();
    vm.lru();
    vm.lfu();
}

// 用户模式
void userMode3() {
    int frame_count;
    std::cout << "\n请输入帧数：";
    std::cin >> frame_count;

    VirtualMemory vm(frame_count);

    int n;
    std::cout << "请输入页面序列的长度：";
    std::cin >> n;

    std::cout << "请输入页面序列（以空格分隔）：";
    for (int i = 0; i < n; ++i) {
        int page;
        std::cin >> page;
        vm.page_sequence.push_back(page);
    }

    int choice;
    std::cout << "\n请选择调度算法：\n1. FIFO\n2. LRU\n3. LFU\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            vm.fifo();
            break;
        case 2:
            vm.lru();
            break;
        case 3:
            vm.lfu();
            break;
        default:
            std::cout << "无效选择！\n";
    }
}



void VirtualMemory::loadPageSequence(const std::string& file_path) {
    std::ifstream file(file_path);
    int page;
    while (file >> page) {
        page_sequence.push_back(page);
    }
}

void VirtualMemory::fifo() {
    resetFrames();
    std::queue<int> fifo_queue; // 用于跟踪页面的加载顺序
    int page_faults = 0;
    int page_hits = 0;

    std::cout << "\n=== FIFO 页面置换 ===\n";

    // 输出表格头
    std::cout << std::setw(8) << "页面号";
    for (int page : page_sequence) {
        std::cout << std::setw(6) << page;
    }
    std::cout << "\n";

    // 表格数据准备
    std::vector<std::vector<std::string>> table(frame_count, std::vector<std::string>(page_sequence.size(), ""));
    std::vector<std::string> fault_info(page_sequence.size(), ""); // 是否缺页
    std::vector<std::string> replace_info(page_sequence.size(), ""); // 是否置换

    for (size_t i = 0; i < page_sequence.size(); ++i) {
        int page = page_sequence[i];
        // 检查页面是否已经在帧中
        auto it = std::find(frames.begin(), frames.end(), page);
        if (it == frames.end()) { // 页面缺失
            page_faults++;
            fault_info[i] = "√";

            if (fifo_queue.size() == frame_count) { // 内存已满，进行替换
                int oldest_page = fifo_queue.front();
                fifo_queue.pop();

                // 查找要替换的帧
                auto replace_it = std::find(frames.begin(), frames.end(), oldest_page);
                if (replace_it != frames.end()) {
                    *replace_it = page;
                    replace_info[i] = "√";
                }
            } else { // 内存未满，找到第一个空帧
                auto empty_it = std::find(frames.begin(), frames.end(), -1);
                if (empty_it != frames.end()) {
                    *empty_it = page;
                }
            }
            fifo_queue.push(page);
        } else { // 页面命中
            page_hits++;
        }

        // 更新表格内容
        for (size_t j = 0; j < frames.size(); ++j) {
            if (frames[j] != -1) {
                table[j][i] = std::to_string(frames[j]);
            }
        }
    }

    // 打印表格
    for (size_t row = 0; row < frame_count; ++row) {
        std::cout << std::setw(8) << ("页面框" + std::to_string(row + 1));
        for (size_t col = 0; col < page_sequence.size(); ++col) {
            std::cout << std::setw(6) << (table[row][col].empty() ? " " : table[row][col]);
        }
        std::cout << "\n";
    }
    std::cout << std::setw(8) << "是否缺页";
    for (const auto& info : fault_info) {
        std::cout << std::setw(6) << info;
    }
    std::cout << "\n";

    std::cout << std::setw(8) << "是否置换";
    for (const auto& info : replace_info) {
        std::cout << std::setw(6) << info;
    }
    std::cout << "\n";

    // 总结
    std::cout << "\n总页面缺失次数: " << page_faults << "\n";
    std::cout << "总页面命中次数: " << page_hits << "\n";
    std::cout << "页面缺失率: " << (double)page_faults / page_sequence.size() * 100 << "%\n";
}

void VirtualMemory::lru() {
    resetFrames();
    std::list<int> memory_list; // 用于跟踪页面使用顺序，front为MRU，back为LRU
    int page_faults = 0;
    int page_hits = 0;

    std::cout << "\n=== LRU 页面置换 ===\n";

    // 输出表格头
    std::cout << std::setw(8) << "页面号";
    for (int page : page_sequence) {
        std::cout << std::setw(6) << page;
    }
    std::cout << "\n";

    // 表格数据准备
    std::vector<std::vector<std::string>> table(frame_count, std::vector<std::string>(page_sequence.size(), ""));
    std::vector<std::string> fault_info(page_sequence.size(), " "); // 是否缺页
    std::vector<std::string> replace_info(page_sequence.size(), " "); // 是否置换

    for (size_t i = 0; i < page_sequence.size(); ++i) {
        int page = page_sequence[i];
        auto it = std::find(memory_list.begin(), memory_list.end(), page);
        if (it == memory_list.end()) { // 页面缺失
            page_faults++;
            fault_info[i] = "√";

            if (memory_list.size() == frame_count) { // 如果内存已满
                int removed_page = memory_list.back(); // 淘汰最久未使用的页面
                memory_list.pop_back();
                replace_info[i] = "√"; // 记录发生置换

                // 查找并替换被移除的页面
                auto replace_it = std::find(frames.begin(), frames.end(), removed_page);
                if (replace_it != frames.end()) {
                    *replace_it = page;
                }
            } else { // 内存未满，找到第一个空帧
                auto empty_it = std::find(frames.begin(), frames.end(), -1);
                if (empty_it != frames.end()) {
                    *empty_it = page;
                }
            }
        } else { // 页面命中
            page_hits++;
            memory_list.erase(it); // 更新为最近使用
        }
        memory_list.push_front(page); // 将页面移到MRU位置

        // 更新表格内容
        // 直接使用 frames[j] 来填充表格，而不是 memory_list
        for (size_t j = 0; j < frames.size(); ++j) {
            if (frames[j] != -1) {
                table[j][i] = std::to_string(frames[j]);
            }
        }
    }

    // 打印表格
    for (size_t row = 0; row < frame_count; ++row) {
        std::cout << std::setw(8) << ("页面框" + std::to_string(row + 1));
        for (size_t col = 0; col < page_sequence.size(); ++col) {
            std::cout << std::setw(6) << (table[row][col].empty() ? " " : table[row][col]);
        }
        std::cout << "\n";
    }

    // 打印是否缺页
    std::cout << std::setw(8) << "是否缺页";
    for (const auto& info : fault_info) {
        std::cout << std::setw(6) << info;
    }
    std::cout << "\n";

    // 打印是否置换
    std::cout << std::setw(8) << "是否置换";
    for (const auto& info : replace_info) {
        std::cout << std::setw(6) << info;
    }
    std::cout << "\n";

    // 总结
    std::cout << "\n总页面缺失次数: " << page_faults << "\n";
    std::cout << "总页面命中次数: " << page_hits << "\n";
    std::cout << "页面缺失率: " << (double)page_faults / page_sequence.size() * 100 << "%\n";
}




void VirtualMemory::lfu() {
    resetFrames(); // Reset frames to -1
    std::unordered_map<int, int> frequency_map; // Record page access frequencies
    std::unordered_map<int, int> load_time_map; // Record page load times for tie-breaking
    int page_faults = 0;
    int page_hits = 0;
    int current_time = 0; // To track load times

    std::cout << "\n=== LFU 页面置换 ===\n";

    // Print page sequence header
    std::cout << std::setw(8) << "页面号";
    for (int page : page_sequence) {
        std::cout << std::setw(6) << page;
    }
    std::cout << "\n";

    // Initialize table
    std::vector<std::vector<std::string>> table(frame_count, std::vector<std::string>(page_sequence.size(), ""));
    std::vector<std::string> fault_info(page_sequence.size(), " "); // 是否缺页
    std::vector<std::string> replace_info(page_sequence.size(), " "); // 是否置换

    for (size_t i = 0; i < page_sequence.size(); ++i) {
        int page = page_sequence[i];
        frequency_map[page]++; // Increase frequency
        current_time++; // Increment time

        // Check if page is already in frames
        auto frame_it = std::find(frames.begin(), frames.end(), page);
        if (frame_it != frames.end()) { // Page hit
            page_hits++;
            // Optional: Update load_time_map for LRU tie-breaking
            // Not strictly necessary for LFU, but useful if implementing LFU with LRU tie-break
        }
        else { // Page miss
            page_faults++;
            fault_info[i] = "√";

            // If there's an empty frame, load the page into it
            auto empty_it = std::find(frames.begin(), frames.end(), -1);
            if (empty_it != frames.end()) {
                *empty_it = page;
                load_time_map[page] = current_time;
            }
            else { // Need to replace a page
                // Find the minimum frequency
                int min_freq = INT32_MAX;
                for (int p : frames) {
                    if (frequency_map[p] < min_freq) {
                        min_freq = frequency_map[p];
                    }
                }

                // Collect all pages with the minimum frequency
                std::vector<int> candidates;
                for (int p : frames) {
                    if (frequency_map[p] == min_freq) {
                        candidates.push_back(p);
                    }
                }

                // Among candidates, find the one with the earliest load time
                int lfu_page = -1;
                int earliest_time = INT32_MAX;
                for (int p : candidates) {
                    if (load_time_map[p] < earliest_time) {
                        earliest_time = load_time_map[p];
                        lfu_page = p;
                    }
                }

                // Replace the LFU page with the new page
                auto replace_it = std::find(frames.begin(), frames.end(), lfu_page);
                if (replace_it != frames.end()) {
                    *replace_it = page;
                    load_time_map.erase(lfu_page); // Remove old page's load time
                    load_time_map[page] = current_time; // Record new page's load time
                    replace_info[i] = "√";
                }
            }
        }

        // Update table content
        for (size_t j = 0; j < frames.size(); ++j) {
            if (frames[j] != -1) {
                table[j][i] = std::to_string(frames[j]);
            }
        }
    }

    // Print table
    for (size_t row = 0; row < frame_count; ++row) {
        std::cout << std::setw(8) << ("页面框" + std::to_string(row + 1));
        for (size_t col = 0; col < page_sequence.size(); ++col) {
            std::cout << std::setw(6) << (table[row][col].empty() ? " " : table[row][col]);
        }
        std::cout << "\n";
    }

    // Print fault_info
    std::cout << std::setw(9) << "是否缺页";
    for (const auto& info : fault_info) {
        std::cout << std::setw(5) << " " << info;
    }
    std::cout << "\n";

    // Print replace_info
    std::cout << std::setw(9) << "是否置换";
    for (const auto& info : replace_info) {
        std::cout << "  " << std::setw(4) << info;
    }
    std::cout << "\n";

    // Print summary
    std::cout << "\n总页面缺失次数: " << page_faults << "\n";
    std::cout << "总页面命中次数: " << page_hits << "\n";
    std::cout << "页面缺失率: " << (double)page_faults / page_sequence.size() * 100 << "%\n";
}





