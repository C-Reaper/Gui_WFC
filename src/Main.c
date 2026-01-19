#if defined __linux__
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif


typedef unsigned char WFC_Option;

#define WFC_DIRECTIONS		4U

#define WFC_OPTIONS			2U
#define WFC_OPTION_NONE		0U
#define WFC_OPTION_1		1U
#define WFC_OPTION_2		2U



typedef struct WFC_Tile {
	WFC_Option u;
	WFC_Option d;
	WFC_Option l;
	WFC_Option r;
} WFC_Tile;

Vic2 WFC_Tile_Direction(unsigned int dir){
	if(dir == 0U) return (Vic2){  0,-1 };
	if(dir == 1U) return (Vic2){  0, 1 };
	if(dir == 2U) return (Vic2){ -1, 0 };
	if(dir == 3U) return (Vic2){  1, 0 };
	return (Vic2){  0, 0 };
}
char WFC_Tile_Visited(WFC_Tile t){
	return !(t.u == WFC_OPTION_NONE && t.d == WFC_OPTION_NONE && t.l == WFC_OPTION_NONE && t.r == WFC_OPTION_NONE);
}
unsigned int WFC_Tile_Index(WFC_Tile t){
	if(t.u == WFC_OPTION_2 && t.d == WFC_OPTION_2 && t.l == WFC_OPTION_2 && t.r == WFC_OPTION_2) 	return 15U;
	if(t.u == WFC_OPTION_2 && t.d == WFC_OPTION_2 && t.r == WFC_OPTION_2) 							return 14U;
	if(t.u == WFC_OPTION_2 && t.d == WFC_OPTION_2 && t.l == WFC_OPTION_2) 							return 13U;
	if(t.d == WFC_OPTION_2 && t.l == WFC_OPTION_2 && t.r == WFC_OPTION_2) 							return 12U;
	if(t.u == WFC_OPTION_2 && t.l == WFC_OPTION_2 && t.r == WFC_OPTION_2) 							return 11U;
	if(t.d == WFC_OPTION_2 && t.r == WFC_OPTION_2) 													return 10U;
	if(t.u == WFC_OPTION_2 && t.r == WFC_OPTION_2) 													return 9U;
	if(t.d == WFC_OPTION_2 && t.l == WFC_OPTION_2) 													return 8U;
	if(t.u == WFC_OPTION_2 && t.l == WFC_OPTION_2) 													return 7U;
	if(t.l == WFC_OPTION_2 && t.r == WFC_OPTION_2) 													return 6U;
	if(t.u == WFC_OPTION_2 && t.d == WFC_OPTION_2) 													return 5U;
	if(t.r == WFC_OPTION_2) 																		return 4U;
	if(t.l == WFC_OPTION_2) 																		return 3U;
	if(t.d == WFC_OPTION_2) 																		return 2U;
	if(t.u == WFC_OPTION_2) 																		return 1U;
	return 0U;
}



#define WFC_TILE_X	100
#define WFC_TILE_Y	100

TransformedView tv;
Vector sprites;
WFC_Tile* map;

