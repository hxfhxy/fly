#include <stdio.h>
#include <graphics.h>

IMAGE bk;//背景图片
IMAGE img_role;//飞机图片
IMAGE bull;//子弹图片
IMAGE bad[2];//敌机图片
IMAGE fat;//肥仔技能

int kill_count = 0;//杀敌数量

//定义子弹结构体
struct Zidan {
    int x;
    int y;
    bool live;
};
Zidan zidan[15];

//定义fat结构体
struct fat {
    int x;
    float y;
    bool live;
    int cd;
};
struct fat jinen;

//定义敌机结构体
struct Enemy {
    int x;
    float y;
    bool live;
    int type; // 敌机类型 0或1
    float speed;
    int hp;
};
Enemy enemies[10]; // 10个敌机

//定义飞机结构体
struct plane {
    float x;
    float y;
    bool live;//是否死亡
    int zidan_num;
    int width;
    int height;
    int hp=5;//血量
    int type;//类型
};
struct plane player;
bool canfire = true;
int time = 0;

//飞机数据
void game() {
    player.x = 390;
    player.y = 900;
    player.live = true;
    player.zidan_num = 15;
    player.hp = 5; // 初始化血量
    kill_count = 0; // 初始化击杀数
    // 初始化所有子弹
    for (int i = 0; i < 15; i++) {
        zidan[i].x = 0;
        zidan[i].y = 0;
        zidan[i].live = false;
    }
    //初始化敌机
    for (int i = 0; i < 10; i++) {
        enemies[i].live = false;
    }
    //初始化肥仔
    jinen.live = false;
    jinen.cd = 0;
}

//初始化
void loadimg() {
    //初始化图片
    loadimage(&bk, "./picture/微信图片_20250915184342_620_27.jpg");
    loadimage(&img_role, "./picture/微信图片_20250915164055_71_1.jpg",70,70);
    loadimage(&bad[1], "./picture/微信图片_20250915183756_38_249.jpg",60,60);
    loadimage(&bad[0], "./picture/微信图片_20250916122621_87_1.jpg", 60, 60);
    loadimage(&bull, "./picture/微信图片_20250915233803_647_27.jpg", 20, 30);
    loadimage(&fat, "./picture/微信图片_20250917160816_108_1.jpg", 300, 300);
}

//子弹敌机碰撞检测函数
void check_collision() {
    for (int i = 0; i < 15; i++) {    // 遍历所有子弹
        if (zidan[i].live) {
            for (int j = 0; j < 10; j++) { // 遍历所有敌机
                if (enemies[j].live) {
                    // 检测碰撞（简单的矩形碰撞检测）
                    if (zidan[i].x > enemies[j].x &&zidan[i].x < enemies[j].x + 60 &&zidan[i].y > enemies[j].y && zidan[i].y < enemies[j].y + 60) {

                        // 命中敌机
                        enemies[j].hp--; // 减少敌机血量
                        zidan[i].live = false; // 子弹消失

                        // 如果敌机血量耗尽
                        if (enemies[j].hp <= 0) {
                            enemies[j].live = false; // 敌机被击毁
                            kill_count++;
                        }
                        break; // 一颗子弹只能打一个敌机
                    }
                }
            }
        }
    }
}

//敌机玩家碰撞检测
void check_player() {
    for (int j = 0; j < 10; j++) {
        if (enemies[j].live) {
            if (player.x+70 > enemies[j].x && player.x<enemies[j].x+60  && player.y+70>enemies[j].y && player.y<enemies[j].y+60) {
                //相撞
                enemies[j].live = false;
                player.hp--;
                kill_count++;
                if (player.hp == 0) {
                    player.live = false;
                    settextstyle(60, 0, "宋体");
                    outtextxy(350, 500, "GAME OVER");
                    // 添加Q键提示
                    settextstyle(30, 0, "宋体");
                    outtextxy(300, 600, "按Q键重新开始");
                    FlushBatchDraw();
                    Sleep(1000);
                }
            }
        }
    }
}

//技能敌军碰撞检测
void check_jinen() {
    if (jinen.live==false) return; // 技能未激活则跳过检测
    for (int j = 0; j < 10; j++) { // 遍历所有敌机
        if (enemies[j].live) {
            // 碰撞检测：匹配肥仔技能图片尺寸（200x200），扩大检测范围
            if (jinen.x + 280 > enemies[j].x &&    // 技能右边界 > 敌机左边界
                jinen.x < enemies[j].x + 60 &&     // 技能左边界 < 敌机右边界
                jinen.y + 280 > enemies[j].y &&    // 技能下边界 > 敌机上边界
                jinen.y < enemies[j].y + 60) {     // 技能上边界 < 敌机下边界
                // 秒杀敌机：直接设为死亡，无需减血量
                enemies[j].live = false;
                kill_count++; // 计入击杀数
            }
        }
    }
}

//绘制所有的图片
void gamedraw() {
    //清除上一帧
    cleardevice();
    //输出背景
    putimage(0, 0, &bk);
    //输出飞机
    putimage(player.x, player.y, &img_role);
    // 绘制所有敌机
    for (int i = 0; i < 10; i++) {
        if (enemies[i].live) {
            putimage(enemies[i].x, enemies[i].y, &bad[enemies[i].type]);
        }
    }
    //绘制所有子弹
    for (int i = 0; i < 15; i++) {
        if (zidan[i].live) {
            putimage(zidan[i].x, zidan[i].y, &bull);
        }
    }
    if (jinen.live) {
        putimage(jinen.x, jinen.y, &fat);
    }
    //技能显示
    if (jinen.live) {
        putimage(jinen.x, jinen.y, &fat);
    }
    // 显示玩家血量
    settextstyle(24, 0, "宋体");
    settextcolor(RED);
    char hp_str[30];
    sprintf_s(hp_str, "当前血量: %d", player.hp);
    outtextxy(20, 20, hp_str);

    // 显示击败敌机数量
    settextcolor(YELLOW);
    char kill_str[30];
    sprintf_s(kill_str, "击毁敌机: %d 架", kill_count);
    outtextxy(20, 50, kill_str);
}

