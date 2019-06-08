#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
 //#include <GL/glew.h>

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <opencv2/imgproc.hpp> // include image processing headers
#include <opencv2/highgui.hpp> // include GUI-related headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MarkerTracker.h"
#include "Ball.hpp"
#include "Player.hpp"
#include "DrawPrimitives.h"
#include "Object.hpp"
#include "GameElements.hpp"
#include "GameLoop.hpp"

void processInput(GLFWwindow *window,std::vector<Ball*>& balls);
void key_Callback(GLFWwindow *window,int key, int scancode,int action,int mods);

bool debugmode = true;
bool balldebug = false;

float resultTransposedMatrix_player1[16];
float resultTransposedMatrix_player2[16];
float resultTransposedMatrix_world[16];
float snowmanLookVector[4];
int towards = 0x005A;
int towardsList[2] = {0x005a, 0x0272};
int towardscounter = 0;

int camera_width;
int camera_height;

const int code_player1 = 0x0b44;
const int code_player2 = 0x1228;
const int code_world = 0x1c44;

const int player_1 = 1;
const int player_2 = 2;
const int OBSTACLE = 0;
const int BOMB = 1;


float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float lastShootingFrame1 = -1.0f;
float lastShootingFrame2 = -1.0f;

int p1Life = 3;
int p2Life = 3;

//const int virtual_camera_angle = 30;

float initMat[16] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,-0.2,1
};

float initMatBall[16] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    -0.08,-0.02,-0.2,1
};

bool sPressed = false;
bool lPressed = false;


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

cv::Mat arrayToMat(float* matrix){
    return cv::Mat(4, 4, CV_32FC1, matrix);
}

void MatToArray(cv::Mat1f matrix, float* mat_array){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            mat_array[i*4 + j] = matrix(i,j);
        }
    }
}

cv::Point3f getPosInWorld(float* resultTransposedMatrix_object, float* resultTransposedMatrix_world){
    cv::Mat Mat_object = arrayToMat(resultTransposedMatrix_object).t();
    cv::Mat Mat_world = arrayToMat(resultTransposedMatrix_world).t();
    
    cv::Mat Mat_world_inv = Mat_world.inv();
    cv::Mat object_pos_mat = Mat_world.inv() * Mat_object;
    
    cv::Point3f object_pos(object_pos_mat.at<float>(0,3), object_pos_mat.at<float>(1,3), object_pos_mat.at<float>(2,3));
    
    return object_pos;
}

bool checkcollisions_player(Ball ball, Player &player)
{
    
    glm::vec3 ballcenter(ball.pos.x, ball.pos.y, ball.pos.z);
    glm::vec3 playerhalf(player.length/2,player.height/2,player.width/2);
    glm::vec3 playercener(player.pos.x, player.pos.y, player.pos.z);
    
    glm::vec3 difference = ballcenter -playercener;
    
    glm::vec3 clamped = glm::clamp(difference,-playerhalf,playerhalf);
    
    glm::vec3 closest = playercener + clamped;
    
    
    
    difference = closest - ballcenter;
    
    
    return glm::length(difference) < (ball.radius);
}

bool checkcollision_element(Ball ball, GameElements element)
{
    
    glm::vec3 ballcenter(ball.pos.x, ball.pos.y, 0);
    glm::vec3 playerhalf(element.length/2,element.height/2,0);
    glm::vec3 playercener(element.pos.x, element.pos.y, 0);
    
    glm::vec3 difference = ballcenter -playercener;
    
    glm::vec3 clamped = glm::clamp(difference,-playerhalf,playerhalf);
    
    glm::vec3 closest = playercener + clamped;
    
    
    
    difference = closest - ballcenter;
    
    
    return glm::length(difference) < (ball.radius);
    
    return 0;
}

