
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <uxr/client/transport.h>
#include <rmw_microros/rmw_microros.h>
#include <uxr/client/profile/transport/custom/custom_transport.h>

#define debug_enabled 0

struct custom_transport_args{
    std::string address;
    uint16_t port;
};

bool custom_transport_open(struct uxrCustomTransport *transport);
bool custom_transport_close(struct uxrCustomTransport *transport);
size_t custom_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *errcode);
size_t custom_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode);


static inline void set_custom_udp_transports(std::string agent_ip, uint16_t agent_port){
	static struct custom_transport_args arg;

	arg.address = agent_ip;
	arg.port = agent_port;

	rmw_uros_set_custom_transport(
		false,
		(void *) &arg,
		custom_transport_open,
		custom_transport_close,
		custom_transport_write,
		custom_transport_read
	);
}

