#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <raymath.h>
#include <unordered_set>

Color green = {170,200,90,255};
Color dgreen = {40,50,24,255};

int cellSize = 25;
int cellCount = 25;
double interval = 0.2;
double lastupdate = 0;
int offset = 50;
int score = 0;

bool EventTriggered(){
    double currentTime = GetTime();
    if(currentTime- lastupdate >= interval){
        lastupdate = currentTime;
        return true;
    }
    return false;

}

bool elementInDeque(Vector2 element, std::deque<Vector2> deque){
    for(unsigned int i=0;i<deque.size();i++){
        if(Vector2Equals(deque[i],element)){
            return true;
        }
    }
    return false;
}

class ResourceManager{

    public:

        class TextureResource{
            private:
                bool loaded;
                Texture2D texture;

            public:

                TextureResource(const std::string& imagePath){ //const is used to ensure that we dont change the path by accident and "&" is for pass by reference so we dont need to make a copy of the path
                    Image image = LoadImage(imagePath.c_str()); // converts the cpp string to a c string probably because of the restrictions of the LoadImage()??
                    if( image.data == nullptr){
                        std::cerr << "Failed to load image: " << imagePath << std::endl;
                        return;                        
                    }
                    texture = LoadTextureFromImage(image);
                    UnloadImage(image);
                    loaded = (texture.id != 0);
                    if(texture.id == 0)
                        std::cerr << "Failed to create texture from image: " << imagePath << std::endl;

                }

                ~TextureResource(){
                    if(loaded){
                        UnloadTexture(texture);
                    }
                }

                Texture2D get() const { return texture;} // to get the texture
                bool isLoaded() const { return loaded;} // to check if loaded or not
        };
        
};


class Snake{
    private:
            std::string positionKey(const Vector2& pos) const {
                return std::to_string(static_cast<int>(pos.x)) + "," + std::to_string(static_cast<int>(pos.y));
    }
    public:
        std::deque<Vector2> body;
        Vector2 direction;
        bool addSegment = false;
        std::unordered_set<std::string> headLessBody;

        Snake(){
            body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
            direction = Vector2{1,0};
            

        }
        void Draw(){
            for(unsigned int i=0;i<body.size();i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle rect = Rectangle{offset+ x*cellSize,offset+y*cellSize,(float)cellCount,(float)cellCount};
                DrawRectangleRounded(rect,0.8,6,dgreen);

            }

        }

        void Update(){
            body.push_front(Vector2Add(body[0],direction));            
            if (addSegment)
                addSegment=false;
            else
                body.pop_back();          
        }

        void reset(){
            float x = GetRandomValue(4,cellCount-4);
            float y = GetRandomValue(4,cellCount-4);
            body = {Vector2{x,y},Vector2{x+1,y},Vector2{x+2,y}};
        }
};


class Food{
    public:
        Vector2 position;
        ResourceManager::TextureResource texture = ResourceManager::TextureResource("resources/fud.png");;
        


    Food(std::deque<Vector2> snakeBody){
        position = GenerateRandomPos(snakeBody);                 
    }



    Vector2 GenerateRandomPos(std::deque <Vector2> snakeBody){
        float x = GetRandomValue(0,cellCount-1);    
        float y = GetRandomValue(0,cellCount-1);    
        Vector2 position = Vector2{x,y};
        if (elementInDeque(position,snakeBody)){
            return GenerateRandomPos(snakeBody);
        }
        else{
            return position;
        }
    }


    void Draw(){
        float scale = (static_cast<float>(cellSize) / texture.get().width)*1.6;
        float adjustment = -0.2;
        DrawTextureEx(texture.get(),{offset+(position.x+adjustment)*cellSize,offset+(position.y+adjustment)*cellSize},0.0f,scale,WHITE);
    }

};


class Game{
    public:
    Snake snake;
    Food food = Food(snake.body);
    bool running = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if(running){
            snake.Update();
            checkCollisionApple();
            checkCollisionEdges();
            checkCollisionSnake();
        }
    }

    void checkCollisionEdges(){
        if(snake.body[0].x==cellCount || snake.body[0].x==-1 || snake.body[0].y==cellCount || snake.body[0].y==-1){
            interval = 0.2;
            score = snake.body.size()-3;
            GameOver();
        }
    }

    void checkCollisionApple(){
        if (Vector2Equals(snake.body[0],food.position)){
            food.position = food.GenerateRandomPos(snake.body);
            interval*=0.99;
            snake.addSegment=true;
        }
    }

    void checkCollisionSnake(){
        std::deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(elementInDeque(snake.body[0],headlessBody)){
            interval = 0.2;
            score = snake.body.size()-3;
            GameOver();
        }
    }

    void GameOver(){
        snake.reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;

    }
};

int main() {
    InitWindow(2*offset +cellSize*cellCount,2*offset +   cellSize*cellCount,"Retro Snake");
    SetTargetFPS(60);
    Rectangle rect;
    rect.x = (float)(offset - 5);
    rect.y = (float)(offset - 5);
    rect.width = (float)(cellSize * cellCount + 10);
    rect.height = (float)(cellSize * cellCount + 10);
    bool buffer = true;

    Game game = Game();

    while(WindowShouldClose()==false){
        BeginDrawing();



        if(EventTriggered()) {
            game.Update();
            buffer = true;
        }

        if (buffer){
            if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y!=1){
                game.snake.direction = {0,-1};
                buffer = false;
            }
            if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y!=-1){
                game.snake.direction = {0,1};
                buffer = false;

            }
            if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x!=1){
                game.snake.direction = {-1,0};
                buffer = false;
            }        
            if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x!=-1){
                game.snake.direction = {1,0};
                buffer = false;
            }        

        }

        DrawRectangleLinesEx(rect,5,dgreen);

        if(game.running){
            DrawText("Snake Game",offset-5,14,28,dgreen);
            game.Draw();
        }       
        else{    
            DrawText("Game Over",(cellCount*cellSize)/2-50,(cellCount*cellSize)/2,40,dgreen);
            DrawText("Score :",(cellCount*cellSize)/2-20,((cellCount*cellSize)/2)+40,33,dgreen);
            DrawText(TextFormat("%d", score), (cellCount*cellSize)/2+120,((cellCount*cellSize)/2)+40, 33, dgreen);    
            DrawText("Press a playable key to continue",180,600,20,dgreen);        
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D)) {
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                    game.snake.direction = {-1,0};
                }
                else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    game.snake.direction = {0,-1};
                }
                else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {              
                    game.snake.direction = {0,1};
                }                
                else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                    game.snake.body={game.snake.body[2],game.snake.body[1],game.snake.body[0]};
                    game.snake.direction = {1,0};
                }         
                game.running=true;
            }
        }
        ClearBackground(green); 

        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}