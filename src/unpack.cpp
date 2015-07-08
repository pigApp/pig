#include "unpack.h"

#include <quazip/quazipfile.h>
#include <quazip/quachecksum32.h>

#include <QCryptographicHash>
#include <QFile>

Unpack::Unpack(QObject *parent) : QObject(parent)
{
}

Unpack::~Unpack()
{
}

bool Unpack::unpack(const QString *_PIG_PATH, const QString path, const QStringList *sums)
{
    /*
    QFile file;

    for (int i=0; i<(*files).count(); i++) {

        file.setFileName((*files)[i]);

        if (file.open(QIODevice::ReadOnly) && ((*sums)[i] == QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex())) {
            file.close();

            QuaZip zip((*files)[i]);

            if (zip.open(QuaZip::mdUnzip)) {
                QuaZipFile zipFile(&zip);

                for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {

                    zipFile.open(QIODevice::ReadOnly);

                    QFile outFile(*_PIG_PATH+"/tmp/"+zipFile.getActualFileName());
                    outFile.open(QIODevice::WriteOnly);

                    if (outFile.write(zipFile.readAll()) == -1) {
                        zipFile.close();
                        zip.close();
                        outFile.close();
                        return false;
                    }

                    zipFile.close();
                    outFile.close();
                }
                zip.close();

            } else {
                return false;
            }
        } else {
            file.close();
            return false;
        }
    }
    */
    return true;
}
