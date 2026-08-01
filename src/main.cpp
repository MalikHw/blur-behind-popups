#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp> // HOLY SHIT THAT'S EASIER
#include "BlurAPI.hpp"

using namespace geode::prelude;


static void tryAddBlur(CCNode* node) {
    if (Mod::get()->getSettingValue<bool>("enabled") && BlurAPI::isBlurAPIEnabled())
        BlurAPI::addBlur(node);
        BlurAPI::getOptions(node)->forcePasses = true;
        BlurAPI::getOptions(node)->passes = Mod::get()->getSettingValue<int>("amount");

}

struct $baseModify(FLAlertLayer) {
    void modify() {
        auto self = reinterpret_cast<FLAlertLayer*>(this);
        auto name = geode::cocos::getObjectName(self);

        if (name == "ColorSelectLiveOverlay" || name == "HSVLiveOverlay" || name == "RewardUnlockLayer" || name == "GJCommentListLayer" || name == "ColorSelectPopup") return;
        auto blurNode = CCLayerColor::create();
        this->addChild(blurNode);
        blurNode->setID("blurNode"_spr);
        blurNode->setZOrder(this->getZOrder() - 1);
        tryAddBlur(blurNode);
    }
};

class $modify(FLAlertLayer) {
    void destructor() {
        BlurAPI::removeBlur(this->getChildByID("blurNode"_spr));
    }
        
};
