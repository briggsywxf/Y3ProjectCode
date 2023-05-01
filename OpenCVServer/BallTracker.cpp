//
// Created by sbrig on 11/02/2023.
//

#include "BallTracker.hpp"

BallTracker::BallTracker(cv::Mat *debugImage, std::vector<int> markerIdsToDetect, int cameraID) {

    this->debugImage = debugImage;

    this->markerIdsToDetect = markerIdsToDetect;

    if (!inputVideo.open(cameraID)) {
//    if (!inputVideo.open("../testvid1.mp4")) {
        throw std::exception("Error opening video");
    }


    inputVideo.set(cv::CAP_PROP_FPS, 60.0);
    inputVideo.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('m', 'j', 'p', 'g'));
    inputVideo.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    inputVideo.set(cv::CAP_PROP_FRAME_WIDTH, 1920 / 2);
    inputVideo.set(cv::CAP_PROP_FRAME_HEIGHT, 1080 / 2);
    inputVideo.set(cv::CAP_PROP_AUTOFOCUS, 0);
    inputVideo.set(cv::CAP_PROP_FOCUS, 0);

    bool correctionSet = false;


    std::cout << "Calculating perspective correction." << std::endl;
    while (!correctionSet) {
        if (!inputVideo.grab()) continue;
        inputVideo.retrieve(rawImage);

//        *debugImage = rawImage.clone();

        correctionSet = calculatePerspectiveCorrection(rawImage);
    }
    // Ignore padding when calculationg pixels per metre
    pixelsPerMetre = {(float) (maxWidthHeight.first - 80) / markerXYDistances.first,
                      (float) (maxWidthHeight.second - 80) / markerXYDistances.second};

    std::cout << "Perspective correction set." << std::endl;

}

bool BallTracker::calculatePerspectiveCorrection(cv::Mat image) {
    cv::aruco::Dictionary markerDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters detectorParams{};
    cv::aruco::ArucoDetector detector(markerDict, detectorParams);
    detector.detectMarkers(image, markerCorners, markerIds, rejectedCandidates);

    std::cout << markerIds.size() << std::endl;

    if (markerIds.size() < 4) {
        return false;
    }

    // Make vector of pairs between ID's and coords to allow sorting
    for (int i = 0; i < markerIds.size(); ++i) {
        markerIDPairs.emplace_back(markerIds[i], markerCorners[i]);
    }

    // Check required markers are detected
    std::sort(markerIds.begin(), markerIds.end());
    std::sort(markerIdsToDetect.begin(), markerIdsToDetect.end());
//    for (const auto &item: markerIds) std::cout << item;
//    std::cout << std::endl;
    bool markersPresent = std::includes(markerIds.begin(), markerIds.end(), markerIdsToDetect.begin(),
                                        markerIdsToDetect.end());
    if (!markersPresent) return false;

    std::sort(markerIDPairs.begin(), markerIDPairs.end(), [](const auto &x, const auto &y) {
        return x.first < y.first;
    });

    // Filter out undesired markers
    // BUG: Indexing changes when element removed. Erased bool is a hacky fix
    bool erased = true;
    while (erased) {
        erased = false;
        // Markers present, so remove any undesired ones
        for (int i = 0; i < markerIDPairs.size(); ++i) {
            // If undesired marker present then remove it
            if (std::find(markerIdsToDetect.begin(), markerIdsToDetect.end(), markerIDPairs[i].first) ==
                std::end(markerIdsToDetect)) {
                markerIDPairs.erase(markerIDPairs.begin() + i);
                erased = true;
                break;
            }
        }
    }

    for (int i = 0; i < markerIDPairs.size(); ++i) {
        printf("Marker ID: %d\n", markerIDPairs[i].first);
    }

    int widthA = (int) sqrtf(powf(markerIDPairs[2].second.data()->x - markerIDPairs[3].second.data()->x, 2) +
                             powf((markerIDPairs[2].second.data()->y - markerIDPairs[3].second.data()->y), 2));
    int widthB = (int) sqrtf(powf(markerIDPairs[1].second.data()->x - markerIDPairs[0].second.data()->x, 2) +
                             powf(markerIDPairs[1].second.data()->y - markerIDPairs[0].second.data()->y, 2));
    int maxWidth = std::max(widthA, widthB);
    maxWidthHeight.first = maxWidth;

    int heightA = (int) sqrtf(powf(markerIDPairs[1].second.data()->x - markerIDPairs[2].second.data()->x, 2) +
                              powf(markerIDPairs[1].second.data()->y - markerIDPairs[2].second.data()->y, 2));
    int heightB = (int) sqrtf(powf(markerIDPairs[0].second.data()->x - markerIDPairs[3].second.data()->x, 2) +
                              powf(markerIDPairs[0].second.data()->y - markerIDPairs[3].second.data()->y, 2));
    int maxHeight = std::max(heightA, heightB);
    maxWidthHeight.second = maxHeight;

//    std::vector<cv::Point2f> dstVec{{40,                                    40},
//                                    {static_cast<float>(maxWidth - 1) - 40, 40},
//                                    {static_cast<float>(maxWidth - 1) - 40, static_cast<float>(maxHeight - 1) - 40},
//                                    {40,                                    static_cast<float>(maxHeight - 1) - 40}};

    std::vector<cv::Point2f> dstVec{
                                    {40,                                    static_cast<float>(maxHeight - 1) - 40},
                                    {static_cast<float>(maxWidth - 1) - 40, static_cast<float>(maxHeight - 1) - 40},
                                    {static_cast<float>(maxWidth - 1) - 40, 40},
                                    {40,                                    40},
    };

    cv::Mat dst = cv::Mat(dstVec);

    std::vector<cv::Point2f> markerVec{{markerIDPairs[0].second.data()->x, markerIDPairs[0].second.data()->y},
                                       {markerIDPairs[1].second.data()->x, markerIDPairs[1].second.data()->y},
                                       {markerIDPairs[2].second.data()->x, markerIDPairs[2].second.data()->y},
                                       {markerIDPairs[3].second.data()->x, markerIDPairs[3].second.data()->y}};
    cv::Mat markerPoints = cv::Mat(markerVec);


    correctionTransform = cv::getPerspectiveTransform(markerPoints, dst);



    // Warp known marker points to fit in new image
    for (const auto &item: markerIDPairs) {
        warpedMarkerPoints.emplace_back(item.second.data()->x, item.second.data()->y);
    }
    cv::perspectiveTransform(warpedMarkerPoints, warpedMarkerPoints, correctionTransform);

//    cv::Matx33d translation(1, 0, 0,
//                            0, 1, 0,
//                            0, 0, 1);
//    std::cout << correctionTransform << std::endl;
//    correctionTransform = correctionTransform.cross(translation);
//    std::cout << correctionTransform << std::endl;


//    if (debug) {
//        cv::aruco::drawDetectedMarkers(image, markerCorners, markerIds);
//    }

    return true;
}

