/**
 * @file udp_client.h
 * @brief UDP client implementation for network data transmission
 * 
 * This header provides a C++ UDP client class for transmitting sensor data
 * over Ethernet networks using Zephyr's networking stack. The client supports
 * connectionless UDP communication to a remote server.
 */

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

#include <cstddef>  // for size_t

/**
 * @brief UDP client class for network data transmission
 * 
 * This class provides a simple interface for sending data packets via UDP
 * to a remote server. It manages socket creation, server address configuration,
 * and data transmission using Zephyr's BSD socket API.
 * 
 * The client is designed for sensor data transmission where reliability is
 * less critical than low latency and minimal overhead. UDP is connectionless,
 * so no connection establishment is required.
 * 
 * Usage example:
 * @code
 * UdpClient client("192.168.1.100", 8888);
 * 
 * SensorData data = {25.5, 60.2, 12345};
 * if (client.send(&data, sizeof(data))) {
 *     printf("Data sent successfully\n");
 * }
 * @endcode
 */
class UdpClient {
public:
    /**
     * @brief Constructor - Initialize UDP client with target server
     * 
     * Creates a UDP client configured to send data to the specified server.
     * The socket is created and the server address is configured during
     * construction. No actual network connection is established (UDP is connectionless).
     * 
     * @param server_ip Target server IP address as string (e.g., "192.168.1.100")
     * @param server_port Target server port number (e.g., 8888)
     * 
     * @note Socket creation may fail if network stack is not initialized
     * @note IP address format must be valid IPv4 dotted decimal notation
     */
    UdpClient(const char* server_ip, uint16_t server_port);

    /**
     * @brief Destructor - Clean up network resources
     * 
     * Automatically closes the UDP socket and releases network resources.
     * This ensures proper cleanup when the UdpClient object is destroyed.
     */
    ~UdpClient();

    /**
     * @brief Send data packet to configured server
     * 
     * Transmits a data buffer to the target server using UDP protocol.
     * The data is sent immediately without connection establishment or
     * acknowledgment (UDP is connectionless and unreliable).
     * 
     * @param data Pointer to data buffer to transmit
     * @param len Size of data buffer in bytes
     * 
     * @return true if data was successfully sent to network stack
     * @return false if transmission failed (network error, invalid socket, etc.)
     * 
     * @note Function may block briefly during network transmission
     * @note No delivery guarantee - UDP is unreliable protocol
     * @note Data should be properly formatted for receiver interpretation
     */
    bool send(const void* data, size_t len);

private:
    int sock_;                      ///< UDP socket file descriptor
    struct sockaddr_in server_addr_; ///< Target server address structure (IPv4)
};
