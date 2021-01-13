#include <QCoreApplication>
#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QElapsedTimer>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("command-line-sql");
    db.setDatabaseName("demo");
    db.setUserName("yiyaowen");
    db.setPassword("wenyiyao");
    db.open();

    // 创建数据表
    QSqlQuery query;
    bool success = query.exec("create table automobile \
        (id int primary key, \
        attribute varchar, \
        type varchar, \
        kind varchar, \
        nation int, \
        carnumber int, \
        distance int, \
        oil int, \
        temperature int)");
    if (success) {
        qDebug() << QObject::tr("数据库表创建成功！");
    }
    else {
        qDebug() << QObject::tr("数据库表创建失败！");
    }

    // 查询数据表
    query.exec("select * from automobile");
    QSqlRecord record = query.record();
    qDebug() << QObject::tr("automobile 表字段数：") << record.count();

    // 插入新数据
    QElapsedTimer time;
    time.start();
    query.prepare("insert into automobile values(?,?,?,?,?,?,?,?,?)");
    long recordCount = 100;
    for (int i = 0; i < recordCount; ++i) {
        query.bindValue(0, i);
        query.bindValue(1, "四轮驱动");
        query.bindValue(2, "跑车");
        query.bindValue(3, "概念型");
        query.bindValue(4, rand() % 100);
        query.bindValue(5, rand() % 10000);
        query.bindValue(6, rand() % 200000);
        query.bindValue(7, rand() % 52);
        query.bindValue(8, rand() % 100);
        success = query.exec();
        if (!success) {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("插入失败"));
        }
    }
    qDebug() << QObject::tr("插入 %1 条记录，耗时：%2 ms").arg(recordCount).arg(time.elapsed());

    // 查询并排序
    time.restart();
    success = query.exec("select * from automobile order by id desc");
    if (success) {
        qDebug() << QObject::tr("排序 %1 条记录，耗时：%2 ms").arg(recordCount).arg(time.elapsed());
    }
    else {
        qDebug() << QObject::tr("排序失败！");
    }

    // 更新原有数据
    time.restart();
    for (int i = 0; i < recordCount; ++i) {
        query.clear();
        query.prepare(QString("update automobile set attribute=?,type=?, \
            kind=?,nation=?,carnumber=?,distance=?,oil=?,temperature=? where id=%1").arg(i));
        query.bindValue(0, i);
        query.bindValue(1, "四轮驱动");
        query.bindValue(2, "跑车");
        query.bindValue(3, "概念型");
        query.bindValue(4, rand() % 100);
        query.bindValue(5, rand() % 10000);
        query.bindValue(6, rand() % 200000);
        query.bindValue(7, rand() % 52);
        query.bindValue(8, rand() % 100);
        success = query.exec();
        if (!success) {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("更新失败！"));
        }
    }
    qDebug() << QObject::tr("更新 %1 条记录，耗时：%2 ms").arg(recordCount).arg(time.elapsed());

    // 删除数据
    time.restart();
    query.exec("delete from automobile where id=15");
    qDebug() << QObject::tr("删除一条记录，耗时：%1 ms").arg(time.elapsed());

    //return a.exec();
    return 0;
}
