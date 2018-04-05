#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>

ros::Publisher pub_img;
cv::Mat mask;
cv::Rect roi_rect;

void img_callback(const sensor_msgs::ImageConstPtr &img_msg)
{
  cv_bridge::CvImagePtr ptr;
  cv::Mat img_raw;
  
  if(sensor_msgs::image_encodings::isColor(img_msg->encoding)){
    ptr = cv_bridge::toCvCopy(img_msg, sensor_msgs::image_encodings::BGR8);
    cv::cvtColor(ptr->image,img_raw,CV_BGR2GRAY);
  }
  else{
    ptr = cv_bridge::toCvCopy(img_msg, sensor_msgs::image_encodings::MONO8);
    img_raw = ptr->image;
  }

  cv::Mat img_roi = img_raw(roi_rect);

  cv_bridge::CvImage out_img(img_msg->header,"mono8",img_roi);
  sensor_msgs::ImagePtr out_msg = out_img.toImageMsg();
  pub_img.publish(out_msg);
  // std::cout << "publish !" << std::endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "img_roi");

  ros::NodeHandle nh("~");

  std::string m_path;

  if (nh.getParam("mask_path", m_path))
    ROS_INFO("Got mask_path: %s", m_path.c_str());
  else
    ROS_ERROR("Failed to get mask_path");

  mask = cv::imread(m_path.c_str(),CV_LOAD_IMAGE_GRAYSCALE);

  if(mask.empty()){
    // std::cout<< "can't load mask image !!!" <<std::endl; // bark!!!
    ROS_ERROR("Can't load mask image !!!");
  }
  cv::threshold(mask,mask,127,255,cv::THRESH_BINARY);
  roi_rect = cv::boundingRect(mask); // compute roi rectangle from mask image

  pub_img = nh.advertise<sensor_msgs::Image>("/img_roi",1);
  ros::Subscriber sub_img = nh.subscribe("/image_raw", 1, img_callback);

  ros::spin();

  return 0;
}

