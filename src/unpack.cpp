#include "unpack.h"

#include <quazip/quazipfile.h>
#include <quazip/quachecksum32.h>

#include <QFile>
#include <QCryptographicHash>

Unpack::Unpack(QObject *parent) :
    QObject(parent)
{
}

Unpack::~Unpack()
{
}

void Unpack::unzip(const QString* const PIG_PATH, const QString *path, const QString *sum)
{
    int exitCode = 0;
    
    QFile origin(*path);

    if ((origin.open(QIODevice::ReadOnly)) &&
        (*sum == (QCryptographicHash::hash(origin.readAll(), QCryptographicHash::Md5).toHex()))) {
        origin.close();

        QuaZip zip(*path);
        if (zip.open(QuaZip::mdUnzip)) {
            QuaZipFile zipFile(&zip);

            for (bool f = zip.goToFirstFile(); f; f = zip.goToNextFile()) {
                zipFile.open(QIODevice::ReadOnly);

                QFile target(*PIG_PATH+"/tmp/update/"+zipFile.getActualFileName());
                target.open(QIODevice::WriteOnly);

                if (target.write(zipFile.readAll()) == -1) {
                    zipFile.close();
                    zip.close();
                    target.close();
                    exitCode = -1;
                }
                zipFile.close();
                target.close();
            }
            zip.close();
        } else {
            exitCode = -1;
        }
    } else {
        if (origin.isOpen())
            origin.close();
        exitCode = -1;
    }
    
    emit sendExitCode(exitCode);
}
