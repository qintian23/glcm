#include "glcm.h"

/*===================================================================
 * ��������getOneChannel
 * ˵�����Ӳ�ɫͨ������ȡһ��ͨ����
 * ������
 *   Mat src:  Դͼ��
 *   Mat& dstChannel:  Դͼ���еĵ�һͨ���������Ϊ�Ҷ�ͼ
 *   RGBChannel channel:  RGBͨ���е�ָ��ͨ��
 * ����ֵ��void
=====================================================================
*/
void GLCM::getOneChannel(Mat src, Mat& dstChannel, RGBChannel channel)
{
    // ������ͼ���Ѿ��ǻҶ�ͼ����ֱ�����
    if (src.channels() == 1)
        dstChannel = src;

    vector<Mat> bgr;
    // ����ͼ��
    split(src, bgr);

    switch (channel)
    {
    case CHANNEL_B: dstChannel = bgr[0]; break;
    case CHANNEL_G: dstChannel = bgr[1]; break;
    case CHANNEL_R: dstChannel = bgr[2]; break;
    default:
        cout << "ERROR in getOneChannel(): No Such Channel." << endl;
        return;
    }
}

/*===================================================================
 * ��������GrayMagnitude
 * ˵�������Ҷ�ͼ�е���������ֵ�����������Ա�����Ϊ4/8/16���ȼ�
 * ������
 *   Mat src:  Դͼ��
 *   Mat& dst:  Ŀ��ͼ��
 *   GrayLevel level:  �Ҷȵȼ�
 * ����ֵ��void
=====================================================================
*/
void GLCM::GrayMagnitude(Mat src, Mat& dst, GrayLevel level)
{
    Mat tmp;
    src.copyTo(tmp);
    if (tmp.channels() == 3)
        cvtColor(tmp, tmp, COLOR_BGR2GRAY);

    // ֱ��ͼ���⻯
    equalizeHist(tmp, tmp);

    for (int j = 0; j < tmp.rows; j++)
    {
        const uchar* current = tmp.ptr<uchar>(j);
        uchar* output = dst.ptr<uchar>(j);

        for (int i = 0; i < tmp.cols; i++)
        {
            switch (level)
            {
            case GRAY_4:
                output[i] = cv::saturate_cast<uchar>(current[i] / 64);
                break;
            case GRAY_8:
                output[i] = cv::saturate_cast<uchar>(current[i] / 32);
                break;
            case GRAY_16:
                output[i] = cv::saturate_cast<uchar>(current[i] / 16);
                break;
            default:
                cout << "ERROR in GrayMagnitude(): No Such GrayLevel." << endl;
                return;
            }
        }
    }
}

