# ROS-Merge-Arrays
My solution to the ROS coding challenge, which is a ROS package containing a node that subscribes to two topics to input two sorted arrays, and publishes a merged version of the two arrays, still sorted.

Node Inputs:
  - topic "/input/array1", which is of type std_msgs/Int32MultiArray and contains a sorted array of type Int32
  - topic "/input/array2", which is of type std_msgs/Int32MultiArray and contains a sorted array of type Int32

Node Outputs:
  - topic "/output/array", also type std_msgs/Int32MultiArray, which contains the merged and sorted version of the input arrays
