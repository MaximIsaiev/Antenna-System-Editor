#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <customuibackend.h>
#include <qtbaseobjectproxy.h>
#include <antsystemfilehandling.h>
#include <antsystemtopology.h>
#include <antsystemnamemodel.h>
#include <antsystemelementsmodel.h>
#include <antsystemtreeviewmodel.h>
#include <logdispatcher.h>
#include "antsystemui_export.h"

namespace dfpost {

class ANTSYSTEMUI_EXPORT AntSystemEditorBackend : public QObjectInheritableBaseObject<CustomUiBackend>
{
    Q_OBJECT

    Q_PROPERTY(QString currAntennaName READ currAntennaName WRITE setCurrAntennaName)
    Q_PROPERTY(AntSystemElementsModel *elementsModel READ elementsModel CONSTANT)
    Q_PROPERTY(AntSystemTreeViewModel *antSystemName READ antSystemName CONSTANT)
    Q_PROPERTY(dfpost::Frequency begFreq READ begFreq WRITE setBegFreq NOTIFY begFreqChanged)
    Q_PROPERTY(dfpost::Frequency endFreq READ endFreq WRITE setEndFreq NOTIFY endFreqChanged)
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(float pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(float yaw READ yaw WRITE setYaw NOTIFY yawChanged)
    Q_PROPERTY(float pointX READ pointX WRITE setPointX NOTIFY pointXYZChanged)
    Q_PROPERTY(float pointY READ pointY WRITE setPointY NOTIFY pointXYZChanged)
    Q_PROPERTY(float pointZ READ pointZ WRITE setPointZ NOTIFY pointXYZChanged)
    Q_PROPERTY(float minAz READ getMinAz WRITE setMinAz NOTIFY minAzChanged)
    Q_PROPERTY(float maxAz READ maxAz WRITE setMaxAz NOTIFY maxAzChanged)
    Q_PROPERTY(float minAng READ minAng WRITE setMinAng NOTIFY minAngChanged)
    Q_PROPERTY(float maxAng READ maxAng WRITE setMaxAng NOTIFY maxAngChanged)
    Q_PROPERTY(QString error READ error WRITE writeError NOTIFY errorLogged)
    Q_PROPERTY(QString information READ information WRITE writeInformation NOTIFY informationLogged)

public:
    enum Direction {
        Clockwise = 0,
        CounterClockwise
    };Q_ENUM(Direction)

    enum Location {
        RoundLocation = 0,
                FirstInCenter,
                LastInCenter
    };Q_ENUM(Location)

    // отрисовка точек-координат элементов
    Q_INVOKABLE void drawPointXYZ();

    // отрисовка точек-координат элементов (для главного вида)
    Q_INVOKABLE void drawPointXYZBackup();

    // функция сохранения файла
    Q_INVOKABLE void saveAntSystemFile(const QUrl filePath);

    // функция загрузки файла в приложение
    Q_INVOKABLE void loadAntSystemFile(const QUrl filePath);

    // создание новой АС
    Q_INVOKABLE bool createNewAntSystem(const QString &newAntennaSystemName);

    // создание новой подпанели
    Q_INVOKABLE bool createNewSubPanel(const QString &newSubPanelName, const QModelIndex index);

    // создание нового элемента АС
    Q_INVOKABLE void createNewAntSystemElement();

    // удаление элемента АС
    Q_INVOKABLE void deleteAntSystemElement(int index);

    // удаление всех элементов АС
    Q_INVOKABLE void deleteAllAntSystemElements();

    // удаление АС
    Q_INVOKABLE void deleteAntSystem(const QModelIndex &index);

    // устанавливаем значения крена, тангажа, рыскания и диапазона частот для выбранной системы
    Q_INVOKABLE void setCurrentAntSystem();

    // установка координат элементов  в модель
    Q_INVOKABLE void setElementsDataForModel();

    // переименование антенной системы
    Q_INVOKABLE bool updateAntSystemName(const QModelIndex &index, const QVariant &newAntSystemName);

    // отрисовка группы элементов
    Q_INVOKABLE void drawGroupElements(const Location location, const Direction direction, double count, double diameter);

    // возвращает число антенных систем
    Q_INVOKABLE int countAntSystems();

    // создание нового файла
    Q_INVOKABLE void createNewFile();

    explicit AntSystemEditorBackend(QObject *parent = nullptr);

    float pointX() const;
    void setPointX(float newPointX);

    float pointY() const;
    void setPointY(float newPointY);

    float pointZ() const;
    void setPointZ(float newPointZ);

    float roll() const;
    void setRoll(float newRoll);

    float pitch() const ;
    void setPitch(float newPitch);

    float yaw() const ;
    void setYaw(float newYaw);

    float getMinAz() const;
    void setMinAz(float newMinAz);

    float maxAz() const;
    void setMaxAz(float newMaxAz);

    float minAng() const;
    void setMinAng(float newMinAng);

    float maxAng() const;
    void setMaxAng(float newMaxAng);

    AntSystemElementsModel *elementsModel() const;
    AntSystemTreeViewModel *antSystemName() const;

    QString currAntennaName() const;
    void setCurrAntennaName(const QString &newCurrAntennaName);

    Frequency begFreq() const;
    void setBegFreq(const Frequency &newBegFreq);

    Frequency endFreq() const;
    void setEndFreq(const Frequency &newEndFreq);

    void writeError(const QString &newError);
    QString error() const;

    void writeInformation(const QString &newInformation);
    QString information() const;

signals:
    void antSystemFilePathChanged();
    void antennaNameChanged();
    void pointXYZChanged();
    void pointXYZBackupChanged();
    void rollChanged();
    void pitchChanged();
    void chartsNeedsClear();
    void begFreqChanged();
    void endFreqChanged();
    void yawChanged();
    void minAzChanged();
    void maxAzChanged();
    void minAngChanged();
    void maxAngChanged();
    void errorLogged();
    void informationLogged();

private slots:
    // обновляем топологию после изменения в полях таблицы
    void updateAntSystemElements();

private:
    void sortAntSystemId();
    bool findIndexByName(size_t &index, const std::vector<AntSystemTopology> &topology) const;
    bool removeChildById(size_t &index, uint32_t child, const std::vector<AntSystemTopology> &topology);
    float m_PointX = {NULL_FLOAT};
    float m_PointY = {NULL_FLOAT};
    float m_PointZ = {NULL_FLOAT};
    int angleRollInt = {NULL_INT};
    int anglePitchInt = {NULL_INT};
    uint32_t m_antSystemId = {NULL_INT};
    QString m_currAntennaName;
    QString m_exception;
    QString m_information;
    QUrl m_antSystemFilePath;
    AntSystemFileHandling m_FileHandling;
    AntSystemElementsModel *m_antSystemElementsModel {nullptr};
    AntSystemTreeViewModel *m_antSystemTreeViewModel {nullptr};
    std::vector<AntSystemTopology> m_topology; // по итогу хранит в себе данные из распарсенного xml-файла
    std::vector<AntSystemTopology> m_topologyBackup;
};

}
