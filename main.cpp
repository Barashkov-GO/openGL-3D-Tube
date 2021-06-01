#include <iostream>

#include "stb_image.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

const char *tex_path[] = {"texture.bmp", "texture2.bmp", "texture3.bmp"};
unsigned int text_id;

float timer;
float per_x = 0;
float per_y = 0;
float per_z = 0;
bool polygon = true;
bool is_twining = false;
bool is_material = false;
bool is_light = false;
bool is_textured = false;
float scale_x = 1;
float scale_y = 1;
float scale_z = 1;
int WIDTH = 1000;
int HEIGHT = 1000;

struct Point {
    float x{};
    float y{};
    float z{};
    Point(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point()= default;
};

Point operator*(Point b, float a) {
    return {b.x * a, b.y * a, b.z * a};
}

struct Vec {
    float x;
    float y;
    float z;

    Vec(float a, float b, float c){
        x = a;
        y = b;
        z = c;
    }

    Vec(Point a, Point b){
        x = b.x - a.x;
        y = b.y - a.y;
        z = b.z - a.z;
    }

    Vec(){
        x = 0;
        y = 0;
        z = 0;
    }

    friend float operator*(const Vec &a, const Vec &b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    friend Vec operator^(const Vec &a, const Vec &b){
        return {a.y * b.z - a.z * b.y, -(a.x * b.z - b.x * a.z), a.x * b.y - b.x * a.y};
    }

};

pair<float, float> tex_coord(int cs, int cis, int i, int j){
    return {1.0f / cs * static_cast<float>(i), 1.0f / cis * static_cast<float>(j)};
}

void load_texture(const char *path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    auto data = stbi_load(path, &width, &height, &nrChannels, 0);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (nrChannels == 3){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if(nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
      cout << "texture error\n";
    }
    stbi_image_free(data);
}

Vec normal_for_face(Point a, Point b, Point c){
    Vec v1 = Vec(a, b);
    Vec v2 = Vec(b, c);
    return v1 ^ v2;
}

Vec balance_norms(Vec a, Vec b, Vec c, Vec d) {
    return {-(a.x + b.x + c.x + d.x) / 4, -(a.y + b.y + c.y + d.y) / 4, -(a.z + b.z + c.z + d.z) / 4};
}

Vec balance_norms(Vec a, Vec b) {
    return {-(a.x + b.x) / 4, -(a.y + b.y) / 4, -(a.z + b.z) / 4};
}

Vec normal_for_vertex(vector<vector<Point>> &circles, int i, int j) {
    int i_prev, j_prev, i_next, j_next;
    j_prev = j - 1;
    j_next = j + 1;
    i_next = i + 1;
    i_prev = i - 1;

    if (j == 0){
        j_prev = circles[i].size() - 1;
    }
    if (j == circles[i].size() - 1){
        j_next = 0;
    }
    if (i == 0){ //посмотреть только 2 плоскости
        return balance_norms(normal_for_face(circles[i][j], circles[i][j_next], circles[i_next][j_next]), \
                            normal_for_face(circles[i][j_prev], circles[i][j], circles[i_next][j]));
    }
    if (i == circles.size() - 1){  //посмотреть только 2 плоскости
        return balance_norms(normal_for_face(circles[i_prev][j_prev], circles[i_prev][j], circles[i][j]), \
                            normal_for_face(circles[i_prev][j], circles[i_prev][j_next], circles[i][j_next]));
    }
    return balance_norms(normal_for_face(circles[i_prev][j_prev], circles[i_prev][j], circles[i][j]), \
                            normal_for_face(circles[i_prev][j], circles[i_prev][j_next], circles[i][j_next]), \
                            normal_for_face(circles[i][j], circles[i][j_next], circles[i_next][j_next]), \
                            normal_for_face(circles[i][j_prev], circles[i][j], circles[i_next][j]));
}

class Tube {
    Point center_big;
    float height_deg{};
    float height{};
    float circle_deg{};
    float radius_low{};
    float radius_high{};

    float xrot = 0;
    float yrot = 0;
    float zrot = 0;

    float tw{};
    float dtw{};

    vector<vector<Point>> circles;


    void set_rotation() const{
        // Вращение при изменении пользователем значений rotate_x и rotate_y
        glRotatef(this->xrot, 1.0, 0.0, 0.0 );
        glRotatef(this->yrot, 0.0, 1.0, 0.0 );
        glRotatef(this->zrot, 0.0, 0.0, 1.0 );
    }

    static void set_scale(){
        glScalef(scale_x, scale_y, scale_z);
    }

public:

    Tube()=default;

    Tube(Point cb, float hd, float h, float cd, float rl, float rh, float tw, float dtw){
        height_deg = hd;
        height = h;
        center_big = cb;
        circle_deg = cd;
        radius_low = rl;
        radius_high = rh;
        this->tw = tw;
        this->dtw = dtw;

        for (int i = 0; i < height_deg; i++) {
            Point center_small = Point(center_big.x + radius_high * cos(2 * M_PI * i / height_deg),\
            center_big.y + radius_high * sin(2 * M_PI * i / height_deg), center_big.z + height / height_deg * i);
            vector<Point> circle;
            for (int j = 0; j < circle_deg + 1; j++) {
                Point next_point = Point(center_small.x + radius_low * cos(2 * M_PI * j / circle_deg),\
                center_small.y + radius_low * sin(2 * M_PI * j / circle_deg), center_small.z);
                circle.push_back(next_point);
            }
            circles.push_back(circle);
            circle.clear();
        }
    }

    void set_rotation_vars(float a, float b, float c){
        this->xrot += a;
        this->yrot += b;
        this->zrot += c;
    }

    void set_rotation_vars_const(float a, float b, float c){
        xrot = a;
        yrot = b;
        zrot = c;
    }

    Point get_rotation_vars() const{
        return {xrot, yrot, zrot};
    }

    void set_twin(float a){
        tw = a;
    }

    Point get_center(){
        return center_big;
    }

    Tube get_tube_with_new_center(Point cb) const{
        auto t1 = Tube(cb, height_deg, height, circle_deg, radius_low, radius_high, tw + dtw, dtw);
        t1.set_rotation_vars(xrot, yrot, zrot);
        return t1;
    }

    pair<float, float> get_degrees(){
        return {height_deg, circle_deg};
    }

    float get_twin() const{
        return tw;
    }

    Point bese() const{
        Point c;
        Point p1, p2, p3, p4;
        p1 = {1, 1, 0};
        p2 = {-1, -1, 0};
        p3 = {-1, -1, 1};
        p4 = {1, 1, 1};

        p1 = p1 * (1 - tw) * (1 - tw) * (1 - tw);
        p2 = p2 * 3 * (1 - tw) * (1 - tw) * tw;
        p3 = p3 * 3 * (1 - tw) * tw * tw;
        p4 = p4 * tw * tw * tw;
        c.x = p1.x + p2.x + p3.x + p4.x;
        c.y = p1.y + p2.y + p3.y + p4.y;
        c.z = p1.z + p2.z + p3.z + p4.z;
        return c;
    }

    Tube twin(){
        if (tw >= 1){
            dtw = -timer;
        }
        if (tw <= 0){
            dtw = timer;
        }
        auto t1 = Tube(bese(), height_deg, height, circle_deg, radius_low, radius_high, tw + dtw, dtw);
        t1.set_rotation_vars(xrot, yrot, zrot);
        return t1;
    }

    Tube set_height_degree(float a){
        auto t1 = Tube(center_big, height_deg + a, height, circle_deg, radius_low, radius_high, tw, dtw);
        t1.set_rotation_vars(xrot, yrot, zrot);
        return t1;
    }

    Tube set_circle_degree(float a){
        auto t1 = Tube(center_big, height_deg, height, circle_deg + a, radius_low, radius_high, tw, dtw);
        t1.set_rotation_vars(xrot, yrot, zrot);
        return t1;
    }

    void draw(){
        set_rotation();
        set_scale();
        glLineWidth(3.0f);
        Vec a;
        pair<float, float> tc;
        int cs = circles.size();
        int cis = circles[0].size();
        for (int i = 0; i < circles.size() - 1; i++){
            for (int j = 0; j < circles[i].size() - 1; j++) {
                glColor3f(0.5f, 0.3f, 0.7f);
                glBegin(GL_POLYGON);

                tc = tex_coord(cs, cis, i, j);
//                cout << tc.first << " " << tc.second << endl;
                glTexCoord2f(tc.first, tc.second);
                a = normal_for_vertex(circles, i, j);
                glNormal3f(a.x, a.y, a.z);
                glVertex3f(circles[i][j].x, circles[i][j].y, circles[i][j].z);

                tc = tex_coord(cs, cis, i, j + 1);
//                cout << tc.first << " " << tc.second << endl;
                glTexCoord2f(tc.first, tc.second);
                a = normal_for_vertex(circles, i, j + 1);
                glNormal3f(a.x, a.y, a.z);
                glVertex3f(circles[i][j + 1].x, circles[i][j + 1].y, circles[i][j + 1].z);

                tc = tex_coord(cs, cis, i + 1, j + 1);
//                cout << tc.first << " " << tc.second << endl;
                glTexCoord2f(tc.first, tc.second);
                a = normal_for_vertex(circles, i + 1, j + 1);
                glNormal3f(a.x, a.y, a.z);
                glVertex3f(circles[i + 1][j + 1].x, circles[i + 1][j + 1].y, circles[i + 1][j + 1].z);

                tc = tex_coord(cs, cis, i + 1, j);
//                cout << tc.first << " " << tc.second << endl;
                glTexCoord2f(tc.first, tc.second);
                a = normal_for_vertex(circles, i + 1, j);
                glNormal3f(a.x, a.y, a.z);
                glVertex3f(circles[i + 1][j].x, circles[i + 1][j].y, circles[i + 1][j].z);

//                cout << endl;
                glEnd();
            }
        }
    }

};

Tube t;

void download_scene(const char *path){
    ifstream file(path, ios::in);
    Point a;
    file >> a.x >> a.y >> a.z;
    t = t.get_tube_with_new_center(a);

    file >> a.x >> a.y >> a.z;
    t.set_rotation_vars_const(a.x, a.y, a.z);
    pair<float, float> b;
    file >> b.first >> b.second;
    t = t.set_height_degree(b.first - t.get_degrees().first);
    t = t.set_circle_degree(b.second - t.get_degrees().second);
    file >> b.first;
    t.set_twin(b.first);
    file >> scale_x >> scale_y >> scale_z;
    unsigned int id;
    file >> id;
    text_id = id;
    int f;
    file >> f;
    polygon = f != 0;
    file >> f;
    is_light = f != 0;
    file >> f;
    is_material = f != 0;
    file >> f;
    is_textured = f != 0;
    file >> f;
    is_twining = f != 0;

    file.close();
}


void upload_scene(const char *path) {
    ofstream file(path, ios::out);
    Point p = t.get_center();
    file << p.x << endl << p.y << endl << p.z << endl;
    auto a = t.get_rotation_vars();
    file << a.x << endl << a.y << endl << a.z << endl;
    auto b = t.get_degrees();
    file << b.first << endl << b.second << endl;
    file << t.get_twin() << endl;
    file << scale_x << endl << scale_y << endl << scale_z << endl;
    file << text_id << endl;
    if (polygon){
        file << 1;
    } else {
        file << 0;
    }
    file << endl;
    if (is_light){
        file << 1;
    } else {
        file << 0;
    }
    file << endl;
    if (is_material){
        file << 1;
    } else {
        file << 0;
    }
    file << endl;
    if (is_textured){
        file << 1;
    } else {
        file << 0;
    }
    file << endl;
    if (is_twining){
        file << 1;
    } else {
        file << 0;
    }
    file.close();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) { //set new viewport
    glViewport(0, 0, width - 100, height - 100);
}

GLFWwindow* init() {
    if (!glfwInit())
        exit(-1);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "lab6", nullptr, nullptr);
    if (!window) {
        cerr << "Unable to open the window" << endl;
        glfwTerminate();
        exit(-1);
    }
    return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
        polygon = !polygon;
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_I) {
        text_id++;
        if (text_id > 2){
            text_id = 0;
        }
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_T) {
        is_twining = !is_twining;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_1) {
        t = t.set_circle_degree(2);
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_L) {
        is_light = !is_light;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_M) {
        is_material = !is_material;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_5) {
        is_textured = !is_textured;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_6) {
        upload_scene("scene_file.txt");
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_7) {
        download_scene("scene_file.txt");
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_2) {
        t = t.set_height_degree(2);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, true);
    }
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        t.set_rotation_vars(0.7, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        t.set_rotation_vars(-0.7, 0, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        t.set_rotation_vars(0, 0.7, 0);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        t.set_rotation_vars(0, -0.7, 0);
    }

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        per_x += 0.05;
        cout << "Perspective X: " << per_x << endl;
    }

    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        per_y += 0.05;
        cout << "Perspective Y: " << per_y << endl;
    }

    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        per_z += 0.05;
        cout << "Perspective Z: " << per_z << endl;
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        scale_x += 0.05;
        scale_y += 0.05;
        scale_z += 0.05;
    }
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        scale_x -= 0.05;
        scale_y -= 0.05;
        scale_z -= 0.05;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        t.set_rotation_vars(0.7, 0, 0);
    }
}