/*===================================================================
 * ��������CalcuOneGLCM
 * ˵��������һ�����󴰿��У�����ĳ������ͳ�ƵĻҶȹ�������
 * ������
 *   Mat src:  Դͼ��
 *   Mat& dst:  Ŀ��Ҷȹ������󣬸���ѡ��ĻҶȵȼ����ߴ�Ϊ4*4, 8*8, 16*16
 *   int src_i:  ���󴰿����ĵ����ֵ
 *   int src_j:  ���󴰿����ĵ����ֵ
 *   int size:  ���ڳߴ磨��֧��5*5, 7*7��
 *   GrayLevel level:  �Ҷȵȼ�
 *   GrayDirection direct:  ͳ�Ʒ���
 * ����ֵ��void
=====================================================================
*/
void GLCM::CalcuOneGLCM(Mat src, Mat& dst, int src_i, int src_j, int size, GrayLevel level, GrayDirection direct)
{
    // �Ҷȹ�������
    Mat glcm;

    // ���ھ���
    Mat srcCut;

    // ԭͼ��ߴ��ж�
    if (src.cols <= 0 || src.rows <= 0)
    {
        cout << "ERROR in CalcuOneGLCM(): source Mat's size is smaller than 0." << endl;
        return;
    }

    // ǿ�ƽ��ߴ�תΪ����
    size = size / 2 * 2 + 1;

    // ��Ե���ִ������ھ���
    if (src_i + (size / 2) + 1 > src.rows
        || src_j + (size / 2) + 1 > src.cols
        || src_i < (size / 2)
        || src_j < (size / 2))
    {
        size = 3;
        if (src_i <= size / 2)
        {
            if (src_j <= size / 2)
                srcCut = Mat(src, Range(0, 3), Range(0, 3));
            else if (src_j + (size / 2) + 1 > src.cols)
                srcCut = Mat(src, Range(0, 3), Range(src.cols - 3, src.cols));
            else
                srcCut = Mat(src, Range(0, 3), Range(src_j - size / 2, src_j + size / 2 + 1));
        }
        else if (src_i >= src.rows - size / 2)
        {
            if (src_j <= size / 2)
                srcCut = Mat(src, Range(src.rows - 3, src.rows), Range(0, 3));
            else if (src_j + (size / 2) + 1 > src.cols)
                srcCut = Mat(src, Range(src.rows - 3, src.rows), Range(src.cols - 3, src.cols));
            else
                srcCut = Mat(src, Range(src.rows - 3, src.rows), Range(src_j - size / 2, src_j + size / 2 + 1));
        }
        else if (src_j <= size / 2)
        {
            if (src_i <= size / 2)
                srcCut = Mat(src, Range(0, 3), Range(0, 3));
            else if (src_i + (size / 2) + 1 > src.rows)
                srcCut = Mat(src, Range(src.rows - 3, src.rows), Range(0, 3));
            else
                srcCut = Mat(src, Range(src_i - size / 2, src_i + size / 2 + 1), Range(0, 3));
        }
        else if (src_j >= src.cols - size / 2)
        {
            if (src_i <= size / 2)
                srcCut = Mat(src, Range(0, 3), Range(src.cols - 3, src.cols));
            else if (src_i + (size / 2) + 1 > src.rows)
                srcCut = Mat(src, Range(src.rows - 3, src.rows), Range(src.cols - 3, src.cols));
            else
                srcCut = Mat(src, Range(src_i - size / 2, src_i + size / 2 + 1), Range(src.cols - 3, src.cols));
        }
        else
            srcCut = Mat(src, Range(src_i - size / 2, src_i + size / 2 + 1), Range(src_j - size / 2, src_j + size / 2 + 1));
    }
    else
        srcCut = Mat(src, Range(src_i - size / 2, src_i + size / 2 + 1), Range(src_j - size / 2, src_j + size / 2 + 1));

    // ���ݻҶȵȼ���ʼ���Ҷȹ�������
    switch (level)
    {
    case GRAY_4:
    {
        glcm = Mat_<uchar>(4, 4);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                glcm.at<uchar>(j, i) = 0;
        break;
    }
    case GRAY_8:
    {
        glcm = Mat_<uchar>(8, 8);
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                glcm.at<uchar>(j, i) = 0;
        break;
    }
    case GRAY_16:
    {
        glcm = Mat_<uchar>(16, 16);
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
                glcm.at<uchar>(j, i) = 0;
        break;
    }
    default:
        cout << "ERROR in CalcuOneGLCM(): No Such Gray Level." << endl;
        break;
    }

    // ����ͳ�Ʒ������Ҷȹ�������
    switch (direct)
    {
    case DIR_0:
        for (int i = 0; i < srcCut.rows; i++)
            for (int j = 0; j < srcCut.cols - 1; j++)
                glcm.at<uchar>(srcCut.at<uchar>(j, i), srcCut.at<uchar>(j + 1, i))++;
        break;
    case DIR_45:
        for (int i = 0; i < srcCut.rows - 1; i++)
            for (int j = 0; j < srcCut.cols - 1; j++)
                glcm.at<uchar>(srcCut.at<uchar>(j, i), srcCut.at<uchar>(j + 1, i + 1))++;
        break;
    case DIR_90:
        for (int i = 0; i < srcCut.rows - 1; i++)
            for (int j = 0; j < srcCut.cols; j++)
                glcm.at<uchar>(srcCut.at<uchar>(j, i), srcCut.at<uchar>(j, i + 1))++;
        break;
    case DIR_135:
        for (int i = 1; i < srcCut.rows; i++)
            for (int j = 0; j < srcCut.cols - 1; j++)
                glcm.at<uchar>(srcCut.at<uchar>(j, i), srcCut.at<uchar>(j + 1, i - 1))++;
        break;
    default:
        cout << "ERROR in CalcuOneGLCM(): No such Direct." << endl;
        break;
    }

    Mat glcm_dst;
    // �Ҷȹ��������һ��
    // Normalize GLCM
    NormalizeMat(glcm, glcm_dst);
    glcm_dst.copyTo(dst);
}

/*===================================================================
 * ��������NormalizeMat
 * ˵��������Ĺ�һ��������������Ԫ�������������Ԫ��֮���������㣬�õ����ʾ���
 * ������
 *   Mat src:  Դͼ��
 *   Mat& dst:  Ŀ����ʾ���
 * ����ֵ��void
=====================================================================
*/
void GLCM::NormalizeMat(Mat src, Mat& dst)
{
    Mat tmp;
    src.convertTo(tmp, CV_32F);

    float sum = 0;
    for (int i = 0; i < tmp.rows; i++)
        for (int j = 0; j < tmp.cols; j++)
            sum += tmp.at<float>(j, i);
    if (sum == 0)    sum = 1;

    for (int i = 0; i < tmp.rows; i++)
        for (int j = 0; j < tmp.cols; j++)
            tmp.at<float>(j, i) /= sum;

    tmp.copyTo(dst);
}

