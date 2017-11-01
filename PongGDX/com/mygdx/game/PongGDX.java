package com.mygdx.game;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;

import java.util.Random;

public class PongGDX extends ApplicationAdapter {

	final int TOUCH_START = 0;
	final int PLAYING = 1;
	final int VICTORY = 2;

	SpriteBatch batch;
	Texture img;
	ShapeRenderer renderer;

	PongBall ball;
	PlayerPaddle playerPaddle;
	PongPaddle cpuPaddle;

	int windowWidth;
	int windowHeight;
	int size;
	int velocity;
	int state;
	int playerScore;
	int cpuScore;
	Random randy;
	
	@Override
	public void create () {
		renderer = new ShapeRenderer();

		windowWidth = Gdx.graphics.getWidth();
		windowHeight = Gdx.graphics.getHeight();
		size = windowHeight/36;
		velocity = size>>1;

		ball = new PongBall(size>>1);
		ball.goTo(windowWidth>>1, windowHeight>>1);

		cpuPaddle = new PongPaddle();
		cpuPaddle.setSize(size, windowHeight/3-size);
		cpuPaddle.setYRange(0, windowHeight);
		cpuPaddle.goTo(windowWidth-size, (windowHeight-cpuPaddle.height)>>1);
		cpuPaddle.setVelocity((int) (velocity*0.9));

		playerPaddle = new PlayerPaddle();
		playerPaddle.setSize(size, windowHeight/3-size);
		playerPaddle.setYRange(0, windowHeight);
		playerPaddle.goTo(0, (windowHeight-playerPaddle.height)>>1);
		playerPaddle.setVelocity(velocity);

		state = TOUCH_START;
		playerScore = 0;
		cpuScore = 0;
		randy = new Random();
	}

	@Override
	public void render () {

		switch(state){
			case TOUCH_START:
				if(Gdx.input.isTouched()){
					state = PLAYING;
					ball.start(windowWidth>>1, windowHeight>>1, randy.nextInt() % 2 == 0 ? velocity : -velocity, randy.nextInt() % 2 == 0 ? velocity : -velocity);
				}
				break;
			case PLAYING:
				if(Gdx.input.isTouched()){
					playerPaddle.move(Gdx.input.getY(0) < windowHeight>>1);
				}
				ball.move();
				ball.bounceY(0, windowHeight);
				if(ball.scoreX(0, windowWidth)) {
					if (ball.getPosition().x <= 0) {
						cpuScore++;
						if(cpuScore > 9){
							System.exit(1);
						}
					} else if (ball.getPosition().x >= windowWidth) {
						playerScore++;
						if(playerScore > 9){
							System.exit(0);
						}
					}
					state = TOUCH_START;
					ball.goTo(windowWidth>>1, windowHeight>>1);
					cpuPaddle.goTo(windowWidth-size, (windowHeight-cpuPaddle.height)>>1);
					playerPaddle.goTo(0, (windowHeight-playerPaddle.height)>>1);
				}else if (playerPaddle.bounceBall(ball.getPosition(), (int) ball.getVelocity().x, size)){
					ball.setVelocity((int) -ball.getVelocity().x, (int) ball.getVelocity().y);
				}else if (cpuPaddle.bounceBall(ball.getPosition(), (int) ball.getVelocity().x, size)){
					ball.setVelocity((int)-ball.getVelocity().x, (int) ball.getVelocity().y);
				}else if(state == PLAYING){
					if(ball.getPosition().x > windowWidth>>1) cpuPaddle.move(ball.getPosition().y > cpuPaddle.getPosition().y);
				}
				break;
			case VICTORY:
				break;
		}

		Gdx.gl.glClearColor(0, 0, 0, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);


		//draw pong ball
		renderer.begin(ShapeRenderer.ShapeType.Filled);
		renderer.circle(ball.getPosition().x+size, ball.getPosition().y, ball.radius);

		//draw player paddle
		renderer.rect(playerPaddle.getPosition().x, playerPaddle.getPosition().y, playerPaddle.width, playerPaddle.height);

		//draw cpu paddle
		renderer.rect(cpuPaddle.getPosition().x, cpuPaddle.getPosition().y, cpuPaddle.width, cpuPaddle.height);

		//draw score

		renderer.end();
	}
	
	@Override
	public void dispose () {
		renderer.dispose();
	}
}