void draw_prepare(){
    glClearColor(0, 0, 0.1, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Сброс трансформаций
    glLoadIdentity(); // если выключить, то нажатие на клавиши - ускорение (скорость поворотов суммируется)
    glEnable(GL_DEPTH_TEST);                                                                                            // если выключить, непонятно что за чем находится
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
}

void light(){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat amb[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat spe[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat pos[] = {1.0, 1.0, 1.3, 0.5};
    GLfloat exp[] = {120};
    GLfloat lin[] = {0.1};

    glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, exp);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lin);
}

void material(){
    GLfloat amb[] = {0.8, 0.7, 0.8, 0.7};
    GLfloat dif[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat shi[] = {85.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT, GL_SHININESS, shi);
}

void perspective(bool f) {
    float matr[16];
    if (f) {
        float matr[16] = {
                1, 0, 0, per_x == 0 ? 0 : 1 / per_x,
                0, 1, 0, per_y == 0 ? 0 : 1 / per_y,
                0, 0, 1, per_z == 0 ? 0 : 1 / per_z,
                0, 0, 0, 1
        };
    } else {
        float matr[16] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1
        };
    }
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matr);
    glMatrixMode(GL_MODELVIEW);
}

void polygonize(bool f){
    if (f) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void draw(){
    draw_prepare();
    perspective(true);
    polygonize(polygon);
    light();
    load_texture(static_cast<const char*>(tex_path[text_id]));
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    if (is_material) {
        material();
    }
    if(is_light) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
    if(is_textured){
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    if (is_twining){
        t = t.twin();
    }
    glOrtho(-1, 1, -1, 1, -1, 1);
//    float I = Ia * ka + Il / (d + K) * (kd * (n * L) + ks * (R * S));
//    Ia - интенсивность рассеянного света
//    ka - коэффициент диффузного отражения рассеянного света
//    Il - интенсивность точечного источника
//    d - ?
//    K - произвольная постоянная
//    kd - коэффициент диффузного отражения
//    n - единичный вектор нормали к грани
//    L - единичный вектор направления к источнику света
//    ks - произвольная постоянная
//    R - вектор отражения
//    S - вектор наблюдения
    t.draw();
}


int main() {
    GLFWwindow* window = init();
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window); //make this window current
    glViewport(0, 0, WIDTH, HEIGHT); // set default viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //set new viewport sizes when user changes window sizes

    t = Tube(Point(-0.1, -0.3, -0.5), 5, 1, 5, 0.2, 0.4, 0.0, 0.01);

    download_scene("scene_file.txt");
    while(!glfwWindowShouldClose(window)) // rendering cycle
    {
        auto start = chrono::steady_clock::now();
        processInput(window);
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
        timer = 1.0 / chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() / 5;
    }
    upload_scene("scene_file.txt");
    glfwTerminate();

    return 0;
}