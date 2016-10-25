#ifndef __BOUNCE_H__
#define __BOUNCE_H__

#define BOUNCE1
#undef BOUNCE1

#define BOUNCE2
//#undef BOUNCE2


#define BLANK 			' '
#define DEL_SYMBOL 		'o'

#define BALL_X_INIT 	11
#define BALL_Y_INIT 	12
#define WIN_HEIGHT 		25
#define WIN_WIDTH 		60
#define TOP_ROW 		5
#define BOT_ROW 		(TOP_ROW + WIN_HEIGHT)
#define LEFT_EDGE 		10
#define RIGHT_EDGE 		(LEFT_EDGE + WIN_WIDTH)

#define BOUNCE_X_INIT 	(LEFT_EDGE + 1)
#define BOUNCE_Y_INIT 	(BOT_ROW - BOUNCE_HEIGHT)
#define BOUNCE_HEIGHT 	2
#define BOUNCE_WIDTH 	15


#define TICKS_PER_SEC 	50

#define X_TTM 			1
#define Y_TTM 			1

typedef struct ppball_tag {
	int y_pos, x_pos,
    	y_ttm, x_ttm,
    	y_ttg, x_ttg,
    	y_dir, x_dir;
	char symbol;
}ppball;

typedef struct ppbounce_tag {
	int y_pos, x_pos,
    	y_dir, x_dir;
}ppbounce;

void ball(void);

#endif // BOUNCE_H
