#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include "Point.h"
#include "Fruit.h"
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <sstream>


using namespace std;

int boundsSet = 0;
int screenWidth = 1366, screenHeight = 768;
bool gameRunning = false;
int numFruit = 6;
vector<Fruit*> fruit;
double armX = 0, armY = 0, sordX = 1000, sordY = 1000;
double width, height;
double fruitTime = 2.0;
int score = 0, level = 1, fruitsKilled = 0, fails = 0;

class DataCollector: public myo::DeviceListener {
public:

	DataCollector(): onArm(true), isUnlocked(true), roll_r(0), pitch_y(0), yaw_x(0), currentPose() {}

	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		roll_r = 0;
		pitch_y = 0;
		yaw_x = 0;
		onArm = false;
		isUnlocked = true;
	}

	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
		using std::atan2;
		using std::asin;
		using std::sqrt;
		using std::max;
		using std::min;

		//float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()), 1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()), 1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

		//roll_r = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 18);
		//pitch_y = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 18);
		//yaw_x = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 18);
		armX = ((yaw + (float)M_PI) / (M_PI * 2.0f) * 18);
		armY = ((pitch + (float)M_PI / 2.0f) / M_PI * 18);
		if (gameRunning) {
			sordX = (screenWidth - ((armX / width)* screenWidth)) * 2;
			sordY = (screenHeight - ((armY / height)* screenHeight)) * 2;
		}
	}

	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
		currentPose = pose;
		if (boundsSet < 2 && currentPose == myo::Pose::fist) {
			if (boundsSet == 0) {
				width = armX;
				height = armY;
			} else if (boundsSet == 1) {
				width == yaw_x;
				height -= pitch_y;
				cout << "Height: " << height << endl;
				cout << "width: " << width << endl;
				gameRunning = true;
			}
			boundsSet++;
			cout << "set corner" << endl;
			myo->vibrate(myo::Myo::vibrationShort);
		} else {
			myo->unlock(myo::Myo::unlockHold);
		}
	}

	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection) {
		onArm = true;
		whichArm = arm;
	}

	void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
		onArm = false;
	}

	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = true;
	}

	void onLock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = false;
	}

	void print() {
		cout << '\r';
		cout << '[' << string(roll_r, '*') << string(18 - roll_r, ' ') << ']' << '[' << string(pitch_y, '*') << string(18 - pitch_y, ' ') << ']' << '[' << string(yaw_x, '*') << string(18 - yaw_x, ' ') << ']';
		if (onArm) {
			string poseString = currentPose.toString();
			cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']' << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']' << '[' << poseString << string(14 - poseString.size(), ' ') << ']';
		} else {
			cout << '[' << string(8, ' ') << ']' << "[?]" << '[' << string(14, ' ') << ']';
		}
		cout << flush;
	}

	bool onArm;
	myo::Arm whichArm;

	bool isUnlocked;

	int roll_r, pitch_y, yaw_x;
	myo::Pose currentPose;
};

