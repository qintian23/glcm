/*=================================================
 * Version:
 * v1.0: 完成了单个GLCM的计算；
 * v1.1: 完成了矩阵归一化、图像纹理特征计算的功能；
 * v1.2: 完成了矩阵边界部分的计算；
 * v1.3: 完成了获取单一通道、灰度图量化为4/8/16等级的功能；
 * v1.4: 完成了整幅图像纹理特征的计算；
 * v1.5: 完成了对纹理特征图像的调整功能，并显示；
 * v1.7: 添加了计算整幅图像的特征值函数；
===================================================
 */

#include "glcm.h"

int main()
{
    char key;
    Mat img;
    GLCM glcm;
    TextureEValues EValues;

    // 程序运行时间统计变量
    double time;
    double start;

    // 纹理特征值矩阵
    Mat imgEnergy, imgContrast, imgHomogenity, imgEntropy;

    // 读取图像
    img = imread("Satellite.jpg");
    imshow("srcimage", img);

    Mat dstChannel;
    glcm.getOneChannel(img, dstChannel, CHANNEL_B);

    // 灰度量化，并统计运算时间
    start = static_cast<double>(getTickCount());
    glcm.GrayMagnitude(dstChannel, dstChannel, GRAY_8);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Magnitude Gray Image: " << time << "ms" << endl << endl;

    // 计算整幅图像的纹理特征值图像，并统计运算时间
    start = static_cast<double>(getTickCount());
    glcm.CalcuTextureImages(dstChannel, imgEnergy, imgContrast, imgHomogenity, imgEntropy, 5, GRAY_8, true);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Generate the whole Image's Calculate Texture Features Image: " << time << "ms" << endl << endl;

    start = static_cast<double>(getTickCount());
    glcm.CalcuTextureEValue(dstChannel, EValues, 5, GRAY_8);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Calculate Texture Features of the whole Image: " << time << "ms" << endl << endl;

    cout << "Image's Texture EValues:" << endl;
    cout << "    Contrast: " << EValues.contrast << endl;
    cout << "    Energy: " << EValues.energy << endl;
    cout << "    EntropyData: " << EValues.entropy << endl;
    cout << "    Homogenity: " << EValues.homogenity << endl;

    imshow("Energy", imgEnergy);
    imshow("Contrast", imgContrast);
    imshow("Homogenity", imgHomogenity);
    imshow("Entropy", imgEntropy);

    key = (char)waitKey(0);

    return 0;
}
