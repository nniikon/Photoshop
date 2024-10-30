#include "ps_windows.h"
#include <cassert>

//#define LOGURU_WITH_STREAMS 1
//#include "loguru.hpp"

using namespace ps;

using namespace psapi;

// IWindow

IWindow::~IWindow() {

}

// IWindowContainer

bool IWindowContainer::isWindowContainer() const {
    return true;
}

// IWindowVector

void IWindowVector::addWindow(std::unique_ptr<IWindow> window) {
    windows_.push_back(std::move(window));
}

void IWindowVector::removeWindow(wid_t id) {
    for (auto it = windows_.begin(); it != windows_.end(); it++) {
        if ((*it)->getId() == id) {
            windows_.erase(it);
            return;
        }
    }
}

IWindow* IWindowVector::getWindowById(wid_t id) {
    return const_cast<IWindow*>(static_cast<const IWindowVector*>(this)->getWindowById(id));
}

const IWindow* IWindowVector::getWindowById(wid_t id) const {
    if (id == kInvalidWindowId) {
        return nullptr;
    }

    for (const auto& window : windows_) {
        IWindow* result = window->getWindowById(id);
        if (result) {
            return result;
        }
    }
    
    return nullptr;
}

bool IWindowVector::isWindowContainer() const {
    return true;
}

// RootWindow implementation

void RootWindow::draw(psapi::IRenderWindow* renderWindow) {
    for (auto& window : windows_) {
        window->draw(renderWindow);
    }
}

bool RootWindow::update(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event) {
    bool result = false;
    for (auto& window : windows_) {
        result |= (window->update(renderWindow, event));
    }
    return result;
}

int64_t RootWindow::getId() const {
    return 0;
}

psapi::vec2i RootWindow::getPos() const {
    return {0, 0};
}

psapi::vec2i RootWindow::getSize() const {
    return {0, 0};
}

void RootWindow::setParent(const IWindow* parent) {
    //LOG_S(ERROR) << "You cannot set the parent of the root window.";
}

void RootWindow::forceActivate() {
    //LOG_S(ERROR) << "Root window is always active.";
}

void RootWindow::forceDeactivate() {
    //LOG_S(ERROR) << "You cannot deactivate the root window.";
}

// getRootwindow singleton implementation

namespace psapi {
    
IWindowContainer* getRootWindow() {
    static RootWindow rootWindow;
    return &rootWindow;
}

generalFunction getGeneralFunction(const std::string& name) {
    //LOG_S(ERROR) << "Luckily, it's not (yet) needed" << name;

    return nullptr;
}

} // namespace
