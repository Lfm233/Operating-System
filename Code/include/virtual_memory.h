//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_VIRTUAL_MEMORY_H
#define OPERATING_SYSTEM_VIRTUAL_MEMORY_H

#include <vector>
#include <string>

class VirtualMemory{
public:
    VirtualMemory(int frame_count);
    //加载页面序列
    void loadPageSequence(const std::string& file_path);
    //页面调度算法
    void fifo(); //先进先出算法
    void lru();//最近最少使用调度算法
    void lfu(); //最近最不常用调度算法
    void resetFrames(); // 添加重置方法
    std::vector<int> page_sequence;
private:
    int frame_count; //内存帧数
    //页面访问序列
    std::vector<int> memory;    //模拟内存
    std::vector<int> frames; // 固定大小的帧
};

void debugMode3();
void userMode3();

#endif //OPERATING_SYSTEM_VIRTUAL_MEMORY_H
