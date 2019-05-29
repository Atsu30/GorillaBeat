
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
 //#include <GL/glew.h>

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <opencv2/imgproc.hpp> // include image processing headers
#include <opencv2/highgui.hpp> // include GUI-related headers


#include "PoseEstimation.h"
#include "MarkerTracker.h"
#include "DrawPrimitives.h"


struct Position { double x,y,z; };

bool debugmode = false;
bool balldebug = false;

float resultTransposedMatrix[16];
float snowmanLookVector[4];
int towards = 0x005A;
int towardsList[2] = {0x005a, 0x0272};
int towardscounter = 0;

//camera settings
const int camera_width  = 848;
const int camera_height = 480;
//const int virtual_camera_angle = 30;

unsigned char bkgnd[camera_width*camera_height*3];

void InitializeVideoStream( cv::VideoCapture &camera ) {
    if( camera.isOpened() )
        camera.release();
    
    camera.open(0); // open the default camera
    if ( camera.isOpened()==false ) {
        std::cout << "No webcam found, using a video file" << std::endl;
        camera.open("MarkerMovie.mpg");
        if ( camera.isOpened()==false ) {
            std::cout << "No video file found. Exiting."      << std::endl;
            exit(0);
        }
    }
}

void multMatrix(float mat[16], float vec[4])
{
    for(int i=0; i<4; i++)
    {
        snowmanLookVector[i] = 0;
        for(int j=0; j<4; j++)
            snowmanLookVector[i] += mat[4*i + j] * vec[j];
    }
}

/* program & OpenGL initialization */
void initGL(int argc, char *argv[])
{
    // initialize the GL library
    
    // pixel storage/packing stuff
    glPixelStorei(GL_PACK_ALIGNMENT, 1);// for glReadPixels
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // for glTexImage2D
    glPixelZoom(1.0, -1.0);
    
    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0, 0, 0, 1.0 );
    
    // enable and set depth parameters
    glEnable( GL_DEPTH_TEST );
    glClearDepth( 1.0 );
    
    // light parameters
    GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_dif[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    
    // enable lighting
    glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
    glLightfv( GL_LIGHT0, GL_AMBIENT,  light_amb );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_dif );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
}

void display(GLFWwindow* window, const cv::Mat &img_bgr, std::vector<Marker> &markers)
{
    //const auto camera_image_size = sizeof(unsigned char) *img_bgr.rows*img_bgr.cols * 3;
    auto background_buffer_size = sizeof(bkgnd);
    memcpy(bkgnd, img_bgr.data, background_buffer_size);
    
    int width0, height0;
    glfwGetFramebufferSize(window, &width0, &height0);
    
    //reshape(window, width, height);
    
    // clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // draw background image
    glDisable( GL_DEPTH_TEST );
    
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, camera_width, 0.0, camera_height );
    
    glRasterPos2i( 0, camera_height-1 );
    
    glDrawPixels( camera_width, camera_height, GL_RGB, GL_UNSIGNED_BYTE, bkgnd );
    
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
    
    //    return;
    
    // move to marker-position
    glMatrixMode( GL_MODELVIEW );
    
    // draw something at marker position
    float resultMatrix_005A[16];
    float resultMatrix_0272[16];
    for(int i=0; i<markers.size(); i++){
        const int code =markers[i].code;
        if(code == 0x005a) {
            for(int j=0; j<16; j++)
                resultMatrix_005A[j] = markers[i].resultMatrix[j];
        }else if(code == 0x0272){
            for(int j=0; j<16; j++)
                resultMatrix_0272[j] = markers[i].resultMatrix[j];
        }
    }
    
    // draw image at the marker(id:005A)
    for (int x=0; x<4; ++x)
        for (int y=0; y<4; ++y)
            resultTransposedMatrix[x*4+y] = resultMatrix_005A[y*4+x];
    
    // Fixed tranlate scale
    float scale = 0.5;
    resultTransposedMatrix[12] *= scale;
    resultTransposedMatrix[13] *= scale;
    
    //glLoadTransposeMatrixf( resultMatrix );
    glLoadMatrixf( resultTransposedMatrix );
    drawSnowman();
    //drawCube();
    
    // draw image at the marker(id:0272)
    for (int x=0; x<4; ++x)
        for (int y=0; y<4; ++y)
            resultTransposedMatrix[x*4+y] = resultMatrix_0272[y*4+x];
    
    glLoadMatrixf( resultTransposedMatrix );
    
    
    int key = cv::waitKey (10);
    if (key == 27) exit(0);
    else if (key == 100) debugmode = !debugmode;
    else if (key == 98) balldebug = !balldebug;
    
}

void reshape( GLFWwindow* window, int width, int height ) {
    
    // set a whole-window viewport
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    
    // create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );
    
    // invalidate display
    //glutPostRedisplay();
}

bool checkMarker(std::vector<Marker> &markers, int check_code){
    for(int i=0; i<markers.size(); i++){
        const int code =markers[i].code;
        if(code == check_code) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    
    cv::VideoCapture cap;
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    
    // initialize the window system
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(camera_width/2, camera_height/2, "Game Window", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );
    
    int window_width, window_height;
    
    glfwGetFramebufferSize(window, &window_width, &window_height);  //window_width:1696 window_height:960
    
    reshape(window, window_width, window_height);
    
    glViewport(0, 0, window_width, window_height);
    
    // initialize the GL library
    initGL(argc, argv);
    
    // setup OpenCV
    cv::Mat img_bgr;
    InitializeVideoStream(cap);
    const double kMarkerSize = 0.08;// 0.03; // [m]
    MarkerTracker markerTracker(kMarkerSize);
    
    std::vector<Marker> markers;
    
    //    float resultMatrix[16];
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        markers.resize(0);
        /* Capture here */
        cap >> img_bgr;
        
        if(img_bgr.empty()){
            std::cout << "Could not query frame. Trying to reinitialize." << std::endl;
            InitializeVideoStream(cap);
            cv::waitKey(1000); /// Wait for one sec.
            continue;
        }
        
        /* Track a marker */
        markerTracker.findMarker( img_bgr, markers);///resultMatrix);
        
//        cv::imshow("img_bgr", img_bgr);
//        cv::waitKey(10); /// Wait for one sec.
        
        
        /* Render here */
        display(window, img_bgr, markers);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    
    return 0;
    
}


