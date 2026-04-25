#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp> // HOLY SHIT THAT'S EASIER
#include "BlurAPI.hpp"
#include <unordered_map>

using namespace geode::prelude;

static CCNode* backdropOf(FLAlertLayer* alert) {
    if (!alert) return nullptr;
    for (auto* n : alert->getChildrenExt<CCNode*>()) {
        if (n && (typeinfo_cast<CCLayerColor*>(n) || typeinfo_cast<CCLayerGradient*>(n)))
            return n;
    }
    if (auto* parent = alert->getParent()) {
        auto z = alert->getZOrder();
        for (auto* n : parent->getChildrenExt<CCNode*>()) {
            if (!n || n == alert) continue;
            if ((typeinfo_cast<CCLayerColor*>(n) || typeinfo_cast<CCLayerGradient*>(n)) && n->getZOrder() <= z)
                return n;
        }
    }
    return nullptr;
}

static std::unordered_map<FLAlertLayer*, CCNode*> s_blurred;

class $modify(FLAlertLayer) {
    void show() {
        FLAlertLayer::show();
        auto name = geode::cocos::getObjectName(this);
        log::info("[blur-behind] show: {}", name.empty() ? "<unnamed>" : name);
        if (name == "ColorSelectLiveOverlay" || name == "HSVLiveOverlay" || name == "RewardUnlockLayer" || name == "RewardsPage" || name == "GJCommentListLayer") {
            log::info("[blur-behind] skipped popup: {}", name);
            return;
        }
        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            log::warn("[blur-behind] mod setting 'enabled' is false");
            return;
        }
        if (!BlurAPI::isBlurAPIEnabled()) {
            log::warn("[blur-behind] blur-api not loaded/enabled");
            return;
        }
        if (auto bg = backdropOf(this)) {
            log::info(
                "[blur-behind] backdrop found: {}",
                geode::cocos::getObjectName(bg).empty() ? "<unnamed>" : geode::cocos::getObjectName(bg)
            );
            BlurAPI::addBlur(bg);
            s_blurred[this] = bg;
            log::info("[blur-behind] blur applied");
            return;
        }
        log::warn(
            "[blur-behind] no backdrop found (children: {}, parent children: {})",
            this->getChildrenCount(),
            this->getParent() ? this->getParent()->getChildrenCount() : 0
        );
    }

    void destructor() {
        if (auto it = s_blurred.find(this); it != s_blurred.end()) {
            if (it->second) BlurAPI::removeBlur(it->second);
            s_blurred.erase(it);
        }
        FLAlertLayer::~FLAlertLayer();
    }
};
