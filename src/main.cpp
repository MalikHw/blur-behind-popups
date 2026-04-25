#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp> // HOLY SHIT THAT'S EASIER
#include "BlurAPI.hpp"

using namespace geode::prelude;

static CCNode* backdropOf(FLAlertLayer* alert) {
    if (!alert) return nullptr;
    for (auto* n : alert->getChildrenExt<CCNode*>()) {
        if (n && (typeinfo_cast<CCLayerColor*>(n) || typeinfo_cast<CCLayerGradient*>(n) || n->getZOrder() < 0))
            return n;
    }
    return nullptr;
}

struct $baseModify(FLAlertLayer) {
    void modify() {
        auto self = reinterpret_cast<FLAlertLayer*>(this);
        auto name = geode::cocos::getObjectName(self);
        if (name == "ColorSelectLiveOverlay" || name == "HSVLiveOverlay" || name == "RewardUnlockLayer" || name == "RewardsPage" || name == "GJCommentListLayer")
            return;
        if (Mod::get()->getSettingValue<bool>("enabled") && BlurAPI::isBlurAPIEnabled())
            if (auto bg = backdropOf(self))
                BlurAPI::addBlur(bg);
    }
};

class $modify(FLAlertLayer) {
    void destructor() {
        if (auto bg = backdropOf(this)) BlurAPI::removeBlur(bg);
        FLAlertLayer::~FLAlertLayer();
    }
};
