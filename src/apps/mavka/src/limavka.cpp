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

void лімавка_заповнити_жовтим() {
    lilka::display.fillScreen(lilka::colors::Yellow);
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

ніщо лімавка_полотно_заповнити(адреса апп, природне колір) {
    static_cast<App*>(апп)->canvas->fillScreen(колір);
}

ніщо лімавка_полотно_запланувати(адреса апп) {
    static_cast<App*>(апп)->queueDraw();
}

ціле лімавка_запустити_файл(адреса апп, п8* дані_шляху, природне розмір_шляху);
}

void limavka_run_file(void* app, const char* path_data, size_t path_size) {
    лімавка_запустити_файл(app, (п8*)path_data, (природне)path_size);
}