#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <QObject>
#include <QString>
#include <QStringList>

struct ProcessingConfig
{
    QString inputDirectory;
    QString fileMask;
    QString outputDirectory;
    bool deleteInput = false;

    enum class ExistingFileAction {
        Overwrite,
        AddCounter
    } existingFileAction = ExistingFileAction::AddCounter;

    bool useTimer = false;
    int timerIntervalMs = 5000;

    quint64 xorKey = 0;
};

class FileProcessor : public QObject
{
    Q_OBJECT
public:
    explicit FileProcessor(QObject *parent = nullptr);

public slots:
    void processFiles(const QStringList &files, const ProcessingConfig &config);

signals:
    void fileProgress(const QString &filePath, qint64 processedBytes, qint64 totalBytes);
    void fileFinished(const QString &filePath);
    void batchFinished();
    void errorOccurred(const QString &message);

private:
    QString buildOutputPath(const QString &inputPath, const ProcessingConfig &config);
    bool processSingleFile(const QString &inputPath, const ProcessingConfig &config, const QByteArray &keyBytes);
};

#endif // FILEPROCESSOR_H

