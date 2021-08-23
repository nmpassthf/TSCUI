#include "include/ffmpegworker.hpp"

// FFmpegWorker::FFmpegWorker(const WorkerInfo& info, const ThreadMgr* mgr)
//     : WorkerInfo(info), QObject(nullptr), BaseWorker(mgr) {
//     if (isDebugMode) {
//         bugCollecter(0, "FFmpegWorker is constructed");
//     }
// }
// // FFmpegWorker::FFmpegWorker(const QString aimExecName,
// //                            const QDir Locate = QDir())
// //     : WorkerInfo(WorkingModes::exec), QObject(nullptr), aimExec(aimExec) {
// //     if (isDebugMode) {
// //         QStringList dMsg;
// dMsg << "FFmpegWorker is constructed =>";
// //         printDebugInfo();
// //     }
// // }

// FFmpegWorker::~FFmpegWorker() {
//     emit workEventOut(WorkerEvent(*this, Events::Type::toStop));
//     if (!isClean) {
//         QThread::msleep(50);
//     }
//     if (isDebugMode) {
//         bugCollecter(0, "FFmpegWorker is destructed");
//     }
//     emit workEventOut(WorkerEvent(*this, Events::Type::stop));
// }

// void FFmpegWorker::run() {
//     int i = 0;
//     for (i; i < 45; ++i) {
//         if (isToStop) break;
//         emit workMessageOut(WorkerMsg(
//             *this, "[" + QString::number(i) + "]" +
//             getBaseWorkerDebugInfo()));
//         QThread::msleep(500);
//     }
// }
// void FFmpegWorker::workEventIn(WorkerEvent event) {
//     {
//         QMutexLocker locker(&mutex);
//         if (event.type == Events::Type::toStop) {
//             this->isToStop = true;
//         }
//     }
// }
// void FFmpegWorker::workEventInWithCommand(WorkerEvent event, WorkerMsg msg)
// {}

FFmpegWorker::FFmpegWorker(CFFInfo info)
    : id(QUuid::createUuid()),
      aimInfo(info.first),
      workDir(info.second),
      consTime(QTime::currentTime()) {
    QStringList dMsg;
    dMsg << "worker constructed=>" << id.toString()
         << aimInfo.absoluteFilePath() << workDir.absolutePath()
         << consTime.toString();
    emit sendDebugMessage(id, dMsg);
}

FFmpegWorker::~FFmpegWorker() {
    {
        QMutexLocker locker(&mutex);
        if (isStopedSign && stopSign) {
            stopSign = true;
        }
    }

    while (!isStoped()) {
        QThread::msleep(50);
    }

    QStringList dMsg;
    dMsg << "worker destructed=>" << id.toString() << aimInfo.absoluteFilePath()
         << workDir.absolutePath();
    emit sendDebugMessage(id, dMsg);
}

void FFmpegWorker::run() {
    runningSign = true;
    emit isRunning();

    int i = 0;
    while (!stopSign) {
        // 核心费时代码
        QStringList dMsg;
        dMsg << QString::number(++i) << id.toString()
             << aimInfo.absoluteFilePath() << workDir.absolutePath();
        emit sendDebugMessage(id, dMsg);

        if (i > 30) break;

        QThread::msleep(500);
    }

    stopSign = isStopedSign = true;

    QStringList dMsg;
    dMsg << "Finished! =>" << id.toString();
    emit sendDebugMessage(id, dMsg);

    emit inFinish(id);
}

void FFmpegWorker::toStop(const QUuid &id) {
    if (id == this->id || id == QUuid()) {
        this->stopSign = true;

        QStringList dMsg;
        dMsg << "\tObject=>" << id.toString() << "get Stop signal!";
        emit sendDebugMessage(id, dMsg);
    }
}

void FFmpegWorker::runAExternalCommand(const QUuid &id,
                                       const QStringList &commands) {
    if (id == this->id) {
        this->additionalCommands.append(commands);
    }
}

bool FFmpegWorker::isRunning() { return runningSign; }
bool FFmpegWorker::isStoped() { return isStopedSign; }

CFFInfo FFmpegWorker::getCFFInfo() { return mkCFFInfo(aimInfo, workDir); }
