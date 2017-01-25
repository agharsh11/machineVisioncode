#ifndef COMMONS_H
#define COMMONS_H

void Erosion(int,void*);
void Dilation(int,void*);
void Enhance(cv::Mat);
cv::Point2f getCenter(std::vector<cv::Point> contour, cv::Mat circles);
cv::Mat getcam(int);
int CContours(cv::Mat threshy);
std::vector<cv::Point> findcontours(cv::Mat threshy);
#endif
