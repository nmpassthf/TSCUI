#include <include/threadMgr.hpp>

ThreadMgr::ThreadMgr(QObject* parent, unsigned int maxThread)
    : QObject(parent), maxThread(maxThread), pool(new QThreadPool(this)) {
    connect(this, &ThreadMgr::refresh, &ThreadMgr::onRefresh);
}
ThreadMgr::~ThreadMgr() {
    qDebug() << "mgr析构开始";

    debugPrintQueue();

    // 析构时发送终止所有线程的信号
    emit stopThread(QUuid());

    qDebug() << "清理等待列表=>";
    // 清除所有等待列表
    watting.clear();
    debugPrintQueue();

    qDebug() << "等待线程结束=>";
    pool->waitForDone();

    done.clear();
    qDebug() << "清理完成列表=>";
    debugPrintQueue();

    qDebug() << "mgr析构完成";
}

void ThreadMgr::debugPrintQueue() {
#ifdef DEBUG
//    qDebug() << "\t\t列表长度为=>" << watting.count() << ";" << active.count()
//             << ";" << done.count();
//    qDebug() << "列表信息=>";

//    qDebug() << "\tWATTING QUEUE=>\t\t\t";
//    for (auto &i : watting) {
//        qDebug() << "\t\t\t" << i.first.absoluteFilePath() << " "
//                 << i.second.absolutePath() << "\t\t\t";
//    }
//    qDebug() << "\tACTIVE QUEUE=>\t\t\t";
//    for (auto& i : active.keys()) {
//        qDebug() << "\t\t\t" << i.toString() << " "
//                 << active[i].first.absoluteFilePath() << " "
//                 << active[i].second.absolutePath() << "\t\t\t";
//    }
//    qDebug() << "\tDONE QUEUE=>\t\t\t";
//    for (auto &i : done.keys()) {
//        qDebug() << "\t\t\t" << i.toString() << " "
//                 << done[i].first.absoluteFilePath() << " "
//                 << done[i].second.absolutePath() << "\t\t\t";
//    }
#endif
}

auto ThreadMgr::getQQueueScreenshot() -> QList<QHash<const QUuid, CFFInfo>> {
    QList<QHash<const QUuid, CFFInfo>> result;
    for (int i = 0; i < 3; ++i) {
        switch (i) {
            case 0:
                for (auto &j : watting) {
                    result[i].insert(QUuid(), j);
                }
                break;
            case 1:
                for (auto j : active.keys()) {
                    result[i].insert(j, active[j]);
                }
                break;
            case 2:
                for (auto j : done.keys()) {
                    result[i].insert(j, done[j]);
                }
                break;
        }
    }
    return result;
}

void ThreadMgr::addWorkerLater(CFFInfo info) {
    watting.enqueue(info);
    qDebug() << "watting enqueue=>" << info.first << " " << info.second;

    emit refresh();
}

void ThreadMgr::changeMaxThread(const unsigned int max) {
    this->maxThread = max;
    this->pool->setMaxThreadCount(max);
    emit refresh();
}

void ThreadMgr::onThreadMsg(const QUuid& id, const QStringList& message) {
    // 信息中转
    emit sendOutMessage(id, UIType::MsgT("ThreadMgr", message));
}
void ThreadMgr::onThreadDebugMsg(const QUuid& id, const QStringList& message) {
    // 信息中转
    emit sendDebugMessage(id, UIType::MsgT("DEBUG", message));
}

void ThreadMgr::onStart(const QUuid& id) {
    emit sendOutEventStart(id, active[id]);
}
void ThreadMgr::onFinish(const QUuid& id) {
    CFFInfo info(active[id]);
    if (!active.remove(id)) {
        qDebug() << "REMOVE ERROR. ID=>" << id;
        debugPrintQueue();
    }
    done.insert(id, info);

    emit sendOutEventStop(id, done[id]);

    emit refresh();
}

void ThreadMgr::onErrored(const QUuid& id) {
    qDebug() << "ERROR IN id=>" << id;
}

void ThreadMgr::onStopSignal(const QUuid& id) {
    watting.clear();
    emit stopThread(id);
}

// 添加新的等待队列或者接受到已完成信号后触发此槽函数
void ThreadMgr::onRefresh() {
    if (active.count() < maxThread && watting.count() != 0) {
        FFmpegWorker* wk = new FFmpegWorker(watting.dequeue());

        active.insert(wk->id, wk->getCFFInfo());
        connect(this, &ThreadMgr::stopThread, wk, &FFmpegWorker::toStop);
        connect(this, &ThreadMgr::addAExtraCommand, wk,
                &FFmpegWorker::runAExternalCommand);

        connect(wk, &FFmpegWorker::sendOutMessage, this,
                &ThreadMgr::onThreadMsg);
        connect(wk, &FFmpegWorker::sendDebugMessage, this,
                &ThreadMgr::onThreadDebugMsg);

        connect(wk, &FFmpegWorker::inStart, this, &ThreadMgr::onStart);
        connect(wk, &FFmpegWorker::inFinish, this, &ThreadMgr::onFinish);
        connect(wk, &FFmpegWorker::inError, this, &ThreadMgr::onErrored);

        pool->start(wk);

        emit refresh();
    }
}

