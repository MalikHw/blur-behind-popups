#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp> // HOLY SHIT THAT'S EASIER
#include "BlurAPI.hpp"

using namespace geode::prelude;

static void tryAddBlur(CCNode* node) {
    if (Mod::get()->getSettingValue<bool>("enabled") && BlurAPI::isBlurAPIEnabled())
        BlurAPI::addBlur(node);
}

struct $baseModify(FLAlertLayer) {
    void modify() {
        auto self = reinterpret_cast<FLAlertLayer*>(this);
        auto name = geode::cocos::getObjectName(self);

        if (name == "ColorSelectLiveOverlay" || name == "HSVLiveOverlay" || name == "RewardUnlockLayer") return; // RewardUnlockLayer bugs as FUCK

        tryAddBlur(self);
    }
};

class $modify(FLAlertLayer) {
    void destructor() {
        BlurAPI::removeBlur(this);
        FLAlertLayer::~FLAlertLayer();
    }
    void onBtn1(CCObject* s) { BlurAPI::removeBlur(this); FLAlertLayer::onBtn1(s); }
    void onBtn2(CCObject* s) { BlurAPI::removeBlur(this); FLAlertLayer::onBtn2(s); }
};
