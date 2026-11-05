#include <lilka.h>
#include "keira/app.h"
#include "./tsilint.h"
#include "./limavka.h"

extern "C" {
адреса лімавка_виділити(природне розмір) {
    return ps_malloc(розмір);
}

адреса лімавка_перевиділити(адреса значення, природне новий_розмір) {
    return ps_realloc(значення, новий_розмір);
}

void лімавка_звільнити(адреса значення) {
    free(значення);
}

void лімавка_вивести_ю8(п8* дані, природне розмір) {
    printf("%.*s", розмір, (char*)дані);
}

логічне лімавка_отримати_р64_з_ю8(п8* дані, природне розмір, р64* вихід) {
    char* str = (char*)ps_malloc(розмір + 1);
    природне len = 0;
    for (природне і = 0; і < розмір; і++) {
        unsigned char c = (unsigned char)дані[і];
        if (c == 0xD0 || c == 0xD1) {
            if (і + 1 < розмір) {
                unsigned char c2 = (unsigned char)дані[і + 1];
                if (c == 0xD0 && c2 == 0xB5) {
                    str[len++] = 'e';
                    і++;
                    continue;
                }
                if (c == 0xD0 && c2 == 0x95) {
                    str[len++] = 'E';
                    і++;
                    continue;
                }
            }
        }

        str[len++] = (char)c;
    }
    str[len] = '\0';

    char* endptr;

    *вихід = strtod(str, &endptr);

    if (str == endptr) {
        free(str);
        return false;
    } else if (errno == ERANGE) {
        free(str);
        return false;
    } else if (*endptr != '\0') {
        free(str);
        return false;
    }

    free(str);
    return true;
}

р64 лімавка_степінь_р64(р64 значення, р64 степінь) {
    return pow(значення, степінь);
}

логічне лімавка_перетворити_п64_в_ю8(п64 значення, п8** вихід_даних, природне* вихід_розміру) {
    char буфер[32];
    int довжина = snprintf(буфер, sizeof(буфер), "%llu", (unsigned long long)значення);
    if (довжина < 0) {
        return false;
    }
    *вихід_розміру = (природне)довжина;
    *вихід_даних = (п8*)ps_malloc(*вихід_розміру);
    if (!*вихід_даних) {
        return false;
    }
    for (природне і = 0; і < *вихід_розміру; і++) {
        (*вихід_даних)[і] = (п8)буфер[і];
    }
    return true;
}

логічне лімавка_перетворити_р64_в_ю8(
    р64 значення, п8** вихід_даних, природне* вихід_розміру, природне* вихід_розміру_експоненти
) {
    char буфер[64];
    int довжина;

    if (isnan(значення)) {
        char nan_str[] = "невизначеність";
        *вихід_розміру = sizeof(nan_str) - 1;
        *вихід_даних = (п8*)ps_malloc(*вихід_розміру);
        if (!*вихід_даних) return false;
        memcpy(*вихід_даних, nan_str, *вихід_розміру);
        *вихід_розміру_експоненти = 0;
        return true;
    }

    if (isinf(значення)) {
        const char* inf_str = значення > 0 ? "нескінченність" : "-нескінченність";
        *вихід_розміру = значення > 0 ? sizeof("нескінченність") - 1 : sizeof("-нескінченність") - 1;
        *вихід_даних = (п8*)ps_malloc(*вихід_розміру);
        if (!*вихід_даних) return false;
        memcpy(*вихід_даних, inf_str, *вихід_розміру);
        *вихід_розміру_експоненти = 0;
        return true;
    }

    // Знаходимо найкоротше представлення, яке round-trips
    // Пробуємо від 15 до 17 цифр точності
    for (int precision = 15; precision <= 17; precision++) {
        довжина = snprintf(буфер, sizeof(буфер), "%.*g", precision, значення);
        if (довжина < 0 || довжина >= (int)sizeof(буфер)) {
            continue;
        }

        // Перевіряємо чи round-trip працює
        р64 parsed = strtod(буфер, NULL);
        if (parsed == значення) {
            break;
        }
    }

    if (довжина < 0 || довжина >= (int)sizeof(буфер)) {
        return false;
    }

    char* e_pos = strchr(буфер, 'e');
    if (e_pos != NULL) {
        *вихід_розміру_експоненти = (ціле)strtol(e_pos + 1, NULL, 10);

        природне довжина_мантиси = (природне)(e_pos - буфер);

        *вихід_розміру = довжина_мантиси;
        *вихід_даних = (п8*)ps_malloc(*вихід_розміру);
        if (!*вихід_даних) return false;

        for (природне і = 0; і < довжина_мантиси; і++) {
            (*вихід_даних)[і] = (п8)буфер[і];
        }
    } else {
        *вихід_розміру_експоненти = 0;

        *вихід_розміру = (природне)довжина;
        *вихід_даних = (п8*)ps_malloc(*вихід_розміру);
        if (!*вихід_даних) return false;

        for (природне і = 0; і < *вихід_розміру; і++) {
            (*вихід_даних)[і] = (п8)буфер[і];
        }
    }

    return true;
}

ціле лімавка_прочитати_файл(п8* дані_шляху, природне розмір_шляху, п8** вихід_даних, природне* вихід_розміру) {
    String path = String(дані_шляху, розмір_шляху);
    String* output;

    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)ps_malloc(fileSize + 1);

    fread(buffer, 1, fileSize, file);

    buffer[fileSize] = 0;

    *вихід_даних = (п8*)buffer;
    *вихід_розміру = fileSize;

    fclose(file);
    return 0;
}

