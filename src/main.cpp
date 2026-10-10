#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

struct Ball {
  sf::CircleShape shape;
  sf::Vector2f velocity;
};

int main() {
  sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Bouncing Balls");
  window.setMinimumSize(sf::Vector2u{100u, 100u});

  std::vector<Ball> balls;
  std::mt19937 randomEngine(std::random_device{}());
  std::uniform_real_distribution<float> radiusDistribution(15.f, 40.f);

  constexpr float twoPi = 2.f * 3.141592f;
  std::uniform_real_distribution<float> angleDistribution(0.f, twoPi);
  std::uniform_int_distribution<int> colorDistribution(50, 255);

  sf::Clock clock;
  float spawnTimer = 0.f;
  constexpr float spawnInterval = 0.1f;

  while (window.isOpen()) {
    const float deltaTime = clock.restart().asSeconds();

    if (deltaTime > 0.f) {
      window.setTitle("Bouncing Balls, FPS: " +
                      std::to_string(static_cast<int>(1.f / deltaTime)));
    }

    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      spawnTimer += deltaTime;

      if (spawnTimer >= spawnInterval) {
        spawnTimer -= spawnInterval;

        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        const float radius = radiusDistribution(randomEngine);
        const float windowWidth = static_cast<float>(window.getSize().x);
        const float windowHeight = static_cast<float>(window.getSize().y);

        sf::CircleShape shape(radius);
        shape.setOrigin({radius, radius});
        shape.setFillColor(sf::Color(
            static_cast<std::uint8_t>(colorDistribution(randomEngine)),
            static_cast<std::uint8_t>(colorDistribution(randomEngine)),
            static_cast<std::uint8_t>(colorDistribution(randomEngine))));

        const float x = std::clamp(static_cast<float>(mousePosition.x), radius,
                                   windowWidth - radius);
        const float y = std::clamp(static_cast<float>(mousePosition.y), radius,
                                   windowHeight - radius);
        shape.setPosition({x, y});

        const float angle = angleDistribution(randomEngine);
        const float speed = 200.f;
        const sf::Vector2f velocity{
            std::cos(angle) * speed,
            std::sin(angle) * speed,
        };

        balls.push_back({shape, velocity});
      }
    } else {
      spawnTimer = spawnInterval;
    }

    const float windowWidth = static_cast<float>(window.getSize().x);
    const float windowHeight = static_cast<float>(window.getSize().y);

    for (Ball &ball : balls) {
      sf::Vector2f position = ball.shape.getPosition();
      const float radius = ball.shape.getRadius();

      position += ball.velocity * deltaTime;

      if (position.x - radius < 0.f) {
        position.x = radius;
        ball.velocity.x = std::abs(ball.velocity.x);
      } else if (position.x + radius > windowWidth) {
        position.x = windowWidth - radius;
        ball.velocity.x = -std::abs(ball.velocity.x);
      }

      if (position.y - radius < 0.f) {
        position.y = radius;
        ball.velocity.y = std::abs(ball.velocity.y);
      } else if (position.y + radius > windowHeight) {
        position.y = windowHeight - radius;
        ball.velocity.y = -std::abs(ball.velocity.y);
      }

      ball.shape.setPosition(position);
    }

    window.clear(sf::Color::Black);

    for (const Ball &ball : balls)
      window.draw(ball.shape);

    window.display();
  }
}
