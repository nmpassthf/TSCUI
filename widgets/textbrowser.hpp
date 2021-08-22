#ifndef TEXTBROWSER_HPP
#define TEXTBROWSER_HPP

#include <QTextBrowser>
#include <QThreadPool>
#include <include/types>

class TextBrowser : public QTextBrowser {
    Q_OBJECT

   public:
    explicit TextBrowser(QWidget *parent = nullptr, bool isAutoScroll = true);
    ~TextBrowser();

   public:
    bool isAutoScroll();

   public slots:
    void setAutoScroll(bool = true);
    void doScroll();

    void setAppendFormated(UIType::MsgT &msg);

   signals:
    void appendFormatedDone(const QString );

   private:
    bool autoScroll;

    QThreadPool *pool;
};

#endif  // TEXTBROWSER_HPP
