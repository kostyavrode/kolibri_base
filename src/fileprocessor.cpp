#include "fileprocessor.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

FileProcessor::FileProcessor(QObject *parent)
    : QObject(parent)
{
}

QString FileProcessor::buildOutputPath(const QString &inputPath, const ProcessingConfig &config)
{
    QFileInfo inInfo(inputPath);
    QString baseName = inInfo.completeBaseName();
    QString suffix = inInfo.completeSuffix();

    QDir outDir(config.outputDirectory.isEmpty() ? inInfo.absolutePath()
                                                 : config.outputDirectory);

    QString fileName = suffix.isEmpty()
            ? baseName
            : baseName + "." + suffix;

    QString candidate = outDir.absoluteFilePath(fileName);

    if (config.existingFileAction == ProcessingConfig::ExistingFileAction::Overwrite) {
        return candidate;
    }

    int counter = 1;
    while (QFileInfo::exists(candidate)) {
        QString numberedName = suffix.isEmpty()
                ? QString("%1_%2").arg(baseName).arg(counter)
                : QString("%1_%2.%3").arg(baseName).arg(counter).arg(suffix);
        candidate = outDir.absoluteFilePath(numberedName);
        ++counter;
    }

    return candidate;
}

bool FileProcessor::processSingleFile(const QString &inputPath, const ProcessingConfig &config, const QByteArray &keyBytes)
{
    QFile inFile(inputPath);
    if (!inFile.open(QIODevice::ReadOnly)) {
        emit errorOccurred(QStringLiteral("не удалось открыть входной файл: %1").arg(inFile.errorString()));
        return false;
    }

    QString outPath = buildOutputPath(inputPath, config);
    QFile outFile(outPath);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        emit errorOccurred(QStringLiteral("не удалось открыть выходной файл: %1").arg(outFile.errorString()));
        return false;
    }

    const qint64 totalBytes = inFile.size();
    qint64 processed = 0;

    const qint64 chunkSize = 64 * 1024;

    while (true) {
        QByteArray buffer = inFile.read(chunkSize);
        if (buffer.isEmpty()) {
            break;
        }

        for (int i = 0; i < buffer.size(); ++i) {
            buffer[i] = buffer[i] ^ keyBytes.at(i % keyBytes.size());
        }

        if (outFile.write(buffer) != buffer.size()) {
            emit errorOccurred(QStringLiteral("ошибка записи в выходной файл"));
            return false;
        }

        processed += buffer.size();
        emit fileProgress(inputPath, processed, totalBytes);
    }

    outFile.flush();
    outFile.close();
    inFile.close();

    if (config.deleteInput) {
        QFile::remove(inputPath);
    }

    emit fileFinished(inputPath);
    return true;
}

void FileProcessor::processFiles(const QStringList &files, const ProcessingConfig &config)
{
    QByteArray keyBytes(8, Qt::Uninitialized);
    for (int i = 0; i < 8; ++i) {
        keyBytes[i] = static_cast<char>((config.xorKey >> (8 * i)) & 0xFF);
    }

    for (const QString &path : files) {
        if (!processSingleFile(path, config, keyBytes)) {
            // - продолжаем обработку остальных файлов даже при ошибке одного
        }
    }

    emit batchFinished();
}

