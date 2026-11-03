#include <cstring>
#include "lilka.h"
#include "keira/keira.h"
#include "./translated/limavka.h"

MavkaFileRunnerApp::MavkaFileRunnerApp(String path) : App("MavkaFileRunner"), path(path) {
    setktStackSize(16384);
}

void MavkaFileRunnerApp::run() {
    limavka_run_file(this, path.c_str(), path.length());
}
