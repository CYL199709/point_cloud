// 2020.02
//点云可视化

#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <iostream>

using namespace std;
using namespace pcl;


int user_data;
void
viewerOneOff(pcl::visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor(255, 255, 255);     //设置背景颜色
	//用于改变显示点云的尺寸，可以利用该方法控制点云在视窗中的显示方法,1设置显示点云大小
	viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 4);
	
	
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr 
Preprocess(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud)
{
	int i = 0;
	float max_x = 0, max_y = 0, max_z = 0;
	float min_x = 0, min_y = 0, min_z = 0;
	float d_x = 0, d_y = 0, d_z = 0;
	for (i = 0; i < cloud->width * cloud->height; i++)
	{
		if (cloud->points[i].x > max_x)
			max_x = cloud->points[i].x;
		if (cloud->points[i].y > max_y)
			max_y = cloud->points[i].y;
		if (cloud->points[i].z > max_z)
			max_z = cloud->points[i].z;
		if (cloud->points[i].x > min_x)
			min_x = cloud->points[i].x;
		if (cloud->points[i].y > min_y)
			min_y = cloud->points[i].y;
		if (cloud->points[i].z > min_z)
			min_z = cloud->points[i].z;
	}
	d_x = (max_x - min_x)/2;
	d_y = (max_y - min_y)/2;
	d_z = (max_z - min_z)/2;
	for (i = 0; i < cloud->width * cloud->height; i++)
	{
		cloud->points[i].x = cloud->points[i].x -  d_x;
		cloud->points[i].y = cloud->points[i].y -  d_y;
		cloud->points[i].z = cloud->points[i].z -  d_z;
	}
	return cloud;
}

void getAllFiles(string path, vector<string>& files)
{
	//文件句柄  
	__int64 hFile = 0;
	//文件信息  
	struct __finddata64_t  fileinfo;  //很少用的文件信息读取结构
	string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	if ((hFile = _findfirst64(p.assign(path).append("/*.pcd").c_str(), &fileinfo)) == -1)
	{
		cout << "No file is found\n" << endl;
	}
	else
	{
		do
		{
			files.push_back(p.assign(path).append("/").append(fileinfo.name));
		} while (_findnext64(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

int main()
{
	//文件名
	string path = "F:\\CYL\\2020\\S\\ASIS与MNFE结果\\ASIS199_4096\\pcd\\";
	vector<string> files;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>());
	getAllFiles(path, files);
	for (auto file : files)
	{

		if (pcl::io::loadPLYFile<pcl::PointXYZRGB>(file, *cloud) == -1) //* 读入文件，如果文件不存在，返回-1	
		{
			PCL_ERROR("Couldn't read file  \n"); //文件不存在时，返回错误，终止程序。	
			system("pause");
			return (-1);
		}
		std::cout << "Loaded "
			<< cloud->width * cloud->height
			<< "  points from " << file
			<< std::endl;


		// Show Point Cloud on Cloud Viewer在点云框中显示点云图像

		pcl::visualization::CloudViewer viewer("Point Cloud Viewer");
		viewer.runOnVisualizationThreadOnce(viewerOneOff);

		while (!viewer.wasStopped())
		{
			viewer.showCloud(cloud);
		}
	}

	
	system("pause");
	return 0;
}
