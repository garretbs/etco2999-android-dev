
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "SFMLHack/DrawableStack.h"
#include "SFMLHack/SoundStreamer.h"
#include <time.h>

// Do we want to showcase direct JNI/NDK interaction?
// Undefine this to get real cross-platform code.
#define USE_JNI

#if defined(USE_JNI)
// These headers are only needed for direct NDK/JDK interaction
#include <jni.h>
#include <android/native_activity.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>

#include <android/log.h>
#include <fstream>

class JavaHandler
{
public:
    JavaHandler()
    {
        activity = sf::getNativeActivity();
        vm = activity->vm;
        env = activity->env;
        JavaVMAttachArgs attachargs;
        attachargs.version = JNI_VERSION_1_6;
        attachargs.name = "NativeThread";
        attachargs.group = NULL;
        jint res = vm->AttachCurrentThread(&env, &attachargs);
    }
    ~JavaHandler()
    {
        vm->DetachCurrentThread();
    }
    void vibrate(sf::Time duration)
    {
        // Retrieve class information
        jclass natact = env->FindClass("android/app/NativeActivity");
        jclass context = env->FindClass("android/content/Context");

        // Get the value of a constant
        jfieldID fid = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
        jobject svcstr = env->GetStaticObjectField(context, fid);

        // Get the method 'getSystemService' and call it
        jmethodID getss = env->GetMethodID(natact, "getSystemService",
                                           "(Ljava/lang/String;)Ljava/lang/Object;");
        jobject vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);

        // Get the object's class and retrieve the member name
        jclass vib_cls = env->GetObjectClass(vib_obj);
        jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V");

        // Determine the timeframe
        jlong length = duration.asMilliseconds();

        // Bzzz!
        env->CallVoidMethod(vib_obj, vibrate, length);

        // Free references
        env->DeleteLocalRef(vib_obj);
        env->DeleteLocalRef(vib_cls);
        env->DeleteLocalRef(svcstr);
        env->DeleteLocalRef(context);
        env->DeleteLocalRef(natact);
    }
    /* to be inserted into the JavaHandler class inside main.cpp */
    void test()
    {
        // Retrieve class information
        jobject nativeActivity = activity->clazz;
        jclass acl = env->GetObjectClass(nativeActivity);
        jmethodID getClassLoader = env->GetMethodID(acl, "getClassLoader", "()Ljava/lang/ClassLoader;");
        jobject cls = env->CallObjectMethod(nativeActivity, getClassLoader);
        jclass classLoader = env->FindClass("java/lang/ClassLoader");
        jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        jstring strClassName = env->NewStringUTF("sfml/com/sfml_dummy/Test");
        jclass testClass = (jclass)(env->CallObjectMethod(cls, findClass, strClassName));
        env->DeleteLocalRef(strClassName);

        // Get the value of a constant
        jmethodID  method = env->GetStaticMethodID(testClass,"print","()Ljava/lang/String;");
        jobject object = env->CallStaticObjectMethod(testClass,method);

        // Free references
        env->DeleteLocalRef(acl);
        env->DeleteLocalRef(classLoader);
        env->DeleteLocalRef(testClass);
        env->DeleteLocalRef(object);
        env->DeleteLocalRef(cls);
    }
    void getRandomData(int sampleSize, short* samples)
    {
        // Retrieve class information
        jobject nativeActivity = activity->clazz;
        jclass acl = env->GetObjectClass(nativeActivity);
        jmethodID getClassLoader = env->GetMethodID(acl, "getClassLoader", "()Ljava/lang/ClassLoader;");
        jobject cls = env->CallObjectMethod(nativeActivity, getClassLoader);
        jclass classLoader = env->FindClass("java/lang/ClassLoader");
        jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        jstring strClassName = env->NewStringUTF("sfml/com/sfml_dummy/Test");
        jclass testClass = (jclass)(env->CallObjectMethod(cls, findClass, strClassName));
        env->DeleteLocalRef(strClassName);

        // Get the value of a constant
        jmethodID method = env->GetStaticMethodID(testClass,"randomSound","()[S");
        jshortArray object = (jshortArray) env->CallStaticObjectMethod(testClass,method); //what's wrong here?

        jsize len = env->GetArrayLength(object);
        jshort* bob = env->GetShortArrayElements(object, false);

        //copy return object to a nice c++ array
        for(int i=0;i<len;i++){
            samples[i] = bob[i];
        }

        env->ReleaseShortArrayElements(object, bob, 0);

        // Free references
        env->DeleteLocalRef(acl);
        env->DeleteLocalRef(classLoader);
        env->DeleteLocalRef(testClass);
        env->DeleteLocalRef(object);
        env->DeleteLocalRef(cls);
    }
private:
    ANativeActivity* activity;
    JavaVM* vm;
    JNIEnv* env;
};

#endif

