#include <lilka.h>
#include "keira/app.h"
#include "./tsilint.h"
#include "./limavka.h"

extern "C" {
void* pristriy_mavky_dlopen(char* path_data, size_t path_size, int* err_out) {
    return NULL;
}

void* pristriy_mavky_dlsym(void* dl, char* name_data, size_t name_size, int* err_out) {
    return NULL;
}

int pristriy_mavky_dlclose(void* dl) {
    return -1;
}

природне лімавка_полотно_ширина(адреса апп) {
    return static_cast<App*>(апп)->canvas->width();
}

природне лімавка_полотно_висота(адреса апп) {
    return static_cast<App*>(апп)->canvas->height();
}

ніщо лімавка_полотно_вдуга(адреса апп, ціле ікс, ціле ігр, ціле р1, ціле р2, р32 пк, р32 кк, природне колір) {
    static_cast<App*>(апп)->canvas->fillArc(ікс, ігр, р1, р2, пк, кк, колір);
}

ніщо лімавка_полотно_веліпс(адреса апп, ціле ікс, ціле ігр, ціле рікс, ціле рігр, природне колір) {
    static_cast<App*>(апп)->canvas->fillEllipse(ікс, ігр, рікс, рігр, колір);
}

ніщо лімавка_полотно_впрямокут(адреса апп, ціле ікс, ціле ігр, ціле вис, ціле шир, природне колір) {
    static_cast<App*>(апп)->canvas->fillRect(ікс, ігр, вис, шир, колір);
}

ніщо лімавка_полотно_втрикут(
    адреса апп, ціле ікс0, ціле ігр0, ціле ікс1, ціле ігр1, ціле ікс2, ціле ігр2, природне колір
) {
    static_cast<App*>(апп)->canvas->fillTriangle(ікс0, ігр0, ікс1, ігр1, ікс2, ігр2, колір);
}

ніщо лімавка_полотно_дуга(адреса апп, ціле ікс, ціле ігр, ціле р1, ціле р2, р32 пк, р32 кк, природне колір) {
    static_cast<App*>(апп)->canvas->drawArc(ікс, ігр, р1, р2, пк, кк, колір);
}

ніщо лімавка_полотно_еліпс(адреса апп, ціле ікс, ціле ігр, ціле рікс, ціле рігр, природне колір) {
    static_cast<App*>(апп)->canvas->drawEllipse(ікс, ігр, рікс, рігр, колір);
}

ніщо лімавка_полотно_відмалювати(адреса апп) {
    static_cast<App*>(апп)->queueDraw();
}

ніщо лімавка_полотно_заповнити(адреса апп, природне колір) {
    static_cast<App*>(апп)->canvas->fillScreen(колір);
}

ніщо лімавка_полотно_прямокут(адреса апп, ціле ікс, ціле ігр, ціле вис, ціле шир, природне колір) {
    static_cast<App*>(апп)->canvas->drawRect(ікс, ігр, вис, шир, колір);
}

ніщо лімавка_полотно_колір565(адреса апп, п8 ч, п8 з, п8 с) {
    static_cast<App*>(апп)->canvas->color565(ч, з, с);
}

ніщо лімавка_полотно_коло(адреса апп, ціле ікс, ціле ігр, ціле р, природне колір) {
    static_cast<App*>(апп)->canvas->drawCircle(ікс, ігр, р, колір);
}

ніщо лімавка_полотно_круг(адреса апп, ціле ікс, ціле ігр, ціле р, природне колір) {
    static_cast<App*>(апп)->canvas->fillCircle(ікс, ігр, р, колір);
}

ніщо лімавка_полотно_лінія(адреса апп, ціле ікс0, ціле ігр0, ціле ікс1, ціле ігр1, природне колір) {
    static_cast<App*>(апп)->canvas->drawLine(ікс0, ігр0, ікс1, ігр1, колір);
}

static const uint8_t* fonts[] = {
    u8g2_font_4x6_t_cyrillic,
    u8g2_font_5x7_t_cyrillic,
    u8g2_font_5x8_t_cyrillic,
    u8g2_font_6x12_t_cyrillic,
    u8g2_font_6x13_t_cyrillic,
    u8g2_font_7x13_t_cyrillic,
    u8g2_font_8x13_t_cyrillic,
    u8g2_font_9x15_t_cyrillic,
    u8g2_font_10x20_t_cyrillic,
};

ніщо лімавка_полотно_текст(
    адреса апп, ціле ікс, ціле ігр, п8* дані_значення, природне розмір_значення, природне шрифт, природне розмір,
    природне колір
) {
    const auto app = static_cast<App*>(апп);
    app->canvas->setCursor(ікс, ігр);
    app->canvas->setFont(fonts[шрифт]);
    app->canvas->setTextSize(розмір);
    app->canvas->setTextColor(колір);
    app->canvas->print(String(дані_значення, розмір_значення));
}

ніщо лімавка_полотно_точка(адреса апп, ціле ікс0, ціле ігр0, природне колір) {
    static_cast<App*>(апп)->canvas->drawPixel(ікс0, ігр0, колір);
}

ніщо лімавка_полотно_трикут(
    адреса апп, ціле ікс0, ціле ігр0, ціле ікс1, ціле ігр1, ціле ікс2, ціле ігр2, природне колір
) {
    static_cast<App*>(апп)->canvas->drawTriangle(ікс0, ігр0, ікс1, ігр1, ікс2, ігр2, колір);
}

typedef struct ЛімавкаСтанКнопки {
    логічне нажато;
    логічне щойно_нажато;
    логічне щойно_відпущено;
} ЛімавкаСтанКнопки;
typedef struct ЛімавкаСтанКнопок {
    ЛімавкаСтанКнопки вверх;
    ЛімавкаСтанКнопки вниз;
    ЛімавкаСтанКнопки вліво;
    ЛімавкаСтанКнопки вправо;
    ЛімавкаСтанКнопки а;
    ЛімавкаСтанКнопки б;
    ЛімавкаСтанКнопки г;
    ЛімавкаСтанКнопки д;
    ЛімавкаСтанКнопки вибрати;
    ЛімавкаСтанКнопки почати;
} ЛімавкаСтанКнопок;

ніщо лімавка_кнопки_отримати_стан(адреса апп, ЛімавкаСтанКнопок* вихід_стану) {
    lilka::State state = lilka::controller.getState();
    lilka::_StateButtons& buttons = *reinterpret_cast<lilka::_StateButtons*>(&state);

    вихід_стану->вверх.нажато = buttons[lilka::Button::UP].pressed;
    вихід_стану->вверх.щойно_нажато = buttons[lilka::Button::UP].justPressed;
    вихід_стану->вверх.щойно_відпущено = buttons[lilka::Button::UP].justReleased;

    вихід_стану->вниз.нажато = buttons[lilka::Button::DOWN].pressed;
    вихід_стану->вниз.щойно_нажато = buttons[lilka::Button::DOWN].justPressed;
    вихід_стану->вниз.щойно_відпущено = buttons[lilka::Button::DOWN].justReleased;

    вихід_стану->вліво.нажато = buttons[lilka::Button::LEFT].pressed;
    вихід_стану->вліво.щойно_нажато = buttons[lilka::Button::LEFT].justPressed;
    вихід_стану->вліво.щойно_відпущено = buttons[lilka::Button::LEFT].justReleased;

    вихід_стану->вправо.нажато = buttons[lilka::Button::RIGHT].pressed;
    вихід_стану->вправо.щойно_нажато = buttons[lilka::Button::RIGHT].justPressed;
    вихід_стану->вправо.щойно_відпущено = buttons[lilka::Button::RIGHT].justReleased;

    вихід_стану->а.нажато = buttons[lilka::Button::A].pressed;
    вихід_стану->а.щойно_нажато = buttons[lilka::Button::A].justPressed;
    вихід_стану->а.щойно_відпущено = buttons[lilka::Button::A].justReleased;

    вихід_стану->б.нажато = buttons[lilka::Button::B].pressed;
    вихід_стану->б.щойно_нажато = buttons[lilka::Button::B].justPressed;
    вихід_стану->б.щойно_відпущено = buttons[lilka::Button::B].justReleased;

    вихід_стану->г.нажато = buttons[lilka::Button::C].pressed;
    вихід_стану->г.щойно_нажато = buttons[lilka::Button::C].justPressed;
    вихід_стану->г.щойно_відпущено = buttons[lilka::Button::C].justReleased;

    вихід_стану->д.нажато = buttons[lilka::Button::D].pressed;
    вихід_стану->д.щойно_нажато = buttons[lilka::Button::D].justPressed;
    вихід_стану->д.щойно_відпущено = buttons[lilka::Button::D].justReleased;

    вихід_стану->вибрати.нажато = buttons[lilka::Button::SELECT].pressed;
    вихід_стану->вибрати.щойно_нажато = buttons[lilka::Button::SELECT].justPressed;
    вихід_стану->вибрати.щойно_відпущено = buttons[lilka::Button::SELECT].justReleased;

    вихід_стану->почати.нажато = buttons[lilka::Button::START].pressed;
    вихід_стану->почати.щойно_нажато = buttons[lilka::Button::START].justPressed;
    вихід_стану->почати.щойно_відпущено = buttons[lilka::Button::START].justReleased;
}

ціле лімавка_запустити_файл(адреса апп, п8* дані_шляху, природне розмір_шляху);
}

void limavka_run_file(void* app, const char* path_data, size_t path_size) {
    лімавка_запустити_файл(app, (п8*)path_data, (природне)path_size);
}