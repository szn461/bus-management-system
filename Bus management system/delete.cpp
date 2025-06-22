#include <stdio.h>
#include <string.h>

void deleteBusInfo(const char *filename,const char *filename2,const char *busID) {
    FILE *srcFile = fopen(filename, "r"); // 打开原始文件
    if (srcFile == NULL) {
        perror("打开文件失败");
        return;
    }
    FILE *tmpFile = fopen("temp.txt", "w"); // 创建临时文件
    if (tmpFile == NULL) {
        perror("创建临时文件失败");
        fclose(srcFile);
        return;
    }
    char line[512]; // 用来存储每一行的内容
    while (fgets(line, sizeof(line), srcFile)) { // 读取文件的每一行
        if (strstr(line, busID) == NULL) { // 如果这一行不包含输入的公交车编号
            fputs(line, tmpFile); // 将这一行写入临时文件
        }
    }
    fclose(srcFile); // 关闭原始文件
    fclose(tmpFile); // 关闭临时文件
    remove(filename); // 删除原始文件
    rename("temp.txt", filename); // 将临时文件重命名为原始文件名
    FILE *srcFile1 = fopen(filename2, "r"); // 打开原始文件
    if (srcFile1 == NULL) {
        perror("打开文件失败");
        return;
    }
    FILE *tmpFile1 = fopen("temp.txt", "w"); // 创建临时文件
    if (tmpFile1 == NULL) {
        perror("创建临时文件失败");
        fclose(srcFile);
        return;
    }
    char line1[512]; // 用来存储每一行的内容
    while (fgets(line1, sizeof(line1), srcFile1)) { // 读取文件的每一行
        if (strstr(line1, busID) == NULL) { // 如果这一行不包含输入的公交车编号
            fputs(line1, tmpFile1); // 将这一行写入临时文件
        }
    }
    fclose(srcFile1); // 关闭原始文件
    fclose(tmpFile1); // 关闭临时文件
    remove(filename2); // 删除原始文件
    rename("temp.txt", filename2); // 将临时文件重命名为原始文件名
}
int main() {
    char filename[] = "E:\\Bus management system\\路线";
    char filename2[] = "E:\\Bus management system\\时刻表";
    char busID[50];
    printf("请输入要删除的公交车编号: ");
    scanf("%s", busID); // 获取用户输入的公交车编号
    deleteBusInfo(filename,filename2,busID); // 调用函数删除对应的公交车信息
    printf("已删除编号为 %s 的公交车信息。\n", busID);
    return 0;
}
