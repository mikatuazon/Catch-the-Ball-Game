#include <stdio.h>
#include <graphics_lib.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <amio.h>

#define arm_length 30
#define thickness 2
#define body_end 70
#define diameter_head 20
#define arm_y_pos 30
#define floor 850
#define vel_MIN 20
#define vel_MAX 60
#define angle_MIN 30
#define angle_MAX 60
#define gravity 9.81
#define window_x 900
#define window_y 900
#define MIN_move_window 50
#define MAX_move_window 850
#define NUMBER_SCORES 20
#define increments 10

typedef struct
{
    int vel_x, vel_y;
    double initial_x, initial_y;
    long double time;
    double vel, angle;
    float x_pro;
    float y_pro;
    int flag;
}proj_struct;

typedef struct
{
    double initial_x_pos;
    int initial_y_pos;
    int move;
}player_struct;

typedef struct
{
    int x1;
    int y1;
    int x2;
    int y2;
    int color;
}button_struct;

typedef struct
{
    char score_str[20];
    char lives_str[5];
    int score;

    int life;
}score_struct;


void stick_person(player_struct *player)
{
    BITMAP waldo = load_bitmap("./data/waldo_head.png");
    draw_bitmap(waldo, player ->initial_x_pos + player->move, player->initial_y_pos);
    line(player->initial_x_pos + player->move, player->initial_y_pos + diameter_head, player->initial_x_pos + player->move, player->initial_y_pos + body_end, thickness);
    line(player->initial_x_pos - arm_length + player->move, player->initial_y_pos + arm_y_pos, player->initial_x_pos + arm_length + player->move, player->initial_y_pos + arm_y_pos, thickness);
    line(player->initial_x_pos + player->move, player->initial_y_pos + body_end, player->initial_x_pos - 20 + player->move, player->initial_y_pos + 120, thickness);
    line(player->initial_x_pos + player->move, player->initial_y_pos + body_end, player->initial_x_pos + 20 + player->move, player->initial_y_pos + 120, thickness);

}

void get_event(int *new_position, player_struct *player)
{
           if (check_if_event())
        {
            /* wait for event  */
            wait_for_event();

                if  (event_key_left_arrow() && player->initial_x_pos+player->move>=MIN_move_window)
                {
                    *new_position -= increments;

                }
                else if (event_key_right_arrow() && player->initial_x_pos+player->move<=MAX_move_window)
                {
                    *new_position += increments;
                }
                else
                {
                    get_mouse_coordinates();
                    *new_position = XMOUSE;
                }
            }


}

void Proj(proj_struct *projectile, player_struct *player, score_struct *score_and_life)
{
    if (projectile->y_pro < floor && projectile->flag == 1)
    {
        if(projectile->x_pro >= 20 + player->move && projectile->x_pro <= 80 +player->move && projectile->y_pro >= 745 && projectile->y_pro <= 755)
        {
            printf("\n\nSCORE\n\n");
            projectile->flag = 0;
            score_and_life->score+=5;

        }
        else if(projectile->x_pro >= 0 && projectile->x_pro <= 900 && projectile->y_pro >= 845)
        {
            printf("\n\nLOST\n\n");
            score_and_life->life--;
            if(score_and_life->life == 0)
            {
                gameover();
            }
        }


        projectile->time =  (projectile-> x_pro - projectile->initial_x) / projectile->vel_x;
        projectile->y_pro = projectile->initial_y - (projectile->vel_y * projectile->time) + (gravity * projectile->time * projectile->time)/2;
        filled_circle(projectile->x_pro, projectile->y_pro, 10, RED);


        projectile->x_pro++;
        projectile->y_pro++;

    }

    else
    {

        projectile->flag = 1;

        amio_add_sample_instance("Main", PLAY_ONCE, 1);
        amio_update_audio();
        projectile->time = 0;
        projectile->vel = rand_number(vel_MIN, vel_MAX);
        projectile->angle = rand_number(angle_MIN, angle_MAX);

        printf("vel is: %lf", projectile->vel);
        printf("angle is: %lf", projectile->angle);
        projectile->angle = (projectile->angle * PI)/180;
        projectile->vel_x = projectile->vel * cos(projectile->angle);
        projectile->vel_y = projectile->vel * sin(projectile->angle);


        projectile->initial_x = 0;
        projectile->initial_y = 200;

        moveto(projectile->initial_x, projectile->initial_y);
        projectile->x_pro = 0;
        projectile->y_pro = 0;

    }

}

