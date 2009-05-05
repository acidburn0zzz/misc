#ifndef __MODELEAJOUTUPDATE_H__
#define __MODELEAJOUTUPDATE_H__

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

#include <vector>

class ModeleAjoutUpdate : public QObject {
    Q_OBJECT;

    public:
        ModeleAjoutUpdate();

        void init();
        void init(int id);

        bool addEntry();
        bool updateEntry();

        int getId();
        QString getType(int index);
        QVariant getValue(int index);
        void setValue(QVariant value, int index);

    private:
        virtual void fillListes();

    protected:
        int _id;
        std::vector<QString> _columnTypes;
        std::vector<QString> _columnNames;
        std::vector<QVariant> _values;
        QString _tableName;
        QString _tableId;
        QSqlQuery _query;
};

#endif //__MODELEAJOUTUPDATE_H__
