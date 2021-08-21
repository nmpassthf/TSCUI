#pragma once
#include <include/ffmpegworker.hpp>
#include <include/includeQt>
// #include <include/worker.hpp>
/*
    !子线程需继承BaseWorker类
    *此管理类的：
        *信号：
            sendToThread(WorkerEvent)
            sendToThread(WorkerEvent,WorkerMsg)
        *槽：
            onThreadMsg(WorkerMsg);
            onThreadEvent(WorkerMsg);

    *子线程的：
        *信号：
            *message仅用于打印文字至线程对应的缓冲区
            workMessageOut(WorkerMsg)
            workEventOut(WorkerEvent)

        *槽：
            workEventIn(WorkerEvent)
            用于输入带命令行的事件
            workEventIn(WorkerEvent,WorkerMsg command)
*/

class ThreadMgr : public QObject {
    Q_OBJECT

   public:
    explicit ThreadMgr(QObject* parent, unsigned int maxThread = 3);
    ~ThreadMgr();

   public:
    // 打印所有的队列
    void debugPrintQueue();

    // 获取当前的队列
    auto getQQueueScreenshot() -> QList<QHash<const QUuid, CFFInfo>>;

    // 添加一个新的线程 chk igl
    void addWorkerLater(CFFInfo info);

    void changeMaxThread(const unsigned int);

   public slots:
    // 线程的信号处理槽
    void onThreadMsg(const QUuid& id, const QStringList& message);
    void onThreadDebugMsg(const QUuid& id, const QStringList& message);
    // void onThreadEvent(WorkerEvent);
    void onStart(const QUuid& id);
    void onFinish(const QUuid& id);
    void onErrored(const QUuid& id);

    void onStopSignal(const QUuid& id);

    // 更新队列
    void onRefresh();

   signals:
    // 从此Obj向外部发送的全体受管理的线程的消息信号接口
    void sendOutMessage(const QUuid& id, const QStringList& message);
    void sendDebugMessage(const QUuid& id, const QStringList& message);

    void sendOutEventStart(const QUuid& id, const CFFInfo& info);
    void sendOutEventStop(const QUuid& id, const CFFInfo& info);

    void stopThread(const QUuid& id);
    void addAExtraCommand(const QUuid& id, const QStringList&);

    // 自身使用的信号
    // 刷新队列的信号
    void refresh();

   private:
    unsigned int maxThread;
    QThreadPool* pool;
    QQueue<CFFInfo> watting;
    QHash<QUuid, CFFInfo> active, done;
};