ціле лімавка_отримати_шлях_до_файлу_кп(
    п8* дані_шляху, природне розмір_шляху, п8** вихід_даних, природне* вихід_розміру
) {
    String localPath = String(дані_шляху, розмір_шляху);
    String cannonicalPath = lilka::fileutils.getCannonicalPath(&SD, localPath);

    п8* data = (п8*)ps_malloc(cannonicalPath.length());
    for (size_t i = 0; i < cannonicalPath.length(); i++) {
        data[i] = cannonicalPath.c_str()[i];
    }

    *вихід_даних = data;
    *вихід_розміру = cannonicalPath.length();

    return true;
}

природне лімавка_полотно_ширина(адреса апп) {
    return static_cast<App*>(апп)->canvas->width();
}

природне лімавка_полотно_висота(адреса апп) {
    return static_cast<App*>(апп)->canvas->height();
}

ніщо лімавка_полотно_вдуга(адреса апп, ціле г, ціле в, ціле р1, ціле р2, р32 пк, р32 кк, природне колір) {
    static_cast<App*>(апп)->canvas->fillArc(г, в, р1, р2, пк, кк, колір);
}

ніщо лімавка_полотно_веліпс(адреса апп, ціле г, ціле в, ціле рг, ціле рв, природне колір) {
    static_cast<App*>(апп)->canvas->fillEllipse(г, в, рг, рв, колір);
}

ніщо лімавка_полотно_вквадрат(адреса апп, ціле г, ціле в, ціле вис, ціле шир, природне колір) {
    static_cast<App*>(апп)->canvas->fillRect(г, в, вис, шир, колір);
}

ніщо лімавка_полотно_втрикут(адреса апп, ціле г0, ціле в0, ціле г1, ціле в1, ціле г2, ціле в2, природне колір) {
    static_cast<App*>(апп)->canvas->fillTriangle(г0, в0, г1, в1, г2, в2, колір);
}

ніщо лімавка_полотно_дуга(адреса апп, ціле г, ціле в, ціле р1, ціле р2, р32 пк, р32 кк, природне колір) {
    static_cast<App*>(апп)->canvas->drawArc(г, в, р1, р2, пк, кк, колір);
}

ніщо лімавка_полотно_еліпс(адреса апп, ціле г, ціле в, ціле рг, ціле рв, природне колір) {
    static_cast<App*>(апп)->canvas->drawEllipse(г, в, рг, рв, колір);
}

ніщо лімавка_полотно_запланувати(адреса апп) {
    static_cast<App*>(апп)->queueDraw();
}

ніщо лімавка_полотно_заповнити(адреса апп, природне колір) {
    static_cast<App*>(апп)->canvas->fillScreen(колір);
}

ніщо лімавка_полотно_квадрат(адреса апп, ціле г, ціле в, ціле вис, ціле шир, природне колір) {
    static_cast<App*>(апп)->canvas->drawRect(г, в, вис, шир, колір);
}