void background(score_struct *score_and_life)
{
    line(0, floor, window_x, floor, thickness);
    outtextxy(400, 10, "CATCH THE BALL, WALDO!");
    outtextxy(10, 60, "SCORE");
    snprintf(score_and_life->score_str, 6, "%d", score_and_life->score);
    outtextxy(80, 60, score_and_life->score_str);
    outtextxy(10, 100, "LIVES LEFT");
    snprintf(score_and_life->lives_str, 6, "%d", score_and_life->life);
    outtextxy(100, 100, score_and_life->lives_str);

}

void introduction(void)
{
    cleardevice();
    bool done = false;
    do
    {
        outtextxy(370, 500, "Catch the ball, Waldo!");
        outtextxy(350, 800, "Press ENTER to continue");
        update_display();
        if (check_if_event())
        {
            wait_for_event();
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                done = true;
            }
        }
    }
    while(!done);


}

int difficulty(void)
{
    int level_button;
    button_struct button[3] = {{350,80,500,120, LIGHTGREEN},
                            {350,180,500,220, LIGHTRED},
                            {350,280,500,320, RED}
                            };
    cleardevice();
    bool done = false;
    do
    {
        outtextxy(350, 10, "Choose difficulty");
        filled_rectangle(button[0].x1, button[0].y1, button[0].x2, button[0].y2, button[0].color);
        outtextxy(400, 100, "EASY");
        filled_rectangle(button[1].x1, button[1].y1, button[1].x2, button[1].y2, button[1].color );
        outtextxy(400,200,"MEDIUM");
        filled_rectangle(button[2].x1, button[2].y1, button[2].x2, button[2].y2, button[2].color );
        outtextxy(400,300,"HARD");
        update_display();

        reg_mouse_events();


        wait_for_event();
        if (event_mouse_left_button_down())
        {
            if(event.mouse.x >= button[0].x1 && event.mouse.x <= button[0].x2 && event.mouse.y >= button[0].y1 && event.mouse.y <= button[0].y2)
            {
                level_button = 1;
                done = true;
            }
            else if(event.mouse.x >= button[1].x1 && event.mouse.x <= button[1].x2 && event.mouse.y >= button[1].y1 && event.mouse.y <= button[1].y2)
            {
                level_button = 2;
                done = true;
            }
            else if(event.mouse.x >= button[2].x1 && event.mouse.x <= button[2].x2 && event.mouse.y >= button[2].y1 && event.mouse.y <= button[2].y2)
            {
                level_button = 3;
                done = true;
            }
        }

    }
    while(!done);

    return level_button;

}

void gameover(score_struct *score_and_life)
{
    cleardevice();
    bool done = false;
    do
    {
       amio_add_sample_instance("gameover", PLAY_ONCE, 1);
       amio_update_audio();
       outtextxy(370, 300, "GAMEOVER");
       outtextxy(350, 550, "press ENTER to continue");
       outtextxy(350, 600, "press q to exit");
       update_display();
       if (check_if_event())
        {
            wait_for_event();
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                done = true;
                main();
            }
            else if(event_key('q'))
            {
                done = true;
                closegraph();
            }
        }
    }
    while(!done);


}
void game(int level)
{
    cleardevice();
    player_struct player;
    proj_struct projectile[level];
    score_struct score_and_life;

    player.initial_x_pos = 50;
	player.initial_y_pos = 725;
    player.move = 0;
    score_and_life.score = 0;
    score_and_life.life = 5;

    setcolor(RED);


    do
    {

        cleardevice();
        background(&score_and_life);
        get_event(&player.move, &player);
        stick_person(&player);


        for (int nBall = 0; nBall < level ; nBall++)
        {

            Proj(&projectile[nBall], &player, &score_and_life);
        }

        pausefor(5);
        update_display();


    }
    while(1);





    getch();
}



/* amio_functions.c defines the implementations of the
   wrapper functions which hide
   complexities of allegro. This is done so that many of the complexities
   of C are hidden from the beginner.*/
   
  /* graphics_lib.h defines functions which hide
   complexities of allegro and allow
   the C programming labscripts to be used
   almost as is. This is done so that many of the complexities
   of C are hidden from the beginner.*/