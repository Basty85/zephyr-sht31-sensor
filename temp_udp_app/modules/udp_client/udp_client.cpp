#include "udp_client.h"

#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>  // Zephyr Socket API
#include <zephyr/posix/unistd.h>  // für close()
#include <zephyr/posix/arpa/inet.h>  // für inet_pton()

LOG_MODULE_REGISTER(udp_client);

UdpClient::UdpClient(const char* server_ip, uint16_t server_port) : sock(-1) {
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // ← Zephyr Socket API
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(server_port);
    
    // inet_pton Rückgabewert prüfen
    int ret = zsock_inet_pton(AF_INET, server_ip, &server_addr_.sin_addr);  // ← Zephyr API
    if (ret <= 0) {
        LOG_ERR("Invalid IP address: %s", server_ip);
        zsock_close(sock);  // ← Zephyr API
        sock = -1;
    } else {
        LOG_INF("UDP client created for %s:%d", server_ip, server_port);
    }
}

UdpClient::~UdpClient() {
    if (sock >= 0) {
        zsock_close(sock);  // ← Zephyr API
        LOG_DBG("UDP socket closed");
    }
}

bool UdpClient::send(const void* data, size_t len) {
    if (sock < 0) {
        LOG_ERR("Socket not initialized");
        return false;
    }

    ssize_t ret = zsock_sendto(sock, data, len, 0,   // ← Zephyr API
                              (struct sockaddr*)&server_addr_, sizeof(server_addr_));
    
    if (ret < 0) {
        LOG_ERR("Send failed: %d", ret);
        return false;
    }
    
    if (ret != static_cast<ssize_t>(len)) {
        LOG_WRN("Partial send: %d of %zu bytes", ret, len);
        return false;
    }
    
    LOG_DBG("Sent %zu bytes", len);
    return true;
}