void docollisions(std::vector<Ball*>& balls,std::vector<GameElements*>& elements, Player player1, Player player2)
{
    for(Ball* ball : balls){
        
        if (ball->player == 2)
        {
            if (checkcollisions_player(*ball, player1))
            {
                std::cout << "touch player1" << std::endl;
                ball->color = 0.1;
                p1Life--;
            }
            else ball->color = 1;
        }
        
        if (ball->player == 1)
        {
            if (checkcollisions_player(*ball, player2))
            {
                std::cout << "touch player2" << std::endl;
                ball->color = 0.1;
                p2Life--;
            }
            else ball->color = 1;
        }
        
        for (GameElements *element : elements)
        {
            if (checkcollision_element(*ball, *element))
            {
                std::cout << "touch element" << std::endl;
                if (element->type == OBSTACLE) ball->destroy = true;
                else if (element->type == BOMB)
                {
                    //player lose life
                }
            }
            
            
        }
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

void update(std::vector<Marker> &markers, std::vector<Ball*>& balls, std::vector<GameElements*>& elements,Player& player1, Player& player2){

    // update position
    for(int i=0; i<markers.size(); i++){
        const int code =markers[i].code;

        // fix scale(translate x, y)
        float scale = 0.4;
        markers[i].resultMatrix[3] *= scale;
        markers[i].resultMatrix[7] *= scale;

        if(code == code_player1) {

            // transpose
            for (int x=0; x<4; ++x)
                for (int y=0; y<4; ++y)
                    resultTransposedMatrix_player1[x*4+y] = markers[i].resultMatrix[y*4+x];

            // change the coordinate to the world coordinate
            cv::Point3f pos = getPosInWorld(resultTransposedMatrix_player1, resultTransposedMatrix_world);

            //player1.setPos(pos);
            //std::cout << "main player1 pos:" << player1.pos << std::endl;

        }else if(code == code_player2){

            // transpose
            for (int x=0; x<4; ++x)
                for (int y=0; y<4; ++y)
                    resultTransposedMatrix_player2[x*4+y] = markers[i].resultMatrix[y*4+x];

            // change the coordinate to the world coordinate
            cv::Point3f pos = getPosInWorld(resultTransposedMatrix_player2, resultTransposedMatrix_world);

            //player2.setPos(pos);

        }else if(code == code_world){

            for (int x=0; x<4; ++x)
                for (int y=0; y<4; ++y)
                    resultTransposedMatrix_world[x*4+y] = markers[i].resultMatrix[y*4+x];
        }
    }
    
    
    for(int i=0;i<balls.size();i++)
    {
        balls[i]->checkdestory();
        if (balls[i]->destroy)
        {
            balls[i] = balls.back();
            balls.pop_back();
        }
    }
}

void display(const cv::Mat &img_bgr, std::vector<Ball*>& balls, std::vector<GameElements*>& elements,Player &player1, Player &player2){

    // clear buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    
    int size = static_cast<int>(balls.size());
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    // draw background image
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, camera_width, 0.0, camera_height );
    glRasterPos2i( 0, camera_height-1 );
    glDrawPixels( camera_width, camera_height, GL_RGB, GL_UNSIGNED_BYTE, img_bgr.data );
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    
    // move to marker-position
    glMatrixMode( GL_MODELVIEW );
    
    for (int i = 0; i < size; i++) {
        balls.at(i)->render();
    }
    
    for(GameElements* element : elements){
        element->render();
    }
    
  
    player1.draw(resultTransposedMatrix_player1);
    player2.draw(resultTransposedMatrix_player2);

    updateLives(p1Life,p2Life);
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

void keyprocess(std::vector<Ball*>& balls, float currentFrame)
{
    if (sPressed && currentFrame-lastShootingFrame1 >1)
    {
        Ball *ball = new Ball(resultTransposedMatrix_player1,currentFrame, player_1);
        balls.push_back(ball);
        sPressed = false;
        lastShootingFrame1 = currentFrame;
    }
    
    
    if (lPressed && currentFrame-lastShootingFrame2 > 1)
    {
        Ball *ball = new Ball(resultTransposedMatrix_player2,currentFrame, player_2);
        balls.push_back(ball);
        lPressed = false;
        lastShootingFrame2 = currentFrame;
    }
    
}



int main(int argc, char* argv[]) {
    
    cv::VideoCapture cap;
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Initialize Camera Size */
    cv::Mat img_bgr;
    InitializeVideoStream(cap);
    while(true){
        cap >> img_bgr;
        if(img_bgr.empty()){
            std::cout << "Could not query frame. Trying to reinitialize." << std::endl;
            InitializeVideoStream(cap);
            cv::waitKey(1000); /// Wait for one sec.
            continue;
        }else{
            break;
        }
    }
    camera_width = img_bgr.cols;
    camera_height = img_bgr.rows;
    
    
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
    const double kMarkerSize = 0.1;// 0.03; // [m]
    MarkerTracker markerTracker(kMarkerSize);
    std::vector<Marker> markers;
    
    // set object
    std::vector<Ball*> balls;
    std::vector<GameElements*> elements;
    Player player1;
    Player player2;
    
    
    GameElements *obstable = new GameElements(0,0,-0.2,OBSTACLE);
    elements.push_back(obstable);
    
    GameElements *bomb = new GameElements(0.05,0.05,-0.2,BOMB);
    elements.push_back(bomb);
    
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
        
        /* debug marker code */
        for (int i=0; i<markers.size(); i++) {
            std::cout << std::hex << markers[i].code << std::endl;
        }
        std::cout << std::endl;
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window,balls);
        
        glfwSetKeyCallback(window, key_Callback);
        

        /* Update balls position */
        update(markers, balls, elements, player1, player2);
        
        /*Prosess the key input accident*/
        
        keyprocess(balls,currentFrame);
        
        /* Render here */
        display(img_bgr, balls, elements,player1,player2);
        
        docollisions(balls, elements, player1, player2);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    
    return 0;
    
}

void processInput(GLFWwindow *window,std::vector<Ball*>& balls)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
};

void key_Callback(GLFWwindow *window,int key, int scancode,int action,int mods)
{
    if (key==GLFW_KEY_S && action == GLFW_PRESS)
    {
        sPressed = true;
    }
    
    if (key==GLFW_KEY_L && action == GLFW_PRESS)
    {
        lPressed = true;
    }
    
};




