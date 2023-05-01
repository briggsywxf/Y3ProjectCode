#include <iostream>
#include "UDPCommunicator.hpp"
#include "BallTracker.hpp"

// Distance between top left marker of camera 1 and camera 2
#define COORD_X_OFFSET 0.4f

void SendTrack();

enum class ServerState : char {
    SS_PreStart = 0,
    SS_SendTrack = 1,
    SS_EndTrack = 2
};

struct TrackerHandle {
    int cameraID = 1;

    std::vector<int> markerIdsToDetect{};

    bool ready = false;

    BallTracker *tracker = nullptr;

    cv::Mat out_debugImage;
    cv::Mat out_lastDebugImage;
//    cv::Mat *out_rawImage;

    std::pair<float, float> latestTrack{};
    bool newTrack = false;

};

struct CommHandle {
    bool ready = false;

    UDPCommunicator *comm = nullptr;

    std::vector<TrackerHandle *> trackers;
};

void initTrackerThread(TrackerHandle *handle) {
    handle->tracker = new BallTracker(&handle->out_debugImage, handle->markerIdsToDetect, handle->cameraID);

    handle->ready = true;


    while (true) {
        handle->latestTrack = handle->tracker->GetTrack();
        handle->out_debugImage.copyTo(handle->out_lastDebugImage);
        handle->newTrack = true;
    }
}

void initCommThread(CommHandle *commHandle) {
    commHandle->comm = new UDPCommunicator();

    commHandle->comm->WaitForStartRequest();

    commHandle->ready = true;

    while (true) {
        bool t1NewTrack = commHandle->trackers[0]->ready && commHandle->trackers[0]->newTrack;
        bool t2NewTrack = commHandle->trackers[1]->ready && commHandle->trackers[1]->newTrack;


        if (t1NewTrack && t2NewTrack) {
            auto t1Track = commHandle->trackers[0]->latestTrack;
            auto t2Track = commHandle->trackers[1]->latestTrack;

            bool t1TrackValid = t1Track.first != 0.f && t1Track.second != 0.f;
            bool t2TrackValid = t2Track.first != 0.f && t2Track.second != 0.f;

            // If both valid, take track on player side (so just check t1 first)
            if (t1TrackValid) {
                // Add x offset to t1 track
                commHandle->trackers[0]->latestTrack.first += COORD_X_OFFSET;

                commHandle->comm->SendTrackingData(t1Track);
                commHandle->trackers[0]->newTrack = false;
            } else if (t2TrackValid) {
                commHandle->comm->SendTrackingData(t2Track);
                commHandle->trackers[1]->newTrack = false;
            }
        }

        // TODO: Refactor all into comm
//        if (t1NewTrack) {
//            commHandle->trackers[0]->latestTrack.first += COORD_X_OFFSET;
//            commHandle->comm->SendTrackingData(commHandle->trackers[0]->latestTrack);
//            commHandle->trackers[0]->newTrack = false;
//        }


//        // If both cameras see object, take average
//        if (t1NewTrack && t2NewTrack) {
//            // Add x offset to t1 track
//            commHandle->trackers[0]->latestTrack.first += COORD_X_OFFSET;
//
//            float x = (commHandle->trackers[0]->latestTrack.first + commHandle->trackers[1]->latestTrack.first) / 2;
//            float y = (commHandle->trackers[0]->latestTrack.second + commHandle->trackers[1]->latestTrack.second) / 2;
//            commHandle->comm->SendTrackingData({x, y});
//            commHandle->trackers[0]->newTrack = false;
//            commHandle->trackers[1]->newTrack = false;
//        }
        // TODO: Base on object coordinates (or refactor to sync cameras)
        // If both cameras see object, take T2 (closer to player)
//        if (t1NewTrack && t2NewTrack) {
//            commHandle->comm->SendTrackingData(commHandle->trackers[1]->latestTrack);
//            commHandle->trackers[1]->newTrack = false;
//        } else if (t1NewTrack) {
//            // Add x offset to t1 track
//            commHandle->trackers[0]->latestTrack.first += COORD_X_OFFSET;
//            commHandle->comm->SendTrackingData(commHandle->trackers[0]->latestTrack);
//            commHandle->trackers[0]->newTrack = false;
//        } else if (t2NewTrack) {
//            commHandle->comm->SendTrackingData(commHandle->trackers[1]->latestTrack);
//            commHandle->trackers[1]->newTrack = false;
//        }
    }
}

