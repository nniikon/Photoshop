#include "ps_bar.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#include <cassert>

using namespace ps;

ABarButton::ABarButton(std::unique_ptr<psapi::sfm::ISprite> sprite,
                       const psapi::IBar* toolbar,
                       std::unique_ptr<ButtonAction> action)
    : id_(psapi::kInvalidWindowId),
      parent_(toolbar),
      toolbar_(toolbar),
      is_active_(true),
      pos_ (0, 0),
      size_(0, 0),
      sprite_(std::move(sprite)),
      action_(std::move(action)) {
}

void ABarButton::draw(psapi::IRenderWindow* renderWindow) {
    if (!is_active_) {
        return;
    }

    psapi::ChildInfo info = toolbar_->getNextChildInfo();
    
    pos_  = info.pos;
    size_ = info.size;

    sprite_->setPosition(static_cast<float>(pos_.x),
                         static_cast<float>(pos_.y));
    float factor_x = static_cast<float>(size_.x) / static_cast<float>(sprite_->getGlobalBounds().width);
    float factor_y = static_cast<float>(size_.y) / static_cast<float>(sprite_->getGlobalBounds().height);
    sprite_->setScale(factor_x, factor_y);

    renderWindow->draw(sprite_.get());

    const_cast<psapi::IBar*>(toolbar_)->finishButtonDraw(renderWindow, this);
}

bool ABarButton::update(const psapi::IRenderWindow* renderWindow,
                        const psapi::sfm::Event& event) {
    if (!is_active_) {
        return false;
    }

    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);
    
    bool is_in_window = pos_.x <= mouse_pos.x && mouse_pos.x < pos_.x + size_.x &&
                        pos_.y <= mouse_pos.y && mouse_pos.y < pos_.y + size_.y;

    if (state_ == psapi::IBarButton::State::Released) {
        if (action_) {
            action_->operator()(renderWindow, event);
            return true;
        }
    }

    // FIXME: rewrite logic
    if (is_in_window && event.type == psapi::sfm::Event::MouseButtonReleased) {
        if (state_ == psapi::IBarButton::State::Press) {
            state_ = psapi::IBarButton::State::Released;
            return true;
        }
        if (state_ == psapi::IBarButton::State::Released) {
            state_ = psapi::IBarButton::State::Normal; 
        }
    }
    if (state_ == psapi::IBarButton::State::Released) {
        return false;
    }
    if (is_in_window && event.type == psapi::sfm::Event::MouseButtonPressed) {
        state_ = psapi::IBarButton::State::Press;
        return true;
    }
    if (is_in_window) {
        if (state_ == psapi::IBarButton::State::Normal) {
            state_ = psapi::IBarButton::State::Hover;
        }
        return true;
    }
    else {
        state_ = psapi::IBarButton::State::Normal;
        return false;
    }
}

psapi::IWindow* ABarButton::getWindowById(psapi::wid_t id) {
    return const_cast<psapi::IWindow*>(static_cast<const ABarButton*>(this)->getWindowById(id));
}

const psapi::IWindow* ABarButton::getWindowById(psapi::wid_t id) const {
    if (id == id_) {
        return this;
    }

    return nullptr;
}

psapi::vec2i ABarButton::getPos() const {
    return pos_;
}

psapi::vec2i ABarButton::getSize() const {
    return size_;
}

psapi::wid_t ABarButton::getId() const {
    return id_;
}

void ABarButton::setParent(const psapi::IWindow* parent) {
    parent_ = parent;
}

void ABarButton::forceDeactivate() {
    is_active_ = false;
}

void ABarButton::forceActivate() {
    is_active_ = true;
}

void ABarButton::setState(State state) {
    state_ = state; 
}

ABarButton::State ABarButton::getState() const {
    return state_;
}

bool ABarButton::isWindowContainer() const {
    return false;
}

// ABar implementation

ABar::ABar(std::unique_ptr<psapi::sfm::ISprite> sprite,
           std::unique_ptr<psapi::sfm::ISprite> hover_button_sprite,
           std::unique_ptr<psapi::sfm::ISprite> pressed_button_sprite,
           std::unique_ptr<psapi::sfm::ISprite> active_button_sprite,
           psapi::vec2i toolbar_pos,
           psapi::vec2i toolbar_size,
           psapi::vec2i side_gap,
           psapi::vec2i inbutton_gap,
           psapi::vec2i button_size,
           int n_buttons_in_row,
           psapi::wid_t id)
    : id_(id),
      parent_(nullptr),
      is_active_(true),
      pos_ (toolbar_pos),
      size_(toolbar_size),
      side_gap_(side_gap),
      inbutton_gap_(inbutton_gap),
      button_size_(button_size),
      n_buttons_in_row_(n_buttons_in_row),
      n_buttons_(0),
      cur_button_it(0),
      sprite_               (std::move(sprite)),
      hover_button_sprite_  (std::move(hover_button_sprite)),
      pressed_button_sprite_(std::move(pressed_button_sprite)),
      active_button_sprite_ (std::move(active_button_sprite)),
      last_active_button_it(-1)
{}

ABar::~ABar()
{}

