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
        self->scheduleOnce(schedule_selector_lambda([self](float) {
            if (!self->getParent()) return; // already gone
            auto name = geode::cocos::getObjectName(self);
            if (name == "ColorSelectLiveOverlay" || 
                name == "HSVLiveOverlay" || 
                name == "RewardUnlockLayer" || 
                name == "RewardsPage" || 
                name == "GJCommentListLayer" || 
                name == "ColorSelectPopup") return;

            tryAddBlur(self);
        }), 0.f, "blur-init");
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
