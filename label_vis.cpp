// 2020.09
//将网络输出的txt文件转为ply文件，不同标签不同颜色，可视化

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
	viewer.setBackgroundColor(0, 0, 0);     //设置背景颜色
	//用于改变显示点云的尺寸，可以利用该方法控制点云在视窗中的显示方法,1设置显示点云大小
	viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 4);


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


//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）  
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}



//int main()
//{
//
//	string save_path = "D:\\cyl\\3D-BoNet-10_3\\log\\test_res\\050\\PC4\\4-1";
//	vector<string> files;
//	getAllFiles(save_path, files);
//
//	string line;
//	float x, y, z;
//	float x1, y1, z1;
//	float r, g, b;
//	float ins_gt, ins_pre;
//	int colour_map[] = { 255, 0, 0,
//		0, 255, 0,
//		0, 0, 255,
//		156, 102, 31,
//		255, 192, 203,
//		255, 0, 255,
//		0, 255, 255,
//		255, 255, 0,
//		51, 161, 201,
//		128, 42, 42,
//		48, 128, 20,
//		160, 32, 240,
//		255, 128, 0 };
//
//
//	pcl::PointXYZRGB point;
//	pcl::PointXYZRGB gt_point;
//	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pred_cloud(new pcl::PointCloud<pcl::PointXYZRGB>());
//	pcl::PointCloud<pcl::PointXYZRGB>::Ptr gt_cloud(new pcl::PointCloud<pcl::PointXYZRGB>());
//
//	int Isshown = 1;
//	int Issave = 0;
//	for (int i = 0; i < files.size(); i++){
//		string file = files[i];
//		ifstream fin(file);
//		
//		int j = 0;
//
//		while (!fin.eof())
//		{
//			
//			for (int i = 0; i < 1024; i++)
//			{
//				getline(fin, line);
//				if (!fin) break; //check for eof
//				istringstream buffer(line);
//				
//				buffer >> x >> y >> z >> r >> g >> b >> x1 >> y1 >> z1 >> ins_gt >> ins_pre;
//				/*cout << "x:" << x << "y:" << y << "z: " << z << "r: " << r << "g: " << g << "b: " << b << "x1: " << x1 << "y1: " << y1
//					<< "z1: " << z1 << "gt: " << ins_gt << "pre: " << ins_pre << endl;*/
//				
//				point.x = x;
//				point.y = y;
//				point.z = z;
//				ins_pre = ins_pre * 3;
//				point.r = colour_map[int(ins_pre)];
//				point.g = colour_map[int(ins_pre + 1)];
//				point.b = colour_map[int(ins_pre + 2)];
//				gt_point = point;
//				ins_gt = ins_gt * 3;
//				gt_point.r = colour_map[int(ins_gt)];
//				gt_point.g = colour_map[int(ins_gt + 1)];
//				gt_point.b = colour_map[int(ins_gt + 2)];
//				pred_cloud->push_back(point);
//				gt_cloud->push_back(gt_point);
//			}
//			
//		}
//		fin.close();
//		
//	}
//	cout << pred_cloud->points.size();
//
//	if (Isshown){
//		// Show Point Cloud on Cloud Viewer在点云框中显示点云图像
//		boost::shared_ptr< pcl::visualization::PCLVisualizer > viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
//		viewer->initCameraParameters();
//
//		//在一个视图里显示2张点云图
//		int v1(0);
//		viewer->createViewPort(0.0, 0.0, 1.0 / 2.0, 1.0, v1);
//		viewer->setBackgroundColor(0, 0, 0, v1);
//		//viewer->addText("Radius:0.01", 10, 10, "v1 text", v1);
//		viewer->addPointCloud<pcl::PointXYZRGB>(pred_cloud, " cloud1", v1);
//
//		int v2(0);
//		viewer->createViewPort(1.0 / 2.0, 0.0, 1.0, 1.0, v2);
//		viewer->setBackgroundColor(0, 0, 0, v2);
//		//viewer->addText("Radius:0.1", 10, 10, "v2 text", v2);
//		viewer->addPointCloud<pcl::PointXYZRGB>(gt_cloud, " cloud2", v2);
//
//		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, " cloud1");
//		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, " cloud2");
//		viewer->spin();
//
//	}
//
//	/*if (Issave){
//	pcl::io::savePLYFile(save_path + "pred_ins\\" + to_string(j) + "pred_ins.ply", *pred_cloud);
//	pcl::io::savePLYFile(save_path + "gt_ins\\" + to_string(j) + "gt_ins.ply", *gt_cloud);
//	cout << "file_saved" << to_string(j) << endl;
//	}*/
//
//
//
//	system("pause");
//	return 0;
//}


int main()
{

	string save_path = "F:\\CYL\\2020\\S\\004植物数据集生成H5过程 8192\\000标记PCD原始文件\\";
	vector<string> files;
	getAllFiles(save_path, files);

	string line;
	float x, y, z;
	float x1, y1, z1;
	float r, g, b,a;
	float ins_gt, ins_pre;
	int colour_map[] = { 255, 0, 0,
		0, 255, 0,
		0, 0, 255,
		156, 102, 31,
		255, 192, 203,
		255, 0, 255,
		0, 255, 255,
		255, 255, 0,
		51, 161, 201,
		128, 42, 42,
		48, 128, 20,
		160, 32, 240,
		255, 128, 0 };


	pcl::PointXYZRGB point;
	pcl::PointXYZRGB gt_point;
	
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr gt_cloud(new pcl::PointCloud<pcl::PointXYZRGB>());

	int Isshown = 1;
	int Issave = 0;
	int flag = 0;
	int label;
	for (int i = 0; i < files.size(); i++){
		string file = files[i];
		ifstream fin(file);
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr pred_cloud(new pcl::PointCloud<pcl::PointXYZRGB>());
		flag = 0;
		cout << file << endl;
		while (!fin.eof())
		{


			getline(fin, line);
			if (!fin) break; //check for eof
			istringstream buffer(line);

			buffer >> x >> y >> z >> label >> ins_pre;
			/*cout << "x:" << x << "y:" << y << "z: " << z << "r: " << r << "g: " << g << "b: " << b << "x1: " << x1 << "y1: " << y1
			<< "z1: " << z1 << "gt: " << ins_gt << "pre: " << ins_pre << endl;*/
			if (x == 0 && y == 0 && z == 0)
			{
				flag = 1;
			}
			if (flag == 1)
			{

				point.x = x;
				point.y = y;
				point.z = z;
				ins_pre = label * 3;
				point.r = colour_map[int(ins_pre)];
				point.g = colour_map[int(ins_pre + 1)];
				point.b = colour_map[int(ins_pre + 2)];
				gt_point = point;

				pred_cloud->push_back(point);
			}

		}
		fin.close();






		cout << pred_cloud->points.size();

		if (Isshown){
			// Show Point Cloud on Cloud Viewer在点云框中显示点云图像
			boost::shared_ptr< pcl::visualization::PCLVisualizer > viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
			viewer->initCameraParameters();


			viewer->addPointCloud<pcl::PointXYZRGB>(pred_cloud, " cloud1");

			viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, " cloud1");
			viewer->spin();

		}
		
	}

	/*if (Issave){
	pcl::io::savePLYFile(save_path + "pred_ins\\" + to_string(j) + "pred_ins.ply", *pred_cloud);
	pcl::io::savePLYFile(save_path + "gt_ins\\" + to_string(j) + "gt_ins.ply", *gt_cloud);
	cout << "file_saved" << to_string(j) << endl;
	}*/



	system("pause");
	return 0;
}







