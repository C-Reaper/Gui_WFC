#if defined __linux__
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif


Vector points;

void Setup(AlxWindow* w){
	points = Vector_New(sizeof(Vec2));
}
void Update(AlxWindow* w){
	
	Clear(BLACK);

	for (int i = 0;i<points.size;i++){
		Vec2 pt = *(Vec2*)Vector_Get(&points,i);
		Circle_RenderX(WINDOW_STD_ARGS,pt,5.0f,WHITE);
	}
}
void Delete(AlxWindow* w){
	Vector_Free(&points);
}

int main(){
    if(Create("Voronoi",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}