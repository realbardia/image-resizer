#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QImageReader>
#include <QImageWriter>
#include <QSize>

void checkSize(const QString &path, int height)
{
    QImageReader reader(path);

    auto size = reader.size();
    if (size.height() <= height)
        return;

    reader.setScaledSize( QSize(size.width() * height / size.height(), height) );

    auto suffixIdx = path.lastIndexOf(".");
    auto suffix = path.mid(suffixIdx + 1);
    auto newPath = path + "_new." + suffix;

    QImageWriter writer(newPath);
    writer.write( reader.read() );

    QFile::remove(path);
    QFile::rename(newPath, path);

    qDebug() << path;
}

void findImages(const QString &path, int height)
{
    auto files = QDir(path).entryList({"*.jpg", "*.png", "*.JPEG", "*.PNG", "*.jpeg", "*.JPG"}, QDir::Files);
    for (auto f: files)
        checkSize(path + "/" + f, height);

    auto dirs = QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto d: dirs)
        findImages(path + "/" + d, height);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    auto args = app.arguments();
    if (args.count() < 3)
    {
        qDebug() << QString("Usage: %1 /path/to/file height").arg(app.applicationName()).toStdString().c_str();
        return 0;
    }

    auto path = args.at(1);
    auto height = args.at(2).toInt();

    findImages(path, height);

    return 0;
}
