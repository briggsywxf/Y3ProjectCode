//
// Created by sbrig on 11/02/2023.
//

#pragma once

#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>

class BallTracker {
public:
    BallTracker(cv::Mat *debugImage, std::vector<int> markerIdsToDetect, int cameraID);

    std::pair<float, float> GetTrack();


private:
    bool calculatePerspectiveCorrection(cv::Mat image);

    std::pair<float, float> markerXYDistances{0.665f,0.848f};

    cv::VideoCapture inputVideo;

    cv::Mat rawImage;

    cv::Mat *debugImage;

    std::vector<int> markerIdsToDetect;

    std::pair<int, int> maxWidthHeight;

    cv::Mat correctionTransform = cv::Mat(3, 3, CV_32F);

    std::vector<std::pair<int, std::vector<cv::Point_<float>>>> markerIDPairs;

    std::vector<cv::Point2f> warpedMarkerPoints;

    std::pair<float, float> pixelsPerMetre;

    bool debug = false;
    bool windowShown = false;
};

