#include <stdlib.h>
#include "primlib.h"
#include "pieces.h"
#include <time.h>


#define BOARD_WIDTH  10
#define BOARD_HEIGHT  20
#define UPPER_BOARD_X 10
#define UPPER_BOARD_Y 30
#define BLOCK_SIZE 400/BOARD_HEIGHT
#define BLOCK_COLOR YELLOW
#define DELAY_TIME 10
#define LOGIC_LOOP_ITERATIONS 10
#define NUMBER_OF_PIECES 7
#define DIFFERENCE_INDEX_AND_POSITION 1
#define YES 1
#define NO 0
#define BOARD_COLOR BLUE
#define BACKGROUND_COLOR BLACK
#define SHOW_NEXT_PIECE_X 300
#define SHOW_NEXT_PIECE_Y 20
#define NEXT_PIECE_BOARD_SIZE 120
#define DIMENSION_OF_PIECE 4

typedef unsigned int block;
typedef unsigned short int flag;
typedef unsigned short int boolean;

void clearScreen(void);
void drawBoard(void);
void drawBlocks(void);
void drawBlock(int positionX, int positionY, enum color blockColor);
void drawMovingPiece(void);
void drawScore(void);
boolean checkMovement(int piece, int nextXPosition, int nextYPosition, int nextRotation);
void freezePiece(int piece, int pieceRotation, int pieceXPosition, int pieceYPosition);
int addingPoints(int numberOfLines);
int cleaningLines(void);
void drawFinishingScreen(int nextPiece);
void movePieces();
void menagingLogic(int nextPiece);
void nextBlock(int Piece);
void rotation(int piece, int pieceRotation, int newPieceRotation, int *changedPieceX, int *changedPieceY);
void setToZero(void);

block tetrisContainer[BOARD_WIDTH][BOARD_HEIGHT];
int piece = 0;
int pieceRotation = 0;
int pieceX = 0;
int pieceY = 0;
int points = 0;
int newPieceX = 0;
int newPieceY = 0;
int newPieceRotation = 0;
int framesCounter = 0;
flag shouldRun = YES;

int main()
{
	srand(time(NULL));
	
	piece = rand() % NUMBER_OF_PIECES;
	pieceRotation = 0;
	pieceY = 0;
	newPieceX = ((BOARD_WIDTH - 4) / 2);
	
	initGraph();
	
	while(shouldRun == YES)
	{
        int nextPiece;
        if(pieceY == 0 && framesCounter==0) nextPiece=rand() % NUMBER_OF_PIECES;
		clearScreen();
		drawBoard();
		drawBlocks();
		drawMovingPiece();
		drawScore();
        nextBlock(nextPiece);
		updateScreen();
		
		newPieceX = pieceX;
		newPieceY = pieceY;
		newPieceRotation = pieceRotation;
		
		movePieces(newPieceX, newPieceY, newPieceRotation, shouldRun, framesCounter);

		if(checkMovement(piece, newPieceX, newPieceY, newPieceRotation) == YES)
		{
			pieceX = newPieceX;
			pieceY = newPieceY;
			pieceRotation = newPieceRotation;
		}
		if(framesCounter >= LOGIC_LOOP_ITERATIONS)
		{
			menagingLogic(nextPiece);
		}
		else
		{
			framesCounter ++;
			SDL_Delay(DELAY_TIME);
		}
	}
	return 0;
}

void clearScreen()
{
	filledRect(0, 0, screenWidth(), screenHeight(), BACKGROUND_COLOR);
}

void drawBoard()
{
	line(UPPER_BOARD_X, UPPER_BOARD_Y, UPPER_BOARD_X, UPPER_BOARD_Y + (BLOCK_SIZE * BOARD_HEIGHT), BOARD_COLOR);
	line(UPPER_BOARD_X, UPPER_BOARD_Y + (BLOCK_SIZE * BOARD_HEIGHT), UPPER_BOARD_X + (BLOCK_SIZE * BOARD_WIDTH), UPPER_BOARD_Y + (BLOCK_SIZE * BOARD_HEIGHT), BOARD_COLOR);
	line(UPPER_BOARD_X + (BLOCK_SIZE * BOARD_WIDTH), UPPER_BOARD_Y + (BLOCK_SIZE * BOARD_HEIGHT), UPPER_BOARD_X + (BLOCK_SIZE * BOARD_WIDTH), UPPER_BOARD_Y, BOARD_COLOR);
    line(UPPER_BOARD_X,UPPER_BOARD_Y, UPPER_BOARD_X + (BLOCK_SIZE * BOARD_WIDTH), UPPER_BOARD_Y, BOARD_COLOR);
}
void setToZero()
{
    int blockX;
	int blockY;
	
	for(blockY = 0; blockY < BOARD_HEIGHT; blockY ++)
	{
		for(blockX = 0; blockX < BOARD_WIDTH; blockX ++)
		{
			
			
				tetrisContainer[blockX][blockY]=0;
			
		}
	}
}

