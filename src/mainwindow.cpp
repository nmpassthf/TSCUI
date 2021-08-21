#include "include/mainwindow.hpp"

#include "./ui_mainwindow.h"

// zhenghuo::zhenghuo(QWidget* parent) {
//     setMouseTracking(true);
//     setFixedSize(600, 600);
//     startTimer(1);
// }
// void zhenghuo::mouseMoveEvent(QMouseEvent* event) {
//     move(event->globalPos() - QPoint(300, 300));
//     raise();
//     activateWindow();
//     HWND hForgroundWnd = GetForegroundWindow();
//     DWORD dwForeID = ::GetWindowThreadProcessId(hForgroundWnd, NULL);
//     DWORD dwCurID = ::GetCurrentThreadId();

//     ::AttachThreadInput(dwCurID, dwForeID, TRUE);
//     ::SetForegroundWindow((HWND)winId());
//     ::AttachThreadInput(dwCurID, dwForeID, FALSE);
// }
// void zhenghuo::timerEvent(QTimerEvent* e) {
//     raise();
//     activateWindow();
//     HWND hForgroundWnd = GetForegroundWindow();
//     DWORD dwForeID = ::GetWindowThreadProcessId(hForgroundWnd, NULL);
//     DWORD dwCurID = ::GetCurrentThreadId();

//     ::AttachThreadInput(dwCurID, dwForeID, TRUE);
//     ::SetForegroundWindow((HWND)winId());
//     ::AttachThreadInput(dwCurID, dwForeID, FALSE);
// }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mgr(this) {
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    mgr.addWorkerLater(mkCFFInfo());
    ui->setupUi(this);

    connect(ui->stop, &QPushButton::clicked,
            [&]() { mgr.onStopSignal(QUuid()); });
    connect(ui->run, &QPushButton::clicked,
            [&]() { mgr.addWorkerLater(mkCFFInfo()); });

    debugWindow = new DebugWindow(this);
    connect(debugWindow, &QDialog::finished, &exit);

    connect(
        &mgr, &ThreadMgr::sendDebugMessage, debugWindow,
        static_cast<void (DebugWindow::*)(const QUuid&, const QStringList&)>(
            &DebugWindow::printDebugMessage));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent* event) {
    QList<QUrl> droppedFiles = event->mimeData()->urls().toList();
    QStringList list;
    for (auto i : droppedFiles) {
        list.append(i.toString());
    }
    debugWindow->printDebugMessage("Drop Event", list);
}

// bool MainWindow::createProcessWithOutput() {
//     //  进入函数体
//     emit sendOutputDbg("in!createProcessWithOutput FUNC");
//     emit sendOutputDbg("Loc:" + cmdLocate.absolutePath() + "\t cmd:" +
//     cmd);

//     // 判断cmd非空
//     if (cmd.isEmpty()) {
//         emit sendOutput("CMD EMPTY!");
//         return false;
//     }

//     //创建子进程
//     QProcess subProcess;
//     subProcess.setWorkingDirectory(cmdLocate.absolutePath());
//     subProcess.

//     //     BOOL bError = FALSE;
//     //     errno_t ErrorCode = 0;

//     //     HANDLE hPipRead = NULL;
//     //     HANDLE hPipWrite = NULL;

//     //     SECURITY_ATTRIBUTES sa = {0};

//     //     STARTUPINFO Si = {0};
//     //     PROCESS_INFORMATION pi = {0};

//     //     // 接收缓冲区
//     //     // char szRecvData[512] = {0};
//     //     QByteArray recData = 0;
//     //     DWORD recSize = 0;

//     //     // std::string szOutputLine;

//     //     // ErrorCode =
//     //     //     _tcscpy_s(szProcessCommand, _tcslen(szCommandLine) + 1,
//     //     //     szCommandLine);
//     //     // if (0 != ErrorCode) {
//     //     //     goto FUN_CLEANUP;
//     //     // }

