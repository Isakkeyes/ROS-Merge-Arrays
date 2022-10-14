///////////////////////////////////////////////////////////////////////////////////////////////////
//
// File Name: merge_arrays_node.cpp
//
// Description: A node that takes in two arrays and creates a merged sorted array.
//
//              The input arrays are from ROS topics "/input/array1" and "/input/array2" and
//              are of type "std_msgs/Int32MultiArray" and contain sorted arrays of Int32.
//              
//              The merged sorted array is published to ROS topic "/output/array" and is also
//              of type "std_msgs/Int32MultiArray".
//              
//
// Author: Isak Keyes
// Email: ikeyes@wisc.edu
//
/////////////////////////////////////////////////////////////////////

#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"

std::vector<int> array1;
std::vector<int> array2;
std::vector<int> mergedArray;
int arr1Size;
int arr2Size;
bool received1 = false;
bool received2 = false;

using std::placeholders::_1;
using namespace std::chrono_literals;

class SubscriberPublisher : public rclcpp::Node
{
public:
    SubscriberPublisher() : Node("subscriber_publisher")
    {
        // create the subscriber to receive array1
        sub1 = this->create_subscription<std_msgs::msg::Int32MultiArray>("/input/array1",
            10, std::bind(&SubscriberPublisher::topic_callback_1, this, _1));

        // create the subscriber to receive array2
        sub1 = this->create_subscription<std_msgs::msg::Int32MultiArray>("/input/array1",
            10, std::bind(&SubscriberPublisher::topic_callback_2, this, _1));

        // create the publisher to send the output array
        pub = this->create_publisher<std_msgs::msg::Int32MultiArray>("/output/array", 10);

        // create a timer to check if arrays have been received and publish every 1/10 of a second
        timer = this->create_wall_timer(
            100ms, std::bind(&SubscriberPublisher::timer_callback, this));
    }

private:
    void topic_callback_1(const std_msgs::msg::Int32MultiArray & msg) const
    {
        array1 = msg.data;
        arr1Size = sizeof(msg.data) / sizeof(msg.data[0]);
	received1 = true;
    }

    void topic_callback_2(const std_msgs::msg::Int32MultiArray & msg) const
    {
        array2 = msg.data;
        arr2Size = sizeof(msg.data) / sizeof(msg.data[0]);
	received2 = true;
    }

    void timer_callback()
    {
        // check if the input arrays have been received yet
        if (received1 || received2)
            return;

        // sort and merge the arrays
        sortMergeArrays();

        // publish the merged array
        auto message = std_msgs::msg::Int32MultiArray();
        message.data = mergedArray;
        pub->publish(message);

    }

    void sortMergeArrays() 
    {
        mergedArray.resize(arr1Size + arr2Size); // TODO - change to int32_t?

        int arr1Index = 0;
        int arr2Index = 0;
        int mergeIndex = 0;

        // add values of the two arrays to mergedArray until the end of either array is reached
        while (arr1Index < arr1Size && arr2Index < arr2Size)
        {
            // add the lesser value of array1 and array2 to mergedArray, and increment indicies
            if (array1[arr1Index] < array2[arr2Index]) 
                mergedArray[mergeIndex++] = array1[arr1Index++];
            else
                mergedArray[mergeIndex++] = array2[arr2Index++];
        }

        // if array1 still has values left to add to mergedArray, add them
        while (arr1Index < arr1Size)
            mergedArray[mergeIndex++] = array1[arr1Index++];

        // if array2 still has values left to add to mergedArray, add them
        while (arr2Index < arr2Size)
            mergedArray[mergeIndex++] = array2[arr2Index++];

	// set the array statuses back to not being received and reset mergedArrays
	received1 = false;
	received2 = false;
	mergedArray.resize(0);
    }


    // declare the sub1, sub2, pub, and timer variables
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub1;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub2;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pub;
    rclcpp::TimerBase::SharedPtr timer;

};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubscriberPublisher>());
    rclcpp::shutdown();
    return 0;
}