void drawBlocks()
{
	int blockX;
	int blockY;
	
	for(blockY = 0; blockY < BOARD_HEIGHT; blockY ++)
	{
		for(blockX = 0; blockX < BOARD_WIDTH; blockX ++)
		{
			if(tetrisContainer[blockX][blockY] != 0)
			{
				drawBlock(blockX, blockY, tetrisContainer[blockX][blockY]);
			}
		}
	}
	
}

void drawBlock(int positionX, int positionY, enum color blockColor)
{
	
    
    filledRect(UPPER_BOARD_X + (positionX * BLOCK_SIZE),
			   UPPER_BOARD_Y + (positionY * BLOCK_SIZE),
			   UPPER_BOARD_X + (positionX * BLOCK_SIZE) + BLOCK_SIZE-2,
			   UPPER_BOARD_Y + (positionY * BLOCK_SIZE)+ BLOCK_SIZE-2,
			   blockColor);
}

void drawMovingPiece()
{
	int blockX;
	int blockY;
	int isEmpty = 0;
	
	for(blockX = 0; blockX < DIMENSION_OF_PIECE; blockX ++)
	{
		for(blockY = 0; blockY < DIMENSION_OF_PIECE; blockY ++)
		{
			if(pieces[piece][pieceRotation][blockX][blockY] != isEmpty)
			{
            
				drawBlock(pieceX + blockX, pieceY + blockY, pieces[piece][pieceRotation][blockX][blockY]);
			}
		}
	}
	SDL_Delay(DELAY_TIME);
}

void drawScore()
{
	int pointsXPosition = 400;
	int pointsYPosition = 380;
	
	char scoreboard[255];
	sprintf(scoreboard, "Points: %d", points);
	textout(pointsXPosition, pointsYPosition, scoreboard, RED);
}

void movePieces()
{
    int changedPieceX, changedPieceY;
   
	switch(pollkey())
	{
		case SDLK_RIGHT:
		{
			newPieceX = pieceX + 1;
				break;
		}
		case SDLK_LEFT:
		{
			newPieceX = pieceX - 1;
			break;
		}
		case SDLK_DOWN:
		{
			newPieceY = pieceY;
			while(checkMovement(piece, pieceX, newPieceY + 1, pieceRotation == YES))
			{
				newPieceY ++;
			}
			
			framesCounter = 0;
			break;
		}
		case SDLK_SPACE:
		{
            if(pieceRotation == 3) newPieceRotation = 0;
			 else newPieceRotation = (pieceRotation + 1);
            rotation(piece, pieceRotation, newPieceRotation, &changedPieceX, &changedPieceY);
              newPieceX = pieceX + changedPieceX;
              newPieceY = pieceY + changedPieceY;
             
			break;
		}
		case SDLK_ESCAPE:
		{
			shouldRun = NO;
			break;
		}
	}
}

boolean checkMovement(int piece, int nextXPosition, int nextYPosition, int nextRotation)
{
	int isEmpty = 0;
	int blockX;
	int blockY;
	
	for(blockX = 0; blockX < DIMENSION_OF_PIECE; blockX ++)
	{
		for(blockY = 0; blockY < DIMENSION_OF_PIECE; blockY +=1)
		{
			if(pieces[piece][nextRotation][blockX][blockY] != isEmpty)
			{
				if(nextXPosition + blockX < 0)
				{
					return NO;
				}
				if(nextXPosition + blockX > BOARD_WIDTH - DIFFERENCE_INDEX_AND_POSITION)
				{
					return NO;
				}
				if(nextYPosition + blockY > BOARD_HEIGHT - DIFFERENCE_INDEX_AND_POSITION)
				{
					return NO;
				}
				if(tetrisContainer[nextXPosition + blockX][nextYPosition + blockY] != isEmpty)
				{
					return NO;
				}
			}
		}
	}
	return YES;
}

void freezePiece(int piece, int pieceRotation, int pieceXPosition, int pieceYPosition)
{
	int isEmpty = 0;
	int blockX;
	int blockY;
	
	for(blockX = 0; blockX < DIMENSION_OF_PIECE; blockX ++)
	{
		for(blockY = 0; blockY < DIMENSION_OF_PIECE; blockY ++)
		{
			if(pieces[piece][pieceRotation][blockX][blockY] != isEmpty)
			{
				tetrisContainer[pieceXPosition + blockX][pieceYPosition + blockY] = YES;
			}
		}
	}
}

int addingPoints(int numberOfLines)
{
	switch(numberOfLines)
	{
		case 1:
			return 10;
			break;
		case 2:
			return 25;
			break;
		case 3:
			return 50;
			break;
		case 4:
			return 100;
			break;
		default:
			return 0;
			break;
	}
}

