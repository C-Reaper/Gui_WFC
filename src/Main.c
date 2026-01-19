#if defined __linux__
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif


typedef unsigned int WFC_Option;

#define WFC_OPTION_NONE		0b0U
#define WFC_OPTION_U		0b1U
#define WFC_OPTION_D		0b10U
#define WFC_OPTION_L		0b100U
#define WFC_OPTION_R		0b1000U
#define WFC_OPTION_LU		0b10000U
#define WFC_OPTION_LD		0b100000U
#define WFC_OPTION_RU		0b1000000U
#define WFC_OPTION_RD		0b10000000U

WFC_Option WFC_Option_Inv(WFC_Option op){
	switch (op){
		case WFC_OPTION_NONE: 	return WFC_OPTION_NONE;
		case WFC_OPTION_U: 		return WFC_OPTION_D;
		case WFC_OPTION_D: 		return WFC_OPTION_U;
		case WFC_OPTION_L: 		return WFC_OPTION_R;
		case WFC_OPTION_R: 		return WFC_OPTION_L;
		case WFC_OPTION_LU: 	return WFC_OPTION_RD;
		case WFC_OPTION_LD: 	return WFC_OPTION_RU;
		case WFC_OPTION_RU: 	return WFC_OPTION_LD;
		case WFC_OPTION_RD: 	return WFC_OPTION_LU;
	}
	return WFC_OPTION_NONE;
}


typedef unsigned int WFC_Tile;

#define WFC_TILE_NONE		WFC_OPTION_NONE
#define WFC_TILE_UP			(WFC_OPTION_L | WFC_OPTION_U | WFC_OPTION_R)
#define WFC_TILE_DOWN		(WFC_OPTION_L | WFC_OPTION_D | WFC_OPTION_R)
#define WFC_TILE_LEFT		(WFC_OPTION_U | WFC_OPTION_L | WFC_OPTION_D)
#define WFC_TILE_RIGHT		(WFC_OPTION_U | WFC_OPTION_R | WFC_OPTION_D)

unsigned int WFC_Tile_Index(WFC_Option op){
	switch (op){
		case WFC_TILE_NONE: 	return 0U;
		case WFC_TILE_UP: 		return 1U;
		case WFC_TILE_DOWN: 	return 2U;
		case WFC_TILE_LEFT: 	return 3U;
		case WFC_TILE_RIGHT: 	return 4U;
	}
	return 0U;
}



#define WFC_TILE_X	100
#define WFC_TILE_Y	100

TransformedView tv;
Vector sprites;
WFC_Tile* map;

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.25f,0.25f },
		(float)GetWidth() / (float)GetHeight()
	);

	sprites = Vector_New(sizeof(Sprite));
	Vector_PushCount(&sprites,(Sprite[]){
		Sprite_Load("./assets/NONE.png"),
		Sprite_Load("./assets/UP.png"),
		Sprite_Load("./assets/DOWN.png"),
		Sprite_Load("./assets/LEFT.png"),
		Sprite_Load("./assets/RIGHT.png")
	},5);

	map = (WFC_Tile*)malloc(sizeof(WFC_Tile) * WFC_TILE_X * WFC_TILE_Y);
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,w->Strokes,GetMouse());

	const Vec2 size = TransformedView_WorldScreenLength(&tv,(Vec2){ 1.0f,1.0f });
	for(int i = 0;i<sprites.size;i++){
		Sprite* sp = (Sprite*)Vector_Get(&sprites,i);
		Sprite_Reload(sp,size.x,size.y);
	}

	Clear(BLACK);

	const Vec2 lt = TransformedView_ScreenWorldPos(&tv,(Vec2){ 0.0f,0.0f });
	const Vec2 rb = TransformedView_ScreenWorldPos(&tv,(Vec2){ GetWidth(),GetHeight() });

	for (float y = lt.y;y < rb.y + 1.0f;y+=1.0f){
		for (float x = lt.x;x < rb.x + 1.0f;x+=1.0f){
			const unsigned int px = (unsigned int)x;
			const unsigned int py = (unsigned int)y;
			WFC_Tile t;

			if(px >= WFC_TILE_X || py >= WFC_TILE_Y)
				continue;
			else
				t = WFC_Tile_Index(map[py * WFC_TILE_X + px]);

			const Vec2 spos = TransformedView_WorldScreenPos(&tv,(Vec2){ px,py });
			
			Sprite* const sp = (Sprite*)Vector_Get(&sprites,t);
			Sprite_Render(WINDOW_STD_ARGS,sp,spos.x,spos.y);

			Rect_RenderXXWire(WINDOW_STD_ARGS,spos.x,spos.y,size.x,size.y,WHITE,1.0f);
		}
	}
}
void Delete(AlxWindow* w){
	for(int i = 0;i<sprites.size;i++){
		Sprite* sp = (Sprite*)Vector_Get(&sprites,i);
		Sprite_Free(sp);		
	}
	Vector_Free(&sprites);

	if(map) free(map);
	map = NULL;
}

int main(){
    if(Create("Wave Function Collapse",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}