//     //     // 创建管道
//     //     sa.nLength = sizeof(sa);
//     //     sa.bInheritHandle = TRUE;
//     //     sa.lpSecurityDescriptor = NULL;

//     //     bError = CreatePipe(&hPipRead, &hPipWrite, &sa, 0);
//     //     if (FALSE == bError || NULL == hPipRead || NULL == hPipWrite)
//     {
//     //         goto FUN_CLEANUP;
//     //     }

//     //     Si.cb = sizeof(Si);
//     //     Si.hStdOutput = hPipWrite;
//     //     Si.hStdError = hPipWrite;
//     //     Si.wShowWindow = SW_HIDE;
//     //     Si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

//     //     // bError = CreateProcess(NULL, szProcessCommand, NULL, NULL,
//     TRUE,
//     //     //                        CREATE_NO_WINDOW, NULL, NULL, &Si,
//     &pi);
//     //     bError = CreateProcess(NULL, lcddd, NULL, NULL, TRUE,
//     //     CREATE_NO_WINDOW,
//     //                            NULL, NULL, &Si, &pi);
//     //     if (FALSE == bError) {
//     //         goto FUN_CLEANUP;
//     //     }

//     //     if (NULL != hPipWrite) {
//     //         CloseHandle(hPipWrite);
//     //         hPipWrite = NULL;
//     //     }

//     //     while (FALSE != ReadFile(hPipRead, szRecvData,
//     _countof(szRecvData) -
//     //     1,
//     //                              &dwRecvSize, NULL)) {
//     //         szRecvData[dwRecvSize] = '\0';

//     //         char* szLinesBegin = szRecvData;
//     //         char* szLinesEnd = NULL;

//     //         while (true) {
//     //             char* szFound = szLinesBegin + strcspn(szLinesBegin,
//     "\r\n");
//     //             szLinesEnd = *szFound != '\0' ? szFound : NULL;

//     //             if (NULL == szLinesEnd) {
//     //                 szOutputLine += szLinesBegin;
//     //                 break;
//     //             }

//     //             *szLinesEnd = '\0';

//     //             if (false == szOutputLine.empty()) {
//     //                 szOutputLine += szLinesBegin;
//     //                 // CallBackFun(szOutputLine.c_str(), lpUser);
//     //                 emit sendOutput(QString("[LINE " +
//     // QString::number(++(*lpUser)) +
//     "]
//     //                                         -->" +
//     // QString(szOutputLine.c_str())));
//     //                 szOutputLine.clear();
//     //             } else {
//     //                 if (*szLinesBegin != '\0') {
//     //                     // CallBackFun(szLinesBegin, lpUser);
//     //                     emit sendOutput(QString("[LINE " +
//     // QString::number(++(*lpUser))
//     //                                             +
//     //                                             "] -->" +
//     // QString(szLinesBegin)));
//     //                 }
//     //             }

//     //             szLinesBegin = szLinesEnd + 1;
//     //         }
//     //     }

//     //     WaitForSingleObject(pi.hProcess, INFINITE);
//     //     bError = TRUE;

//     // FUN_CLEANUP:
//     //     if (NULL != hPipRead) {
//     //         CloseHandle(hPipRead);
//     //         hPipRead = NULL;
//     //     }

//     //     if (NULL != hPipWrite) {
//     //         CloseHandle(hPipWrite);
//     //         hPipWrite = NULL;
//     //     }

//     //     if (NULL != szProcessCommand) {
//     //         delete[] szProcessCommand;
//     //         szProcessCommand = NULL;
//     //     }

//     //     if (NULL != pi.hProcess) {
//     //         CloseHandle(pi.hProcess);
//     //         pi.hProcess = NULL;
//     //     }

//     //     if (NULL != pi.hThread) {
//     //         CloseHandle(pi.hThread);
//     //         pi.hThread = NULL;
//     //     }

//     //     return bError;
// }
