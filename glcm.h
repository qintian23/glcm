#pragma once
#ifndef GLCM_H
#define GLCM_H

#include <math.h>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

// �Ҷȵȼ�
enum GrayLevel
{
    GRAY_4,
    GRAY_8,
    GRAY_16
};

// �Ҷ�ͳ�Ʒ���
// (Choose in 0��, 45��, 90��, 135��)
enum GrayDirection
{
    DIR_0,
    DIR_45,
    DIR_90,
    DIR_135
};

// ��ɫͼ�е�ָ��ͨ��
enum RGBChannel
{
    CHANNEL_R,
    CHANNEL_G,
    CHANNEL_B
};

// ��������ֵ�ṹ��
struct TextureEValues
{
    // ����
    float energy;
    // �Աȶ�
    float contrast;
    // ��ض�
    float homogenity;
    // ��
    float entropy;
};

class GLCM
{
public:
    // �Ӳ�ɫͨ������ȡһ��ͨ��
    void getOneChannel(Mat src, Mat& dstChannel, RGBChannel channel = CHANNEL_R);

    // ���Ҷ�ͼ�е���������ֵ�����������Ա�����Ϊ4/8/16���ȼ�
    void GrayMagnitude(Mat src, Mat& dst, GrayLevel level = GRAY_8);

    // ����һ�����󴰿��У�����ĳ������ͳ�ƵĻҶȹ�������
    void CalcuOneGLCM(Mat src, Mat& dst, int src_i, int src_j, int size, GrayLevel level = GRAY_8, GrayDirection direct = DIR_0);

    // ����Ĺ�һ��������������Ԫ�������������Ԫ��֮���������㣬�õ����ʾ���
    void NormalizeMat(Mat src, Mat& dst);

    // ���㵥�����ھ����ͼ����������ֵ�������������Աȶȡ���ضȡ���
    void CalcuOneTextureEValue(Mat src, TextureEValues& EValue, bool ToCheckMat = false);

    // ����ȫͼ��ͼ����������ֵ�������������Աȶȡ���ضȡ���
    void CalcuTextureEValue(Mat src, TextureEValues& EValue,
        int size = 5, GrayLevel level = GRAY_8);

    // ��������ͼ�����������
    void CalcuTextureImages(Mat src, Mat& imgEnergy, Mat& imgContrast, Mat& imgHomogenity, Mat& imgEntropy,
        int size = 5, GrayLevel level = GRAY_8, bool ToAdjustImg = false);
};

#endif // GLCM_H
