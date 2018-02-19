#include "core.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Vector2D.h"
#include <math.h>
#include <vector>


#define WINDOW_WIDTH  1500
#define WINDOW_HEIGHT 1000

unsigned int lastTime = 0, currentTime;


static SDL_Window* window;
static SDL_Renderer* renderer;

SDL_Texture* LoadTexture(std::string path);

struct PhysicsComp
{
	double mass;
	Vector2D velocity;
};

struct Obj
{
	int x, y, w, h;
	Vector2D position;
	SDL_Rect rect;
	SDL_Texture* texture;
	PhysicsComp* physics;
};

struct ObjPool
{
	int objNum, freeIndex;
	Obj* objects[10];
};

void ObjInit(Obj* object, int _x, int _y, int _w, int _h, std::string imgPath);
void ObjDraw(Obj* object);
void DrawObjects(ObjPool* objs);
void ObjMove(Obj* object, double dt);

ObjPool objs;

std::vector<Obj*> physObjs;

void Gravitation(Obj* obj, double dt);
double DistanceInPx(Obj* obj1, Obj* obj2);

double ratio;

std::vector<Vector2D> trail;
void DrawTrail();

int core::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("SDLtest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
		return 2;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		return 3;
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
			return 4;
	}

	// game objects etc

	Obj* earth = new Obj;
	ObjInit(earth, 200, 200, 50, 50, "earth.png");
	objs.objects[objs.freeIndex] = earth;
	objs.freeIndex++; objs.objNum++;
	physObjs.push_back(earth);

	Obj* sun = new Obj;
	ObjInit(sun, (int)(WINDOW_WIDTH/2) - 100, (int)(WINDOW_HEIGHT / 2) - 100, 200, 200, "sun.png");
	objs.objects[objs.freeIndex] = sun;
	objs.freeIndex++; objs.objNum++;
	physObjs.push_back(sun);


	Obj* test = new Obj;
	ObjInit(test, 600, 200, 40, 40, "earth.png");
	objs.objects[objs.freeIndex] = test;
	objs.freeIndex++; objs.objNum++;
	physObjs.push_back(test);
	
	earth->physics = new PhysicsComp{ 5.974f*powf(10, 24), Vector2D(1.4f, -2.2f) };
	sun->physics = new PhysicsComp{ 333000 * (5.974f*powf(10, 24)), Vector2D(0, 0) };
	test->physics = new PhysicsComp{ 5.774f*powf(10, 24), Vector2D(2.8f, -2.5f) };


	//earth->physics = new PhysicsComp{ 1, Vector2D(-0.6f, 1.5f) };
	//sun->physics = new PhysicsComp{ 333000 * earth->physics->mass, Vector2D(0, 0) };
	//test->physics = new PhysicsComp{ 2.5f, Vector2D(0.3f, 1.0f) };

	std::cout << DistanceInPx(earth, sun) << std::endl;

	ratio = 149600000 / DistanceInPx(earth, sun);
	//ratio = DistanceInPx(earth, sun) / 0.05f; //385000;
	std::cout << "ratio: " << ratio << std::endl;

	return 0;
}

int core::update()
{
	bool quit = false;
	SDL_Event e;

	// inputs

	while (!quit)
	{
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 10)
		{
			for (size_t i = 0; i < physObjs.size(); i++)
			{
				Gravitation(physObjs[i], 1);
			}
			lastTime = currentTime;
		}


		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				//if (objs.objNum < 10)
				//{
				//	printf("object added\n");
				//	int x = e.button.x;
				//	int y = e.button.y;
				//
				//	Obj* obj = new Obj;
				//	ObjInit(obj, x, y, 100, 100, "texturetest.png");
				//	objs.objects[objs.freeIndex] = obj;
				//	objs.freeIndex++; objs.objNum++;
				//}
				//else
				//{
				//	printf("too many objects\n");
				//}
			}

			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_p:
				printf("asd\n");
				//ObjMove(earth);
				break;
			}
		}

		// rendering

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderClear(renderer);

		DrawObjects(&objs);
		//DrawTrail();

		SDL_RenderPresent(renderer);
	}
	
	return 0;
}

int core::cleanUp()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}

SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* newTexture = 0;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL)
		return 0;
	else 
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if(!newTexture)
			return 0;

		SDL_FreeSurface(loadedSurface); 
	} 
	return newTexture;
}

void ObjInit(Obj* object, int _x, int _y, int _w, int _h, std::string imgPath)
{
	object->x = _x; object->y = _y;
	object->w = _w; object->h = _h;
	object->position = Vector2D((float)(object->x + object->w / 2), (float)(object->y + object->h / 2));

	object->texture = LoadTexture(imgPath);

	//SDL_QueryTexture(object->texture, NULL, NULL, width, height);
	SDL_Rect _r
	{
		_x, _y,
		_w, _h
	};
	object->rect = _r;

}

void DrawTrail()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (size_t i = 0; i < trail.size(); i++)
	{
		SDL_RenderDrawPoint(renderer, trail[i].x, trail[i].y);
	}
}

void ObjDraw(Obj* object)
{
	SDL_Rect _r
	{
		object->x, object->y,
		object->w, object->h
	};
	object->rect = _r;
	SDL_RenderCopy(renderer, object->texture, 0, &object->rect);
}

void DrawObjects(ObjPool* objs)
{
	for (int i = 0; i < objs->objNum; i++)
	{
		ObjDraw(objs->objects[i]);
	}
}

void ObjMove(Obj* object, double dt)
{
	object->position = object->position + object->physics->velocity * dt;
	object->x = (int)object->position.x - (int)(object->w/2);
	object->y = (int)object->position.y - (int)(object->h / 2);

	//trail.push_back(Vector2D(object->position.x, object->position.y));
}

void Gravitation(Obj* obj, double dt)
{
	for (size_t i = 0; i < objs.objNum; i++)
	{
		Obj* other = objs.objects[i];
		if (other != obj)
		{
			double gF; // gravitational force
			double y = 6.67428f * powf(10, -11); // gravitational constant
			double distance = DistanceInPx(obj, other) * ratio;

			gF = y*(other->physics->mass * obj->physics->mass / powf(distance, 2));
			double accel = gF / obj->physics->mass;
			Vector2D direction = other->position - obj->position;
			direction.Normalize();
			Vector2D newVelocity = obj->physics->velocity + (direction * (accel * dt / ratio));
			obj->physics->velocity = newVelocity;

			ObjMove(obj, dt);
		}
	}
}

double DistanceInPx(Obj* obj1, Obj* obj2)
{
	Vector2D dist = obj1->position - obj2->position;
	return dist.GetLength();
}
