// serverTest.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#include <iostream>
#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256
int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata); //���� �ʱ�ȭ

    char server_ip[40] = "";
    printf("���� IP:");
    gets_s(server_ip, sizeof(server_ip));
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���ɻ���
    SOCKADDR_IN servaddr = { 0 };
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);//������ s_addr�� �����Է��ؾ��Ѵ�. ��õ�� ������.
    servaddr.sin_port = htons(PORT_NUM);

    //����õ�
    if (connect(sock, (SOCKADDR*)&servaddr, sizeof(servaddr)) == -1) {
        return -1;
    }
    char msg[MAX_MSG_LEN] = "";
    while (true) {
        gets_s(msg, MAX_MSG_LEN);
        send(sock, msg, sizeof(msg), 0);
        if (strcmp(msg, "exit") == 0) {
            break;
        }
        recv(sock, msg, sizeof(msg), 0);
        printf("����: %s\n", msg);
        //closesocket(sock); ���⿡�� sock�� �ݾƼ� ������ ���ϴ� ����� ������ �ʾҴ�.
        //�ű��� ���� Ŭ���̾�Ʈ���� closesocket()�ߴµ��� send, recv�� �� �Ǿ��ٴ� ����. 
        //���� �������ε� ��� ������ �ְ� �޾���.
    }
    closesocket(sock);

    return 0;
}




