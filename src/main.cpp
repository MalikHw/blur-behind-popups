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
    return nullptr;
}

static std::unordered_map<FLAlertLayer*, CCNode*> s_blurred;

class $modify(FLAlertLayer) {
    void show() {
        FLAlertLayer::show();
        auto name = geode::cocos::getObjectName(this);
        if (name == "ColorSelectLiveOverlay" || name == "HSVLiveOverlay" || name == "RewardUnlockLayer" || name == "RewardsPage" || name == "GJCommentListLayer")
            return;
        if (!Mod::get()->getSettingValue<bool>("enabled") || !BlurAPI::isBlurAPIEnabled())
            return;
        if (auto bg = backdropOf(this)) {
            BlurAPI::addBlur(bg);
            s_blurred[this] = bg;
        }
    }

    void destructor() {
        if (auto it = s_blurred.find(this); it != s_blurred.end()) {
            if (it->second) BlurAPI::removeBlur(it->second);
            s_blurred.erase(it);
        }
        FLAlertLayer::~FLAlertLayer();
    }
};
