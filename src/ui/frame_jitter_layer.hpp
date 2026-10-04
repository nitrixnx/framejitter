#pragma once

#include "../includes.hpp"

// Popup with two number boxes: how many frames (max) clicks / releases
// can be displaced from the frame they were recorded on during playback.
class FrameJitterLayer : public geode::Popup<>, public TextInputDelegate {

private:

	TextInput* clicksInput = nullptr;
	TextInput* releasesInput = nullptr;

	STATIC_CREATE(FrameJitterLayer, 250, 190)

	void textChanged(CCTextInputNode*) override {
		auto& g = Global::get();

		int clicks = geode::utils::numFromString<int>(clicksInput->getString()).unwrapOr(0);
		int releases = geode::utils::numFromString<int>(releasesInput->getString()).unwrapOr(0);

		g.jitterClicks = std::max(0, clicks);
		g.jitterReleases = std::max(0, releases);

		g.mod->setSavedValue<int64_t>("jitter_clicks", g.jitterClicks);
		g.mod->setSavedValue<int64_t>("jitter_releases", g.jitterReleases);

		// new values apply from the next attempt
		g.jitterInputs.clear();
	}

	TextInput* makeInput(float y, int value) {
		TextInput* input = TextInput::create(60, "0", "bigFont.fnt");
		input->setPosition({ 175, y });
		input->setScale(0.675f);
		input->setString(value > 0 ? std::to_string(value).c_str() : "");
		input->getInputNode()->setDelegate(this);
		input->getInputNode()->setAllowedChars("0123456789");
		input->getInputNode()->setMaxLabelLength(3);
		m_mainLayer->addChild(input);
		return input;
	}

	CCLabelBMFont* makeLabel(const char* text, float y, float scale) {
		CCLabelBMFont* lbl = CCLabelBMFont::create(text, "bigFont.fnt");
		lbl->setPosition({ 28, y });
		lbl->setAnchorPoint({ 0, 0.5f });
		lbl->setScale(scale);
		m_mainLayer->addChild(lbl);
		return lbl;
	}

	bool setup() override {
		setTitle("Frame Jitter");
		m_title->setScale(0.575f);
		m_title->setPositionY(171);

		auto& g = Global::get();

		CCLabelBMFont* info = CCLabelBMFont::create("Max frames early or late", "bigFont.fnt");
		info->setPosition({ m_size.width / 2, 148 });
		info->setScale(0.3f);
		info->setOpacity(130);
		m_mainLayer->addChild(info);

		makeLabel("Clicks", 112, 0.4f);
		makeLabel("Releases", 72, 0.4f);

		clicksInput = makeInput(112, g.jitterClicks);
		releasesInput = makeInput(72, g.jitterReleases);

		CCLabelBMFont* hint = CCLabelBMFont::create("0 = exact frame. Enable with the toggle.", "chatFont.fnt");
		hint->setPosition({ m_size.width / 2, 44 });
		hint->setScale(0.5f);
		hint->setOpacity(130);
		m_mainLayer->addChild(hint);

		ButtonSprite* btnSpr = ButtonSprite::create("Ok");
		btnSpr->setScale(0.7f);
		CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(FrameJitterLayer::onClose));
		btn->setPosition({ m_size.width / 2, 20 });
		m_buttonMenu->addChild(btn);

		return true;
	}

public:

	void open(CCObject*) {
		create()->show();
	}

};
