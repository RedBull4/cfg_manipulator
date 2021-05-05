#include <string.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "cfg_manipulator.hpp"

using namespace std;

float movement_speed = 0;

map<string, sf::Vector2i> positions;

cfg_manipulator::cfg_file settings;

sf::RenderWindow window;
sf::Event event;
sf::RectangleShape character;

struct window_settings {
	sf::Vector2u size;
	sf::Vector2i position;
	string title;
} window_settings;

void controller() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		character.move(sf::Vector2f(-movement_speed, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		character.move(sf::Vector2f(movement_speed, 0));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		character.move(sf::Vector2f(0, -movement_speed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		character.move(sf::Vector2f(0, movement_speed));
}

int main() {
	settings.open("settings.cfg");

	window_settings.size.x = atoi(settings.read("window", "width"));
	window_settings.size.y = atoi(settings.read("window", "height"));
	window_settings.title = settings.read("window", "title");

	positions.insert(pair<string, sf::Vector2i>(
		"CENTERED", sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2
									 - window_settings.size.x / 2,
								 sf::VideoMode::getDesktopMode().height / 2
									 - window_settings.size.y / 2)));

	window_settings.position
		= positions.at(settings.read("window", "position"));

	window.create(sf::VideoMode(window_settings.size.x, window_settings.size.y),
				  window_settings.title, sf::Style::Close);
	window.setPosition(window_settings.position);

	if (strcmp(settings.read("window", "vertical_sync"), "true") == 0)
		window.setVerticalSyncEnabled(true);
	else if (strcmp(settings.read("window", "vertical_sync"), "false") == 0)
		window.setVerticalSyncEnabled(false);

	character.setSize(sf::Vector2f(atoi(settings.read("character", "width")),
								   atoi(settings.read("character", "height"))));
	character.setPosition(sf::Vector2f(atof(settings.read("character", "x")),
									   atof(settings.read("character", "y"))));
	character.setFillColor(sf::Color::Cyan);

	movement_speed = atof(settings.read("character", "movement_speed"));

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();

				settings.change_value(
					"character", "x",
					to_string(character.getPosition().x).c_str());
				settings.change_value(
					"character", "y",
					to_string(character.getPosition().y).c_str());
			}
		}

		controller();

		window.clear();

		window.draw(character);

		window.display();
	}
}