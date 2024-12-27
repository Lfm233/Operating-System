//
// Created by muyu on 2024/12/23.
//

#include "../include/file.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// 调试模式
void debugMode4() {
    std::cout << "\n=== 文件管理 - 调试模式 ===\n";
    FileManager fm(50); // 假设初始磁头位置为 50
    fm.loadTrackSequence(R"(D:\operating system\Code\data\track_sequence.txt)"); // 调试模式加载固定文件
    fm.fcfs();
    fm.sstf();
    fm.scan();
}

// 用户模式
void userMode4() {
    int start_position;
    int disk_size = 200; // 假设磁盘大小为 200
    std::cout << "\n=== 文件管理 - 用户模式 ===\n";
    std::cout << "提示：磁头初始位置和磁盘请求序列应在 [0, " << disk_size << "] 范围内。\n";

    // 输入磁头初始位置
    while (true) {
        std::cout << "请输入磁头初始位置：";
        std::cin >> start_position;
        if (start_position >= 0 && start_position <= disk_size) {
            break;
        } else {
            std::cout << "无效的输入！磁头初始位置必须在 [0, " << disk_size << "] 范围内。\n";
        }
    }

    FileManager fm(start_position, disk_size);

    // 输入磁盘请求序列长度
    int n;
    while (true) {
        std::cout << "请输入磁道请求序列的长度（正整数）：";
        std::cin >> n;
        if (n > 0) {
            break;
        } else {
            std::cout << "无效的输入！长度必须为正整数。\n";
        }
    }

    // 输入磁盘请求序列
    std::cout << "请输入磁道请求序列（用空格分隔的 " << n << " 个数字，范围 [0, " << disk_size << "]）：\n";
    for (int i = 0; i < n; ++i) {
        int track;
        while (true) {
            std::cin >> track;
            if (track >= 0 && track <= disk_size) {
                fm.track_sequence.push_back(track);
                break;
            } else {
                std::cout << "无效的输入！磁道请求必须在 [0, " << disk_size << "] 范围内。\n";
            }
        }
    }

    // 打印输入的初始状态供用户确认
    std::cout << "\n您输入的初始磁头位置为：" << start_position << "\n";
    std::cout << "您输入的磁道请求序列为：";
    for (int track : fm.track_sequence) {
        std::cout << track << " ";
    }
    std::cout << "\n";

    // 选择调度算法
    int choice;
    while (true) {
        std::cout << "\n请选择调度算法：\n";
        std::cout << "1. FCFS (先来先服务)\n";
        std::cout << "2. SSTF (最短寻道时间优先)\n";
        std::cout << "3. SCAN (电梯算法)\n";
        std::cout << "请输入您的选择 (1/2/3)：";
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            break;
        } else {
            std::cout << "无效选择！请输入 1、2 或 3。\n";
        }
    }

    // 调用相应算法
    switch (choice) {
        case 1:
            fm.fcfs();
            break;
        case 2:
            fm.sstf();
            break;
        case 3:
            fm.scan();
            break;
    }
}




FileManager::FileManager(int start_position, int disk_size)
        : current_position(start_position), disk_size(disk_size), total_distance(0) {}

void FileManager::loadTrackSequence(const std::string& file_path) {
    std::ifstream file(file_path);
    int track;
    while (file >> track) {
        track_sequence.push_back(track);
    }
}

// 先来先服务
void FileManager::fcfs() {
    total_distance = 0;
    access_sequence.clear(); // 清空之前的访问顺序
    int previous_position = current_position;

    for (int track : track_sequence) {
        total_distance += std::abs(track - previous_position);
        previous_position = track;
        access_sequence.push_back(track); // 记录访问顺序
    }

    printResults("FCFS");
}

// 最短寻道优先
void FileManager::sstf() {
    total_distance = 0;
    access_sequence.clear(); // 清空之前的访问顺序
    int current_position = this->current_position; // 保持初始磁头位置

    std::vector<int> remaining_tracks = track_sequence;

    while (!remaining_tracks.empty()) {
        // 找到与当前磁头位置最近的磁道
        auto closest = std::min_element(remaining_tracks.begin(), remaining_tracks.end(),
                                        [current_position](int a, int b) {
                                            return std::abs(a - current_position) < std::abs(b - current_position);
                                        });

        total_distance += std::abs(*closest - current_position); // 累计移动距离
        current_position = *closest; // 更新磁头位置
        access_sequence.push_back(*closest); // 记录访问顺序
        remaining_tracks.erase(closest); // 移除已处理的磁道
    }

    printResults("SSTF");
}




// 电梯算法
void FileManager::scan() {
    total_distance = 0;
    access_sequence.clear(); // 清空之前的访问顺序
    int current_position = this->current_position; // 保持初始磁头位置

    std::sort(track_sequence.begin(), track_sequence.end());

    // 分为两部分：小于当前磁头位置的（left），大于等于当前磁头位置的（right）
    std::vector<int> left, right;
    for (int track : track_sequence) {
        if (track < current_position) {
            left.push_back(track);
        } else {
            right.push_back(track);
        }
    }

    // 按升序访问右侧请求
    for (int track : right) {
        total_distance += std::abs(track - current_position); // 累计移动距离
        current_position = track; // 更新磁头位置
        access_sequence.push_back(track); // 记录访问顺序
    }

    // 按降序访问左侧请求
    for (auto it = left.rbegin(); it != left.rend(); ++it) {
        total_distance += std::abs(*it - current_position); // 累计移动距离
        current_position = *it; // 更新磁头位置
        access_sequence.push_back(*it); // 记录访问顺序
    }

    printResults("SCAN");
}


// 打印函数
void FileManager::printResults(const std::string& method) {
    std::cout << method << " 磁盘调度过程:\n";

    int previous_position = this->current_position; // 保持初始磁头位置

    // 打印磁头访问顺序
    std::cout << "处理顺序: ";
    for (int track : access_sequence) {
        std::cout << track << " ";
    }
    std::cout << "\n";

    // 打印每一步的详细过程
    for (int track : access_sequence) {
        std::cout << "从 " << previous_position << " 移动到 " << track
                  << " （移动距离: " << std::abs(track - previous_position) << "）\n";
        previous_position = track;
    }
    std::cout << "总移动距离: " << total_distance << "\n";

    // 输出结果到 result.txt
    std::ofstream result_file(R"(D:\operating system\Code\data\result.txt)", std::ios::app);
    if (result_file.is_open()) {
        result_file << method << " 磁盘调度结果:\n";
        result_file << "处理顺序: ";
        for (int track : access_sequence) {
            result_file << track << " ";
        }
        result_file << "\n";
        result_file << "总移动距离: " << total_distance << "\n";
        result_file << "=====================\n";
        result_file.close();
        std::cout << "结果已保存到 result.txt 文件中。\n";
    } else {
        std::cerr << "无法打开 result.txt 文件进行写入。\n";
    }
}






