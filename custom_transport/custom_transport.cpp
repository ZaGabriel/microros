#include <custom_transport/custom_transport.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/poll.h>

static struct pollfd poll_fd;

bool custom_transport_open(struct uxrCustomTransport *transport) {
    struct custom_transport_args *args = (struct custom_transport_args *) transport->args;

    if(debug_enabled) printf("Opening\n");

    bool rv = false;

    // Create the socket
    poll_fd.fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (poll_fd.fd == -1) {
        if(debug_enabled) printf("Socket creation failed");
        return false;
    }

    struct addrinfo hints;
    struct addrinfo *result, *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // Use IPv4
    hints.ai_socktype = SOCK_DGRAM;  // Datagram socket

    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", args->port);

    // Resolve address
    if (getaddrinfo(args->address.c_str(), port_str, &hints, &result) == 0) {
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
            // Attempt to connect the socket
            if (connect(poll_fd.fd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
                poll_fd.events = POLLIN;
                rv = true;
                break;
            }
        }
        freeaddrinfo(result);
    } else {
        if(debug_enabled) printf("Address resolution failed");
    }

    return rv;
}

bool custom_transport_close(struct uxrCustomTransport *transport) {
    (void) transport;

    if(debug_enabled) printf("Closing\n");

    if (poll_fd.fd != -1) {
        if (close(poll_fd.fd) == 0) {
            poll_fd.fd = -1;
            return true;
        } else {
            if(debug_enabled) printf("Socket close failed");
        }
    }
    return false;
}

size_t custom_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *errcode) {
    (void) transport;

    size_t rv = 0;

    ssize_t bytes_sent = send(poll_fd.fd, buf, len, 0);
    if (bytes_sent != -1) {
        rv = (size_t) bytes_sent;
        *errcode = 0;
    } else {
        *errcode = 1;
        if(debug_enabled) printf("Send failed");
    }

    if(debug_enabled) printf("\tWrote %ld B\n", rv);
    return rv;
}

size_t custom_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode) {
    (void) transport;

    size_t rv = 0;

    // Wait for data with a timeout
    int poll_rv = poll(&poll_fd, 1, timeout);
    if (poll_rv > 0) {
        // Data is ready to be read
        ssize_t bytes_received = recv(poll_fd.fd, buf, len, 0);
        if (bytes_received != -1) {
            rv = (size_t) bytes_received;
            *errcode = 0;
        } else {
            *errcode = 1;
            if(debug_enabled) printf("Receive failed");
        }
    } else {
        // Timeout or error
        *errcode = (poll_rv == 0) ? 0 : 1;
        if (poll_rv < 0) {
            if(debug_enabled) printf("Poll failed");
        }
    }

    if(debug_enabled) printf("\tRead %ld B\n", rv);
    return rv;
}
