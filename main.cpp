#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>



#define loop(i,n) for(int i = 0; i < n; ++i)
std::mutex frameMutex;

void RunVideo(const std::string window_name, int video_num)
{
  std::string path = "/home/ieisw/security_barrier/resources/video"+std::to_string(video_num)+".mp4";
  cv::VideoCapture cap(path);

  while(true)
  {
    cv::Mat frame;
    auto start = std::chrono::system_clock::now();
    frameMutex.lock();
    if(cap.read(frame))
    {

      cv::resize(frame,frame,cv::Size(),0.2,0.2);
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-start;
      auto fps = 1/elapsed_seconds.count();
      cv::putText(frame,"FPS: " + std::to_string(fps) ,cv::Point(50,50),cv::FONT_HERSHEY_COMPLEX_SMALL,1.0,cv::Scalar(255,255,255));
      cv::imshow(window_name,frame);
      frameMutex.unlock();
    }
    else {
      frameMutex.unlock();
      cv::destroyWindow(window_name);
      break;
    }
    if (cv::waitKey(10) == 27)
    {
      std::cout<<"Esc. key pressed by user "<<std::endl;
      cv::destroyWindow(window_name);
      break;
    }
  }

}

int main()
{

  std::vector<std::thread> threadpool;
  for(int i = 1; i < 3; ++i)
  {
    std::string window_name = "window_" + std::to_string(i);
    threadpool.push_back(std::thread(RunVideo,window_name,i));
  }

  for(auto &t : threadpool)
  {
    t.join();
  }
  return 0;
}