/*===================================================================
 * ��������CalcuOneTextureEValue
 * ˵�������㵥�����ھ����ͼ����������ֵ�������������Աȶȡ���ضȡ���
 * ������
 *   Mat src:  Դ���󣬴��ھ���
 *   TextureEValues& EValue:  ��������ֵ����
 *   bool ToCheckMat:  �����������Ƿ�Ϊ���ʾ���
 * ����ֵ��void
=====================================================================
*/
void GLCM::CalcuOneTextureEValue(Mat src, TextureEValues& EValue, bool ToCheckMat)
{
    if (ToCheckMat)
    {
        float sum = 0;
        for (int i = 0; i < src.rows; i++)
            for (int j = 0; j < src.cols; j++)
                sum += src.at<float>(j, i);
        if (sum < 0.99 || sum > 1.01)
        {
            cout << "ERROR in CalcuOneTextureEValue(): Sum of the Mat is not equal to 1.00." << endl;
            return;
        }
    }

    EValue.contrast = 0;
    EValue.energy = 0;
    EValue.entropy = 0;
    EValue.homogenity = 0;

    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
        {
            EValue.energy += powf(src.at<float>(j, i), 2);
            EValue.contrast += (powf((i - j), 2) * src.at<float>(j, i));
            EValue.homogenity += (src.at<float>(j, i) / (1 + fabs((float)(i - j))));
            if (src.at<float>(j, i) != 0)
                EValue.entropy -= (src.at<float>(j, i) * log10(src.at<float>(j, i)));
        }
}

/*===================================================================
 * ��������CalcuTextureEValue
 * ˵��������ȫͼ��ͼ����������ֵ�������������Աȶȡ���ضȡ���
 * ������
 *   Mat src:  Դ���󣬴��ھ���
 *   TextureEValues& EValue:  ���Ŀ�꣬ȫͼ����������ֵ����
 *   int size:  ���ڳߴ磨��֧��5*5, 7*7��
 *   GrayLevel level:  �Ҷȵȼ�
 * ����ֵ��void
=====================================================================
*/
void GLCM::CalcuTextureEValue(Mat src, TextureEValues& EValue, int size, GrayLevel level)
{
    // ԭͼ��ĻҶ�ͼ
    Mat imgGray;

    // ���ھ���
    Mat glcm_win;

    // ��һ����ĸ��ʾ���
    Mat glcm_norm;

    // ��������ֵ�������
    TextureEValues EValue_temp;

    // ��ʼ��Ŀ����������ֵ
    EValue.contrast = 0; EValue.energy = 0; EValue.entropy = 0; EValue.homogenity = 0;

    // �������ͼ���Ƿ�Ϊ��ͨ��ͼ��������ǣ���ת�����ʽ
    if (src.channels() != 1)
        cvtColor(src, imgGray, COLOR_BGR2GRAY);
    else
        src.copyTo(imgGray);

    for (int i = 0; i < imgGray.rows; i++)
    {
        for (int j = 0; j < imgGray.cols; j++)
        {
            // ��������ͳ�Ʒ���ĻҶȹ����������Ӧ������ֵ�����ۼ������������
            float energy, contrast, homogenity, entropy;
            energy = contrast = homogenity = entropy = 0;

            CalcuOneGLCM(imgGray, glcm_win, i, j, size, level, DIR_0);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue_temp, false);
            energy += EValue_temp.energy; contrast += EValue_temp.contrast;
            homogenity += EValue_temp.homogenity; entropy += EValue_temp.entropy;

            CalcuOneGLCM(imgGray, glcm_win, i, j, size, level, DIR_45);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue_temp, false);
            energy += EValue_temp.energy; contrast += EValue_temp.contrast;
            homogenity += EValue_temp.homogenity; entropy += EValue_temp.entropy;

            CalcuOneGLCM(imgGray, glcm_win, i, j, size, level, DIR_90);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue_temp, false);
            energy += EValue_temp.energy; contrast += EValue_temp.contrast;
            homogenity += EValue_temp.homogenity; entropy += EValue_temp.entropy;

            CalcuOneGLCM(imgGray, glcm_win, i, j, size, level, DIR_135);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue_temp, false);
            energy += EValue_temp.energy; contrast += EValue_temp.contrast;
            homogenity += EValue_temp.homogenity; entropy += EValue_temp.entropy;

            // �����з������õ�������ֵƽ�������õ���ֵ��������ͳ�Ʒ���Ӱ��
            energy /= 4; contrast /= 4;
            homogenity /= 4; entropy /= 4;

            // �ۼӵ�ǰ�������ڵ���������ֵ����Ϊ����ͼ�����������ֵ
            EValue.contrast += contrast;
            EValue.energy += energy;
            EValue.entropy += entropy;
            EValue.homogenity += homogenity;
        }
    }
}

