#include "detectFeature.hpp"

Mat DetectFeature::Process( Mat img_object, Mat img_scene )
{
	printMatType(img_object,"img_object");
	printMatType(img_scene,"img_scene");
	
	cvtColor(img_object, img_object, CV_BGR2GRAY);
	cvtColor(img_scene, img_scene, CV_BGR2GRAY);

	printMatType(img_object,"img_object");
	printMatType(img_scene,"img_scene");

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector( minHessian );

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect( img_object, keypoints_object );
	detector.detect( img_scene, keypoints_scene );
	
	printMatType(img_object,"img_object");
	printMatType(img_scene,"img_scene");
	
	cout<<"end step 1"<<endl;

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute( img_object, keypoints_object, descriptors_object );
	extractor.compute( img_scene, keypoints_scene, descriptors_scene );
	
	printMatType(descriptors_object,"descriptors_object");
	printMatType(descriptors_scene,"descriptors_scene");
	
	//test
	Mat new_descriptors_object;
	descriptors_object.convertTo(new_descriptors_object,CV_32F);
	printMatType(new_descriptors_object,"new_descriptors_object");
	Mat new_descriptors_scene;
	descriptors_scene.convertTo(new_descriptors_scene,CV_32F);
	printMatType(new_descriptors_scene,"new_descriptors_scene");
	
	/*if ( new_descriptors_object.empty() )
		cvError(0,"MatchFinder","new_descriptors_object is empty",__FILE__,__LINE__);    
	if ( new_descriptors_scene.empty() )
		cvError(0,"MatchFinder","new_descriptors_scene is empty",__FILE__,__LINE__);*/
	
	if(new_descriptors_scene.empty()){
		cout<<"Error: new_descriptors_scene is empty."<<endl;
		return img_scene;
	}
	
	cout<<"end step 2"<<endl;

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	//matcher.match( descriptors_object, descriptors_scene, matches );
	matcher.match( new_descriptors_object, new_descriptors_scene, matches );

	double max_dist = 0; double min_dist = 100;
	
	cout<<"end step 3"<<endl;

	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_object.rows; i++ ){
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	cout<<"-- Max dist : "<<max_dist<<endl;
	cout<<"-- Min dist : "<<min_dist<<endl;

	cout<<"end step 4"<<endl;

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for( int i = 0; i < descriptors_object.rows; i++ ){ 
		if( matches[i].distance < 3*min_dist ) {
			good_matches.push_back( matches[i]);
		}
	}

	Mat img_matches;
	drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
	good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	cout<<"end step 5"<<endl;

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( int i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}
	if (obj.size()<4 || scene.size()<4){
		cout<<"Error: obj or scene less then 4 good matches."<<endl;
		return img_scene;
	}
	
	Mat H = findHomography( obj, scene, CV_RANSAC );

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	return img_matches;
}
