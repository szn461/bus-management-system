#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char start_time[20];
    char end_time[20];
    char interval[20];
    char averagetime[20];
} BusInfo;
//获取时刻表
BusInfo get_bus_info(const char *busnumber) {
    FILE *file = fopen("E:\\Bus management system\\时刻表", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[256];
    BusInfo info = {"", "", "",""};
    while (fgets(line, sizeof(line), file)) {
        // 使用 strncmp 确保行以 busnumber 开头，并且后面紧接着是空格或换行符
        if (strncmp(line, busnumber, strlen(busnumber)) == 0 &&
            (line[strlen(busnumber)] == ' ' || line[strlen(busnumber)] == '\n')) {
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (strstr(token, "首发车时间:")) {
                    sscanf(token + 11, "%5s", info.start_time);
                } else if (strstr(token, "末班车时间:")) {
                    sscanf(token + 11, "%5s", info.end_time);
                } else if (strstr(token, "发车间隔:")) {
                    sscanf(token + 9, "%6s", info.interval);
                } else if (strstr(token, "平均运行时长:")) {
                    sscanf(token + 13, "%6s", info.averagetime);
                }
                token = strtok(NULL, " ");
                }
                break;
            }
        }
    fclose(file);
    return info;
}


