#include <iostream>
using namespace std;

// 常量定义
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo . #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

// 枚举类型
enum Object {
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN
};

// 函数声明
void Initialize(Object* state, int w, int h, const char* stageData);
void Draw(const Object* state, int w, int h);
void Update(Object* state, char input, int w, int h);
bool CheckClear(const Object* state, int w, int h);

int main()
{
    // 创建状态数组
    Object* state = new Object[gStageWidth * gStageHeight];
    // 初始化场景
    Initialize(state, gStageWidth, gStageHeight, gStageData);
    // 主循环
    while (true)
    {
        // 首先绘制
        Draw(state, gStageWidth, gStageHeight);
        // 通关检测
        if (CheckClear(state, gStageWidth, gStageHeight))
            break;
        // 获取输入
        cout << "[a]:Left [s]:Down [d]:Right [w]:Up, Input your command:" << endl;
        char input;
        cin >> input;
        // 更新数据
        Update(state, input, gStageWidth, gStageHeight);
    }
    
    // 胜利时的提示
    cout << "Congratulations! You win. :)" << endl;
    // 通关后处理
    delete[] state;
    state = 0;

    return 0;
}

// 函数实现
void Initialize(Object* state, int width, int height, const char* stageData)
{
    const char* d = stageData;
    int x = 0, y = 0;
    while (*d != '\0')
    {
        Object t;
        switch (*d)
        {
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O': t = OBJ_BLOCK_ON_GOAL; break;
            case '.': t = OBJ_GOAL; break;
            case 'p': t = OBJ_MAN; break;
            case 'P': t = OBJ_MAN_ON_GOAL; break;
            case '\n':
                x = 0;
                ++y;
                t = OBJ_UNKNOWN;
                break;
            default: t = OBJ_UNKNOWN; break;
        }
        ++d;
        if (t != OBJ_UNKNOWN)
        {
            state[y * width + x] = t;
            ++x;
        }
    }
}

void Draw(const Object* state, int width, int height)
{
    const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Object o = state[y*width + x];
            cout << font[o];
        }
        cout << endl;
    }
}

void Update(Object* state, char input, int width, int height)
{
    // 输入
    int dx = 0, dy = 0;
    switch(input)
    {
        case 'a': dx = -1; break;
        case 's': dy = 1; break;
        case 'd': dx = 1; break;
        case 'w': dy = -1; break;
    }

    // 检测玩家位置
    int i = -1;
    for (i = 0; i < width * height; ++i)
    {
        if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
        {
            break;
        }
    }
    
    // 移动
    int x = i % width;
    int y = i / width;
    int tx = x + dx;
    int ty = y + dy;
    // 若超出范围，则无视
    if (tx < 0 || tx >= width || ty < 0 || ty >= height)
        return;
    // 检测要移动到的地方是否为空地
    int p = i; // 玩家当前位置
    int tp = ty * width + tx; // 玩家移动的目标位置
    if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
    {
        // => 空地 / 目标点
        state[tp] = state[tp] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        state[p] = state[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
    }
    else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
    {
        // => 箱子
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || tx2 >= width || ty2 < 0 || ty >= height)
            return;
        int tp2 = ty2 * width + tx2;
        if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
        {
            state[tp2] = state[tp2] == OBJ_GOAL ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            state[tp] = state[tp] == OBJ_BLOCK_ON_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            state[p] = state[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool CheckClear(const Object* state, int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (state[y * width + x] == OBJ_BLOCK)
                return false;
        }
    }
    return true;
}