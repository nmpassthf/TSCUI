#ifndef DEBUGWINDOW_HPP
#define DEBUGWINDOW_HPP

#include <TSCInclude>
#include <widgets/textbrowser.hpp>

namespace Ui {
class DebugWindow;
}

class DebugWindowViewItem : public QObject, public QListWidgetItem {
    Q_OBJECT

   public:
    explicit DebugWindowViewItem(QWidget *parent);
    ~DebugWindowViewItem();

   public slots:
    void storageMessage(UIType::MsgT msg);

   public:
    QList<UIType::MsgT *> getAllMessage();
    QMutex mutex;

   private:
    QList<UIType::MsgT *> savedMessage;
};

class DebugWindow : public QDialog {
    Q_OBJECT

   public:
    explicit DebugWindow(QWidget *parent = nullptr);
    ~DebugWindow();

   private:
    Ui::DebugWindow *ui;

   public slots:
    void onDebugMessageIn(const QString &aimLabel, UIType::MsgT msg);
    void onDebugMessageIn(const QUuid &id, UIType::MsgT msg);

    void reflush(DebugWindowViewItem *item, UIType::MsgT msg);

    virtual void resizeEvent(QResizeEvent *);

   private:
    // storage Label & DebugWindowViewItem*
    QHash<QString, DebugWindowViewItem *> items;
    QThreadPool *pool;

    bool isAutoScroll = true;
};

#endif  // DEBUGWINDOW_HPP
