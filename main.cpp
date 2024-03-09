#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementinDeque(Vector2 element, deque<Vector2> deque) {

    for(unsigned int i = 0; i < deque.size(); i++) {

        if(Vector2Equals(deque[i], element)) {

            return true; 
        }
    }
    return false;
}

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake {
    
    public :
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;

        void Draw() {

            for(unsigned int i = 0; i < body.size(); i++){

                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        void Update() {

            body.push_front(Vector2Add(body[0], direction));

            if(addSegment == true) {
                
                addSegment = false;

            } else {
                body.pop_back();
            }

        }

        void Reset() {
            body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
            direction = {1,0};
        }

};

class Food {

    public:
    Vector2 position;
    Texture2D texture;
    
    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        //DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell(){

        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {

        Vector2 position = GenerateRandomCell();
        while(ElementinDeque(position, snakeBody)){

            position = GenerateRandomCell();
        }
        
    return position;
    }

};

class Game {

    public: 
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true; 

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update () {
        if(running) {
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood() {

        if(Vector2Equals(snake.body[0], food.position)) {

            //cout << "Food has been eaten!" << endl;
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
        };
    };

    void CheckCollisionWithEdges() {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1) {

            GameOver();

        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1) {

            GameOver();

        }
    }

    void GameOver() {
        //cout << "Game over" << endl;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;

        //can eventually add a game over window with a dotted line border
    }

    void CheckCollisionWithTail() {

        deque<Vector2> headlessBody = snake.body; 
        headlessBody.pop_front();
        if(ElementinDeque(snake.body[0], headlessBody)) {

            GameOver();
        }
    }
};

int main () {

    cout << "Starting the game" << endl;
    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    // Food food = Food();
    // Snake snake = Snake();
    Game game = Game();

    while(WindowShouldClose() == false) {

        BeginDrawing();

        if(EventTriggered(0.2)){
            game.Update();
        }

        //KEY_UP OR W
        if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1) {

            game.snake.direction = {0, -1};
            game.running = true;

        }
        
        //KEY_DOWN or S
        if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1) {

            game.snake.direction = {0, 1};
            game.running = true;
    
        }

        //KEY_LEFT or A
        if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1) {

            game.snake.direction = {-1, 0};
            game.running = true;
    
        }

        //KEY_RIGHT or D
        if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1) {

            game.snake.direction = {1, 0};
            game.running = true;
    
        }

        ClearBackground(green);

        game.Draw();
         DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}