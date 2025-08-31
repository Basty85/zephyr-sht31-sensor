#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

#include <cstddef>  // für size_t

class UdpClient {
   public:
    UdpClient(const char* server_ip, uint16_t server_port);
    ~UdpClient();

    // Datenblock senden
    bool send(const void* data, size_t len);

   private:
    int sock;
    struct sockaddr_in server_addr_;
};
