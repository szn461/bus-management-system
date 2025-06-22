#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 检查公交车编号是否已经存在
int checkBusNumberExists(const char *filename, const char *busNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // 文件不存在，认为编号不存在
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, busNumber) != NULL) {
            fclose(file);
            return 1; // 找到了匹配的编号
        }
    }
    fclose(file);
    return 0; // 未找到匹配的编号
}
// 向txt文件中添加公交车信息
void addBusInfo(const char *filename, const char *busNumber, const char *startStation, const char *endStation, int numStations, char *stations[]) {
    FILE *file = fopen(filename, "a"); // 以追加模式打开文件
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    // 检查公交车编号是否已经存在
    if (checkBusNumberExists(filename, busNumber)) {
        printf("添加失败：编号 %s 已经存在\n", busNumber);
        fclose(file);
        return;
    }
    // 构造新的公交车信息字符串
    char newBusInfo[256];
    fprintf(file,"\n");
    snprintf(newBusInfo, sizeof(newBusInfo), "%s(%s->%s):%s", busNumber, startStation, endStation, startStation);
    strcat(newBusInfo, "->");
    strcat(newBusInfo, stations[0]);
    for (int i = 1; i < numStations ; i++) {
        strcat(newBusInfo, "->");
        strcat(newBusInfo, stations[i]);
    }
    strcat(newBusInfo, "->");
    strcat(newBusInfo, endStation);
    // 将新的公交车信息写入文件
    fputs(newBusInfo, file);
    printf("添加成功\n");
    fclose(file);
}
int main() {
    char filename[] = "E:\\Bus management system\\路线";
    char busNumber[50];
    char startStation[50], endStation[50];
    int numStations;
    char *stations[50];
    printf("请输入公交车编号：");
    scanf("%s", busNumber);
    printf("请输入起始站名字：");
    scanf("%s",startStation);
    printf("请输入终点站名字：");
    scanf("%s",endStation);
    printf("请输入站数：");
    scanf("%d", &numStations);
    printf("请输入其余各站名字（用空格隔开）：");
    for (int i = 0; i < numStations; i++) {
        stations[i] = (char*)malloc(50 * sizeof(char));
        scanf("%s", stations[i]);
    }
    addBusInfo(filename, busNumber, startStation, endStation, numStations, stations);
    for (int i = 0; i < numStations; i++) {
        free(stations[i]);
    }
    return 0;
}