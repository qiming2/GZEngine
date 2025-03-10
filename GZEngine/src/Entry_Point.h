#pragma once
#include "App.h"
#include "Log.h"

namespace GZ {
    extern App *create_application();
}

// This gets 
int main(int argc, char **argv) {
    
    // Init core stuff
    GZ::Log::init();

    
    GZ::App *newApp = GZ::create_application();
    newApp->run();
    delete newApp;
    return 0;
}