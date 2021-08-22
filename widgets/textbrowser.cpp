#include "textbrowser.hpp"

TextBrowser::TextBrowser(QWidget *parent, bool isAutoScroll)
    : QTextBrowser(parent),
      autoScroll(isAutoScroll),
      pool(new QThreadPool(this)) {
    pool->setMaxThreadCount(1);

    connect(this, &TextBrowser::textChanged,this,  &TextBrowser::doScroll,Qt::QueuedConnection);

    connect(this, &TextBrowser::appendFormatedDone,this,
            [=](const QString text) { this->insertPlainText(text); },Qt::QueuedConnection);
}

TextBrowser::~TextBrowser() { pool->waitForDone(); }

bool TextBrowser::isAutoScroll() { return autoScroll; }
void TextBrowser::setAutoScroll(bool is) {
    autoScroll = is;
    doScroll();
}
void TextBrowser::doScroll() {
    if (autoScroll) {
        QTextCursor tCur(this->textCursor());
        tCur.movePosition(QTextCursor::End);
        setTextCursor(tCur);
    }
}

void TextBrowser::setAppendFormated(UIType::MsgT &msg) {
    auto title = msg.label;
    auto textList = msg.message;

    pool->start([&, title, textList]() -> void {
        QString text{};
        quint64 count = 0;

        text.append("[" + title + "] ->{\n ");
        for (auto &i : textList) {
            text.append("\t" + QString::number(++count) + "-->" + i + "\n");
        }
        text.append("\t}\n");

        emit appendFormatedDone(text);
    });
}
