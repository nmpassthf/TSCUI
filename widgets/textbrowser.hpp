#ifndef TEXTBROWSER_HPP
#define TEXTBROWSER_HPP

#include <QAbstractScrollArea>
#include <QEvent>
#include <QScrollBar>
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
    void appendFormatedDone(const QString);

   private:
    bool autoScroll;

    QThreadPool *pool;
    bool eventFilter(QObject *target, QEvent *e);
};

#endif  // TEXTBROWSER_HPP
