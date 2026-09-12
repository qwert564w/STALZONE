#pragma once
#include <string>
class Module {
public:
    std::string name;
    bool enabled = false;
    virtual void OnUpdate() {}
    virtual void OnRenderMenu() {}
    virtual void OnRenderOverlay() {}
};
