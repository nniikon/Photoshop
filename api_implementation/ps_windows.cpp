#include "ps_windows.h"
#include <cassert>

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

using namespace ps;

// AWindow implementation

int64_t AWindow::getId() const {
    return id_;
}

psapi::IWindow* AWindow::getWindowById(psapi::wid_t id) {
    if (id == id_) {
        return this;
    }
    return nullptr;
}

const psapi::IWindow* AWindow::getWindowById(psapi::wid_t id) const {
    if (id == id_) {
        return this;
    }
    return nullptr;
}

// AWindowContainer implementation 

int64_t AWindowContainer::getId() const {
    return id_;
}

// AWindowVector implementation

void AWindowVector::drawChildren(psapi::ARenderWindow* renderWindow) {
    for (auto& window : windows_) {
        window->draw(renderWindow);
    }
}

bool AWindowVector::updateChildren(const psapi::ARenderWindow* renderWindow,
                                   const psapi::sfm::Event& event) {
    bool was_updated = false;
    for (auto& window : windows_) {
        was_updated |= window->update(renderWindow, event);
    }
    return was_updated;
}

int64_t AWindowVector::getId() const {
    return id_;
}

psapi::IWindow* AWindowVector::getWindowById(psapi::wid_t id) {
    return const_cast<psapi::IWindow*>(static_cast<const AWindowVector*>(this)->getWindowById(id));
}

const psapi::IWindow* AWindowVector::getWindowById(psapi::wid_t id) const {
    if (id == id_) {
        return this;
    }

    for (const auto& window : windows_) {
        psapi::IWindow* result = window->getWindowById(id);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

void AWindowVector::assertDuplicateId(psapi::wid_t id) {
    if (id == psapi::kInvalidWindowId) {
        return;
    }

    for (const auto& window : windows_) {
        if (window->getId() == id) {
            LOG_S(FATAL) << "Duplicate ID:" << id;
            assert(0);
        }
    }
}

void AWindowVector::addWindow(std::unique_ptr<IWindow> window) {
    assertDuplicateId(window->getId());

    windows_.push_back(std::move(window));
}

void AWindowVector::removeWindow(psapi::wid_t id) {
    for (auto it = windows_.begin(); it != windows_.end(); it++) {
        if ((*it)->getId() == id) {
            windows_.erase(it);
            return;
        }
    }
}

// RootWindow implementation

void RootWindow::draw(psapi::ARenderWindow* renderWindow) {
    drawChildren(renderWindow);
}

bool RootWindow::update(const psapi::ARenderWindow* renderWindow,
                        const psapi::sfm::Event& event) {
    return updateChildren(renderWindow, event);
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
    LOG_S(ERROR) << "You cannot set the parent of the root window.";
}

void RootWindow::forceActivate() {
    LOG_S(ERROR) << "Root window is always active.";
}

void RootWindow::forceDeactivate() {
    LOG_S(ERROR) << "You cannot deactivate the root window.";
}

// getRootwindow singleton implementation

namespace psapi {
    
IWindowContainer* getRootWindow() {
    static RootWindow rootWindow;
    return &rootWindow;
}

generalFunction getGeneralFunction(const std::string& name) {
    LOG_S(ERROR) << "Luckily, it's not (yet) needed" << name;

    return nullptr;
}

} // namespace
