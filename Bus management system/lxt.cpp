#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char busnumber[50];
    char zhan[200];
    int op;
    char bus[200][50];
} BusInfo1;
//输出所有车次编号
BusInfo1 read_bus_number(){
    FILE *file = fopen("E:\\Bus management system\\路线", "r"); // 打开文件
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    char line[1000]; // 用于存储读取的每一行文本
    BusInfo1 info = {"","",1,""};
    while (fgets(line, sizeof(line), file)) { // 逐行读取文件内容
        char *token = strtok(line, ":"); // 分割每一行文本，获取车次编号
        if (token != NULL) {
            char *remember[] = {token}; // 将车次编号存储到数组中
            int rememberSize = sizeof(remember) / sizeof(char*); // 计算数组元素个数
            for (int i = 0; i < rememberSize; i++) { // 按顺序输出车次编号
                printf("%d.%s\n",info.op ,remember[i]);
                strcpy(info.bus[info.op], token);
                info.op++;
            }
        }
    }
    fclose(file);
    return info;
}
//获取路线
BusInfo1 get_bus_road(const char *busnumber){
    FILE *file = fopen("E:\\Bus management system\\路线", "r");
    if (!file) {
        perror("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    char line[256];
    BusInfo1 info = {"",""};
    while (fgets(line, sizeof(line), file)) {
        char *token,*token2;
        token=strtok(line, ":");
        token2= strtok(NULL,":");
        if (strcmp(token,busnumber)==0) {
            strcpy(info.zhan,token2);
            break;
        }
    }
    fclose(file);
    return info;
}