
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SFMLHack/DrawableStack.h"
#include "SFMLHack/SoundStreamer.h"
//#include <SFML/Network.hpp>

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

// This is the actual Android example. You don't have to write any platform
// specific code, unless you want to use things not directly exposed.
// ('vibrate()' in this example; undefine 'USE_JNI' above to disable it)
int main(int argc, char *argv[]) {
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