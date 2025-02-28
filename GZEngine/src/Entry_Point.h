#ifndef GZ_ENTRY_POINT_H
#define GZ_ENTRY_POINT_H
#include "GZApp.h"

namespace GZ {
    extern GZApp *create_application();
}

int main(int argc, char **argv) {
    GZ::GZApp *newApp = GZ::create_application();
    newApp->run();
    delete newApp;
}

#endif