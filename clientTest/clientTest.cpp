// serverTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256
int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata); //윈속 초기화

    char server_ip[40] = "";
    printf("서버 IP:");
    gets_s(server_ip, sizeof(server_ip));
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소케생성
    SOCKADDR_IN servaddr = { 0 };
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);//마지막 s_addr은 직접입력해야한다. 추천을 안해줌.
    servaddr.sin_port = htons(PORT_NUM);

    //연결시도
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
        printf("수신: %s\n", msg);
        //closesocket(sock); 여기에서 sock를 닫아서 서버에 원하는 출력이 나오질 않았다.
        //신기한 것은 클라이언트에서 closesocket()했는데도 send, recv는 잘 되었다는 것임. 
        //닫힌 소켓으로도 계속 연결을 주고 받았음.
    }
    closesocket(sock);

    return 0;
}