//UDPCommunicator *comm;
//BallTracker *tracker;

int main() {
//    ServerState state = ServerState::SS_PreStart;

//// CAMERA SETTINGS DEBUG
//    cv::VideoCapture testVid;
//    if (!testVid.open(0, cv::CAP_DSHOW)) {
//        exit(1);
//    } else {
//        testVid.set(cv::CAP_PROP_FPS, 60.0);
//        testVid.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('m','j','p','g'));
//        testVid.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
//        testVid.set(cv::CAP_PROP_FRAME_WIDTH, 1920 / 2);
//        testVid.set(cv::CAP_PROP_FRAME_HEIGHT, 1080 / 2);
//        testVid.set(cv::CAP_PROP_AUTOFOCUS, 0);
//        testVid.set(cv::CAP_PROP_FOCUS, 0);
//        cv::namedWindow("Test");
////        testVid.set(cv::CAP_PROP_SETTINGS, 1);
//        while(testVid.grab()) {
//            cv::Mat img;
//            testVid.read(img);
//
//            cv::aruco::Dictionary markerDict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
///
//            std::vector<int> markerIds;
//            std::cout << markerIds.size();
//            std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
//            cv::aruco::DetectorParameters detectorParams{};
//            cv::aruco::ArucoDetector detector(markerDict, detectorParams);
//            detector.detectMarkers(img, markerCorners, markerIds, rejectedCandidates);
//
//            cv::aruco::drawDetectedMarkers(img, markerCorners, markerIds);
//
////            cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
////            cv::inRange(img, cv::Scalar(30, 80, 40), cv::Scalar(90, 255, 255), img);
//
//            cv::imshow("Test", img);
//            cv::waitKey(1);
//        }
//    }

    TrackerHandle *t1Handle;
    TrackerHandle *t2Handle;
    CommHandle *commHandle;

    // TODO: camera id param
    t1Handle = new TrackerHandle{0, {4, 5, 6, 7}};
    t2Handle = new TrackerHandle{1, {0, 1, 2, 3}};
//    t1Handle->out_debugImage = new cv::M2at{};
    std::thread t1Thread(&initTrackerThread, t1Handle);
    std::thread t2Thread(&initTrackerThread, t2Handle);

    commHandle = new CommHandle{};
    commHandle->trackers.emplace_back(t1Handle);
    commHandle->trackers.emplace_back(t2Handle);
    std::thread commThread(&initCommThread, commHandle);

    cv::Mat debugImage;
    cv::namedWindow("Image");
    cv::namedWindow("Image 2");

    cv::Mat placeholderImage;
    placeholderImage = cv::imread("../opencvlogo.png");

    cv::imshow("Image", placeholderImage);
    cv::imshow("Image 2", placeholderImage);
    cv::waitKey(1);

    while (true) {
        if (t1Handle->ready && !t1Handle->out_lastDebugImage.empty() && t2Handle->ready &&
            !t2Handle->out_lastDebugImage.empty()) {
//        if (t1Handle->ready && !t1Handle->out_lastDebugImage.empty()) {
            cv::imshow("Image", t1Handle->out_lastDebugImage);
            cv::imshow("Image 2", t2Handle->out_lastDebugImage);
            cv::waitKey(30);
        } else {
            cv::imshow("Image", placeholderImage);
            cv::imshow("Image 2", placeholderImage);
            cv::waitKey(30);
        }
    }
// TODO: Cleanup
}
