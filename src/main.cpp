#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/DailyLevelPage.hpp>
#include <Geode/modify/SetIDPopup.hpp>
#include <Geode/modify/SetTextPopup.hpp>
#include <Geode/modify/GJPathsLayer.hpp>

#include "BlurAPI.hpp"

using namespace geode::prelude;

static void tryAddBlur(CCNode* node) {
    if (Mod::get()->getSettingValue<bool>("enabled") && BlurAPI::isBlurAPIEnabled())
        BlurAPI::addBlur(node);
}

class $modify(FLAlertLayer) {
    void show() {
        tryAddBlur(this); FLAlertLayer::show();
    }
    void destructor() {
        BlurAPI::removeBlur(this); FLAlertLayer::~FLAlertLayer();
    }
    void onBtn1(CCObject* s) { BlurAPI::removeBlur(this); FLAlertLayer::onBtn1(s); }
    void onBtn2(CCObject* s) { BlurAPI::removeBlur(this); FLAlertLayer::onBtn2(s); }
};

#define justBlur(ClassName)\
class $modify(ClassName) {\
    void show() {\
        tryAddBlur(this); ClassName:show();\
    }\
};

justBlur(DailyLevelPage)
justBlur(SetIDPopup)
justBlur(SetTextPopup)
justBlur(GJPathsLayer)