//创造子弹
void create() {
    for (int i = 0; i < 15; i++) {
        if (zidan[i].live == false) {
            zidan[i].y =player.y+5;
            zidan[i].x = player.x+25;
            zidan[i].live = true;
            break; 
        }
    }
}

//子弹移动
void zidan_move() {
    for (int i = 0; i < 15; i++) {
        if (zidan[i].live) {
            zidan[i].y -= 1;
            if (zidan[i].y < 0) {
                zidan[i].live = false;
            }
        }
    }
}

//创建肥仔技能
void create_fat() {
    // 冷却未结束或技能已激活时，无法再次发射
    if (jinen.cd > 0 || jinen.live) {
        return;
    }
    // 技能从飞机中心发射（匹配200x200图片，居中对齐）
    jinen.y = player.y - 60;
    jinen.x = player.x - 65;
    jinen.live = true;
    jinen.cd = 15000; // 技能冷却时间（数值越大冷却越长）
}

//技能移动
void jinen_move() {
    // 技能冷却倒计时
    if (jinen.cd > 0) jinen.cd--;
    // 技能移动：速度0.1
    if (jinen.live) {
        jinen.y -= 0.1;
        // 技能飞出屏幕顶部后，设为未激活
        if (jinen.y < -200) {
            jinen.live = false;
        }
    }
}

//创建敌机
void create_enemy() {
    static int enemy_timer = 0;
    enemy_timer++;

    if (enemy_timer >= 500) { // 每500帧创建一个敌机
        enemy_timer = 0;
        for (int i = 0; i < 10; i++) {
            if (!enemies[i].live) {
                enemies[i].x = rand() % 750; // 随机位置0-749
                enemies[i].y = -50; // 从屏幕上方出现
                enemies[i].live = true;
                enemies[i].type = rand() % 2; // 随机类型0-1
                enemies[i].speed = 0.3; // 0.3的速度

                // 设置不同敌机的血量
                if (enemies[i].type == 0) { // bad[0]类型
                    enemies[i].hp = 5; // 血量为10
                }
                else { // bad[1]类型
                    enemies[i].hp = 10; // 血量为1
                }
                break;
            }
        }
    }
}

//敌机移动
void enemy_move() {
    for (int i = 0; i < 10; i++) {
        if (enemies[i].live) {
            enemies[i].y += enemies[i].speed; // 敌机向下移动

            // 如果敌机飞出屏幕底部，设为不活跃
            if (enemies[i].y > 1000) {
                enemies[i].live = false;
            }
        }
    }
}

void reset_game() {
    // 重置玩家状态
    player.x = 390;
    player.y = 900;
    player.live = true;
    player.hp = 5;
    kill_count = 0;

    // 重置子弹
    for (int i = 0; i < 15; i++) {
        zidan[i].live = false;
    }

    // 重置敌机
    for (int i = 0; i < 10; i++) {
        enemies[i].live = false;
    }

    // 重置技能
    jinen.live = false;
    jinen.cd = 0;
}

//控制移动射击
void move(float speed) {
    // W - 向上移动
    if (GetAsyncKeyState('W') & 0x8000) {
        if (player.y > 0) {
            player.y -= speed;
        }
    }
    // S - 向下移动
    if (GetAsyncKeyState('S') & 0x8000) {
        if (player.y < 930) {
            player.y += speed;
        }
    }
    // A - 向左移动
    if (GetAsyncKeyState('A') & 0x8000) {
        if (player.x > 0) {
            player.x -= speed;
        }
    }
    // D - 向右移动
    if (GetAsyncKeyState('D') & 0x8000) {
        if (player.x < 730) {
            player.x += speed;
        }
    }
    //空格开火
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        create();
    }
    //肥仔
    if (GetAsyncKeyState('R') & 0x8000) {
        create_fat();
    }
}

int main() {
    initgraph(800, 1000, 0);//绘制画布
    loadimg();//函数   初始化图片
    game();//让飞机出现在中间，初始化子弹敌机
    //双缓存机制，让图片不一闪一闪
    BeginBatchDraw();//开始绘制
    while (1) {
        if (GetAsyncKeyState('Q') & 0x8000) {
            reset_game();
            Sleep(200); // 防止连续触发
        }
    
        // 1. 输入控制（先处理按键，避免技能创建被延后）
        move(0.7);

        // 2. 实体创建（敌机、子弹由按键触发，已在move中处理）
        create_enemy();

        // 3. 实体移动
        zidan_move();    // 子弹移动
        enemy_move();    // 敌机移动
        jinen_move();    // 技能移动（必须在碰撞检测前）

        // 4. 碰撞检测
        check_collision(); // 子弹-敌机
        check_jinen();     // 技能-敌机
        check_player();    // 敌机-玩家

        // 5. 绘制所有内容（最后执行，确保用最新状态）
        gamedraw();
        FlushBatchDraw();
    }

    closegraph();
    return 0;
}