void ABar::drawChildren(psapi::IRenderWindow* renderWindow) {
    for (const auto& window : windows_) {
        window->draw(renderWindow);
    }
}

void ABar::drawMyself(psapi::IRenderWindow* renderWindow) {
    sprite_->setPosition(static_cast<float>(pos_.x),
                         static_cast<float>(pos_.y));

    psapi::vec2i current_size = sprite_->getSize();

    float factor_x = static_cast<float>(current_size.x) / static_cast<float>(size_.x);
    float factor_y = static_cast<float>(current_size.y) / static_cast<float>(size_.y);

    sprite_->setScale(factor_x, factor_y);
    
    renderWindow->draw(sprite_.get());
}

void ABar::draw(psapi::IRenderWindow* renderWindow) {
    if (!is_active_) {
        return;
    }

    // LOG_F(INFO, "Drawing ABar");
    
    drawMyself  (renderWindow);
    drawChildren(renderWindow);
}

void ABar::handleDoubleActiveButton() {
    ssize_t last_active = last_active_button_it;

    int n_active_buttons = 0;
    ssize_t new_active = -1;
    for (size_t i = 0; i < windows_.size(); i++) {
        if (windows_[i]->getState() == psapi::IBarButton::State::Released) {         
            n_active_buttons++; 
            new_active = static_cast<ssize_t>(i);
        }
    }

    if (n_active_buttons > 1) {
        windows_[(size_t)last_active]->setState(psapi::IBarButton::State::Normal);
    }
    else if (n_active_buttons == 1) {
        last_active_button_it = new_active;
    }
}

bool ABar::updateChildren(const psapi::IRenderWindow* renderWindow,
                          const psapi::sfm::Event& event) {
    bool result = false;
    for (const auto& window : windows_) {
        result |= window->update(renderWindow, event);
    }

    handleDoubleActiveButton();

    return result;
}

bool ABar::update(const psapi::IRenderWindow* renderWindow,
                  const psapi::sfm::Event& event) {
    if (!is_active_) {
        return false;
    }

    if (updateChildren(renderWindow, event)) {
        return true;
    }

    return false;
}

const psapi::IWindow* ABar::getWindowById(psapi::wid_t id) const {
    if (id == id_) {
        return this;
    }

    for (auto& window : windows_) {
        if (window->getId() == id) {
            return window.get();
        }
    }

    return nullptr;
}

psapi::IWindow* ABar::getWindowById(psapi::wid_t id) {
    return const_cast<psapi::IWindow*>(static_cast<const ABar*>(this)->getWindowById(id));
}

psapi::vec2i ABar::getPos()  const {
    return pos_;
}

psapi::vec2i ABar::getSize() const {
    return size_;
}

psapi::wid_t ABar::getId() const {
    return id_;
}

void ABar::setParent(const IWindow* parent) {
    parent_ = parent;
}

void ABar::forceDeactivate() {
    is_active_ = false;
}

void ABar::forceActivate() {
    is_active_ = true;
}

void ABar::addWindow(std::unique_ptr<psapi::IWindow> window) {
    window->setParent(this);

    // Cast unique_ptr<IWindow> to unique_ptr<IBarButton> :vomiting:
    windows_.push_back(std::unique_ptr<psapi::IBarButton>(dynamic_cast<psapi::IBarButton*>(window.release())));

    n_buttons_ += 1;
}

void ABar::removeWindow(psapi::wid_t id) {
    for (auto it = windows_.begin(); it != windows_.end(); ++it) {
        if ((*it)->getId() == id) {
            windows_.erase(it);
            break;
        }
    }
    
    n_buttons_ -= 1;
}

psapi::ChildInfo ABar::getNextChildInfo() const {
    int row = cur_button_it / n_buttons_in_row_;
    int col = cur_button_it % n_buttons_in_row_;

    int x = side_gap_.x + col * (button_size_.x + inbutton_gap_.x) + pos_.x;
    int y = side_gap_.y + row * (button_size_.y + inbutton_gap_.y) + pos_.y;

    cur_button_it = (cur_button_it + 1) % n_buttons_;

    return psapi::ChildInfo{{x, y}, {button_size_.x, button_size_.y}};
}

void ABar::finishButtonDraw(psapi::IRenderWindow* renderWindow,
                            const psapi::IBarButton* button) {

    psapi::vec2i button_pos = button->getPos();
    auto posf_x = static_cast<float>(button_pos.x);
    auto posf_y = static_cast<float>(button_pos.y);

    switch (button->getState()) {
        case psapi::IBarButton::State::Normal:
            break;

        case psapi::IBarButton::State::Hover:
            hover_button_sprite_->setPosition(posf_x, posf_y);
            renderWindow->draw(hover_button_sprite_.get());
            break;

        case psapi::IBarButton::State::Press:
            pressed_button_sprite_->setPosition(posf_x, posf_y);
            renderWindow->draw(pressed_button_sprite_.get());
            break;

        case psapi::IBarButton::State::Released:
            active_button_sprite_->setPosition(posf_x, posf_y);
            renderWindow->draw(active_button_sprite_.get());
            break;

        default:
            assert(0);
            break;
    }
}
