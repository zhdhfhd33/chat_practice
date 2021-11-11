// serverTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(short pnum, int blog);
void AcceptLoop(SOCKET sock);
void Doit(SOCKET dosock);
int main()
{
    //초기화
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);//0x0202를 만들어 주는 메크로임. 16진수 2자리는 1byte임.

    //구현
    SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);//대기소켓가동
    AcceptLoop(sock);
    closesocket(sock);//소켓해제
    WSACleanup();//윈속 해제


    //end
    WSACleanup();
    return 0;
}

//중요하게 봐야할 것은 TCP서버의 기본 절차와 AcceptLoop이다.
SOCKET SetTCPServer(short pnum, int blog) {
    SOCKET sock;
    //AF_INET -> IPv4를 사용하겠다. AF_INET6 -> IPv6를 사용하겠다. 강의자료에는 PF_INET을 사용하기도 하는데 둘은 정확하게 같다.
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓생성. IPv4, stream, tcp방식
    //_IN이 붙으면 IPv4를 사용하겠다는 말.
    SOCKADDR_IN servaddr = { 0 };//서버주소
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = GetDefaultMyIP();//나의 IP주소가 들어간다. IN_ADDR타입을 반환
    servaddr.sin_port = htons(PORT_NUM); //port는 2바이트 값. int가 4바이트잖아. 2의 16승만큼 표현가능.
    //client에서는 servaddr.sin_addr.s_addr을 사용했는데 여기서는 그렇게 사용안하네?

    //winsock은 TCP/IP뿐만 아니라 다양한 네트워크 통신이 가능한 lib임. 그래서 캐스팅해준다. 어떤 네트워크 주소 family를 사용하는지에 따라
    //길이도 다를 수 있기 때문에 sizeof()를 통해 길이도 넣음.
    //네트워크 인터페이스와 소켓 결합.
    if (bind(sock, (SOCKADDR*)&servaddr, sizeof(servaddr)) == -1) {//결합실패하면 -1 = SOCKET_ERROR 반환
        return 0;
    }

    // 지금 사용하는 sock은 대기소켓이니까 백로그의 크기까지 넣어주는게 타당하게 느껴진다.
    //listen 함수는 연결을 수신하는상태로 소켓의 상태를 변경한다. 즉, 소켓을 접속 대기 상태로 만들어준다. 
    //백로그 큐 크기 설정
    if (listen(sock, blog) == -1) {
        return 0;
    }
    printf("%s:%d Setup\n", inet_ntoa(servaddr.sin_addr), pnum);
    return sock;

}

void AcceptLoop(SOCKET sock) {//매개변수로 들어오는 소켓은 대기소켓임.
    SOCKET dosock;//실제로 통신을 하는 소켓
    SOCKADDR_IN cliaddr = { 0 };//얘는 배열인건가?? NO 구조체임. 구조체 초기화 할 때 이렇게 했었잖아.
    int len = sizeof(cliaddr);
    while (true) {
        //accept()이 대기하다가 수락까지 한다.
        //누군가 접속하면 cliaddr에 그 정보가 채워지게 된다.
        //생각해보니 포인터로 캐스팅하는 것은 다른 의미가 있나? -> 그냥 캐스팅이랑 똑같을 듯. 그 주소의 메모리를 이 타입으로 읽어라는 말임.
        //accept()자체가 입력을 받을 때 처럼 프로그램의 흐름을 잠깐 중지시키고 계속 대기하게 만든다. 연결되면 다시 프로그램의 흐름을 시작한다.
        //계속 while이 돌아가는게 아님.
        dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);//하나의 접속이 들어오면 Doit()호출해 주고 다시 다음 연결 대기상태로 만들기 위해서 while안에 있는 듯.
        if (dosock == -1) {
            perror("accept실패");
            break;
        }
        //sin_addr : 실제 IP주소, sin_port : 포트주소
        printf("%s : %d의 연결 요청 수락", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        Doit(dosock);
    }
}
void Doit(SOCKET dosock) {
    char msg[MAX_MSG_LEN]="";
        //상대방이 연결을 끊으면 0이 된다. 음수가 반환되면 문제가 있는 것
    //마지막 매개변수는 옵션인데 안알려줌
    //수신되면 msg에 수신된 내용을 담는다.
    while (true) {//정상적 수신 
        int tmp = recv(dosock, msg, sizeof(msg), 0);
        printf("tmp : %d", tmp);
        if (!(tmp > 0)) {
            break;
        }
        printf("recv:%s\n", msg);
        //echo라서 그대로 출력해주기만 하면 된다
        send(dosock, msg, sizeof(msg), 0);
    }
    //작업이 끝나면 소켓닫는다.
    closesocket(dosock);
}




