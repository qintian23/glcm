#pragma once
#ifndef GLCM_H
#define GLCM_H

#include <math.h>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

// 灰度等级
enum GrayLevel
{
    GRAY_4,
    GRAY_8,
    GRAY_16
};

// 灰度统计方向
// (Choose in 0°, 45°, 90°, 135°)
enum GrayDirection
{
    DIR_0,
    DIR_45,
    DIR_90,
    DIR_135
};

// 彩色图中的指定通道
enum RGBChannel
{
    CHANNEL_R,
    CHANNEL_G,
    CHANNEL_B
};

// 纹理特征值结构体
struct TextureEValues
{
    // 能量
    float energy;
    // 对比度
    float contrast;
    // 相关度
    float homogenity;
    // 熵
    float entropy;
};

class GLCM
{
public:
    // 从彩色通道中提取一个通道
    void getOneChannel(Mat src, Mat& dstChannel, RGBChannel channel = CHANNEL_R);

    // 将灰度图中的所有像素值量级化，可以被量化为4/8/16个等级
    void GrayMagnitude(Mat src, Mat& dst, GrayLevel level = GRAY_8);

    // 计算一个矩阵窗口中，按照某个方向统计的灰度共生矩阵
    void CalcuOneGLCM(Mat src, Mat& dst, int src_i, int src_j, int size, GrayLevel level = GRAY_8, GrayDirection direct = DIR_0);

    // 矩阵的归一化，将矩阵所有元素与矩阵中所有元素之和作除运算，得到概率矩阵
    void NormalizeMat(Mat src, Mat& dst);

    // 计算单个窗口矩阵的图像纹理特征值，包括能量、对比度、相关度、熵
    void CalcuOneTextureEValue(Mat src, TextureEValues& EValue, bool ToCheckMat = false);

    // 计算全图的图像纹理特征值，包括能量、对比度、相关度、熵
    void CalcuTextureEValue(Mat src, TextureEValues& EValue,
        int size = 5, GrayLevel level = GRAY_8);

    // 计算整幅图像的纹理特征
    void CalcuTextureImages(Mat src, Mat& imgEnergy, Mat& imgContrast, Mat& imgHomogenity, Mat& imgEntropy,
        int size = 5, GrayLevel level = GRAY_8, bool ToAdjustImg = false);
};

#endif // GLCM_H
