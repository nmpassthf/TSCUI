#ifndef DEBUGWINDOW_HPP
#define DEBUGWINDOW_HPP

#include <include/includeQt>

namespace Ui {
class DebugWindow;
}

class DebugWindowViewItem : public QObject, public QListWidgetItem {
    Q_OBJECT

   public:
    explicit DebugWindowViewItem(QWidget *parent);
    ~DebugWindowViewItem();

   public slots:
    void updateMessage(const QStringList msg);

   public:
    QString savedMessage;
};

class DebugWindow : public QDialog {
    Q_OBJECT

   public:
    explicit DebugWindow(QWidget *parent = nullptr);
    ~DebugWindow();

   public slots:
    void errorCritical(const QUuid &id, const QStringList &errorMessage);

    void printDebugMessage(const QString &label, const QStringList &message);
    void printDebugMessage(const QUuid &id, const QStringList &message);

    // void printDebugMessageL(const QString &label, const QStringList
    // &message);
    void reflush();

    virtual void resizeEvent(QResizeEvent *);

   private:
    Ui::DebugWindow *ui;

   private slots:
    void textBrowserAutoScroll();

   private:
    QHash<QString, DebugWindowViewItem *> labels;

    QTimer *timer;
};

#endif  // DEBUGWINDOW_HPP
