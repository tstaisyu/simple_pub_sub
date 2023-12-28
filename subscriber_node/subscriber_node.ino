#include <M5Stack.h>
#include <micro_ros_arduino.h>
#include <std_msgs/msg/String.h>

rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;
rcl_subscription_t subscriber;
std_msgs__msg__String msg;

void subscription_callback(const void * msgin)
{
  const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;
  Serial.println(msg->data.data);
}

void setup() {
  Serial.begin(115200);
  set_microros_transports();
  
  rclc_support_init(&support, 0, NULL, &allocator);
  rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support);
  
  rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
    "topic");
  
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA);
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  delay(10);
}
