//
// Created by muyu on 2024/12/23.
//

#include "../include/file.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// ����ģʽ
void debugMode4() {
    std::cout << "\n=== �ļ����� - ����ģʽ ===\n";
    FileManager fm(50); // �����ʼ��ͷλ��Ϊ 50
    fm.loadTrackSequence(R"(D:\operating system\Code\data\track_sequence.txt)"); // ����ģʽ���ع̶��ļ�
    fm.fcfs();
    fm.sstf();
    fm.scan();
}

// �û�ģʽ
void userMode4() {
    int start_position;
    int disk_size = 200; // ������̴�СΪ 200
    std::cout << "\n=== �ļ����� - �û�ģʽ ===\n";
    std::cout << "��ʾ����ͷ��ʼλ�úʹ�����������Ӧ�� [0, " << disk_size << "] ��Χ�ڡ�\n";

    // �����ͷ��ʼλ��
    while (true) {
        std::cout << "�������ͷ��ʼλ�ã�";
        std::cin >> start_position;
        if (start_position >= 0 && start_position <= disk_size) {
            break;
        } else {
            std::cout << "��Ч�����룡��ͷ��ʼλ�ñ����� [0, " << disk_size << "] ��Χ�ڡ�\n";
        }
    }

    FileManager fm(start_position, disk_size);

    // ��������������г���
    int n;
    while (true) {
        std::cout << "������ŵ��������еĳ��ȣ�����������";
        std::cin >> n;
        if (n > 0) {
            break;
        } else {
            std::cout << "��Ч�����룡���ȱ���Ϊ��������\n";
        }
    }

    // ���������������
    std::cout << "������ŵ��������У��ÿո�ָ��� " << n << " �����֣���Χ [0, " << disk_size << "]����\n";
    for (int i = 0; i < n; ++i) {
        int track;
        while (true) {
            std::cin >> track;
            if (track >= 0 && track <= disk_size) {
                fm.track_sequence.push_back(track);
                break;
            } else {
                std::cout << "��Ч�����룡�ŵ���������� [0, " << disk_size << "] ��Χ�ڡ�\n";
            }
        }
    }

    // ��ӡ����ĳ�ʼ״̬���û�ȷ��
    std::cout << "\n������ĳ�ʼ��ͷλ��Ϊ��" << start_position << "\n";
    std::cout << "������Ĵŵ���������Ϊ��";
    for (int track : fm.track_sequence) {
        std::cout << track << " ";
    }
    std::cout << "\n";

    // ѡ������㷨
    int choice;
    while (true) {
        std::cout << "\n��ѡ������㷨��\n";
        std::cout << "1. FCFS (�����ȷ���)\n";
        std::cout << "2. SSTF (���Ѱ��ʱ������)\n";
        std::cout << "3. SCAN (�����㷨)\n";
        std::cout << "����������ѡ�� (1/2/3)��";
        std::cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            break;
        } else {
            std::cout << "��Чѡ�������� 1��2 �� 3��\n";
        }
    }

    // ������Ӧ�㷨
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

// �����ȷ���
void FileManager::fcfs() {
    total_distance = 0;
    access_sequence.clear(); // ���֮ǰ�ķ���˳��
    int previous_position = current_position;

    for (int track : track_sequence) {
        total_distance += std::abs(track - previous_position);
        previous_position = track;
        access_sequence.push_back(track); // ��¼����˳��
    }

    printResults("FCFS");
}

// ���Ѱ������
void FileManager::sstf() {
    total_distance = 0;
    access_sequence.clear(); // ���֮ǰ�ķ���˳��
    int current_position = this->current_position; // ���ֳ�ʼ��ͷλ��

    std::vector<int> remaining_tracks = track_sequence;

    while (!remaining_tracks.empty()) {
        // �ҵ��뵱ǰ��ͷλ������Ĵŵ�
        auto closest = std::min_element(remaining_tracks.begin(), remaining_tracks.end(),
                                        [current_position](int a, int b) {
                                            return std::abs(a - current_position) < std::abs(b - current_position);
                                        });

        total_distance += std::abs(*closest - current_position); // �ۼ��ƶ�����
        current_position = *closest; // ���´�ͷλ��
        access_sequence.push_back(*closest); // ��¼����˳��
        remaining_tracks.erase(closest); // �Ƴ��Ѵ���Ĵŵ�
    }

    printResults("SSTF");
}




// �����㷨
void FileManager::scan() {
    total_distance = 0;
    access_sequence.clear(); // ���֮ǰ�ķ���˳��
    int current_position = this->current_position; // ���ֳ�ʼ��ͷλ��

    std::sort(track_sequence.begin(), track_sequence.end());

    // ��Ϊ�����֣�С�ڵ�ǰ��ͷλ�õģ�left�������ڵ��ڵ�ǰ��ͷλ�õģ�right��
    std::vector<int> left, right;
    for (int track : track_sequence) {
        if (track < current_position) {
            left.push_back(track);
        } else {
            right.push_back(track);
        }
    }

    // ����������Ҳ�����
    for (int track : right) {
        total_distance += std::abs(track - current_position); // �ۼ��ƶ�����
        current_position = track; // ���´�ͷλ��
        access_sequence.push_back(track); // ��¼����˳��
    }

    // ����������������
    for (auto it = left.rbegin(); it != left.rend(); ++it) {
        total_distance += std::abs(*it - current_position); // �ۼ��ƶ�����
        current_position = *it; // ���´�ͷλ��
        access_sequence.push_back(*it); // ��¼����˳��
    }

    printResults("SCAN");
}


// ��ӡ����
void FileManager::printResults(const std::string& method) {
    std::cout << method << " ���̵��ȹ���:\n";

    int previous_position = this->current_position; // ���ֳ�ʼ��ͷλ��

    // ��ӡ��ͷ����˳��
    std::cout << "����˳��: ";
    for (int track : access_sequence) {
        std::cout << track << " ";
    }
    std::cout << "\n";

    // ��ӡÿһ������ϸ����
    for (int track : access_sequence) {
        std::cout << "�� " << previous_position << " �ƶ��� " << track
                  << " ���ƶ�����: " << std::abs(track - previous_position) << "��\n";
        previous_position = track;
    }
    std::cout << "���ƶ�����: " << total_distance << "\n";

    // �������� result.txt
    std::ofstream result_file(R"(D:\operating system\Code\data\result.txt)", std::ios::app);
    if (result_file.is_open()) {
        result_file << method << " ���̵��Ƚ��:\n";
        result_file << "����˳��: ";
        for (int track : access_sequence) {
            result_file << track << " ";
        }
        result_file << "\n";
        result_file << "���ƶ�����: " << total_distance << "\n";
        result_file << "=====================\n";
        result_file.close();
        std::cout << "����ѱ��浽 result.txt �ļ��С�\n";
    } else {
        std::cerr << "�޷��� result.txt �ļ�����д�롣\n";
    }
}






