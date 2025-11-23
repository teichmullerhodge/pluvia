#include "nett_utils.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

bool has_internet() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 0;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);  
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    int result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);

    return result == 0; 
}
