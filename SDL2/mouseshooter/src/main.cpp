#include <SDL2/SDL.h>
#include <vector>

const double BULLET_SPEED = 5.0;
const int WIDTH = 640;
const int HEIGHT = 480;
const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 20;
const int BULLET_WIDTH = 5;
const int BULLET_HEIGHT = 5;

struct Player {
    double x;
    double y;
};

struct Bullet {
    double x;
    double y;
    double vx;
    double vy;
};
std::vector<Bullet> bullets;

Player player;
SDL_Renderer* renderer;
void shoot(int x, int y) {
//	printf("Shooting at x: %d, y: %d\n", x, y);
    // Get the angle to the mouse pointer
    double angle = atan2(y - player.y, x - player.x);
    if (angle < 0) {
    angle += 2 * M_PI;
}
  //  printf("Angle: %f\n", angle);
    // Set the bullet velocity
    double bulletVx = cos(angle) * BULLET_SPEED;
    double bulletVy = sin(angle) * BULLET_SPEED;
    // Create a new bullet
    Bullet bullet;
    bullet.x = player.x + PLAYER_WIDTH/2;
    bullet.y = player.y + PLAYER_HEIGHT/2;
    bullet.vx = bulletVx;
    bullet.vy = bulletVy;
    bullets.push_back(bullet);
}





/*
void shoot(int x, int y) {
    // Get the angle between the player's position and the mouse pointer
    double angle = atan2(y - player.y, x - player.x);

    // Create a bullet at the player's position
    Bullet bullet;
    bullet.x = player.x;
    bullet.y = player.y;

    // Set the bullet's velocity based on the angle
    bullet.vx = cos(angle) * BULLET_SPEED;
    bullet.vy = sin(angle) * BULLET_SPEED;

    // Add the bullet to the bullet list
    bullets.push_back(bullet);
}
*/
void update() {
    //update the player position based on user input
    //update the bullet position
    for (auto &bullet : bullets) {
        bullet.x += bullet.vx;
        bullet.y += bullet.vy;
    }
    // check for bullet-out-of-screen
for(size_t i = 0; i < bullets.size(); i++){
    if(bullets[i].x < 0 || bullets[i].x > WIDTH || bullets[i].y < 0 || bullets[i].y > HEIGHT) {
        bullets.erase(bullets.begin() + i);
    }
}




}

void render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the player
    SDL_Rect playerRect = { (int)player.x, (int)player.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerRect);

    // Draw the bullets
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto &bullet : bullets) {
        SDL_Rect bulletRect = { (int)bullet.x, (int)bullet.y, BULLET_WIDTH, BULLET_HEIGHT };
        SDL_RenderFillRect(renderer, &bulletRect);
    }

    // Show the rendered frame
    SDL_RenderPresent(renderer);
}


int main() {

	   player.x = WIDTH / 2;
    player.y = HEIGHT / 2;
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);
    // Main loop
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto end;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) 
	    {
		   // printf("Left mouse button clicked\n");
                // Get the mouse position
                // Get the mouse position
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Shoot in the direction of the mouse pointer
		
                shoot(x, y);
           
            }
        //update the bullet position
    for (auto &bullet : bullets) {
            bullet.x += bullet.vx;
            bullet.y += bullet.vy;
        }
        // Update and render
        update();
        render();



        }

	for (size_t i = 0; i < bullets.size(); i++) {
    if (bullets[i].x < 0 || bullets[i].x > WIDTH || bullets[i].y < 0 || bullets[i].y > HEIGHT) {
        bullets.erase(bullets.begin() + i);
    }
}
        // Sleep for a bit
       // SDL_Delay(1000 / 60);
    }

end:
    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

