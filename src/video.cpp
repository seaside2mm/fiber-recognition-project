//
// Created by kubota on 6/2/18.
//
#include "../include/video.h"


/*TODO:计算图像相似度
 *。然后输入视频，针对视频的前后相关的帧计算出一个合适的阈值，作为判断依据。
 * 如算法在找出最清晰的图像时，图像与图像之间没有关联性计算，那么只需给出单幅图像的清晰度评价值。
 */
bool Video::computeAssociation(string video_dir)
{

    vector<Scalar> association;
    float gate = 0.7;

    VideoCapture capture(video_dir);    // 获取视频
    if (!capture.isOpened() ) { return -1; }

    Size frame_size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH),                 // 视频帧的大小
                     (int)capture.get(CAP_PROP_FRAME_HEIGHT));


    cout << "Reference frame resolution: Width=" << frame_size.width << "  Height=" << frame_size.height
         << " of total_frame#: " << capture.get(CAP_PROP_FRAME_COUNT) << endl;

    const char* WIN_UT = "Frame";                                          // 显示窗口
    const char* WIN_RF = "Reference Frame";
    namedWindow(WIN_RF, WINDOW_AUTOSIZE);











    namedWindow(WIN_UT, WINDOW_AUTOSIZE);
    moveWindow(WIN_RF, 0, 0);
    moveWindow(WIN_UT, frame_size.width, 0);

    Mat frame, frame_reference;
    double psnrV;                                                               // PSNR方法
    Scalar mssimV;                                                              // SSIM方法

    while(true)
    {
        capture >> frame;
        if (frame.empty())
        {
            cout << "The End" << endl;
            break;
        }
        capture >>frame_reference;

         //setiosflags(ios::fixed)用定点方式显示实数，setprecision(n)可控制输出流显示浮点数的数字个数
        mssimV = getMSSIM(frame, frame_reference);

        cout << "图像帧之间关联度:" << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[2] * 100 << "%"
             << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[1] * 100 << "%"
             << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[0] * 100 << "%" << endl;

        association.push_back(mssimV);


        imshow(WIN_RF, frame);
        imshow(WIN_UT, frame_reference);

//        char c = (char)waitKey(500);
//        if (c == 27) break;
        waitKey(0);

    }

    float total_association;

    for(int i=0; i < association.size(); i++)
    {
        total_association += (association[i][0] + association[i][0]+ association[i][0]) /3;
    }

    float video_association = total_association / association.size();
    cout << "视频关联度：  " << video_association << endl;
    if(video_association > gate)
        return true;
    else
        return false;
}


/*
 * 参考：OpenCV 视频输入与相似度测量
 * https://blog.csdn.net/u013165921/article/details/78566932
 */

double Video::getPSNR(const Mat& I1, const Mat& I2)                        // PSNR方法
{
    Mat s1;
    absdiff(I1, I2, s1);                                            // |I1 - I2|
    s1.convertTo(s1, CV_32F);                                       // 转换为32位进行运算
    s1 = s1.mul(s1);                                                // |I1 - I2|^2

    Scalar s = sum(s1);                                             // 各个通道求和

    double sse = s.val[0] + s.val[1] + s.val[2];                    // 所有通道的值相加在一起
    if (sse <= 1e-10)       // 当值太小时近似于0，由公式可知分母为0时需另外对待，使用SSIM方法
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());    // 公式
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

/*TODO：调整
 * 将rgb三个空间平均为一个空间,同上函数只产生一个double值
*/

Scalar Video::getMSSIM(const Mat& i1, const Mat& i2)                       // SSIM方法
{
    const double C1 = 6.5025, C2 = 58.5225;

    Mat I1, I2;
    i1.convertTo(I1, CV_32F);                                       // 转换为32位进行运算
    i2.convertTo(I2, CV_32F);

    Mat I1_2 = I1.mul(I1);                                          // I1^2
    Mat I2_2 = I2.mul(I2);                                          // I2^2
    Mat I1_I2 = I1.mul(I2);                                         // I1 * I2

    Mat sigma1_2, sigma2_2, sigma12;                                // 先平方再高斯滤波
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);

    Mat mu1, mu2;
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2 = mu1.mul(mu1);                                       // 先高斯滤波再平方
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);

    sigma1_2 -= mu1_2;                                              // 两种方式的差值
    sigma2_2 -= mu2_2;
    sigma12 -= mu1_mu2;

    Mat t1, t2, t3, t4;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                                                // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t4 = t1.mul(t2);                                                // t4 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t4, ssim_map);                                       // ssim_map =  t3./t4;
    Scalar mssim = mean(ssim_map);                                  // ssim map矩阵的平均值


    return mssim;
}

