

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    const int width = 800;
    const int height = 600;
    const int paddleW = 20;
    const int paddleH = 100;
    const int BallR = 10;
    float CurBallSpeed = 4.0f;
    const float paddleSpeed = 5.0f;
    int player1Score = 0;
    int player2Score = 0;
    int StartR = 1;

    float beginnerBallS = 3.0f;
    float intermediateBallS = 5.0f;
    float advancedBallS = 7.0f;

    sf::RenderWindow window(sf::VideoMode(width, height), "Pong");
    window.setFramerateLimit(60);

    // Load the background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg"))
    {
        // Error handling if the background texture fails to load
        return EXIT_FAILURE;
    }

    // Create the a sprite for the background
    sf::Sprite background(backgroundTexture);

    // Create the two paddles
    sf::RectangleShape player1Paddle(sf::Vector2f(paddleW, paddleH));
    player1Paddle.setFillColor(sf::Color::White);
    player1Paddle.setPosition(50, height / 2 - paddleH / 2);

    sf::RectangleShape player2Paddle(sf::Vector2f(paddleW, paddleH));
    player2Paddle.setFillColor(sf::Color::White);
    player2Paddle.setPosition(width - paddleW - 50, height / 2 - paddleH / 2);

    // Create the ball
    sf::CircleShape ball(BallR);
    ball.setFillColor(sf::Color::White);
    int w = width / 2;
    int h = height / 2;
    ball.setPosition(w - BallR, h - BallR);
    sf::Vector2f BallV(CurBallSpeed, CurBallSpeed);

    // Create the middle line
    sf::RectangleShape MiddleLine(sf::Vector2f(2, height));
    MiddleLine.setFillColor(sf::Color::White);
    MiddleLine.setPosition(width / 2 - 1, 0); // Position in the middle of the screen

    sf::Clock clock; // Clock for measuring elapsed time

    // Create game mode text
    sf::Font font;
    if (!font.loadFromFile("Nexa Light.otf"))
        return 1;

    sf::Text gameModeText("Select game mode:\n \n1. Solo with AI\n \n2. Two players", font, 35);
    gameModeText.setPosition(w - gameModeText.getGlobalBounds().width / 2, h - gameModeText.getGlobalBounds().height / 2);

    int gameMode = 0;
    bool ModeS = false;

    // Create difficulty level text
    sf::Text DifficultyLevelT("Select difficulty level:\n \n1. Beginner\n \n2. Intermediate\n \n3. Advanced", font, 35);
    DifficultyLevelT.setPosition(w - (DifficultyLevelT.getGlobalBounds().width / 2), h - (DifficultyLevelT.getGlobalBounds().height / 2));

    int difficultyLevel = 0;
    bool DLevelSelected = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered && !ModeS)
            {
                if (event.text.unicode == '1' || event.text.unicode == '2')
                {
                    gameMode = event.text.unicode - '0';
                    ModeS = true;
                }
            }
            else if (event.type == sf::Event::TextEntered && ModeS && !DLevelSelected)
            {
                if (event.text.unicode == '1' || event.text.unicode == '2' || event.text.unicode == '3')
                {
                    difficultyLevel = event.text.unicode - '0';
                    DLevelSelected = true;
                }
            }
        }

        if (!ModeS)
        {
            window.clear();
            window.draw(background);
            window.draw(gameModeText);
            window.display();
            continue;
        }

        if (ModeS && !DLevelSelected)
        {
            window.clear();
            window.draw(background);
            window.draw(DifficultyLevelT);
            window.display();
            continue;
        }

        // Move player paddles
        if (gameMode == 2) // Two players
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player1Paddle.getPosition().y > 0)
                player1Paddle.move(0, -paddleSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player1Paddle.getPosition().y < height - paddleH)
                player1Paddle.move(0, paddleSpeed);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player2Paddle.getPosition().y > 0)
                player2Paddle.move(0, -paddleSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player2Paddle.getPosition().y < height - paddleH)
                player2Paddle.move(0, paddleSpeed);
        }
        else if (gameMode == 1) // Solo with AI
        {
            float paddleCenterY = player2Paddle.getPosition().y + paddleH / 2;
            float ballCenterY = ball.getPosition().y + BallR;

            // Move player paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player1Paddle.getPosition().y > 0)
                player1Paddle.move(0, -paddleSpeed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player1Paddle.getPosition().y < height - paddleH)
                player1Paddle.move(0, paddleSpeed);

            // Move opponent paddle based on the selected difficulty level
            if (difficultyLevel == 1) // Beginner
            {

                if (ball.getPosition().y < paddleCenterY && player2Paddle.getPosition().y > 0)
                    player2Paddle.move(0, -paddleSpeed);
                if (ball.getPosition().y > paddleCenterY && player2Paddle.getPosition().y < height - paddleH)
                    player2Paddle.move(0, paddleSpeed);
            }
            else if (difficultyLevel == 2) // Intermediate
            {

                if (paddleCenterY < ballCenterY && player2Paddle.getPosition().y < height - paddleH)
                    player2Paddle.move(0, paddleSpeed);
                if (paddleCenterY > ballCenterY && player2Paddle.getPosition().y > 0)
                    player2Paddle.move(0, -paddleSpeed);
            }
            else if (difficultyLevel == 3) // Advanced
            {
                if (paddleCenterY < ballCenterY && player2Paddle.getPosition().y < height - paddleH)
                    player2Paddle.move(0, paddleSpeed + 2.5f);
                if (paddleCenterY > ballCenterY && player2Paddle.getPosition().y > 0)
                    player2Paddle.move(0, -paddleSpeed - 2.5f);
            }
        }


        // Move the ball
        if (BallV.x > 0 && BallV.y > 0) {
            ball.move(CurBallSpeed, CurBallSpeed);
        }
        else if (BallV.x > 0 && BallV.y < 0) {
            ball.move(CurBallSpeed, -1 * CurBallSpeed);
        }
        else if (BallV.x < 0 && BallV.y > 0) {
            ball.move(-1 * CurBallSpeed, CurBallSpeed);
        }
        else if (BallV.x < 0 && BallV.y < 0) {
            ball.move(-1 * CurBallSpeed, -1 * CurBallSpeed);
        }


        // Check collisions with the walls
        if (ball.getPosition().y < 0 || ball.getPosition().y > height - BallR * 2)
            BallV.y = -BallV.y;

        // When collisions happens with the two paddles, the speed of the ball increases
        if (ball.getGlobalBounds().intersects(player1Paddle.getGlobalBounds()) ||
            ball.getGlobalBounds().intersects(player2Paddle.getGlobalBounds()))
        {
            BallV.x = -BallV.x;
            if (StartR == 1) {
                if (difficultyLevel == 1) {
                    CurBallSpeed = beginnerBallS;
                }
                else if (difficultyLevel == 2) {
                    CurBallSpeed = intermediateBallS;
                }
                else if (difficultyLevel == 3) {
                    CurBallSpeed = advancedBallS;
                }
                StartR = 0;
            }
            CurBallSpeed += 1.0f;
            std::cout << CurBallSpeed;
        }

        // ocheck out of bounce
        if (ball.getPosition().x < 0)
        {
            player2Score++;
            ball.setPosition(w - BallR, h - BallR);
            // Reset the ball speed

            if (StartR == 1) {
                if (difficultyLevel == 1) {
                    CurBallSpeed = beginnerBallS;
                }
                else if (difficultyLevel == 2) {
                    CurBallSpeed = intermediateBallS;
                }
                else if (difficultyLevel == 3) {
                    CurBallSpeed = advancedBallS;
                }
                StartR = 0;
            }


            if (player2Score == 3)
            {
                sf::Text winnerText("Player 2 Won!", font, 48);
                winnerText.setPosition(w - winnerText.getGlobalBounds().width / 2, h - winnerText.getGlobalBounds().height / 2);
                window.draw(winnerText);
                window.display();
                sf::sleep(sf::seconds(3));
                return 0;
            }
        }
        else if (ball.getPosition().x > width - BallR * 2)
        {
            player1Score++;
            ball.setPosition(width / 2 - BallR, height / 2 - BallR);
            if (StartR == 1) {
                if (difficultyLevel == 1) {
                    CurBallSpeed = beginnerBallS;
                }
                else if (difficultyLevel == 2) {
                    CurBallSpeed = intermediateBallS;
                }
                else if (difficultyLevel == 3) {
                    CurBallSpeed = advancedBallS;
                }
                StartR = 0;
            }
            if (player1Score == 3)
            {
                sf::Text winnerT("Player 1 Won!", font, 48);
                winnerT.setPosition(w - winnerT.getGlobalBounds().width / 2, h - winnerT.getGlobalBounds().height / 2);
                window.draw(winnerT);
                window.display();
                sf::sleep(sf::seconds(3));
                return 0;
            }
        }

        // Clear the window
        window.clear();

        // Draw the background
        window.draw(background);

        // Draw the paddles
        window.draw(player1Paddle);
        window.draw(player2Paddle);

        // Draw the ball
        window.draw(ball);

        // Draw the middle line
        window.draw(MiddleLine);

        // Display the player scores
        sf::Text Player1ScoreT("Player 1: " + std::to_string(player1Score), font, 24);
        Player1ScoreT.setPosition(width / 2 - Player1ScoreT.getGlobalBounds().width - 10, 10);
        window.draw(Player1ScoreT);

        sf::Text Player2ScoreT("    Player 2: " + std::to_string(player2Score), font, 24);
        Player2ScoreT.setPosition(width / 2 + 10, 10);
        window.draw(Player2ScoreT);

        // Display the updated window
        window.display();
    }

    return 0;
}
