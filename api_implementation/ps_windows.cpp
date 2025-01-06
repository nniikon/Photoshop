#include "ps_windows.h"
#include <cassert>

//#define LOGURU_WITH_STREAMS 1
//#include "loguru.hpp"

using namespace ps;

using namespace psapi;

// IWindowContainer

bool IWindowContainer::isWindowContainer() const {
    return true;
}

#if 0

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

#endif // 0

// ActionController

namespace psapi {

AActionController* getActionController() {
    static ActionController controller;

    return &controller;
}

}

ActionController::ActionController(size_t maxActions)
    : maxActions_(maxActions) {}

bool ActionController::execute(std::unique_ptr<psapi::IAction> action) {
    if (!actionExecute(action.get())) {
        return false;
    }

    // A new action invalidates the redo history
    undone_actions_.clear();

    if (isUndoableAction(action.get())) {
        executed_actions_.emplace_back(std::move(action));
        enforceStackLimit(executed_actions_);
    }

    return true;
}

bool ActionController::undo() {
    if (executed_actions_.empty()) {
        return false;
    }

    auto last_action = std::move(executed_actions_.back());
    executed_actions_.pop_back();

    auto* undoableAction = dynamic_cast<psapi::IUndoableAction*>(last_action.get());
    if (actionUndo(undoableAction)) {
        undone_actions_.emplace_back(
            std::unique_ptr<psapi::IUndoableAction>(
                static_cast<psapi::IUndoableAction*>(last_action.release())
            )
        );
        enforceStackLimit(undone_actions_);
        return true;
    }

    return false;
}

bool ActionController::redo() {
    if (undone_actions_.empty()) {
        return false;
    }

    auto lastUndoneAction = std::move(undone_actions_.back());
    undone_actions_.pop_back();

    auto* undoableAction = dynamic_cast<psapi::IUndoableAction*>(lastUndoneAction.get());
    if (actionRedo(undoableAction)) {
        executed_actions_.emplace_back(
            std::unique_ptr<psapi::IAction>(
                static_cast<psapi::IAction*>(lastUndoneAction.release())
            )
        );
        enforceStackLimit(executed_actions_);
        return true;
    }

    return false;
}

template <typename T>
void ActionController::enforceStackLimit(std::deque<T>& stack) {
    while (stack.size() > maxActions_) {
        stack.pop_front();
    }
}

// RootWindow implementation

void RootWindow::draw(psapi::IRenderWindow* renderWindow) {
    for (auto& window : windows_) {
        window->draw(renderWindow);
    }
    size_ = renderWindow->getSize();
}

class WindowContainerAction : public psapi::IAction
{
public:
    WindowContainerAction(std::vector<std::unique_ptr<psapi::IWindow>>& windows_,
                          const psapi::IRenderWindow* render_window,
                          const psapi::sfm::Event& event)
        : windows_     (windows_),
          render_window_(render_window),
          event(event) {}

    bool execute(const psapi::IAction::Key& key) override {
        auto action_controller = psapi::getActionController();
        bool result = false;
        for (auto& window : windows_) {
            result |= action_controller->execute(window->createAction(render_window_, event));
        }
        return result;
    }

    bool isUndoable(const psapi::IAction::Key&) override {
        return false;
    }

private:
    std::vector<std::unique_ptr<psapi::IWindow>>& windows_;
    const psapi::IRenderWindow* render_window_;
    const psapi::sfm::Event& event;
};

std::unique_ptr<psapi::IAction> RootWindow::createAction(const psapi::IRenderWindow* renderWindow,
                                                         const psapi::sfm::Event& event) {
    return std::make_unique<WindowContainerAction>(windows_, renderWindow, event);
}

int64_t RootWindow::getId() const {
    return 0;
}

psapi::vec2i RootWindow::getPos() const {
    return {0, 0};
}

psapi::vec2u RootWindow::getSize() const {
    return size_;
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

bool RootWindow::isActive() const {
    return true;
}

void RootWindow::setPos(const psapi::vec2i& pos) {
    assert(0);
}

void RootWindow::setSize(const psapi::vec2u& size) {
    assert(0);
}

bool RootWindow::isWindowContainer() const {
    return true;
}

layer_id_t RootWindow::getUpperLayerId() const {
    assert(0);
    return 0;
}

layer_id_t RootWindow::increaseLayerId() {
    assert(0);
    return 0;
}

layer_id_t RootWindow::decreaseLayerId() {
    assert(0);
    return 0;
}

const IWindow* RootWindow::getWindowById(psapi::wid_t id) const {
    for (auto& window : windows_) {
        if (window->getWindowById(id) != nullptr) {
            return window->getWindowById(id);
        }
    }
    return nullptr;
}

IWindow* RootWindow::getWindowById(psapi::wid_t id) {
    return const_cast<IWindow*>(static_cast<const RootWindow*>(this)->getWindowById(id));
}

void RootWindow::addWindow(std::unique_ptr<IWindow> window) {
    window->setParent(this);
    windows_.push_back(std::move(window));
}

void RootWindow::removeWindow(psapi::wid_t id) {
    assert(0);
}

// getRootwindow singleton implementation

namespace psapi {
    
IRootWindow* getRootWindow() {
    static RootWindow root_window;

    return &root_window;
}

generalFunction getGeneralFunction(const std::string& name) {
    //LOG_S(ERROR) << "Luckily, it's not (yet) needed" << name;

    return nullptr;
}

} // namespace
