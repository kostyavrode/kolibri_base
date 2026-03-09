#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "fileprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onBrowseInput();
    void onBrowseOutput();
    void onProcessOnce();
    void onToggleTimer();
    void onTimerTick();

    void onFileProgress(const QString &filePath, qint64 processedBytes, qint64 totalBytes);
    void onFileFinished(const QString &filePath);
    void onBatchFinished();
    void onProcessingError(const QString &message);

private:
    Ui::MainWindow *ui;

    QTimer m_timer;
    bool m_processing = false;

    QThread *m_workerThread = nullptr;
    FileProcessor *m_processor = nullptr;
    QStringList m_pendingFiles;

    void setupConnections();
    ProcessingConfig readConfig(bool *ok) const;
    QStringList findInputFiles(const ProcessingConfig &config) const;
    void startProcessing(const QStringList &files, const ProcessingConfig &config);
    void resetWorker();
};

#endif // MAINWINDOW_H

