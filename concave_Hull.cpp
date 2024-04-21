#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/surface/concave_hull.h>
#include <iostream>
#include <cstdlib> // Để sử dụng std::getenv
#include <pcl/filters/crop_hull.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/common/transforms.h>
#include <pcl/common/common.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/surface/concave_hull.h>
int main() {
    std::string pgScansDirectory;
    pgScansDirectory = std::getenv("HOME"); // Lấy biến môi trường HOME
    pgScansDirectory += "/Downloads/Scans_test/"; // Thêm phần đường dẫn còn lại

    // Tạo và kiểm tra thư mục tồn tại, nếu không tồn tại thì tạo mới
    if (system(("test -d " + pgScansDirectory).c_str()) != 0) {
        system(("mkdir -p " + pgScansDirectory).c_str());
    }

    std::string file_path = "/home/phucvinh/Downloads/save_map/Scans/000002.pcd";
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::io::loadPCDFile<pcl::PointXYZ>(file_path, *cloud);   

    // Tạo điểm mây với các điểm nằm trên cùng một mặt phẳng
    pcl::PointCloud<pcl::PointXYZ>::Ptr plane_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    // plane_cloud->push_back(pcl::PointXYZ(0.0, 0.0, 0.0));
    plane_cloud->push_back(pcl::PointXYZ(10,5,0));
    plane_cloud->push_back(pcl::PointXYZ(-10,-5,0));
    plane_cloud->push_back(pcl::PointXYZ(8,-10,0));
    plane_cloud->push_back(pcl::PointXYZ(-8,10,0));
    


    // Tính toán lồi dựa trên các điểm mây trên cùng một mặt phẳng
    pcl::PointCloud<pcl::PointXYZ>::Ptr concave_hull(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::ConcaveHull<pcl::PointXYZ> concave_hull_calculator;
    std::vector<pcl::Vertices> polygons;
    concave_hull_calculator.setInputCloud(plane_cloud);
    concave_hull_calculator.setAlpha(20); // Điều chỉnh giá trị Alpha theo yêu cầu của bạn
    concave_hull_calculator.reconstruct(*concave_hull, polygons); //calculate the area polygon pcl
    int dim = concave_hull_calculator.getDimension (); //define the number of dimension

    // Crop Hull
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::CropHull<pcl::PointXYZ> crop_filter;
    crop_filter.setInputCloud (cloud);
    crop_filter.setHullCloud (concave_hull);
    crop_filter.setHullIndices (polygons);
    crop_filter.setDim (dim);
    crop_filter.filter (*cloud_out);

    // Hiển thị kết quả

    pcl::visualization::CloudViewer viewer3("Filtered Cloud");
    viewer3.showCloud(filtered_cloud);
    while (!viewer3.wasStopped()) {
        // Đợi người dùng đóng cửa sổ
    }

    // pcl::io::savePCDFileBinary(pgScansDirectory + "crop_cloud.pcd", *cloud_out);
    // pcl::io::savePCDFileBinary(pgScansDirectory + "outside.pcd", *filtered_cloud);
    // In kích thước của điểm mây lồi
    std::cout << "Concave Hull Size: " << filtered_cloud->size() << std::endl;

    return 0;
}