ніщо лімавка_полотно_колір565(адреса апп, п8 ч, п8 з, п8 с) {
    static_cast<App*>(апп)->canvas->color565(ч, з, с);
}

ніщо лімавка_полотно_коло(адреса апп, ціле г, ціле в, ціле р, природне колір) {
    static_cast<App*>(апп)->canvas->drawCircle(г, в, р, колір);
}

ніщо лімавка_полотно_круг(адреса апп, ціле г, ціле в, ціле р, природне колір) {
    static_cast<App*>(апп)->canvas->fillCircle(г, в, р, колір);
}

ніщо лімавка_полотно_лінія(адреса апп, ціле г0, ціле в0, ціле г1, ціле в1, природне колір) {
    static_cast<App*>(апп)->canvas->drawLine(г0, в0, г1, в1, колір);
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
    адреса апп, ціле г, ціле в, п8* дані_значення, природне розмір_значення, природне шрифт, природне розмір,
    природне колір
) {
    const auto app = static_cast<App*>(апп);
    app->canvas->setCursor(г, в);
    app->canvas->setFont(fonts[шрифт]);
    app->canvas->setTextSize(розмір);
    app->canvas->setTextColor(колір);
    app->canvas->print(String(дані_значення, розмір_значення));
}

ніщо лімавка_полотно_точка(адреса апп, ціле г0, ціле в0, природне колір) {
    static_cast<App*>(апп)->canvas->drawPixel(г0, в0, колір);
}

ніщо лімавка_полотно_трикут(адреса апп, ціле г0, ціле в0, ціле г1, ціле в1, ціле г2, ціле в2, природне колір) {
    static_cast<App*>(апп)->canvas->drawTriangle(г0, в0, г1, в1, г2, в2, колір);
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

    вихід_стану->вверх.нажато = buttons[0].pressed;
    вихід_стану->вверх.щойно_нажато = buttons[0].justPressed;
    вихід_стану->вверх.щойно_відпущено = buttons[0].justReleased;
    вихід_стану->вниз.нажато = buttons[1].pressed;
    вихід_стану->вниз.щойно_нажато = buttons[1].justPressed;
    вихід_стану->вниз.щойно_відпущено = buttons[1].justReleased;
    вихід_стану->вліво.нажато = buttons[2].pressed;
    вихід_стану->вліво.щойно_нажато = buttons[2].justPressed;
    вихід_стану->вліво.щойно_відпущено = buttons[2].justReleased;
    вихід_стану->вправо.нажато = buttons[3].pressed;
    вихід_стану->вправо.щойно_нажато = buttons[3].justPressed;
    вихід_стану->вправо.щойно_відпущено = buttons[3].justReleased;
    вихід_стану->а.нажато = buttons[4].pressed;
    вихід_стану->а.щойно_нажато = buttons[4].justPressed;
    вихід_стану->а.щойно_відпущено = buttons[4].justReleased;
    вихід_стану->б.нажато = buttons[5].pressed;
    вихід_стану->б.щойно_нажато = buttons[5].justPressed;
    вихід_стану->б.щойно_відпущено = buttons[5].justReleased;
    вихід_стану->г.нажато = buttons[6].pressed;
    вихід_стану->г.щойно_нажато = buttons[6].justPressed;
    вихід_стану->г.щойно_відпущено = buttons[6].justReleased;
    вихід_стану->д.нажато = buttons[7].pressed;
    вихід_стану->д.щойно_нажато = buttons[7].justPressed;
    вихід_стану->д.щойно_відпущено = buttons[7].justReleased;
    вихід_стану->вибрати.нажато = buttons[8].pressed;
    вихід_стану->вибрати.щойно_нажато = buttons[8].justPressed;
    вихід_стану->вибрати.щойно_відпущено = buttons[8].justReleased;
    вихід_стану->почати.нажато = buttons[9].pressed;
    вихід_стану->почати.щойно_нажато = buttons[9].justPressed;
    вихід_стану->почати.щойно_відпущено = buttons[9].justReleased;
}

ціле лімавка_запустити_файл(адреса апп, п8* дані_шляху, природне розмір_шляху);
}

void limavka_run_file(void* app, const char* path_data, size_t path_size) {
    лімавка_запустити_файл(app, (п8*)path_data, (природне)path_size);
}