#include<iostream>
#include<fstream>
#include<thread>
#include<string>
#include<vector>
#include<mutex>
#include<stdio.h>
#include<unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>


std::mutex frameMutex;




static void show_usage(std::string name)
{
      std::cerr << " Usage: " << std::string(name.begin()+2,name.end()) << " <option(s)> SOURCES"
                << " Options:\n"
                << "\t-h,--help\t\tShow this help message\n"
                << "\t-f,\t\tSpecify the video file path"
                << std::endl;
}


static void get_arguments(std::string& PathToFile, const int argc, const char * argv[])
    {
     int c;
     while((c = getopt(argc,argv,"f:"))!=-1)
      {
        switch(c)
        {
          case 'f':
            if(optarg) PathToFile = optarg;
            break;
          default:
            show_usage(argv[0]);
            break;
        }
      }

      if (c==-1)
      {
        show_usage(argv[0]);
        return;
      }
    }
void RunVideo(const std::string window_name, int video_num, std::string path_to_video)
{
  cv::VideoCapture cap(path_to_video);

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

void ReadFile(std::fstream& fileread,const std::string PathToFile,std::vector<std::string>& PathToVideos)
{
  fileread.open(PathToFile);
  std::string line;
  if(!fileread.is_open())
  {
    std::cerr << "[ERROR] " << "Error opening file." << std::endl;
    return;
  }

  while(std::getline(fileread,line))
  {
    PathToVideos.push_back(line);
  }

}
int main(int argc, char* argv[])
{
  std::string PathToFile;
  std::fstream FileRead;
  std::vector<std::string> PathToVideos;

  get_arguments(PathToFile,argc,argv);

  ReadFile(FileRead,PathToFile,PathToVideos);


  std::vector<std::thread> threadpool;
  for(int i = 0; i < PathToVideos.size(); ++i)
  {
    std::string window_name = "window_" + std::to_string(i);
    threadpool.push_back(std::thread(RunVideo,window_name,i,PathToVideos[i]));
  }

  for(auto &t : threadpool)
  {
    t.join();
  }
  return 0;
}
