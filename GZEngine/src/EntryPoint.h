#pragma once
#include "App.h"
#include "Log.h"

namespace GZ {
    extern App *create_application();
}

// Try hot reloading


int main(int argc, char **argv) {
    
    
    GZ::App *newApp = GZ::create_application();
    // Init core stuff
    
    newApp->run();    
    
    delete newApp;
    return 0;
}