/*
 * bounce2.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "bounce.h"

static ppball the_ball;
static ppbounce the_bounce;
static WINDOW *g_win;
static WINDOW *bounce_win;

static void set_ticker(int);
static void set_up();
static void wrap_up();
static void ball_move(int);
static int bounce_or_lose(ppball * bp);
static void init_win(void);
static void do_animate(int signum);
static void bounce_move(int signum);
static void do_collision(void);

void ball(void)
{
	int c;						//用户输入

	set_up();

	while ((c = getch()) != 'Q')
	{
		switch (c)
		{
			case 's':				//move left
				the_bounce.x_dir = -1;
				break;
			case 'f':				//move right
				the_bounce.x_dir = 1;
				break;
		}
	}

	wrap_up();

	return;
}

static void set_up()
{
	the_ball.x_pos = BALL_X_INIT;
	the_ball.y_pos = BALL_Y_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttm = the_ball.x_ttg = X_TTM;
	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = DEL_SYMBOL;

	the_bounce.x_dir = 1;
	the_bounce.x_pos = BOUNCE_X_INIT;
	the_bounce.y_pos = BOUNCE_Y_INIT;

	initscr();
	noecho();
	crmode();

	//draw window
	init_win();

	signal(SIGINT, SIG_IGN);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	refresh();

	signal(SIGALRM, do_animate);
	set_ticker(50);

	return;
}

static void wrap_up()
{
	set_ticker(0);
	delwin(g_win);
	delwin(bounce_win);
	endwin();

	exit(0);
}

static void init_win(void)
{
	refresh();
	g_win = newwin(WIN_HEIGHT, WIN_WIDTH, TOP_ROW, LEFT_EDGE);
	box(g_win, 0, 0);
	wrefresh(g_win);
	bounce_win =
		newwin(BOUNCE_HEIGHT, BOUNCE_WIDTH, the_bounce.y_pos, the_bounce.x_pos);
	box(bounce_win, '*', '*');
	wrefresh(bounce_win);

	return;
}

static void do_collision(void)
{
	if ((the_ball.x_pos >= the_bounce.x_pos)
		&& (the_ball.x_pos <= (the_bounce.x_pos + BOUNCE_WIDTH)))
	{
		//ball正好在bounce的区域内
		if (the_ball.y_pos >= the_bounce.y_pos
			&& (the_ball.y_pos <= (the_bounce.y_pos + BOUNCE_HEIGHT)))
		{
			the_ball.y_dir = -1;
			//TODO 加分
		}
	} 
	else
	{
		//ball落在bounce的区域外 ， Game
		if (the_ball.y_pos >= the_bounce.y_pos)
		{
			//wrap_up(); TODO
		}
	}

	return;
}

static void do_animate(int signum)
{
	signal(SIGALRM, SIG_IGN);

	clear();
	refresh();					//清空屏幕

	bounce_move(signum);

	touchwin(g_win);
	wrefresh(g_win);
	ball_move(signum);

	touchwin(bounce_win);
	wrefresh(bounce_win);

	do_collision();				//检测ball和bounce的冲突

	signal(SIGALRM, do_animate);

	return;
}

static void ball_move(int signum)
{
	int y_cur, x_cur, moved;

	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;

	if (the_ball.y_ttm > 0)
	{
		the_ball.y_ttg = the_ball.y_ttm;
		the_ball.y_pos += (the_ball.y_dir * the_ball.y_ttg);
		moved = 1;
	}

	if (the_ball.x_ttm > 0)
	{
		the_ball.x_ttg = the_ball.x_ttm;
		the_ball.x_pos += (the_ball.x_dir * the_ball.x_ttg);
		moved = 1;
	}

	if (moved)
	{
		//printf("%d %d;%d %d,", the_ball.y_pos, the_ball.x_pos, y_cur, x_cur);
		mvaddch(y_cur, x_cur, BLANK);
		//mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES - 1, COLS - 1);
		refresh();
	}

	return;
}

static void bounce_move(int signum)
{
	if ((the_bounce.x_pos + BOUNCE_WIDTH) >= RIGHT_EDGE)
		the_bounce.x_dir = -1;
	if (the_bounce.x_pos <= LEFT_EDGE)
		the_bounce.x_dir = 1;

	the_bounce.x_pos += the_bounce.x_dir;
	mvwin(bounce_win, the_bounce.y_pos, the_bounce.x_pos);
	wrefresh(bounce_win);

	return;
}

static int bounce_or_lose(ppball * bp)
{
	int return_val = 0;

	if (bp->y_pos <= (TOP_ROW + 1))
	{
		bp->y_dir = 1;
		return_val = 1;
	} 
	else if (bp->y_pos >= (BOT_ROW - 2))
	{
		bp->y_dir = -1;
		return_val = 1;
	} 
	else if (bp->x_pos <= (LEFT_EDGE + 1))
	{
		bp->x_dir = 1;
		return_val = 1;
	} 
	else if (bp->x_pos >= (RIGHT_EDGE - 2))
	{
		bp->x_dir = -1;
		return_val = 1;
	}

	return return_val;
}

static void set_ticker(int delay_msec)
{
	struct itimerval new_timeset;
	long n_sec, n_usec;
	n_sec = delay_msec / 1000L;
	n_usec = delay_msec * 1000L;

	// printw("n_sec %ld, n_usec %ld", n_sec, n_usec);

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;

	setitimer(ITIMER_REAL, &new_timeset, NULL);

	return;
}



