#include <stdio.h>
#include <string.h>

void deleteBusInfo(const char *filename,const char *filename2,const char *busID) {
    FILE *srcFile = fopen(filename, "r"); // ��ԭʼ�ļ�
    if (srcFile == NULL) {
        perror("���ļ�ʧ��");
        return;
    }
    FILE *tmpFile = fopen("temp.txt", "w"); // ������ʱ�ļ�
    if (tmpFile == NULL) {
        perror("������ʱ�ļ�ʧ��");
        fclose(srcFile);
        return;
    }
    char line[512]; // �����洢ÿһ�е�����
    while (fgets(line, sizeof(line), srcFile)) { // ��ȡ�ļ���ÿһ��
        if (strstr(line, busID) == NULL) { // �����һ�в���������Ĺ��������
            fputs(line, tmpFile); // ����һ��д����ʱ�ļ�
        }
    }
    fclose(srcFile); // �ر�ԭʼ�ļ�
    fclose(tmpFile); // �ر���ʱ�ļ�
    remove(filename); // ɾ��ԭʼ�ļ�
    rename("temp.txt", filename); // ����ʱ�ļ�������Ϊԭʼ�ļ���
    FILE *srcFile1 = fopen(filename2, "r"); // ��ԭʼ�ļ�
    if (srcFile1 == NULL) {
        perror("���ļ�ʧ��");
        return;
    }
    FILE *tmpFile1 = fopen("temp.txt", "w"); // ������ʱ�ļ�
    if (tmpFile1 == NULL) {
        perror("������ʱ�ļ�ʧ��");
        fclose(srcFile);
        return;
    }
    char line1[512]; // �����洢ÿһ�е�����
    while (fgets(line1, sizeof(line1), srcFile1)) { // ��ȡ�ļ���ÿһ��
        if (strstr(line1, busID) == NULL) { // �����һ�в���������Ĺ��������
            fputs(line1, tmpFile1); // ����һ��д����ʱ�ļ�
        }
    }
    fclose(srcFile1); // �ر�ԭʼ�ļ�
    fclose(tmpFile1); // �ر���ʱ�ļ�
    remove(filename2); // ɾ��ԭʼ�ļ�
    rename("temp.txt", filename2); // ����ʱ�ļ�������Ϊԭʼ�ļ���
}
int main() {
    char filename[] = "E:\\Bus management system\\·��";
    char filename2[] = "E:\\Bus management system\\ʱ�̱�";
    char busID[50];
    printf("������Ҫɾ���Ĺ��������: ");
    scanf("%s", busID); // ��ȡ�û�����Ĺ��������
    deleteBusInfo(filename,filename2,busID); // ���ú���ɾ����Ӧ�Ĺ�������Ϣ
    printf("��ɾ�����Ϊ %s �Ĺ�������Ϣ��\n", busID);
    return 0;
}
