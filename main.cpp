#include <vector>
#include <stdlib.h>
// sleep
#include <unistd.h>

// -------- haar ----------------------------
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

#define CAM_WIDTH 320
#define CAM_HEIGHT 240

// -------- boost --------------------------
#include <list>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bg = boost::geometry;

typedef boost::geometry::model::d2::point_xy<double> point_type;
typedef boost::geometry::model::polygon<point_type> polygon_type;


// ---------split ----------------------
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>


// ------- socketio-cpp-client --------------
#include "../socket.io-client-cpp/src/sio_client.h"

#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

#ifdef WIN32
#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl

#include <stdio.h>
#include <tchar.h>
#define MAIN_FUNC int _tmain(int argc, _TCHAR* argv[])
#else
#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#define MAIN_FUNC int main(int argc ,const char* args[])
#endif

using namespace sio;
using namespace std;
std::mutex _lock;

std::condition_variable_any _cond;
bool connect_finish = false;

/** Global variables */
String face_cascade_name;
CascadeClassifier face_cascade;
String window_name = "Face detection";
string coordinates_string_total;
string coordinates_face_total;
socket::ptr current_socket;
vector<Point> points;
int reset_flag = 0;


class connection_listener
{
    sio::client &handler;

public:  
    connection_listener(sio::client& h):
    handler(h)
    {
    }
    
    void on_connected()
    {
        _lock.lock();
        _cond.notify_all();
        std::cout << "Connected to the server." << std::endl;
        connect_finish = true;
        _lock.unlock();
    }
    void on_close(client::close_reason const& reason)
    {
        std::cout<<"sio closed "<<std::endl;
        exit(0);
    }
    
    void on_fail()
    {
        std::cout<<"sio failed "<<std::endl;
        exit(0);
    }
};

void bind_events()
{
	current_socket->on("coordinates", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp)
        {
            _lock.lock();
            points.clear();
            string coordinates = data->get_string();

            //string input("geeks\tfor\tgeeks");
            std::vector<string> result;
            boost::split(result, coordinates, boost::is_any_of(" "));
            result.pop_back();
            //cout << result.size() << endl;

            /*
            for (int i = 0; i < result.size(); i++)
                cout << result[i] << endl;
            */
            
            std::vector<int> coordinates_int;
            std::transform(result.begin(), result.end(), std::back_inserter(coordinates_int),
                    [](const std::string& str) { return std::stoi(str); });
            
            /*
            for (int i = 0; i < coordinates_int.size(); i = i+2)
                cout << "x : " << coordinates_int[i] << ", y : " << coordinates_int[i+1] << endl;
            */

            for (int i = 0; i < coordinates_int.size(); i = i+2)
                points.push_back(Point(coordinates_int[i] , coordinates_int[i+1]));

            string coordinates_string;
            
            for (int i = 0; i < result.size(); i = i+2) {
                coordinates_string += result[i] +" " + result[i+1];
                if(i != result.size()-2) {
                    coordinates_string += ",";
                }
            }

            coordinates_string += "," + result[0] + " " + result[1];
            
            coordinates_string_total = "POLYGON((" + coordinates_string + "))";
        
            std::cout << "New boundary coordinates for making polygon: "<< std::endl 
            << coordinates_string_total << std::endl;


            _lock.unlock();
        }));
    
   current_socket->on("resetFromHtml", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp)
        {
            _lock.lock();
            std::cout << "Processing boundary reset." << std::endl;
            reset_flag = 1;

            _lock.unlock();
        }));

}

// -------- base64 --------------------------
#include "base64.h"
// ------------------------------------------


// This code is written at BigVision LLC. It is based on the OpenCV project. It is subject to the license terms in the LICENSE file found in this distribution and at http://opencv.org/license.html

// Usage example:  ./object_detection_yolo.out --video=run.mp4
//                 ./object_detection_yolo.out --image=bird.jpg
#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace dnn;
using namespace std;

// Initialize the parameters
float confThreshold = 0.5; // Confidence threshold
float nmsThreshold = 0.4;  // Non-maximum suppression threshold
int inpWidth = 416;  // Width of network's input image
int inpHeight = 416; // Height of network's input image
vector<string> classes;

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(Mat& frame, const vector<Mat>& out);

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);

// Get the names of the output layers
vector<String> getOutputsNames(const Net& net);


