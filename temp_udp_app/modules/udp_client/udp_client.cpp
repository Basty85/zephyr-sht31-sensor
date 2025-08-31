/**
 * @file udp_client.cpp
 * @brief Implementation of UDP client for network data transmission
 * 
 * This file implements the UdpClient class using Zephyr's BSD socket API.
 * It provides UDP communication functionality for transmitting sensor data
 * to remote servers over Ethernet networks.
 */

#include "udp_client.h"

#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>           // Zephyr Socket API
#include <zephyr/posix/unistd.h>         // For close() function
#include <zephyr/posix/arpa/inet.h>      // For inet_pton() function

LOG_MODULE_REGISTER(udp_client);

/**
 * @brief Constructor - Initialize UDP client with target server configuration
 * 
 * Creates a UDP socket and configures the target server address for data transmission.
 * The socket is created using Zephyr's BSD socket API and the server address
 * structure is populated with IPv4 address and port information.
 * 
 * @param server_ip Target server IP address in dotted decimal notation
 * @param server_port Target server port number for UDP communication
 * 
 * @note If socket creation or IP address parsing fails, the socket is marked invalid
 * @note Error conditions are logged for debugging purposes
 */
UdpClient::UdpClient(const char* server_ip, uint16_t server_port) : sock_(-1) {
    // Create UDP socket using Zephyr socket API
    sock_ = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ < 0) {
        LOG_ERR("Failed to create UDP socket: error code %d", sock_);
        return;
    }

    // Initialize server address structure
    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;                    // IPv4 address family
    server_addr_.sin_port = htons(server_port);           // Convert port to network byte order
    
    // Convert IP address string to binary format
    int ret = zsock_inet_pton(AF_INET, server_ip, &server_addr_.sin_addr);
    if (ret <= 0) {
        LOG_ERR("Invalid IP address format: %s", server_ip);
        // Clean up socket on address parsing failure
        zsock_close(sock_);
        sock_ = -1;
    } else {
        LOG_INF("UDP client initialized for target %s:%d", server_ip, server_port);
    }
}

/**
 * @brief Destructor - Clean up UDP socket resources
 * 
 * Automatically closes the UDP socket when the UdpClient object is destroyed.
 * This ensures proper resource cleanup and prevents socket leaks.
 * 
 * @note Socket is only closed if it was successfully created
 */
UdpClient::~UdpClient() {
    if (sock_ >= 0) {
        zsock_close(sock_);
        LOG_DBG("UDP socket closed and resources released");
    }
}

/**
 * @brief Send data packet to configured target server
 * 
 * Transmits a data buffer to the target server using UDP protocol via Zephyr's
 * socket API. The function performs validation checks and handles partial sends.
 * 
 * @param data Pointer to data buffer to transmit
 * @param len Size of data buffer in bytes
 * 
 * @return true if complete data packet was successfully transmitted
 * @return false if transmission failed or was incomplete
 * 
 * @note Function validates socket state before attempting transmission
 * @note Partial sends are treated as failures for data integrity
 * @note All error conditions are logged for debugging
 */
bool UdpClient::send(const void* data, size_t len) {
    // Validate socket is properly initialized
    if (sock_ < 0) {
        LOG_ERR("Cannot send data: UDP socket not initialized");
        return false;
    }

    // Transmit data using Zephyr socket API
    ssize_t bytes_sent = zsock_sendto(sock_, data, len, 0,
                                     (struct sockaddr*)&server_addr_, sizeof(server_addr_));
    
    // Check for transmission errors
    if (bytes_sent < 0) {
        LOG_ERR("UDP transmission failed with error code: %d", bytes_sent);
        return false;
    }
    
    // Verify complete data transmission (no partial sends)
    if (bytes_sent != static_cast<ssize_t>(len)) {
        LOG_WRN("Incomplete UDP transmission: %d of %zu bytes sent", bytes_sent, len);
        return false;
    }
    
    LOG_DBG("UDP packet transmitted successfully: %zu bytes", len);
    return true;
}
