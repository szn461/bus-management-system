#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//定义公交车信息结构体
typedef struct {
    char busnumber[50];
    int number_of_stations;
} BusInfo2;
//计算给定公交车号在路线文件中经过的站点数量
BusInfo2 get_station_number(const char *busnumber) {
    FILE *file = fopen("E:\\Bus management system\\路线", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    char line[256];
    char str[500];
    char *p = str;
    BusInfo2 info = {"", 0};
    while (fgets(line, sizeof(line), file)) {
        char *token,*token2;
        token = strtok(line, ":");
        token2= strtok(NULL,":");
        if (strcmp(token, busnumber) == 0) {
            sscanf(token2,"%s",str);
            break;
        }
    }
    while (*p != '\0') {
        if (*p == '>') {
            info.number_of_stations++;
        }
        p++;
    }
    fclose(file);
    return info;
}
