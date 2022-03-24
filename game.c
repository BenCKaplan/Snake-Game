#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"
#include "food.h"
#include "game_window.h"
#include "key.h"
#include "game.h"
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}
void game(){
    FILE *snakeData;
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    WINDOW *pause;
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)

    const int height = 30;
    const int width = 70;
    char ch;
    int i = 0;
    int points = 0;//declared variable to count points that player recieved

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;
    start_color();
    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);
            ch = get_char();
            int lives = 3;

            /*Setting height and width of the board*/
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            init_pair(1, COLOR_RED, COLOR_BLACK);
            init_pair(2, COLOR_GREEN, COLOR_BLACK);
            init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

            mvprintw(20, 20, "Welcome to the snake game! Here are some rules:");//instructions/controls menu print statements start
            attron(COLOR_PAIR(1));
            mvprintw(22, 20, "s/S - start the game");
            mvprintw(24, 20, "p/P - pause the game");
            mvprintw(26, 20, "q/Q - quit the game");
            mvprintw(28, 20, "Use the arrow keys to move");
            mvprintw(30, 20, "press + to increase the game window");
            mvprintw(32, 20, "Press - to decrease the game window");

            mvprintw(40, 20, "Scoring:");
            attron(COLOR_PAIR(2));
            mvprintw(42, 20, "You get 20 points if you eat the good food (O)");
            mvprintw(44, 20, "You lose 10 points if you eat the bad food (X)");

            attron(COLOR_PAIR(3));
            mvprintw(60, 20, "Press s/S to start the game: %c", ch); //instructions/controls menu print statements end
            //mvprintw(62, 20, "%d", y_offset + (height/2));
            if(ch == 's' || ch == 'S') {//if player chooses s or S, it will start the snake game
                /*Init board*/
                window = init_GameWindow(x_offset, y_offset, width, height);
                draw_Gamewindow(window);

                /*Init snake*/
                snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));

                /*Init foods*/
                int food_x, food_y, i;
                enum Type type;
                    /*Generate 20 foods*/
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand()%4 > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
                foods = create_food(food_x, food_y, type);
                for(i = 1; i < 10; i++){
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                    while (food_exists(foods,food_x, food_y))
                        generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                    type = (rand() > RAND_MAX/4) ? Increase : Decrease;
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food);
                }
                state = ALIVE;
            }
            break;

        case ALIVE:
            ch = get_char();
            /* Write your code here */
            if(ch != NOCHAR){
                switch(ch) {//initialize switch statement for different arrow key functions
                    case LEFT: //Left arrow-key case
                        snake->next->next->x = snake->next->x;
                        snake->next->next->y = snake->next->y;
                        snake->next->x = snake->x;
                        snake->next->y = snake->y;
                        move_snake(snake, LEFT);
                        break;

                    case RIGHT://case for right arrow
                        snake->next->next->x = snake->next->x;
                        snake->next->next->y = snake->next->y;
                        snake->next->x = snake->x;
                        snake->next->y = snake->y;
                        move_snake(snake, RIGHT);
                        break;

                    case UP:/*case for up arrow*/
                        snake->next->next->x = snake->next->x;
                        snake->next->next->y = snake->next->y;
                        snake->next->x = snake->x;
                        snake->next->y = snake->y;
                        move_snake(snake, UP);
                        break;

                    case DOWN:/*case for down arrow*/
                        snake->next->next->x = snake->next->x;
                        snake->next->next->y = snake->next->y;
                        snake->next->x = snake->x;
                        snake->next->y = snake->y;
                        move_snake(snake, DOWN);
                        break;

                    case 'Q':
                    case 'q'://quit case, directs state to being dead to end the game.
                        state = DEAD;
                        break;

                    case 'P':
                    case 'p'://pause case
                        pause = create_newwin(4, 1, width, height);
                        box(pause, 0, 0);
                        refresh();
                        wrefresh(pause);
                        keypad(pause, true);
                        char* options[4] = {"Resume", "Load save files", "Load 10 best scores", "Quit"};
                        int ch;
                        int highlight = 0;
                        while(true) {
                            for(i = 0; i < 4; i++) {
                                if(i == highlight)
                                    wattron(pause, A_REVERSE);
                                mvwprintw(pause, i+1, i, options[i]);
                                wattroff(pause, A_REVERSE);
                            }
                            ch = get_char(pause);

                            switch(ch) {
                                case KEY_UP:
                                    highlight--;
                                    if(highlight == -1) {
                                        highlight == 0;
                                    }
                                    break;
                                case KEY_DOWN:
                                    highlight++;
                                    if(highlight == 4) {
                                        highlight == 3;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            if(ch == 10) {
                                break;
                            }
                            printw("Your choice was: %s", options[highlight]);
                        }
                        

                        mvprintw(16, 20, "You have paused the game");
                        newwin(x_offset, y_offset, width, height);
                        while(true) {
                            ch = get_char();
                            if(ch == 'p' || ch == 'P') {
                                break;
                            }
                        }
                        break;

                    case '+'://case for expanding game border: undraws, resizes, and then re-draws the game window with bigger dimensions.
                        if(window->width <= 80 && window->height <= 40){
                            undraw_Gamewindow(window);//undraw
                            changeGameWindow(window->upper_left_x, window->upper_left_y, window->width+5, window->height + 5, window);//change dimensions
                            draw_Gamewindow(window);//re-draw
                        }
                        break;

                     case '-'://case for making game border smaller: undraws, resizes, and then re-draws the game window with smaller dimensions.
                        if(window->width >= 60 && window->height >= 20) {//checks to see if the border has been decreased three times already
                                undraw_Gamewindow(window);//undraw
                                changeGameWindow(window->upper_left_x, window->upper_left_y, window->width - 5, window->height - 5, window);//change dimensions
                                draw_Gamewindow(window);//re-draw with new dimensions
                        }
                       break;
                    case NOCHAR:
                        if(foods->type == Increase) {
                            if(foods->x == snake->x && foods->y == snake->y) {//checks if the x and y values of the snake head are equal to those of the food
                                points += 20;//add 20 to variable points if good food is eaten
                                remove_eaten_food(foods, foods->x, foods->y);
                            }
                        }
                        else if(foods->type == Decrease) {
                            if(foods->x == snake->x && foods->y == snake->y) {
                                points -= 10; //subtract 10 from points if bad food is eaten
                                remove_eaten_food(foods, foods->x, foods->y);//didn't fully implement remove_eaten_food, but this would remove the food that the snake ate
                            }
                        }

                }/*end switch*/
            }
            if(snake->x = window) {
                state == DEAD;
            }

            /*Draw everything on the screen*/
            clear();
            mvprintw(22, 60, "tail position: %d, %d", snake->next->next->x, snake->next->next->y);//Data to help while coding - START
            mvprintw(80, 60, "Snake body position: %d, %d", snake->next->x, snake->next->y);
            mvprintw(19, 60, "Food position: %d, %d",foods->x, foods->y);
            mvprintw(18, 60, "newFood position: %d, %d", new_food->x, new_food->y);
            mvprintw(20, 60, "Snake position: %d, %d", snake->x, snake->y);
            mvprintw(10, 10, "snake length: %d", len(snake));//data to help while coding - END
            mvprintw(20, 20, "Key entered: %c", ch);//prints key pressed
            mvprintw(18, 20, "Points: %d", points);//prints points
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods->type == Increase);
            break;

        case DEAD:
            if(lives > 0) {
                snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
                draw_snake(snake);
                state = ALIVE;
            }
            else {
                if((snakeData = fopen("snake.game", "w")) == NULL) {
                    mvprintw(15, 20, "Error opening snake.game");
                }
                else{
                    for(i = 0; i < 10; i++) {
                        fwrite(&points, sizeof(points), 1, snakeData);
                    }
                }
                clear();
                endwin();
                mvprintw(50, 50, "Thanks for playing!");
                mvprintw(60, 50, "Points: %d", points);
            }
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
}
