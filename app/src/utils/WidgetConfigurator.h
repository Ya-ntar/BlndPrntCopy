#pragma once

#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class WidgetConfigurator {
 public:
  static void configureButton(std::shared_ptr<tgui::Button> button,
                              const tgui::Color &backgroundColor = tgui::Color::Black,
                              const tgui::Color &borderColor = tgui::Color::White,
                              const tgui::Color &textColor = tgui::Color::White) {
    button->getRenderer()->setBorderColor(borderColor);
    button->getRenderer()->setBackgroundColor(backgroundColor);
    button->getRenderer()->setTextColor(textColor);
  }

  static void configureRichTextLabel(std::shared_ptr<tgui::RichTextLabel> label,
                                     const tgui::Color &backgroundColor = tgui::Color::Black,
                                     const tgui::Color &borderColor = tgui::Color::White,
                                     const tgui::Color &textColor = tgui::Color::White,
                                     tgui::VerticalAlignment verticalAlign = tgui::VerticalAlignment::Center,
                                     tgui::HorizontalAlignment horizontalAlign = tgui::HorizontalAlignment::Left,
                                     unsigned int textSize = 0,
                                     tgui::TextStyle textStyle = tgui::TextStyle::Regular) {
    label->setSize("100%", "100%");
    if (textSize > 0) {
      label->setTextSize(textSize);
    }
    label->getRenderer()->setBackgroundColor(backgroundColor);
    label->getRenderer()->setBorderColor(borderColor);
    label->getRenderer()->setTextColor(textColor);
    label->setVerticalAlignment(verticalAlign);
    label->setHorizontalAlignment(horizontalAlign);
    label->getRenderer()->setTextStyle(textStyle);
  }

  static void configurePanel(std::shared_ptr<tgui::Panel> panel,
                             const tgui::Color &backgroundColor = tgui::Color::Black) {
    panel->setSize("100%", "100%");
    panel->getRenderer()->setBackgroundColor(backgroundColor);
  }

  static void configureScrollablePanel(std::shared_ptr<tgui::ScrollablePanel> panel,
                                       const tgui::Color &backgroundColor = tgui::Color::Transparent) {
    panel->setSize("80%", "80%");
    panel->setPosition("10%", "10%");
    panel->getRenderer()->setBackgroundColor(backgroundColor);
  }

  static void configureBackButton(std::shared_ptr<tgui::Button> backButton,
                                  const std::string &text = "back") {
    backButton->onPress.disconnectAll();
    backButton->setText(text);
  }

  static void configureMenuLayout(std::shared_ptr<tgui::Container> layout) {
    layout->setPosition({"1%", "0%"});
    layout->setSize({"50%", "50%"});
  }

  static void configureLabel(std::shared_ptr<tgui::Label> label,
                            const tgui::Color &textColor = tgui::Color::White,
                            unsigned int textSize = 0,
                            tgui::HorizontalAlignment align = tgui::HorizontalAlignment::Left) {
    label->getRenderer()->setTextColor(textColor);
    if (textSize > 0) label->setTextSize(textSize);
    label->setHorizontalAlignment(align);
  }
}; 