// SDL_FirstApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "SDL3/SDL.h"

static int done = 0;

static void print_string(char** text, size_t* maxlen, const char* fmt, ...)
{
    int len;
    va_list ap;

    va_start(ap, fmt);
    len = SDL_vsnprintf(*text, *maxlen, fmt, ap);
    if (len > 0) {
        *text += len;
        if (((size_t)len) < *maxlen) {
            *maxlen -= (size_t)len;
        }
        else {
            *maxlen = 0;
        }
    }
    va_end(ap);
}

static void print_modifiers(char** text, size_t* maxlen, SDL_Keymod mod)
{
    print_string(text, maxlen, " modifiers:");
    if (mod == SDL_KMOD_NONE) {
        print_string(text, maxlen, " (none)");
        return;
    }
    if ((mod & SDL_KMOD_SHIFT) == SDL_KMOD_SHIFT) {
        print_string(text, maxlen, " SHIFT");
    }
    else {
        if (mod & SDL_KMOD_LSHIFT) {
            print_string(text, maxlen, " LSHIFT");
        }
        if (mod & SDL_KMOD_RSHIFT) {
            print_string(text, maxlen, " RSHIFT");
        }
    }
    if ((mod & SDL_KMOD_CTRL) == SDL_KMOD_CTRL) {
        print_string(text, maxlen, " CTRL");
    }
    else {
        if (mod & SDL_KMOD_LCTRL) {
            print_string(text, maxlen, " LCTRL");
        }
        if (mod & SDL_KMOD_RCTRL) {
            print_string(text, maxlen, " RCTRL");
        }
    }
    if ((mod & SDL_KMOD_ALT) == SDL_KMOD_ALT) {
        print_string(text, maxlen, " ALT");
    }
    else {
        if (mod & SDL_KMOD_LALT) {
            print_string(text, maxlen, " LALT");
        }
        if (mod & SDL_KMOD_RALT) {
            print_string(text, maxlen, " RALT");
        }
    }
    if ((mod & SDL_KMOD_GUI) == SDL_KMOD_GUI) {
        print_string(text, maxlen, " GUI");
    }
    else {
        if (mod & SDL_KMOD_LGUI) {
            print_string(text, maxlen, " LGUI");
        }
        if (mod & SDL_KMOD_RGUI) {
            print_string(text, maxlen, " RGUI");
        }
    }
    if (mod & SDL_KMOD_NUM) {
        print_string(text, maxlen, " NUM");
    }
    if (mod & SDL_KMOD_CAPS) {
        print_string(text, maxlen, " CAPS");
    }
    if (mod & SDL_KMOD_MODE) {
        print_string(text, maxlen, " MODE");
    }
    if (mod & SDL_KMOD_SCROLL) {
        print_string(text, maxlen, " SCROLL");
    }
}

static void PrintKey(SDL_KeyboardEvent* event)
{
    char message[512];
    char* spot;
    size_t left;

    spot = message;
    left = sizeof(message);

    /* Print the keycode, name and state */
    if (event->key) {
        print_string(&spot, &left,
            "Key %s:  raw 0x%.2x, scancode %d = %s, keycode 0x%08X = %s ",
            event->down ? "pressed " : "released",
            event->raw,
            event->scancode,
            event->scancode == SDL_SCANCODE_UNKNOWN ? "UNKNOWN" : SDL_GetScancodeName(event->scancode),
            event->key, SDL_GetKeyName(event->key));
    }
    else {
        print_string(&spot, &left,
            "Unknown Key (raw 0x%.2x, scancode %d = %s) %s ",
            event->raw,
            event->scancode,
            event->scancode == SDL_SCANCODE_UNKNOWN ? "UNKNOWN" : SDL_GetScancodeName(event->scancode),
            event->down ? "pressed " : "released");
    }
    print_modifiers(&spot, &left, event->mod);
    if (event->repeat) {
        print_string(&spot, &left, " (repeat)");
    }
    SDL_Log("%s\n", message);
}

static void loop(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            PrintKey(&event.key);
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                case SDLK_BACKSPACE:
                    break;
                case SDLK_RETURN:
                    break;
                case SDLK_ESCAPE:
                    done = 1;
                    std::cout << "Escape key pressed..." << std::endl;
                    break;
                default:
                    break;
                }
            }
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            std::cout << "Window minimized" << std::endl;
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            std::cout << "Window maximized" << std::endl;
            break;
        case SDL_EVENT_TEXT_EDITING:
        {
            break;
        }
        case SDL_EVENT_TEXT_INPUT:
            break;
        case SDL_EVENT_FINGER_DOWN:
        {
            SDL_Window* window = SDL_GetWindowFromEvent(&event);
            if (SDL_TextInputActive(window)) {
                SDL_Log("Stopping text input for window %" SDL_PRIu32 "\n", event.tfinger.windowID);
                SDL_StopTextInput(window);
            }
            else {
                SDL_Log("Starting text input for window %" SDL_PRIu32 "\n", event.tfinger.windowID);
                SDL_StartTextInput(window);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_RIGHT) {
                SDL_Window* window = SDL_GetWindowFromEvent(&event);
                if (SDL_TextInputActive(window)) {
                    SDL_Log("Stopping text input for window %" SDL_PRIu32 "\n", event.button.windowID);
                    SDL_StopTextInput(window);
                }
                else {
                    SDL_Log("Starting text input for window %" SDL_PRIu32 "\n", event.button.windowID);
                    SDL_StartTextInput(window);
                }
            }
            break;
        case SDL_EVENT_KEYMAP_CHANGED:
            SDL_Log("Keymap changed!\n");
            break;
        case SDL_EVENT_QUIT:
            done = 1;
            std::cout << "Quitting..." << std::endl;
            break;
        default:
            break;
        }
    }
}

// 参考SDL源码 \SDL\test\checkkeys.c
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "无法初始化SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    Uint32 flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("SDL_Main",
        640, 480,
        flags);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_ShowWindow(window);

    // 消息循环
    while (!done) {
        loop();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
