//
//  main.cpp
//  Feb21GHT
//
//  Created by Kai Zhang on 2017-02-21.
//  Copyright © 2017 Kai Zhang. All rights reserved.
//

#include <iostream>
#include "stdlib.h"
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

void createTemplate(){
    Mat input_img = imread("template_K.png", 1);
    Mat src_gray;
    Mat detected_edges;
    src_gray.create( Size(input_img.cols, input_img.rows), CV_8UC1);
    cvtColor(input_img, src_gray, CV_BGR2GRAY);
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, 1, 100, 3 );
    imwrite("K_edge.png", detected_edges);
}

void createImageEdge(){
    Mat input_img = imread("letters.png", 1);
    Mat src_gray;
    Mat detected_edges;
    src_gray.create( Size(input_img.cols, input_img.rows), CV_8UC1);
    cvtColor(input_img, src_gray, CV_BGR2GRAY);
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, 1, 100, 3 );
    imwrite("letters_edge.png", detected_edges);
}

int main(int argc, const char * argv[]) {
    // insert code here...
//    cout<< "CV_VERSION: " << CV_VERSION << endl;
//    char * dir = getcwd(NULL, 0); // Platform-dependent, see reference link below
//    printf("Current dir: %s", dir);
//    createTemplate();
    createImageEdge();
    Mat templ = imread("template_K.png", IMREAD_GRAYSCALE);
    Mat image = imread("letters.png", IMREAD_GRAYSCALE);
    vector<Vec4f> position;
    Ptr<GeneralizedHough> alg;
    
    Ptr<GeneralizedHoughGuil> guil = createGeneralizedHoughGuil();
    guil->setMinDist(50);
    guil->setLevels(60);
    guil->setDp(2);
    guil->setMaxBufferSize(1000);
    
    guil->setMinAngle(0);
    guil->setMaxAngle(30);
    guil->setAngleStep(1);
    guil->setAngleThresh(1000);
    
    guil->setMinScale(0.5);
    guil->setMaxScale(1.5);
    guil->setScaleStep(0.05);
    guil->setScaleThresh(100);
    
    guil->setPosThresh(10);
    
    alg = guil;
    
    alg->setTemplate(templ);
    alg->detect(image, position);
    
    cout << "Found : " << position.size() << " objects" << endl;
    
    Mat out;
    cv::cvtColor(image, out, COLOR_GRAY2BGR);
    
    for (size_t i = 0; i < position.size(); ++i)
    {
        Point2f pos(position[i][0], position[i][1]);
        float scale = position[i][2];
        float angle = position[i][3];
        
        RotatedRect rect;
        rect.center = pos;
        rect.size = Size2f(templ.cols * scale, templ.rows * scale);
        rect.angle = angle;
        
        Point2f pts[4];
        rect.points(pts);
        
        line(out, pts[0], pts[1], Scalar(0, 0, 255), 3);
        line(out, pts[1], pts[2], Scalar(0, 0, 255), 3);
        line(out, pts[2], pts[3], Scalar(0, 0, 255), 3);
        line(out, pts[3], pts[0], Scalar(0, 0, 255), 3);
    }
    
    imshow("out", out);
    waitKey();

    return 0;
}
