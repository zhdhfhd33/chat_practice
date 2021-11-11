
//�ҽ��ڵ�
#include "common.h"
//���� ������ �ڵ�� ""�� include�ؾ���.

//in_addr��ü�� ���ϴ� ���� ��ǥ��.
IN_ADDR GetDefaultMyIP() {
    char localhostname[MAX_PATH];
    IN_ADDR addr = { 0 };
    //localhostname�� �̸��� �����ϴ� �Լ�
    if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR) {//�ùٸ��� ȣ��Ʈ������ �������� ���ϸ� �Լ�����
        return addr;
    }

    //������ ���� localhostname�� ����ؼ� host�� ���Ѵ�.
    //host�� ���������� Ž���ϴ� �����ε�.

    HOSTENT* ptr = gethostbyname(localhostname);
    while (ptr && ptr->h_name) { //ptr�� ptr->h_name�� ��� ��ȿ�� ��
        if (ptr->h_addrtype == PF_INET) { //�ּ�Ÿ���� IPv4���� Ȯ���ϴ� �ڵ�
            memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
            break;
        }
        ++ptr;
    }
    return addr;
}

