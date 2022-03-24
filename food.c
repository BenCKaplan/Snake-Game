/*food.c -------
*
* Filename: food.c
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:16:12 2020
* Last-Updated: September 12 16:51 2020
*	  By: Manisha Suresh Kumar
*
*/

/* Commentary:
*
*
*
*/

/* Change log:
*
*
*/

/*Copyright (c) 2016 The Trustees of Indiana University and
* Indiana University Research and Technology Corporation.
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "food.h"

int rand_symbol;
//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        for(int i = 0; i < 1; i++) {
            rand_symbol = rand()%2 + 1;
            if(rand_symbol == 1) {
                new_food->type = 'O';
            }
            else if(rand_symbol == 2) {
                new_food->type = '+';
            }
        }
    }
    else if(type == Decrease){
        attro(COLOR_PAIR(8));
        for(int i = 0; i < 1; i++) {
            rand_symbol = rand()%2+1;
            if(rand_symbol == 1) {
                new_food->type = 'X';
            }
            else if(rand_symbol == 2) {
                new_food->type = '-';
            }
        }
    }
    new_food->next = NULL;

    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y) {
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food) {
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type remove_eaten_food(Food* foods, int x, int y) {
    //Implement the code to remove food eaten by the snake
    while(foods->next) {
        if(foods->x == x && foods->y == y){
            foods->x = 0;
            foods->y = 0;
            if(foods->type == 'O' || foods->type == '+'){
                return Increase;
            }
            else if(foods->type == 'X' || foods->type == '-') {
                return Decrease;
            }
        }
        foods = foods->next;
    }
}

// Display all the food
void draw_food (Food *foods) {
    Food* temp = foods;
    while(temp) {
        mvprintw(temp->y, temp->x, "%c", temp->type);
        temp = temp->next;
    }
}