bool saveAndSend(sf::Image* image){

    //BIG ISSUE: cannot save file. not sure why this happens
    /*if(!image->saveToFile("drawing.png")){
        return EXIT_FAILURE;
    }
    int bob = 1/0;
    return true;*/


    /*const int width = image->getSize().x;
    const int height = image->getSize().y;
    unsigned short bmp[width*height];
    int i = 0;

    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            bmp[i++] = 0xff00 | image->getPixel(x, y).r;
        }
    }
    i++;
    i <<= 1;*/
    sf::TcpSocket socket;
    socket.connect("206.21.94.253", 1101);
    unsigned int size = image->getSize().x * image->getSize().y * 4;
    int width = image->getSize().x;
    int height = image->getSize().y;
    //socket.send(&size, 4);
    int i=0;
    unsigned char data[width*height];
    for (int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            data[i++] = image->getPixel(x, y).r == 0 ? 'B' : 'W';
        }
    }
    socket.send(&width, 4);
    socket.send(&height, 4);
    socket.send(data, width*height);
    //socket.send(image->getPixelsPtr(), 1024);
    //socket.send(&i, 4);
    //socket.send(bmp, i);
    socket.disconnect();
    return true;
}

int main(int argc, char *argv[]) {
    DrawableStack* myDrawableStack = new DrawableStack();
    JavaHandler* jh = new JavaHandler();

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    sf::Color fg = sf::Color::White;
    sf::Color bg = sf::Color::Black;

    sf::Image image;
    image.create(window.getSize().x, window.getSize().y-64, fg);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite(texture);
    myDrawableStack->addDrawable(&sprite, 2);

    sf::Font font_;
    if(!font_.loadFromFile("sansation.ttf")){
        return EXIT_FAILURE;
    }

    int fontSize = 32;
    sf::Text clearText;
    clearText.setFont(font_);
    clearText.setPosition(window.getSize().x>>1 + 48, window.getSize().y - fontSize - 4);
    clearText.setFillColor(fg);
    clearText.setCharacterSize(fontSize);
    clearText.setString("Clear");
    myDrawableStack->addDrawable(&clearText, 3);

    sf::Text sendText;
    sendText.setFont(font_);
    sendText.setPosition(fontSize, window.getSize().y - fontSize - 4);
    sendText.setFillColor(fg);
    sendText.setCharacterSize(fontSize);
    sendText.setString("Save & send");
    myDrawableStack->addDrawable(&sendText, 3);

    //sf::Sensor::setEnabled(sf::Sensor::Type::Gyroscope, true);

    sf::View view = window.getDefaultView();

    sf::Event event;
    sf::Vector2i lastTouch;
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width / 2, event.size.height / 2);
                    window.setView(view);
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0) {
                        if(event.touch.y > window.getSize().y-64){
                            if(event.touch.x > window.getSize().x>>1){
                                //clear screen
                                image.create(window.getSize().x, window.getSize().y-64, fg);
                            }else{
                                if(!saveAndSend(&image)){
                                    return EXIT_FAILURE;
                                }
                            }
                        }else{
                            lastTouch.x = event.touch.x;
                            lastTouch.y = event.touch.y;
                            for(int i=0;i<8;i++){
                                for(int j=0;j<8;j++){
                                    image.setPixel(event.touch.x+i, event.touch.y+j, bg);
                                }
                            }
                        }
                        texture.update(image);
                    }
                    break;
                case sf::Event::TouchMoved:
                    //check if touch was on the canvas or below it
                    //image.setPixel(event.touch.x, event.touch.y, bg);
                    if(event.touch.finger == 0) {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 8; j++) {
                                image.setPixel(event.touch.x + i, event.touch.y + j, bg);
                            }
                        }
                        texture.update(image);
                    }
                    break;
            }
        }
        window.clear(bg);
        window.draw(*myDrawableStack);
        window.display();

        //draw a pic through setpixel and loadimage, send through network, have server save as .png
        //there is a savetofile function in sfml, so hooray backups!

        //perhaps save as file, then send file itself as a byte stream, then save on desktop?
        //this would bypass having to implement png header stuff
    }

    delete myDrawableStack;
    delete jh;
    return 0;
}

int radioMain(int argc, char *argv[]) {
    DrawableStack* myDrawableStack = new DrawableStack();
    JavaHandler* jh = new JavaHandler();

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    int sampleCount = 44100;
    short samples[sampleCount];
    jh->getRandomData(44100, samples);

    SoundStreamer* mySoundStreamer = new SoundStreamer(sampleCount);
    mySoundStreamer->setLoop(true);
    mySoundStreamer->passJniData(samples);
    mySoundStreamer->play();

    //sf::Sensor::setEnabled(sf::Sensor::Type::Gyroscope, true);

    sf::View view = window.getDefaultView();

    sf::Event event;
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width / 2, event.size.height / 2);
                    window.setView(view);
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0) {
                        jh->getRandomData(44100, samples);
                    }
                    break;
            }
        }
        window.clear(sf::Color::White);
        window.draw(*myDrawableStack);
        window.display();

        //Todo: network service discovery, feed data from java side, stream mic audio
        //figure out why the hell this lags so much when you go to app view
    }

    delete mySoundStreamer;
    delete myDrawableStack;
    delete jh;
    return 0;
}