/*===================================================================
 * ��������CalcuTextureImages
 * ˵������������ͼ���������������������������Ӧ������
 * ������
 *   Mat src:  ԭͼ��
 *   Mat& imgEnergy:  Ŀ����������
 *   Mat& imgContrast:  Ŀ��ԱȶȾ���
 *   Mat& imgHomogenity:  Ŀ����ضȾ���
 *   Mat& imgEntropy:  Ŀ���ؾ���
 *   int size:  ���ڳߴ磨��֧��5*5, 7*7��
 *   GrayLevel level:  �Ҷȵȼ�
 *   bool ToAdjustImg:  �Ƿ�����������������ͼ��
 * ����ֵ��void
=====================================================================
*/
void GLCM::CalcuTextureImages(Mat src, Mat& imgEnergy, Mat& imgContrast, Mat& imgHomogenity, Mat& imgEntropy,
    int size, GrayLevel level, bool ToAdjustImg)
{
    // ���ھ���
    Mat glcm_win;

    // ��һ����ĸ��ʾ���
    Mat glcm_norm;

    // ��������ֵ�������
    TextureEValues EValue;

    imgEnergy.create(src.size(), CV_32FC1);
    imgContrast.create(src.size(), CV_32FC1);
    imgHomogenity.create(src.size(), CV_32FC1);
    imgEntropy.create(src.size(), CV_32FC1);

    for (int i = 0; i < src.rows; i++)
    {
        float* energyData = imgEnergy.ptr<float>(i);
        float* contrastData = imgContrast.ptr<float>(i);
        float* homogenityData = imgHomogenity.ptr<float>(i);
        float* entropyData = imgEntropy.ptr<float>(i);

        for (int j = 0; j < src.cols; j++)
        {
            // ��������ͳ�Ʒ���ĻҶȹ����������Ӧ������ֵ�����ۼ������������
            float energy, contrast, homogenity, entropy;
            energy = contrast = homogenity = entropy = 0;

            CalcuOneGLCM(src, glcm_win, i, j, size, level, DIR_0);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue, false);
            energy += EValue.energy; contrast += EValue.contrast;
            homogenity += EValue.homogenity; entropy += EValue.entropy;

            CalcuOneGLCM(src, glcm_win, i, j, size, level, DIR_45);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue, false);
            energy += EValue.energy; contrast += EValue.contrast;
            homogenity += EValue.homogenity; entropy += EValue.entropy;

            CalcuOneGLCM(src, glcm_win, i, j, size, level, DIR_90);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue, false);
            energy += EValue.energy; contrast += EValue.contrast;
            homogenity += EValue.homogenity; entropy += EValue.entropy;

            CalcuOneGLCM(src, glcm_win, i, j, size, level, DIR_135);
            NormalizeMat(glcm_win, glcm_norm);
            CalcuOneTextureEValue(glcm_norm, EValue, false);
            energy += EValue.energy; contrast += EValue.contrast;
            homogenity += EValue.homogenity; entropy += EValue.entropy;

            // �����з������õ�������ֵƽ�������õ���ֵ��������ͳ�Ʒ���Ӱ��
            energy /= 4; contrast /= 4;
            homogenity /= 4; entropy /= 4;

            energyData[j] = energy;
            contrastData[j] = contrast;
            homogenityData[j] = homogenity;
            entropyData[j] = entropy;
        }
    }

    // �����������ͼ��������CV_32FC1��ΪCV_8UC1��ȡֵ��Χ0--255
    if (ToAdjustImg)
    {
        cv::normalize(imgEnergy, imgEnergy, 0, 255, NORM_MINMAX);
        cv::normalize(imgContrast, imgContrast, 0, 255, NORM_MINMAX);
        cv::normalize(imgEntropy, imgEntropy, 0, 255, NORM_MINMAX);
        cv::normalize(imgHomogenity, imgHomogenity, 0, 255, NORM_MINMAX);
        imgEnergy.convertTo(imgEnergy, CV_8UC1);
        imgContrast.convertTo(imgContrast, CV_8UC1);
        imgEntropy.convertTo(imgEntropy, CV_8UC1);
        imgHomogenity.convertTo(imgHomogenity, CV_8UC1);
    }
}
