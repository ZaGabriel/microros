#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <rmw_microros/rmw_microros.h>
#include <custom_transport/custom_transport.h>

#include <stdio.h>
#include <string>
#include <unistd.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return 1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

std::string agent_ip = "127.0.0.1";
uint16_t agent_port = 8888;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time){
	printf("Timer call back...\n");
	(void) last_call_time;
	if (timer != NULL){
		RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
		printf("Sent: %d\n", msg.data);
		msg.data++;
	}
}

int main(){	
	// set custom transport
	set_custom_udp_transports(agent_ip, agent_port);

	rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;

	printf("Init...\n");
	// create init_options
	RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

	printf("Node...\n");
	// create node
	rcl_node_t node;
	RCCHECK(rclc_node_init_default(&node, "int32_publisher_rclc", "", &support));

	printf("Publisher...\n");
	// create publisher
	RCCHECK(rclc_publisher_init_default(
		&publisher,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
		"std_msgs_msg_Int32"));

	printf("Timer...\n");
	// create timer,
	rcl_timer_t timer;
	const unsigned int timer_timeout = 1000;
	RCCHECK(rclc_timer_init_default(
		&timer,
		&support,
		RCL_MS_TO_NS(timer_timeout),
		timer_callback));

	printf("Executor...\n");
	// create executor
	rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
	RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
	RCCHECK(rclc_executor_add_timer(&executor, &timer));

	msg.data = 0;
	
	printf("Spin...\n");
  	rclc_executor_spin(&executor);
	printf("fni...\n");
	RCCHECK(rcl_publisher_fini(&publisher, &node));
	RCCHECK(rcl_node_fini(&node));

}