void WFC_Tile_SetR(WFC_Tile* map,unsigned int w,unsigned int h,const Vic2 c){
	WFC_Tile* sett = map + c.y * w + c.x;
	WFC_Option* opt = (WFC_Option*)sett;
	
	for(unsigned int i = 0;i<WFC_DIRECTIONS;i++){

		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h){
			//opt[i] = (WFC_Option)Random_u32_MinMax(1U,1U + WFC_OPTIONS);
			opt[i] = 1U;
			continue;
		}

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		const unsigned int invi = (i / 2 * 2) + ((i + 1) % 2);
		WFC_Option const op = nop[invi];

		if(op > 0U)	opt[i] = op;
		else		opt[i] = (WFC_Option)Random_u32_MinMax(1U,1U + WFC_OPTIONS);
	}
}
void WFC_Tile_AllN(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	if(c.x >= w || c.y >= h) return;

	WFC_Tile_SetR(map,w,h,c);
	
	for(int i = 0;i<WFC_DIRECTIONS;i++){
		const Vic2 d = WFC_Tile_Direction(i);
		const Vic2 p = Vic2_Add(c,d);

		if(p.x >= w || p.y >= h)
			continue;

		WFC_Tile* const nt = map + p.y * w + p.x;
		WFC_Option* const nop = (WFC_Option*)nt;

		if(!WFC_Tile_Visited(*nt)){
			WFC_Tile_AllN(map,w,h,p);
		}
	}
}
void WFC_Tile_Fill(WFC_Tile* map,unsigned int w,unsigned int h,Vic2 c){
	WFC_Tile_AllN(map,w,h,c);
}

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.02f,0.02f },
		(float)GetWidth() / (float)GetHeight()
	);

	sprites = Vector_New(sizeof(Sprite));
	Vector_PushCount(&sprites,(Sprite[]){
		Sprite_Load("./assets/NONE.png"),
		Sprite_Load("./assets/U.png"),
		Sprite_Load("./assets/D.png"),
		Sprite_Load("./assets/L.png"),
		Sprite_Load("./assets/R.png"),
		Sprite_Load("./assets/UD.png"),
		Sprite_Load("./assets/LR.png"),
		Sprite_Load("./assets/LU.png"),
		Sprite_Load("./assets/LD.png"),
		Sprite_Load("./assets/RU.png"),
		Sprite_Load("./assets/RD.png"),
		Sprite_Load("./assets/LUR.png"),
		Sprite_Load("./assets/LDR.png"),
		Sprite_Load("./assets/ULD.png"),
		Sprite_Load("./assets/URD.png"),
		Sprite_Load("./assets/UDLR.png")
	},16);

	map = (WFC_Tile*)malloc(sizeof(WFC_Tile) * WFC_TILE_X * WFC_TILE_Y);
	memset(map,WFC_OPTION_NONE,sizeof(WFC_Tile) * WFC_TILE_X * WFC_TILE_Y);

	WFC_Tile_Fill(map,WFC_TILE_X,WFC_TILE_Y,(Vic2){ WFC_TILE_X / 2,WFC_TILE_Y / 2 });
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,w->Strokes,GetMouse());

	const Vec2 size = TransformedView_WorldScreenLength(&tv,(Vec2){ 1.0f,1.0f });
	for(int i = 0;i<sprites.size;i++){
		Sprite* sp = (Sprite*)Vector_Get(&sprites,i);
		Sprite_Reload(sp,size.x + 1,size.y + 1);
	}

	Clear(BLACK);

	const Vec2 lt = TransformedView_ScreenWorldPos(&tv,(Vec2){ 0.0f,0.0f });
	const Vec2 rb = TransformedView_ScreenWorldPos(&tv,(Vec2){ GetWidth(),GetHeight() });

	for (float y = lt.y;y < rb.y + 1.0f;y+=1.0f){
		for (float x = lt.x;x < rb.x + 1.0f;x+=1.0f){
			const unsigned int px = (unsigned int)x;
			const unsigned int py = (unsigned int)y;
			unsigned int it;

			if(px >= WFC_TILE_X || py >= WFC_TILE_Y)
				continue;
			else
				it = WFC_Tile_Index(map[py * WFC_TILE_X + px]);

			const Vec2 spos = TransformedView_WorldScreenPos(&tv,(Vec2){ px,py });
			
			Sprite* const sp = (Sprite*)Vector_Get(&sprites,it);
			Sprite_Render(WINDOW_STD_ARGS,sp,spos.x,spos.y);

			//WFC_Tile* sett = map + py * WFC_TILE_X + px;
			//WFC_Option* opt = (WFC_Option*)sett;
			//
			//for(unsigned int i = 0;i<WFC_DIRECTIONS;i++){
			//	const Vic2 d = WFC_Tile_Direction(i);
			//	const Vic2 pn = Vic2_Add((Vic2){ px,py },d);
			//	
			//	if(opt[i] == WFC_OPTION_2){
			//		Line_RenderX(WINDOW_STD_ARGS,
			//			(Vec2){ spos.x + size.x / 2,spos.y + size.y / 2 },
			//			(Vec2){ spos.x + size.x / 2 + d.x * size.x / 2,spos.y + size.y / 2 + d.y * size.y / 2 },
			//			BLACK,
			//			1.0f
			//		);
			//	}
			//}
			//Rect_RenderXXWire(WINDOW_STD_ARGS,spos.x,spos.y,size.x,size.y,WHITE,1.0f);
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