std::pair<float, float> BallTracker::GetTrack() {
    // Wait for next frame
    while (!inputVideo.grab()) continue;

    inputVideo.retrieve(rawImage);

    if (rawImage.empty()) return {0, 0};

    cv::Mat image = rawImage.clone();

//    cv::flip(image, image, 0);


    cv::warpPerspective(image, image, correctionTransform, cv::Size(maxWidthHeight.first, maxWidthHeight.second));

    if (debugImage) {
        *debugImage = image.clone();
    }

//    cv::GaussianBlur(image, image, cv::Size(11, 1), 0);

    // Convert to HSV
    cv::cvtColor(image, image, cv::COLOR_BGR2HSV);

//            cv::inRange(image, cv::Scalar(110 / 1.5,148 / 1.5,184 / 1.5), cv::Scalar(110 * 1.2,148 * 1.2,184 * 1.2), image);
    // Day
//    cv::inRange(image, cv::Scalar(90, 180, 30), cv::Scalar(110, 255, 255), image);
    // Artificial light
//    cv::inRange(image, cv::Scalar(40, 110, 40), cv::Scalar(255, 255, 255), image);
    // Lab/blue board
    cv::inRange(image, cv::Scalar(30, 40, 0), cv::Scalar(90, 255, 255), image);
    cv::erode(image, image, NULL, cv::Point(-1, -1), 2);
    cv::dilate(image, image, NULL, cv::Point(-1, -1), 2);

//    if (debugImage) {
//        *debugImage = image.clone();
//    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (!contours.empty()) {
        std::sort(contours.begin(), contours.end(), [](const auto lhs, const auto rhs) {
            return cv::contourArea(lhs) > cv::contourArea(rhs);
        });
        auto maxCont = contours[0];
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(maxCont, center, radius);

        // TODO: Measure from marker points

        if (radius > 6) {
            // Put text to show distances from markers
            float topLeftXDistance = (center.x - 0) / pixelsPerMetre.first;
            float topLeftYDistance = (center.y - 0) / pixelsPerMetre.second;
            float topLeftDistance = sqrtf(powf(topLeftXDistance, 2) + powf(topLeftYDistance, 2));

            float XDistance = (center.x - warpedMarkerPoints[3].x) / pixelsPerMetre.first;
            float YDistance = (center.y - warpedMarkerPoints[3].y) / pixelsPerMetre.second;

            // Draw outline
            if (debugImage) {
                cv::circle(*debugImage, center, radius, cv::Scalar(255, 0, 0), 2);
                cv::circle(*debugImage, center, 3, cv::Scalar(0, 255, 0), 2);
                // Draw lines from markers to circle
                for (const auto &markerPoint: warpedMarkerPoints) {
                    cv::line(*debugImage, markerPoint, center, cv::Scalar(0, 0, 255), 2);
                }
                cv::putText(*debugImage, cv::String("X Distance from top left: " + std::to_string(XDistance)),
                            cv::Point(20, 20), 0, 0.5, cv::Scalar(0, 0, 0));
                cv::putText(*debugImage, cv::String("Y Distance from top left: " + std::to_string(YDistance)),
                            cv::Point(20, 40), 0, 0.5, cv::Scalar(0, 0, 0));

            }
            return {XDistance, YDistance};
        }
//        throw;
    }

    return {0, 0};

}