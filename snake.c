#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define SPEED 200


typedef struct{
	SDL_Rect body;
	struct Node *next;
}Node;

typedef struct{
	Node *head;
	Node *tail;
}Snake;

Snake *initialiseSnake(){
	Snake *snake=malloc(sizeof(Snake));
	if(snake!=NULL){
		snake->head=NULL;
		snake->tail=NULL;
	}
	return snake;
}

void addHead(Snake *snake,int x,int y){
	Node *node=malloc(sizeof(Node));
	if(snake->head==NULL){
		node->next=NULL;
		snake->tail->next=node;
		node->body.w=24; node->body.h=24;
		node->body.x=x;
		node->body.y=y;
		snake->head=node;
	}
	else{
		node->next=NULL;
		node->body.w=24; node->body.h=24;
		node->body.x=x;
		node->body.y=y;
		snake->head->next=node;
		snake->head=node;
	}
}

void addTail(Snake *snake,int x,int y){
	Node *node=malloc(sizeof(Node));
	if(snake->tail==NULL){
		node->next=snake->head;
		node->body.w=24; node->body.h=24;
		node->body.x=x;
		node->body.y=y;
		snake->tail=node;
	}
	else{
		node->body.w=24; node->body.h=24;
		node->body.x=x; 
		node->body.y=y;
		node->next=snake->tail;
		snake->tail=node;
	}
}

void deleteTail(Snake *snake){
	Node *node=snake->tail;
	snake->tail=snake->tail->next;
	free(node);
	
}

void main(){
	if(SDL_Init(SDL_INIT_VIDEO)!=0){
		printf("Error: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window *window=SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);

	Uint32 flags=SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend=SDL_CreateRenderer(window,-1,flags);

	SDL_SetRenderDrawColor(rend,64,63,63,0);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);


	Snake *snake=initialiseSnake();
	float pos_x=(WIDTH)/2; float pos_y=(HEIGHT)/2;
	addTail(snake,pos_x-24,pos_y);
	addHead(snake,pos_x,pos_y);
	addTail(snake,pos_x-2*24,pos_y);
	addTail(snake,pos_x-3*24,pos_y);
		
	SDL_Rect food;
	food.w=20; food.h=20;
	float x_vel=SPEED,y_vel=SPEED;

	int up=0,down=0,left=0,right=0;

	int close=0;
	while(!close){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
					close=1;
					break;

				case SDL_KEYDOWN:
					switch(e.key.keysym.scancode){
						case SDL_SCANCODE_W:
							up=1;
							down=0;
							left=0;
							right=0;
							break;

						case SDL_SCANCODE_S:
							down=1;
							up=0;
							left=0;
							right=0;
							break;

						case SDL_SCANCODE_A:
							left=1;
							up=0;
							down=0;
							right=0;
							break;

						case SDL_SCANCODE_D:
							right=1;
							left=0;
							up=0;
							down=0;
							break;
					}
			}
		}

		int head_x=snake->head->body.x;
		int head_y=snake->head->body.y;

		if(up&&!down){
			addHead(snake,head_x,head_y-24);
			deleteTail(snake);
		}
		if(down&&!up){
			addHead(snake,head_x,head_y+24);
			deleteTail(snake);
		}
		if(left&&!right){
			addHead(snake,head_x-24,head_y);
			deleteTail(snake);
		}
		if(right&&!left){
			addHead(snake,head_x+24,head_y);
			deleteTail(snake);
		}

		if(head_x>WIDTH) snake->head->body.x=0;
		if(head_x<0) snake->head->body.x=WIDTH;

		if(head_y>HEIGHT) snake->head->body.y=0;
		if(head_y<0) snake->head->body.y=HEIGHT;

		if((head_x<=food.x+snake->head->body.w&&head_x>=food.x-snake->head->body.w)&&(head_y<=food.y+snake->head->body.h&&head_y>=food.y-snake->head->body.h)){
			srand(time(0));
			food.x=rand()%(WIDTH-food.w)+1;
			food.y=rand()%(HEIGHT-food.y)+1;
			addTail(snake,snake->tail->body.x-24,snake->tail->body.y);
		}

		SDL_SetRenderDrawColor(rend,64,63,63,0);
		SDL_RenderClear(rend);
		SDL_RenderPresent(rend);

		Node *node=snake->tail;
		while(node!=NULL){
			SDL_SetRenderDrawColor(rend,207,97,72,0);
			SDL_RenderFillRect(rend,&node->body);
			SDL_RenderDrawRect(rend,&node->body);
		
			// SDL_Surface *s=IMG_Load("doge.png");
			// SDL_Texture *doge=SDL_CreateTextureFromSurface(rend,s);
			// SDL_FreeSurface(s);
			// SDL_RenderCopy(rend,doge,NULL,&node->body);		
			SDL_RenderPresent(rend);
			node=node->next;	
		}	

//		SDL_SetRenderDrawColor(rend,207,97,72,0);
//		SDL_RenderFillRect(rend,&rect);
//		SDL_RenderDrawRect(rend,&rect);
//		SDL_RenderPresent(rend);

		SDL_SetRenderDrawColor(rend,156, 204, 108,0);
		SDL_RenderFillRect(rend,&food);
		SDL_RenderDrawRect(rend,&food);
		SDL_RenderPresent(rend);
	
      		// SDL_Surface *s=IMG_Load("icecream.png");
        //         SDL_Texture *ic=SDL_CreateTextureFromSurface(rend,s);
        //         SDL_FreeSurface(s);
        //         SDL_RenderCopy(rend,ic,NULL,&food);
        //         SDL_RenderPresent(rend);


		SDL_Delay(70);
	}

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

