#ifndef BUTTONBOXMODEL_H
#define BUTTONBOXMODEL_H

#include <QAbstractListModel>

class ButtonBoxTuple
{
public:
    ButtonBoxTuple(const QString &, const QString &, const QString &);
    
    QString buttonName() const;
    QString iconName() const;
    QString dialogName() const;
    
private:
    QString m_buttonName;
    QString m_iconName;
    QString m_dialogName;
};

class ButtonBoxModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum ButtonBoxRoles {
        ButtonName = Qt::UserRole + 1,
        IconName,
        DialogName
    };
    
    explicit ButtonBoxModel(QObject* parent = 0);
    
    void addTuple(const ButtonBoxTuple &);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
    
private:
    QList<ButtonBoxTuple> tuples;
};

#endif