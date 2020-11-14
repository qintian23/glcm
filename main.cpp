/*=================================================
 * Version:
 * v1.0: ����˵���GLCM�ļ��㣻
 * v1.1: ����˾����һ����ͼ��������������Ĺ��ܣ�
 * v1.2: ����˾���߽粿�ֵļ��㣻
 * v1.3: ����˻�ȡ��һͨ�����Ҷ�ͼ����Ϊ4/8/16�ȼ��Ĺ��ܣ�
 * v1.4: ���������ͼ�����������ļ��㣻
 * v1.5: ����˶���������ͼ��ĵ������ܣ�����ʾ��
 * v1.7: ����˼�������ͼ�������ֵ������
===================================================
 */

#include "glcm.h"

int main()
{
    char key;
    Mat img;
    GLCM glcm;
    TextureEValues EValues;

    // ��������ʱ��ͳ�Ʊ���
    double time;
    double start;

    // ��������ֵ����
    Mat imgEnergy, imgContrast, imgHomogenity, imgEntropy;

    // ��ȡͼ��
    img = imread("Satellite.jpg");
    imshow("srcimage", img);

    Mat dstChannel;
    glcm.getOneChannel(img, dstChannel, CHANNEL_B);

    // �Ҷ���������ͳ������ʱ��
    start = static_cast<double>(getTickCount());
    glcm.GrayMagnitude(dstChannel, dstChannel, GRAY_8);
    time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
    cout << "Time of Magnitude Gray Image: " << time << "ms" << endl << endl;

    // ��������ͼ�����������ֵͼ�񣬲�ͳ������ʱ��
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
