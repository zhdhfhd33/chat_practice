
//소스코드
#include "common.h"
//내가 정의한 코드는 ""로 include해야함.

//in_addr객체를 구하는 것이 목표임.
IN_ADDR GetDefaultMyIP() {
    char localhostname[MAX_PATH];
    IN_ADDR addr = { 0 };
    //localhostname에 이름을 대입하는 함수
    if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR) {//올바르게 호스트네임을 가져오지 못하면 함수종료
        return addr;
    }

    //위에서 구한 localhostname을 사용해서 host를 구한다.
    //host를 선형적으로 탐색하는 과정인듯.

    HOSTENT* ptr = gethostbyname(localhostname);
    while (ptr && ptr->h_name) { //ptr과 ptr->h_name이 모두 유효할 때
        if (ptr->h_addrtype == PF_INET) { //주소타입이 IPv4인지 확인하는 코드
            memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
            break;
        }
        ++ptr;
    }
    return addr;
}