MAIN_FUNC
{
// ------- socketio --------------------------------------------------------------------------------------
    sio::client h;
    connection_listener l(h);
    
    h.set_open_listener(std::bind(&connection_listener::on_connected, &l));
    h.set_close_listener(std::bind(&connection_listener::on_close, &l,std::placeholders::_1));
    h.set_fail_listener(std::bind(&connection_listener::on_fail, &l));
    h.connect("http://127.0.0.1:3001");
    //h.connect("http://34.64.166.61:3001");
    _lock.lock();
    
    if(!connect_finish)
    {
        _cond.wait(_lock);
    }
    _lock.unlock();
	current_socket = h.socket();
    bind_events();
// ----------------------------------------------------------------------------------------

    // Load names of classes
    string classesFile = "obj.names";
    ifstream ifs(classesFile.c_str());
    string line;
    while (getline(ifs, line)) classes.push_back(line);

    string device = "cpu";
    // Give the configuration and weight files for the model
    String modelConfiguration = "yolov3-tiny_training.cfg";
    String modelWeights = "yolov3-tiny_training_fourth.weights";

    // Load the network
    Net net = readNetFromDarknet(modelConfiguration, modelWeights);

    if (device == "cpu")
    {
        //cout << "Using CPU device" << endl;
        net.setPreferableBackend(DNN_TARGET_CPU);
    }
    else if (device == "gpu")
    {
        //cout << "Using GPU device" << endl;
        net.setPreferableBackend(DNN_BACKEND_CUDA);
        net.setPreferableTarget(DNN_TARGET_CUDA);
    }


    // VideoCapture cam;
    Mat blob;
    Mat frame;
    VideoCapture cam(0);
    if (!cam.isOpened()) { printf("--(!)Error opening video cam\n"); return -1; }
    cam.set(CAP_PROP_FRAME_WIDTH, 320);
    cam.set(CAP_PROP_FRAME_HEIGHT, 240);
    int clear_result = system("clear");
    // Process frames.
    while (cam.read(frame))
    {
        
        // Create a 4D blob from a frame.
        blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth, inpHeight), Scalar(0,0,0), true, false);

        net.setInput(blob);

        vector<Mat> outs;
        net.forward(outs, getOutputsNames(net));
        
        // Remove the bounding boxes with low confidence
        postprocess(frame, outs);
        
        // Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
        vector<double> layersTimes;
        double freq = getTickFrequency() / 1000;
        double t = net.getPerfProfile(layersTimes) / freq;
        
        /*
        string label = format("Inference time for a frame : %.2f ms", t);
        putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        */

        if (!points.empty()) {
            polylines(frame, points, true, Scalar(0, 0, 255), 2);
        }


        if (!coordinates_string_total.empty() && !coordinates_face_total.empty()) {
            bg::model::polygon<bg::model::d2::point_xy<double> > poly1;
            bg::read_wkt(coordinates_string_total, poly1);

            bg::model::polygon<bg::model::d2::point_xy<double> > poly2;
            bg::read_wkt(coordinates_face_total, poly2);
            bool check_covered = bg::covered_by(poly2, poly1);
            if (!check_covered) {
                std::cout << "The Baby is out of safety zone!" << std::endl;
                string data = "baby_out";
                current_socket->emit("baby_out");
                sleep(3);
            }
            
            coordinates_face_total.clear();
        }

        // base64 encoding 
        vector<uchar> buff;//buffer for coding
	    imencode(".jpg",frame, buff);
	    std::string s(buff.begin(), buff.end());
        std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
        current_socket->emit("image", encoded);

        if (reset_flag == 1) {
            //std::cout << "empty" << std::endl;
            points.clear();
            coordinates_string_total.clear();
            
            cam.read(frame);
            
            // base64 encoding 
            vector<uchar> buff_reset;//buffer for coding
            imencode(".jpg",frame, buff_reset);
            std::string s(buff_reset.begin(), buff_reset.end());
            std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
            current_socket->emit("image", encoded);

            while (points.empty())
            {
                sleep(3);                
            }
            reset_flag = 0;
            std::cout << "Boundary reset completed." << std::endl;
        }
        
        char c = (char)waitKey(10);
        if (c == 27) { break; }
    }
    
    cam.release();
 
    h.sync_close();
    h.clear_con_listeners();

    return 0;
}

// Remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(Mat& frame, const vector<Mat>& outs)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;
    
    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                
                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }
    
    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    vector<int> indices;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y,
                 box.x + box.width, box.y + box.height, frame);
    }
}

// Draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
{
    //Draw a rectangle displaying the bounding box
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);
    
    string s_x1 = to_string(left);
    string s_y1 = to_string(top);
    string s_x2 = to_string(right);
    string s_y2 = to_string(bottom);

    string coordinates_face_string;
    coordinates_face_string = s_x1 + " " + s_y1 + "," + s_x2 + " " + s_y1 + ","
                            + s_x2 + " " + s_y2 + "," + s_x1 + " " + s_y2 + ","
                            + s_x1 + " " + s_y1;

    coordinates_face_total = "POLYGON((" + coordinates_face_string + "))";

    //std::cout << s_x1 << s_y1 << s_x2 << s_y2 << std::endl;

    //Get the label for the class name and its confidence
    string label = format("%.2f", conf);
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        //label = classes[classId] + ":" + label;
        label = classes[classId];
    }
    
    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);
}

// Get the names of the output layers
vector<String> getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = net.getUnconnectedOutLayers();
        
        //get the names of all the layers in the network
        vector<String> layersNames = net.getLayerNames();
        
        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}