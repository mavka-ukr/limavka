#pragma once

#include <lilka.h>
#include "keira/app.h"

class MavkaFileRunnerApp : public App {
public:
    explicit MavkaFileRunnerApp(String path);
    void run() override;

private:
    String path;
};
