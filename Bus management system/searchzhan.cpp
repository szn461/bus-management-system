#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    int nowstation;
} BusInfo4;
//查询当前站是第几站
BusInfo4 get_station(const char *busnumber,const char *stationname){
    char line[1000];
    BusInfo4 info = {0};
    FILE *file = fopen("E:\\Bus management system\\路线", "r");
    if (file == NULL) {
        printf("无法打开文件！\n");
        exit(EXIT_FAILURE);
    }
    int stationIndex = -1; // 初始化车站索引为-1，表示未找到
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, busnumber, strlen(busnumber)) == 0) { // 检查车次编号是否匹配
            char *token = strtok(line, ":"); // 使用冒号分割字符串，获取车站信息部分
            if (token != NULL) { // 确保有车站信息部分
                token = strtok(NULL, "->"); // 使用箭头和冒号分割字符串，获取车站信息
                int index = 0;
                while (token != NULL) {
                    if (strcmp(token, stationname) == 0) { // 检查车站是否匹配
                        stationIndex = index + 1; // 计算车站的索引，从1开始计数
                        break; // 找到后退出循环
                    }
                    token = strtok(NULL, "->"); // 继续分割字符串，获取下一个车站信息
                    index++;
                }
            }
            break; // 找到后退出循环
        }
    }
    fclose(file); // 关闭文件
    if (stationIndex != -1) { // 如果找到了车站索引，则输出该索引
        info.nowstation=stationIndex;
    } else { // 如果没有找到车站索引，则输出错误信息
        printf("未找到对应的车站！\n");
        info.nowstation=stationIndex;
    }
}