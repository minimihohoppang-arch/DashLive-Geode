#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class $modify(DashLiveMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) {
            return true;
        }

        auto icon = CCSprite::create("dashlive_button.png"_spr);
        if (!icon) {
            icon = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
        }

        if (!icon) {
            return true;
        }

        icon->setScale(0.74f);

        auto button = CCMenuItemSpriteExtra::create(
            icon,
            this,
            menu_selector(DashLiveMenuLayer::onDashLive)
        );

        if (!button) {
            return true;
        }

        button->setID("dashlive-button"_spr);
        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onDashLive(CCObject*) {
        web::openLinkInBrowser(
            "https://geometrydashkoreahub.yslovelyu.chatgpt.site"
        );
    }
};
