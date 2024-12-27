//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_FILE_H
#define OPERATING_SYSTEM_FILE_H

#include <vector>
#include <string>

class FileManager{
public:
    //构造函数
    explicit FileManager(int start_position,int disk_size = 200);
    //加载磁道序列
    void loadTrackSequence(const std::string& file_path);
    //获得磁头总距离
    int getTotalDistance() const {
        return total_distance;
    }
    // 磁盘调度算法
    void fcfs();
    void sstf();
    void scan();

    std::vector<int>track_sequence;
    std::vector<int> access_sequence; // 保存磁头的访问顺序

private:
    int current_position;   //当前磁头位置
    int disk_size;          //磁道总磁道数
    //磁道请求序列
    int total_distance;     //总磁头移动距离

    // 辅助函数
    void printResults(const std::string& method);
};

void debugMode4();
void userMode4();


#endif //OPERATING_SYSTEM_FILE_H
