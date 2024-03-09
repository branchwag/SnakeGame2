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
        deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
        Vector2 direction = {1,0};

        void Draw() {

            for(unsigned int i =0; i < body.size(); i++){

                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        void Update() {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }

};

class Food {

    public:
    Vector2 position;
    Texture2D texture;
    
    Food() {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        //DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomPos() {

        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

};

class Game {
    public: 
    Snake snake = Snake();
    Food food = Food(); 

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update () {
        snake.Update();
    }
};

int main () {

    cout << "Starting the game" << endl;
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
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

        }
        
        //KEY_DOWN or S
        if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1) {

            game.snake.direction = {0, 1};
    
        }

        //KEY_LEFT or A
        if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1) {

            game.snake.direction = {-1, 0};
    
        }

        //KEY_RIGHT or D
        if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1) {

            game.snake.direction = {1, 0};
    
        }

        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}