int main(int argc, char** argv) {
	sf::RenderWindow* window(new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Myo-Fruit", sf::Style::None));
	window->setMouseCursorVisible(false);
	sf::View view(sf::FloatRect(0, 0, screenWidth, screenHeight));
	window->setView(view);
	sf::Font font;
	if (!font.loadFromFile("data/fonts/font.ttf"))
		cerr << "Could not load font" << endl;
	sf::SoundBuffer slashBuffer, splashBuffer, bombBuffer;
	if (!slashBuffer.loadFromFile("data/sounds/slash.wav"))
		cerr << "Could not load slash sound" << endl;
	if (!splashBuffer.loadFromFile("data/sounds/splash.wav"))
		cerr << "Could not load splash sound" << endl;
	if (!bombBuffer.loadFromFile("data/sounds/bomb.wav"))
		cerr << "Could not load bomb sound" << endl;
	sf::Sound slashSound, splashSound, bombSound;
	slashSound.setBuffer(slashBuffer);
	splashSound.setBuffer(splashBuffer);
	bombSound.setBuffer(bombBuffer);
	sf::Texture backgroundTexture, calDotTexture, pointerTexture, bombTexture1, bombTexture2, bombTexture3, xTexture;
	if (!backgroundTexture.loadFromFile("data/images/background.png"))
		cerr << "Could not load background" << endl;
	if (!calDotTexture.loadFromFile("data/images/calibrationDot.png"))
		cerr << "Could not load calibration Dot" << endl;
	if (!pointerTexture.loadFromFile("data/images/pointer.png"))
		cerr << "Could not load calibration Dot" << endl;
	if (!bombTexture1.loadFromFile("data/images/bomb1.png"))
		cerr << "Could not load bomb" << endl;
	if (!bombTexture2.loadFromFile("data/images/bomb2.png"))
		cerr << "Could not load bomb" << endl;
	if (!bombTexture3.loadFromFile("data/images/bomb3.png"))
		cerr << "Could not load bomb" << endl;
	if (!xTexture.loadFromFile("data/images/x.png"))
		cerr << "Could not load bombt" << endl;
	sf::Sprite background(backgroundTexture), calDot(calDotTexture), pointer(pointerTexture), x1Sprite(xTexture), x2Sprite(xTexture), x3Sprite(xTexture);
	vector<Point*> pointers;
	background.setScale(screenWidth / background.getLocalBounds().width, screenHeight / background.getLocalBounds().height);
	try {
		window->setFramerateLimit(0.f);
		myo::Hub hub("com.justin.myo-fruit-ninja");
		cout << "Attempting to find a Myo..." << endl;
		myo::Myo* myo = hub.waitForMyo(10000);
		if (!myo)
			throw runtime_error("Unable to find a Myo!");
		cout << "Connected to a Myo armband!" << endl << endl;
		myo->unlock(myo::Myo::unlockHold);
		DataCollector collector;
		sf::Time time;
		sf::Clock clock;
		vector<sf::Texture> fruitTextures;
		for (int i = 0; i < numFruit * 3; i++) {
			fruitTextures.push_back(sf::Texture());
			std::ostringstream oss;
			oss << i;
			string fileVal = "data/images/" + oss.str() + ".png";
			if (!fruitTextures[i].loadFromFile(fileVal))
				cerr << "Could not load fruit" << fileVal << endl;
		}
		hub.addListener(&collector);
		while (window->isOpen()) {
			hub.run(10);
			sf::Event event;
			while (window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window->close();
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window->close();
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) {
					gameRunning = !gameRunning;
					boundsSet = 0;
					for (size_t i = 0; i < fruit.size(); i++) {
						delete fruit[i];
					}
				}
			}
			window->draw(background);
			if (boundsSet == 0) {
				calDot.setPosition(0, 0);
				window->draw(calDot);
			} else if (boundsSet == 1) {
				calDot.setPosition(screenWidth - 50, screenHeight - 50);
				window->draw(calDot);
			} else {
				if (gameRunning) {
					if (clock.getElapsedTime().asSeconds() > fruitTime) {
						bool isBomb = rand() % 5 == 1;
						int type = (rand() % numFruit) * 3;
						Fruit *newFruit = new Fruit(isBomb, type, isBomb ? bombTexture1 : (fruitTextures[type]));
						newFruit->split = false;
						newFruit->dead = false;
						newFruit->setOrigin(newFruit->getOrigin().x + 37.5, newFruit->getOrigin().y + 37.5);
						fruit.push_back(newFruit);
						clock.restart();
						cout << "added fruit" << endl;
					}
					for (int i = 0; i < fruit.size(); i++) {
						fruit[i]->fly();
						for (int j = 0; j < pointers.size(); j++) {
							if (pointers[j] && !fruit[i]->dead && !fruit[i]->split && abs(fruit[i]->getPosition().x - pointers[j]->x) < 37.5 && abs(fruit[i]->getPosition().y - pointers[j]->y) < 37.5) {
								Fruit *newFruit = new Fruit(fruit[i]->isBomb, fruit[i]->type, fruit[i]->isBomb ? bombTexture2 : (fruitTextures[fruit[i]->type + 1]));
								newFruit->ySpeed = fruit[i]->ySpeed - 0.5;
								newFruit->setPosition(fruit[i]->getPosition());
								newFruit->setOrigin(newFruit->getOrigin().x + 37.5, newFruit->getOrigin().y + 37.5);
								newFruit->split = true;
								newFruit->dead = false;
								fruit.push_back(newFruit);
								Fruit *newFruit2 = new Fruit(fruit[i]->isBomb, fruit[i]->type, fruit[i]->isBomb ? bombTexture3 : (fruitTextures[fruit[i]->type + 2]));
								newFruit2->ySpeed = fruit[i]->ySpeed - 0.5;
								newFruit2->setPosition(fruit[i]->getPosition());
								newFruit2->setOrigin(newFruit2->getOrigin().x + 37.5, newFruit2->getOrigin().y + 37.5);
								newFruit2->split = true;
								newFruit2->dead = false;
								fruit.push_back(newFruit2);
								fruit[i]->dead = true;
								if (fruit[i]->isBomb) {
									myo->vibrate(myo::Myo::vibrationShort);
									bombSound.play();
									fails++;
									score -= 100 * level;
								} else {
									splashSound.play();
									fruitsKilled++;
									level = fruitsKilled % 10;
									fruitTime = 2 - (level*0.1);
									score += 50 * level;
								}
							}
						}
						if (!fruit[i]->dead && !fruit[i]->split && fruit[i]->getPosition().y > screenHeight && fruit[i]->ySpeed > 0) {
							if (!fruit[i]->isBomb)
								fails++;
							fruit[i]->dead = true;
						}
					}
				}
				if (gameRunning) {
					for (int i = 0; i < fruit.size(); i++) {
						if (fruit[i]->dead) {
							swap(fruit[i], fruit.back());
							fruit.pop_back();
							cout << "fruit gone" << endl;
						}
					}
				}
				if (gameRunning) {
					pointers.push_back(new Point(sordX, sordY));
					for (int i = 1; i < pointers.size(); i++) {
						if (pointers[i]) {
							sf::Vertex line[] = {sf::Vertex(sf::Vector2f(pointers[i - 1]->x, pointers[i - 1]->y)), sf::Vertex(sf::Vector2f(pointers[i]->x, pointers[i]->y)), };
							window->draw(line, 2, sf::Lines);
							if (i>25) {
								pointers.erase(pointers.begin());
							}
						}
					}
				}
			}
			for (int i = 0; i < fruit.size(); i++) {
				fruit[i]->draw(*window);
			}
			sf::Text levelText;
			levelText.setFont(font);
			std::ostringstream oss;
			oss << level;
			string levelWords = "level: " + oss.str();
			levelText.setString(levelWords);
			levelText.setCharacterSize(24);
			levelText.setColor(sf::Color::Cyan);
			levelText.setPosition(screenWidth - levelText.findCharacterPos(levelWords.length() - 1).x - levelText.findCharacterPos(0).x - 30, screenHeight - 88);
			window->draw(levelText);
			sf::Text scoreText;
			scoreText.setFont(font);
			std::ostringstream osss;
			osss << score;
			string scoreWords = "Score: " + osss.str();
			scoreText.setString(scoreWords);
			scoreText.setCharacterSize(24);
			scoreText.setColor(sf::Color::Cyan);
			scoreText.setPosition(screenWidth - scoreText.findCharacterPos(scoreWords.length() - 1).x - scoreText.findCharacterPos(0).x - 30, screenHeight - 44);
			window->draw(scoreText);

			x1Sprite.setPosition(screenWidth - 180, 10);
			x2Sprite.setPosition(screenWidth - 120, 10);
			x3Sprite.setPosition(screenWidth - 60, 10);
			if (fails > 0)
				window->draw(x1Sprite);
			if (fails > 1)
				window->draw(x2Sprite);
			if (fails > 2)
				window->draw(x3Sprite);
			if (fails > 3) {
				boundsSet = 0;
				gameRunning = false;
				int numFruit = 6;
				vector<Fruit*> fruit;
				armX = 0;
				armY = 0;
				sordX = 1000;
				sordY = 1000;
				score = 0;
				level = 1;
				fruitsKilled = 0;
				fails = 0;
			}
			window->setView(window->getDefaultView());
			window->display();
		}
	} catch (exception& e) {
		gameRunning = false;
		cerr << "Error: " << e.what() << endl;
		cerr << "Press enter to continue.";
		cin.ignore();
	}
	for (size_t i = 0; i < fruit.size(); i++) {
		delete fruit[i];
	}
	delete window;
	return 0;
}