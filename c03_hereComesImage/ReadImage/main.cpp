#include "Tool.h"
#include "Image.h"
#include <fstream>

namespace GameLib
{
    // ö������
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

    static bool isInit;
    static bool isOver;
    static Object* state;

    int gMapWidth = 0;
    int gMapHeight = 0;

	Image* img;

    // Framework
    void GetDimension(const char* content, int& width, int& height);
    void Recycle();
    // Logic
    void Initialize(Object* state, int w, int h, const char* stageData);
    void Update(Object* state, char input, int w, int h);
    bool CheckClear(const Object* state, int w, int h);
    // Render
    void DrawCell(int pixelX, int pixelY, GAME_IMAGE type);
    void Draw(const Object* state, int w, int h);

	void Framework::update()
	{
        if (isOver)
            return;

        if (!isInit)
        {
            // ��ȡͼƬ������
            img = new Image("nimotsuKunImage.dds");
            // ��ȡ�����ļ�
            char* content = ReadFile("input.txt");
            GetDimension(content, gMapWidth, gMapHeight);
            // ����״̬����
            state = new Object[gMapWidth * gMapHeight];
            // ��ʼ������
            Initialize(state, gMapWidth, gMapHeight, content);
            // ����
            Draw(state, gMapWidth, gMapHeight);

            // Clear
            delete[] content;
            content = 0;

            isInit = true;
            return;
        }

        // ��ѭ��
        // ͨ�ؼ��
        if (CheckClear(state, gMapWidth, gMapHeight))
        {
            // ʤ��ʱ����ʾ
            cout << "Congratulations! You win. :)" << endl;
            // ͨ�غ���
            Recycle();

            isOver = true;
            return;
        }

        cout << "[a]:Left [s]:Down [d]:Right [w]:Up, Input your command:" << endl;
        char input;
        cin >> input;

        // ��������
        Update(state, input, gMapWidth, gMapHeight);
        // ����
        Draw(state, gMapWidth, gMapHeight);

        if (input == 'q')
        {
            // Clear
            Recycle();
        }
	}

    void GetDimension(const char* content, int& width, int& height)
    {
        const char* p = content;
        int w = 0, h = 1;

        while (*p != '\0')
        {
            if (w == 0)
            {
                while (*p != '\n' && *p != '\r')
                {
                    ++w;
                    p = p + 1;
                }
                ++h;
                p = p + 1;
            }
            else
            {
                p = p + 1;
                if (*p == '\n')
                    ++h;
            }
        }

        width = w;
        height = h;
    }

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
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                GAME_IMAGE type;
                switch (state[y * width + x])
                {
                case OBJ_BLOCK_ON_GOAL:
                    type = GAME_IMAGE_BLOCK_ON_GOAL;
                    break;
                case OBJ_BLOCK:
                    type = GAME_IMAGE_BLOCK;
                    break;
                case OBJ_MAN:
                case OBJ_MAN_ON_GOAL:
                    type = GAME_IMAGE_PLAYER;
                    break;
                case OBJ_GOAL:
                    type = GAME_IMAGE_GOAL;
                    break;
                case OBJ_WALL:
                    type = GAME_IMAGE_WALL;
                    break;
                default:
                    type = GAME_IMAGE_PLAIN;
                    break;
                }
                DrawCell(x, y, type);
            }
        }
    }

    void Update(Object* state, char input, int width, int height)
    {
        // ����
        int dx = 0, dy = 0;
        switch (input)
        {
        case 'a': dx = -1; break;
        case 's': dy = 1; break;
        case 'd': dx = 1; break;
        case 'w': dy = -1; break;

        case 'q': Framework::instance().requestEnd(); break;
        }

        // ������λ��
        int i = -1;
        for (i = 0; i < width * height; ++i)
        {
            if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
            {
                break;
            }
        }

        // �ƶ�
        int x = i % width;
        int y = i / width;
        int tx = x + dx;
        int ty = y + dy;
        // ��������Χ��������
        if (tx < 0 || tx >= width || ty < 0 || ty >= height)
            return;
        // ���Ҫ�ƶ����ĵط��Ƿ�Ϊ�յ�
        int p = i; // ��ҵ�ǰλ��
        int tp = ty * width + tx; // ����ƶ���Ŀ��λ��
        if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
        {
            // => �յ� / Ŀ���
            state[tp] = state[tp] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            state[p] = state[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
        }
        else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
        {
            // => ����
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

    void Recycle()
    {
        delete[] state;
        state = 0;
    }

    void DrawCell(int cellX, int cellY, GAME_IMAGE type)
    {
        unsigned winWidth = Framework::instance().width();
        unsigned* vram = Framework::instance().videoMemory();
        const unsigned* beg = img->GetBlockBegCoord(type);
        int offset = cellY * img->BlockSize() * winWidth + cellX * img->BlockSize();

        for (unsigned y = 0; y < img->BlockSize(); ++y)
        {
            for (unsigned x = 0; x < img->BlockSize(); ++x)
            {
                vram[offset + y * winWidth + x] = beg[y * img->Width() + x];
            }
        }
    }
}
