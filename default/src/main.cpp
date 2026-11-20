#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>

#include <algorithm>

using namespace geode::prelude;

namespace {
constexpr auto DASHLIVE_URL = "https://geometrydashkoreahub.yslovelyu.chatgpt.site";

CCMenuItemSpriteExtra* makeButton(
    char const* text,
    CCObject* target,
    SEL_MenuHandler callback,
    char const* background = "GJ_button_01.png"
) {
    auto sprite = ButtonSprite::create(
        text, 125, 0, 0.75f, true,
        "bigFont.fnt", background, 30
    );
    return CCMenuItemSpriteExtra::create(sprite, target, callback);
}
}

class DashLivePopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("DashLive");

        auto subtitle = CCLabelBMFont::create(
            "Korean Geometry Dash Live", "goldFont.fnt"
        );
        subtitle->setScale(0.55f);
        subtitle->setPosition({210.f, 205.f});
        m_mainLayer->addChild(subtitle);

        auto panel = CCScale9Sprite::create("square02_001.png");
        panel->setContentSize({360.f, 115.f});
        panel->setOpacity(90);
        panel->setPosition({210.f, 128.f});
        m_mainLayer->addChild(panel);

        auto liveText = CCLabelBMFont::create("LIVE STREAMS", "bigFont.fnt");
        liveText->setScale(0.55f);
        liveText->setPosition({210.f, 168.f});
        m_mainLayer->addChild(liveText);

        auto platformText = CCLabelBMFont::create(
            "CHZZK   YOUTUBE   SOOP   TWITCH", "bigFont.fnt"
        );
        platformText->setScale(0.38f);
        platformText->setColor({180, 255, 210});
        platformText->setPosition({210.f, 139.f});
        m_mainLayer->addChild(platformText);

        auto infoText = CCLabelBMFont::create(
            "Choose live streams or free chat.", "chatFont.fnt"
        );
        infoText->setScale(0.55f);
        infoText->setPosition({210.f, 108.f});
        m_mainLayer->addChild(infoText);

        auto menu = CCMenu::create();
        menu->setPosition({210.f, 63.f});

        auto liveButton = makeButton(
            "LIVE LIST", this,
            menu_selector(DashLivePopup::onLive)
        );
        liveButton->setPosition({-75.f, 0.f});
        menu->addChild(liveButton);

        auto chatButton = makeButton(
            "FREE CHAT", this,
            menu_selector(DashLivePopup::onChat),
            "GJ_button_02.png"
        );
        chatButton->setPosition({75.f, 0.f});
        menu->addChild(chatButton);

        m_mainLayer->addChild(menu);
        return true;
    }

    void onLive(CCObject*) {
        createQuickPopup(
            "DashLive",
            "Open the <cg>live broadcast list</c>?\nThe browser opens only after you press <cy>Open</c>.",
            "Cancel", "Open",
            [](FLAlertLayer*, bool open) {
                if (open) web::openLinkInBrowser(DASHLIVE_URL);
            }
        );
    }

    void onChat(CCObject*) {
        createQuickPopup(
            "Free Chat",
            "Open the <cb>DashLive free chat</c>?\nThe browser opens only after you press <cy>Open</c>.",
            "Cancel", "Open",
            [](FLAlertLayer*, bool open) {
                if (open) {
                    web::openLinkInBrowser(
                        "https://geometrydashkoreahub.yslovelyu.chatgpt.site/#chat"
                    );
                }
            }
        );
    }

public:
    static DashLivePopup* create() {
        auto popup = new DashLivePopup();
        if (popup && popup->initAnchored(420.f, 250.f, "GJ_square01.png")) {
            popup->autorelease();
            return popup;
        }
        CC_SAFE_DELETE(popup);
        return nullptr;
    }
};

class $modify(DashLiveMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = typeinfo_cast<CCMenu*>(
            this->getChildByID("bottom-menu")
        );
        if (!bottomMenu || bottomMenu->getChildByID("dashlive-button"_spr)) {
            return true;
        }

        auto icon = CCSprite::create("dashlive_button.png"_spr);
        if (!icon) {
            log::error("DashLive icon resource was not found");
            return true;
        }

        auto size = icon->getContentSize();
        if (size.width > 0.f && size.height > 0.f) {
            auto scale = std::min(42.f / size.width, 42.f / size.height);
            icon->setScale(scale);
        }

        auto button = CCMenuItemSpriteExtra::create(
            icon, this,
            menu_selector(DashLiveMenuLayer::onDashLive)
        );
        button->setID("dashlive-button"_spr);
        bottomMenu->addChild(button);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        bottomMenu->setContentSize({
            winSize.width / 2.f,
            bottomMenu->getScaledContentSize().height
        });
        bottomMenu->updateLayout();
        return true;
    }

    void onDashLive(CCObject*) {
        if (auto popup = DashLivePopup::create()) popup->show();
    }
};