int cleaningLines()
{
	int blockX;
	int blockY;
	boolean isEmpty = NO;
	int numberOfLines = 0;
	int nextBlockY = 0;
	
	for(blockY = 0; blockY < BOARD_HEIGHT; blockY ++)
	{
		isEmpty = NO;
		for(blockX = 0; blockX < BOARD_WIDTH; blockX ++)
		{
			if(tetrisContainer[blockX][blockY] == 0)
			{
				isEmpty = YES;
				break;
			}
		}	
		if(isEmpty == NO)
		{
			blockX = 0;
			for(blockX = 0; blockX < BOARD_WIDTH; blockX ++)
			{
				nextBlockY = blockY;
				for( ; nextBlockY > 0; nextBlockY -= 1)
				{
					tetrisContainer[blockX][nextBlockY] =
						tetrisContainer[blockX][nextBlockY - DIFFERENCE_INDEX_AND_POSITION];
				}
			}
			numberOfLines ++;
		}
	}
	return numberOfLines;
}

void drawFinishingScreen(int nextPiece)
{
	textout(screenWidth() / 2, screenHeight() / 2, "GAME OVER!!! PRESS ENTER TO PLAY AGAIN", WHITE);
	updateScreen();
	//while (getkey() != SDLK_ESCAPE);
     if((getkey() == SDLK_RETURN)) {setToZero();shouldRun = YES; }
            else if((getkey() == SDLK_ESCAPE)) shouldRun = NO;
            else return;
}


void menagingLogic(int nextPiece)
{
	newPieceY ++;
	if(checkMovement(piece, pieceX, newPieceY, pieceRotation) == YES)
	{
		pieceY = newPieceY;
	}
	else
	{
		freezePiece(piece, pieceRotation, pieceX, pieceY);
		points += addingPoints(cleaningLines());
		piece = nextPiece;
		pieceRotation = 0;
		pieceY = 0;
		pieceX = ((BOARD_WIDTH - 4) / 2);
		if(checkMovement(piece, pieceX, pieceY, pieceRotation) == NO)
		{
			drawFinishingScreen(nextPiece);
			
            //if((getkey() == SDLK_RETURN)) {shouldRun = YES; clearScreen(); menagingLogic(nextPiece);}
           // else shouldRun = NO;
		}
		
	}
	framesCounter = 0;
}

void nextBlock(int piece)
{
    int blockX, blockY;
    
    line(SHOW_NEXT_PIECE_X, SHOW_NEXT_PIECE_Y, SHOW_NEXT_PIECE_X + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_Y, BOARD_COLOR);
	line(SHOW_NEXT_PIECE_X, SHOW_NEXT_PIECE_Y, SHOW_NEXT_PIECE_X, SHOW_NEXT_PIECE_Y + NEXT_PIECE_BOARD_SIZE, BOARD_COLOR);
    line(SHOW_NEXT_PIECE_X, SHOW_NEXT_PIECE_Y + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_X + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_Y + NEXT_PIECE_BOARD_SIZE, BOARD_COLOR);
    line(SHOW_NEXT_PIECE_X + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_Y + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_X + NEXT_PIECE_BOARD_SIZE, SHOW_NEXT_PIECE_Y, BOARD_COLOR);
    
    for(blockX = 0; blockX < DIMENSION_OF_PIECE; blockX ++)
    {
        for(blockY = 0; blockY < DIMENSION_OF_PIECE; blockY ++)
        {
          
            if(pieces[piece][0][blockX][blockY] == 0) continue;
            else filledRect((SHOW_NEXT_PIECE_X+20)+(BLOCK_SIZE*blockX), (SHOW_NEXT_PIECE_Y+20)+(BLOCK_SIZE*blockY),(SHOW_NEXT_PIECE_X+20)+(BLOCK_SIZE*blockX)+BLOCK_SIZE-2, (SHOW_NEXT_PIECE_Y+20)+(BLOCK_SIZE*blockY)+BLOCK_SIZE-2, RED);
            
        }
    }
}

void rotation(int piece, int pieceRotation, int newPieceRotation, int* changedPieceX, int* changedPieceY)
{
    int blockX, blockY, newBlockX, newBlockY;
    
    
 for(blockX=0; blockX < DIMENSION_OF_PIECE; blockX ++)
 {
     for(blockY=0; blockY < DIMENSION_OF_PIECE; blockY ++)
     {
         if(pieces[piece][pieceRotation][blockY][blockX]==2) break;
     }
     if(pieces[piece][pieceRotation][blockY][blockX]==2) break;
 }
 
 for(newBlockX=0; newBlockX < DIMENSION_OF_PIECE; newBlockX ++)
 {
     for(newBlockY=0; newBlockY < DIMENSION_OF_PIECE; newBlockY ++)
     {
         if(pieces[piece][newPieceRotation][newBlockY][newBlockX]==2) break;
     }
     if(pieces[piece][newPieceRotation][newBlockY][newBlockX]==2) break;
 }
 
     *changedPieceY = blockX - newBlockX;
     *changedPieceX = blockY - newBlockY;
     
     
    
 
}
