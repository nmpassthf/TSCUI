#ifndef FFMPEGWORKER_HPP
#define FFMPEGWORKER_HPP

#include <include/includeQt>

using CFFInfo = QPair<QFileInfo, QDir>;
auto mkCFFInfo = [](const QFileInfo &f = QFileInfo(),
                    const QDir &d = QDir()) -> CFFInfo {
    return CFFInfo(f, d);
};
class FFmpegWorker : public QObject, public QRunnable {
    Q_OBJECT
   public:
    explicit FFmpegWorker(CFFInfo info);

    ~FFmpegWorker();

   public:
    virtual void run();

   public slots:
    void toStop(const QUuid &id);
    void runAExternalCommand(const QUuid &,
                             const QStringList &additionalCommands);

   public:
    bool isRunning();
    bool isStoped();

    CFFInfo getCFFInfo();

   signals:
    void sendOutMessage(const QUuid &id, const QStringList &msg);
    void sendDebugMessage(const QUuid &id, const QStringList &msg);

    void inStart(const QUuid &);
    void inFinish(const QUuid &);
    void inError(const QUuid &, const QString &msg);

   public:
    const QUuid id;
    const QFileInfo aimInfo;
    const QDir workDir;
    const QTime consTime;

   private:
    bool runningSign = false, stopSign = false, isStopedSign = false;
    QMutex mutex;
    QStringList additionalCommands;
};

#endif  // FFMPEGWORKER_HPP