// WorkerMsg ThreadMgr::getThreadInfoNew(WorkerInfo& workerUUID) {
//     WorkerMsg msg(workerUUID, "");
//     {
//         QMutexLocker locker(&mutex);
//         for (auto i : activeQueue) {
//             if (i.second->info.uuid == workerUUID.uuid)
//                 msg.wkInfo.isWorkActive = true;
//         }
//         for (auto i : wattingQueue) {
//             if (i.uuid == workerUUID.uuid) msg.wkInfo.isWorkWatting = true;
//         }
//         for (auto i : doneQueue) {
//             if (i.uuid == workerUUID.uuid) msg.wkInfo.isWorkActive = true;
//         }
//     }
//     return msg;
//     // 是否在watting active done.
// }

// void ThreadMgr::debugPrintQueue() {
//     {
//         QMutexLocker locker(&mutex);
//         QString s[3];
//         for (auto i : s) i.push_back("----------\n");
//         for (auto i : wattingQueue) {
//             s[0].append(i.dbPrintToQString());
//         }
//         for (auto i : activeQueue) {
//             s[0].append(i.second->info.dbPrintToQString());
//         }
//         for (auto i : doneQueue) {
//             s[0].append(i.dbPrintToQString());
//         }
//         qDebug() << "[wait]->" << s[0] << "\n[active]->" << s[1] <<
//         "\n[done]->"
//                  << s[2];
//     }
// }

// void ThreadMgr::onThreadMsg(WorkerMsg msg, UI_IO_CODE code) {
//     // TODO
//     emit entireMsgs(msg);
// }
// // TODO
// void ThreadMgr::onThreadEvent(WorkerMsg msg) {}

// void ThreadMgr::addThreadLater(WorkerInfo info) {
//     wattingQueue.enqueue(info);
//     refresh();
// }

// void ThreadMgr::onStopAllThreads() {
//     {
//         QMutexLocker locker(&mutex);
//         wattingQueue.clear();
//         for (auto i : activeQueue) {
//             i.first->deleteLater();
//         }
//         activeQueue.clear();
//     }
//     // TODO
//     emit stopAimThread(WorkerInfo::getSetToAllStopObj());
// }
// void ThreadMgr::onStopThreadLater(WorkerInfo info) {
//     unsigned char sign = 0;
//     {
//         QMutexLocker locker(&mutex);

//         // 从等待队列中查找并移除
//         QQueue<WorkerInfo> cache;
//         for (auto i : wattingQueue) {
//             if (i.uuid == info.uuid) {
//                 sign = 1U;
//                 continue;
//             }
//             cache.enqueue(i);
//         }
//         wattingQueue.clear();
//         for (auto i : cache) {
//             wattingQueue.enqueue(i);
//         }

//         // 从线程列表中查找并移除
//         QQueue<Tht> cache2;
//         for (auto i : activeQueue) {
//             if (i.second->info.uuid == info.uuid) {
//                 sign = 2U;
//                 i.first->deleteLater();
//                 continue;
//             }
//             cache2.enqueue(i);
//         }
//         activeQueue.clear();
//         for (auto i : cache2) {
//             activeQueue.enqueue(i);
//         }
//     }
//     if (sign == 2) {
//         // 发送线程停止信号
//         emit stopAimThread(info);
//     } else if (sign == 1)
//         ;
//     else
//         // 有无效的删除请求
//         assert(1);

//     refresh();
// }

// // 当添加队列、完成线程和自身被触发的时候触发此函数
// void ThreadMgr::refresh() {
//     if (activeQueue.size() < maxThread && !wattingQueue.isEmpty()) {
//         FFmpegWorker* wk = new FFmpegWorker(wattingQueue.dequeue(), nullptr);
//         QThread* th = new QThread(this);
//         // 移动至线程
//         wk->moveToThread(th);

//         connect(wk, &FFmpegWorker::sendOut, this, &ThreadMgr::onThreadMsg);
//         connect(wk, &FFmpegWorker::sendOutDbg, this,
//         &ThreadMgr::onThreadMsg); connect(wk, &FFmpegWorker::sendCodeToMgr,
//         this,
//                 &ThreadMgr::onThreadEvent);
//         connect(this, &ThreadMgr::startThread, wk,
//                 &FFmpegWorker::startTranscoding);
//         th->start();

//         if (th->isRunning()) qDebug() << "running";

//         emit startThread(wk->info);

//         // 放入线程池
//         activeQueue.enqueue(Tht(th, wk));

//         // 递归开始添加下一个线程
//         refresh();
//     }
// }
