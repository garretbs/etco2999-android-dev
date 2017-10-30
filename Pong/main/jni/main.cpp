
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PongBall.h"

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
#include "PongBall.h"
#include "PongPaddle.h"
#include "PlayerPaddle.h"

#define TOUCH_START 0
#define PLAYING 1
#define VICTORY 2

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
	JavaHandler* jh = new JavaHandler();
	
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
	
	sf::SoundBuffer touchSeBuffer;
	sf::Sound touchSe;
	touchSe.setBuffer(touchSeBuffer);
	if(!touchSeBuffer.loadFromFile("ding.ogg")){
	return EXIT_FAILURE;
	}
	
	sf::Music music;
	if (!music.openFromFile("Andromeda.ogg")) {
		return EXIT_FAILURE;
	}
    music.setLoop(true);

    sf::SoundBuffer jingleBuffer;
	sf::Sound jingle;
	jingle.setBuffer(jingleBuffer);
	if(!jingleBuffer.loadFromFile("jingle.ogg")){
		return EXIT_FAILURE;
	}

    const char scores[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    sf::Font font_;
    if(!font_.loadFromFile("sansation.ttf")){
        return EXIT_FAILURE;
    }
    sf::Text pScoreText;
    sf::Text cpuScoreText;

    sf::View view = window.getDefaultView();
    int windowWidth = view.getSize().x;
    int windowHeight = view.getSize().y;

    int size = windowHeight/36;
    int velocity = size>>1;

    PongBall ball(size>>1);
    ball.goTo(windowWidth>>1, windowHeight>>1);

    PongPaddle cpuPaddle;
    cpuPaddle.setSize(size, windowHeight/3);
    cpuPaddle.setYRange(0, windowHeight);
    cpuPaddle.goTo(windowWidth-size, (windowHeight-cpuPaddle.height)>>1);
    cpuPaddle.setVelocity(velocity*0.9);

    PlayerPaddle playerPaddle;
    playerPaddle.setSize(size, windowHeight/3);
    playerPaddle.setYRange(0, windowHeight);
    playerPaddle.goTo(0, (windowHeight-playerPaddle.height)>>1);
    playerPaddle.setVelocity(velocity);


    int playerScore = 0;
    pScoreText.setFont(font_);
    pScoreText.setPosition(windowWidth>>2, 0);
    pScoreText.setCharacterSize(size<<1);
    pScoreText.setString(scores[playerScore]);


    int cpuScore = 0;
    cpuScoreText.setFont(font_);
    cpuScoreText.setPosition(windowWidth*0.75, 0);
    pScoreText.setCharacterSize(size<<1);
    cpuScoreText.setString(scores[cpuScore]);


    int state = TOUCH_START;
    sf::Vector2i touchPos = playerPaddle.getPosition();

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
                    windowWidth = view.getSize().x;
                    windowHeight = view.getSize().y;
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0) {
                        if(state == TOUCH_START){
                            state = PLAYING;
							music.play();
                            ball.start(windowWidth>>1, windowHeight>>1, rand() % 2 == 0 ? velocity : -velocity, rand() % 2 == 0 ? velocity : -velocity);
                        }else if(state == PLAYING){
                            //move player paddle (with a finite velocity)
                            //or by player drag?!
                            touchPos = sf::Vector2i(event.touch.x, event.touch.y);
                      }
                    }
                    break;
                case sf::Event::TouchMoved:
                    if(state == PLAYING){
                        touchPos = sf::Vector2i(event.touch.x, event.touch.y);
                    }
                    break;
            }

      }
        if(state == PLAYING && sf::Touch::isDown(0)){
            playerPaddle.move(touchPos.y > windowHeight>>1);
        }
      ball.move();
      if(ball.bounceY(0, windowHeight)){
          touchSe.play();
      }else if(ball.scoreX(0, windowWidth)){
		  music.stop();
          jingle.play();

          if(ball.getPosition().x <= 0){
              cpuScoreText.setString(scores[++cpuScore]);
              if(cpuScore == 10){
                  return EXIT_SUCCESS;
              }
          }else if (ball.getPosition().x >= windowWidth){
              pScoreText.setString(scores[++playerScore]);
              if(playerScore == 10){
                  return EXIT_SUCCESS;
              }
          }

          state = TOUCH_START;
          ball.goTo(windowWidth>>1, windowHeight>>1);
          cpuPaddle.goTo(windowWidth-size, (windowHeight-cpuPaddle.height)>>1);
          playerPaddle.goTo(0, (windowHeight-playerPaddle.height)>>1);

      }else if (playerPaddle.bounceBall(ball.getPosition(), ball.getVelocity().x, size)){
          ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y); //change velocity based on y position difference
          touchSe.play();
      }else if (cpuPaddle.bounceBall(ball.getPosition(), ball.getVelocity().x, size)){
          ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y); //change velocity based on y position difference
          touchSe.play();
      }else if(state == PLAYING){
          if(ball.getPosition().x > windowWidth>>1) cpuPaddle.move(ball.getPosition().y > cpuPaddle.getPosition().y);
      }

        window.clear(sf::Color::Black);
        window.draw(ball.sprite);
        window.draw(playerPaddle.sprite);
        window.draw(cpuPaddle.sprite);
        window.draw(cpuScoreText);
        window.draw(pScoreText);

        window.display();
  }

  delete jh;
}