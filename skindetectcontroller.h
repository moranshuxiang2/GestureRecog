/*
	Modified from ColorDetector from chapter 3 of the cookbook:
	Computer Vision Programming using the OpenCV Library.
	by Robert Laganiere, Packt Publishing, 2011.
*/

#if !defined SKN_CNTRLLR
#define SKN_CNTRLLR


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "skindetector.h"

class SkinDetectController
{
	private:

		static SkinDetectController *singleton;

		SkinDetector *sknDetect;

		cv::Vec3b targetHSV;
		int range;

		// image storage
		cv::Mat image;
		cv::Mat resultImg;

	public:
		SkinDetectController()
		{
			sknDetect = new SkinDetector();
			setThreshold(10);
		}

		// Sets the input image. Reads it from file.
		bool setInputImage(std::string filename)
		{

		  image= cv::imread(filename);

		  if (!image.data)
		      return false;
		  else
		      return true;
		}

		// Sets the input image.
		bool setInputImage(cv::Mat imgIn)
		{

		  image = imgIn;

		  if (!image.data)
		      return false;
		  else
		      return true;
		}

		// Returns the current input image.
		const cv::Mat getInputImage() const 
		{
			return image;
		}

		void setThreshold(int range)
		{
			this->range = range;
			cv::Vec3b min, max;
			for(int i =0; i <3; i++)
			{
				min[i] = targetHSV[i] - (range/2);
				max[i] = targetHSV[i] + (range/2);
			}
			sknDetect->setThreshold(min, max);
		}

		int getThreshold()
		{
			return range;
		}

		void setHSV(int hue, int saturation, int value)
		{
			targetHSV[0] = hue;
			targetHSV[1] = saturation;
			targetHSV[2] = value;
		}

		void process() 
		{
			resultImg = sknDetect->processHSV(image);
		}

		// Deletes all processor objects created by the controller.
		~SkinDetectController() 
		{

		  delete sknDetect;
		}

		// Singleton static members
		static SkinDetectController *getInstance() 
		{

		  if (singleton == 0)
		    singleton= new SkinDetectController;

		  return singleton;
		}

		// Releases the singleton instance of this controller.
		static void destroy() 
		{
		  if (singleton != 0)
		  {
		      delete singleton;
		      singleton= 0;
		  }
		}
};

#endif