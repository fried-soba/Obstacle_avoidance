#include "Define.h"
#include "AI.h"
#include <DxLib.h>
#include <time.h>

//‘åˆæ•Ï”‚ÌéŒ¾‚ğW–ñ

int tmp = SRand((unsigned int)time(NULL));
Grid **g_grid = new Grid*[Define::WIN_W];
Goal g_goal;