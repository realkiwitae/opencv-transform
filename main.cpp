#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat rotate(const Mat array, double angle_in_degrees);
Mat rotate_and_translate(const Mat array, double angle_in_degrees , int cx , int cy);
Mat rotate_and_translate2(const Mat array, double angle_in_degrees , int cx , int cy);

int main() {
    Mat image = imread("./media/lena.png");
    Mat result;

    // create a vector ofimages
    vector<Mat> images = {
        image,
        rotate(image, 45),
        rotate_and_translate(image, 45,image.cols/2,image.rows/2),
        rotate_and_translate2(image, 45,image.cols/2,image.rows/2)
    };

    // concatenate the images horizontally
    hconcat(images, result);

    // display the concatenated image
    imshow("Lena - My function - Rotated&Translated - Rotated&Translated&Wrapped", result);
    waitKey(0);
    imwrite("./media/output.jpg", result);
    imwrite("./media/myfunction.jpg", images[1]);
    imwrite("./media/rotated_translated.jpg", images[2]);
    imwrite("./media/rotated_translated_wrapped.jpg", images[3]);


    return 0;
}


Mat rotate(const Mat array, double angle_in_degrees)
{
    // rotate image with angle . Center of the rotation is topleft corner.

    int h = array.rows;
    int w = array.cols;
    Mat result = Mat::zeros(h, w, array.type());
    double angle = angle_in_degrees * M_PI / 180;
    Mat rotation_matrix = (Mat_<double>(2,2) << cos(angle), sin(angle), -sin(angle), cos(angle));

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Vec3b pixels = array.at<Vec3b>(y, x);
            Mat point = (Mat_<double>(2,1) << x, y);
            Mat new_point = rotation_matrix * point;
            int new_x = round(new_point.at<double>(0,0));
            int new_y = round(new_point.at<double>(1,0));
            if (0 <= new_x && new_x < w && 0 <= new_y && new_y < h) {
                result.at<Vec3b>(new_y, new_x) = pixels;
            }
        }
    }

    return result;
}

Mat rotate_and_translate(const Mat array, double angle_in_degrees , int cx , int cy)
{
    // transform T'*R*T image

    int h = array.rows;
    int w = array.cols;
    Mat result = Mat::zeros(h, w, array.type());
    double angle = angle_in_degrees * M_PI / 180;
    Mat rotation_matrix = (Mat_<double>(3,3) << cos(angle), sin(angle), 0,-sin(angle), cos(angle),0,0,0,1);
    Mat translation_matrix = (Mat_<double>(3,3) << 1, 0, -cx,0,1,-cy,0,0,1);
    Mat translation_inv_matrix = (Mat_<double>(3,3) << 1, 0, cx,0,1,cy,0,0,1);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            Vec3b pixels = array.at<Vec3b>(y, x);
            Mat point = (Mat_<double>(3,1) << x, y,1);
            Mat new_point = translation_inv_matrix*rotation_matrix *translation_matrix *point;
            int new_x = round(new_point.at<double>(0,0));
            int new_y = round(new_point.at<double>(1,0));
            if (0 <= new_x && new_x < w && 0 <= new_y && new_y < h) {
                result.at<Vec3b>(new_y, new_x) = pixels;
            }
        }
    }

    return result;
}

Mat rotate_and_translate2(const Mat array, double angle_in_degrees , int cx , int cy)
{
    // transform T'*R*T image + interpolation 

    int h = array.rows;
    int w = array.cols;
    Mat result(h, w, array.type());
    double angle = angle_in_degrees * M_PI / 180;
    Mat rotation_matrix = (Mat_<double>(3,3) << cos(angle), sin(angle), 0,-sin(angle), cos(angle),0,0,0,1);
    Mat translation_matrix = (Mat_<double>(3,3) << 1, 0, -cx,0,1,-cy,0,0,1);
    Mat translation_inv_matrix = (Mat_<double>(3,3) << 1, 0, cx,0,1,cy,0,0,1);
    
    Mat transform = translation_inv_matrix*rotation_matrix *translation_matrix;
    
    warpPerspective(array, result, transform, Size(w, h));


    return result;
}





