#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <include/hs.h>

#include <include/includeQt>
#include <include/threadMgr.hpp>
// #include <include/worker.hpp>
#include <widgets/include/debugWindow.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

   private:
    Ui::MainWindow *ui;
    DebugWindow *debugWindow;
    ThreadMgr mgr;

    // bool createProcessWithOutput(const QDir cmdLocate, const QString cmd);

   signals:

    // TODO void updateThreadHash(WorkerInfo wkInfo, QObject *threadObjP,
    //                       bool toDelete = true);
};

// class zhenghuo : public QWidget {
//     Q_OBJECT
//    public:
//     zhenghuo(QWidget *parent);

//    protected:
//     void mouseMoveEvent(QMouseEvent *event);
//     void timerEvent(QTimerEvent *);
// };

#endif  // MAINWINDOW_HPP
